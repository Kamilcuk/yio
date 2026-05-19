#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Sema/Sema.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/ParsedAttr.h"
#include "clang/Basic/DiagnosticSema.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/DenseMap.h"
#include "fstring_common.hpp"
#include "fstring_helpers.hpp"
#include <string>
#include <vector>
#include <map>
#include <optional>

using namespace clang;

bool Wfstring_format = true;
bool Werror_fstring_format = false;
bool ffstring_optimizations = false;

namespace {
llvm::DenseMap<const FunctionDecl*, int> FStringFormatDecls;

struct FStringFormatAttrInfo : public ParsedAttrInfo {
  static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, "fstring_format"}};

  FStringFormatAttrInfo() {
    Spellings = S;
    NumArgs = 1;
  }

  bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr,
                            const Decl *D) const override {
    if (!isa<FunctionDecl>(D)) {
      S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type)
          << Attr << ExpectedFunction;
      return false;
    }
    return true;
  }

  AttrHandling handleDeclAttribute(Sema &S, Decl *D,
                                   const ParsedAttr &Attr) const override {
    if (Attr.getNumArgs() != 1) return AttributeNotApplied;
    Expr *Arg = Attr.getArgAsExpr(0);
    Expr::EvalResult Eval;
    if (!Arg->EvaluateAsInt(Eval, S.Context)) {
      S.Diag(Arg->getBeginLoc(), diag::err_attribute_argument_type)
          << Attr << AANT_ArgumentIntegerConstant;
      return AttributeNotApplied;
    }

    llvm::APSInt Result = Eval.Val.getInt();
    FStringFormatDecls[cast<FunctionDecl>(D)->getCanonicalDecl()] = Result.getExtValue();

    return AttributeApplied;
  }
};
}
constexpr ParsedAttrInfo::Spelling FStringFormatAttrInfo::S[];

static ParsedAttrInfoRegistry::Add<FStringFormatAttrInfo> Y("fstring_format", "fstring format attribute");

class FStringParser : public FStringParserBase<SourceLocation> {
  DiagnosticsEngine &Diags;
public:
  FStringParser(DiagnosticsEngine &Diags) 
    : FStringParserBase<SourceLocation>([this](SourceLocation l, const std::string& msg) {
        unsigned ID = this->Diags.getCustomDiagID(getDiagLevel(this->Diags), "%0");
        this->Diags.Report(l, ID) << msg;
      }), Diags(Diags) {}
};

class FStringTransformer : public RecursiveASTVisitor<FStringTransformer> {
  CompilerInstance &CI;
  ASTContext &Context;
  FunctionDecl *CurrentFD = nullptr;
  std::vector<Stmt*> ParentStack;

public:
  FStringTransformer(CompilerInstance &CI, ASTContext &Context) 
    : CI(CI), Context(Context) {}

  void report(SourceLocation Loc, const char *Msg, const std::string &Arg1 = "", const std::string &Arg2 = "") {
    reportDiag(CI, Loc, Msg, Arg1, Arg2);
  }

  bool TraverseDecl(Decl *D) {
    if (auto *FD = dyn_cast_or_null<FunctionDecl>(D)) {
      FunctionDecl *PrevFD = CurrentFD;
      CurrentFD = FD;
      bool res = RecursiveASTVisitor<FStringTransformer>::TraverseDecl(D);
      CurrentFD = PrevFD;
      return res;
    }
    return RecursiveASTVisitor<FStringTransformer>::TraverseDecl(D);
  }

  bool TraverseStmt(Stmt *S) {
    if (!S) return true;
    ParentStack.push_back(S);
    bool res = RecursiveASTVisitor<FStringTransformer>::TraverseStmt(S);
    ParentStack.pop_back();
    return res;
  }

  bool VisitCallExpr(CallExpr *CE) {
    FunctionDecl *FD = CE->getDirectCallee();
    if (!FD) return true;

    auto it = FStringFormatDecls.find(FD->getCanonicalDecl());
    if (it != FStringFormatDecls.end()) {
      validateFStringFormat(CE, it->second - 1);
    }

    std::vector<Expr*> NewArgs;

    bool modified = false;
    for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
      Expr *Arg = CE->getArg(i)->IgnoreParenCasts();
      if (CallExpr *SubCE = dyn_cast<CallExpr>(Arg)) {
        FunctionDecl *SubFD = SubCE->getDirectCallee();
        if (SubFD && SubFD->getNameAsString() == "__builtin_fstring") {
          bool magic = false;
          if (i + 1 < CE->getNumArgs()) {
            Expr *Next = CE->getArg(i + 1)->IgnoreParenCasts();
            if (const StringLiteral *SL = dyn_cast<StringLiteral>(Next)) {
              if (SL->getLength() == 0) magic = true;
            }
          }
          if (magic) {
            std::vector<Expr*> Expanded = expandFString(SubCE);
            if (!Expanded.empty()) {
              NewArgs.insert(NewArgs.end(), Expanded.begin(), Expanded.end());
              i++; 
              modified = true;
              continue;
            }
          }
        }
      }
      NewArgs.push_back(CE->getArg(i));
    }

    if (modified) {
      CallExpr *NewCE = CallExpr::Create(Context, CE->getCallee(), NewArgs, CE->getType(), 
                                         CE->getValueKind(), CE->getRParenLoc(), FPOptionsOverride());
      replaceInParent(CE, NewCE);
    }

    return true;
  }

private:
  void validateTypeSpec(QualType QT, const FStringToken &Tok, SourceLocation Loc) {
    if (Tok.spec.empty() || QT.isNull()) return;
    if (!Tok.parsed_to_end) {
      report(Loc, "invalid format specification");
    }
    char type_char = Tok.type;
    QT = QT.getCanonicalType();
    if (QT->isIntegerType()) {
      const char *integral_specs = "duoxbDUOXBc";
      if (type_char != '\0' && !strchr(integral_specs, type_char)) {
        report(Loc, "invalid format specifier %0 for integer. use one of %1 instead", std::string(1, type_char), integral_specs);
      }
    } else if (QT->isRealFloatingType()) {
      const char *float_specs = "fegaFEGA";
      if (type_char != '\0' && !strchr(float_specs, type_char)) {
        report(Loc, "invalid format specifier %0 for float. use one of %1 instead", std::string(1, type_char), float_specs);
      }
    } else if (QT->isPointerType()) {
      QualType Pointee = QT->castAs<PointerType>()->getPointeeType();
      if (Pointee->isCharType()) {
        if (type_char != '\0' && type_char != 's') {
          report(Loc, "invalid format specifier %0 for char*. use s instead", std::string(1, type_char));
        }
      } else {
        if (type_char != '\0' && type_char != 'p') {
          report(Loc, "invalid format specifier %0 for pointer. use p instead", std::string(1, type_char));
        }
      }
    }
  }

  void replaceInParent(Stmt *Old, Stmt *New) {
    if (ParentStack.size() < 2) return;
    Stmt *Parent = ParentStack[ParentStack.size() - 2];
    if (CompoundStmt *CS = dyn_cast<CompoundStmt>(Parent)) {
      for (auto it = CS->body_begin(); it != CS->body_end(); ++it)
        if (*it == Old) { *it = New; return; }
    } else if (CallExpr *PCE = dyn_cast<CallExpr>(Parent)) {
      for (unsigned i = 0; i < PCE->getNumArgs(); ++i)
        if (PCE->getArg(i) == Old) { PCE->setArg(i, cast<Expr>(New)); return; }
    } else if (ReturnStmt *RS = dyn_cast<ReturnStmt>(Parent)) {
      if (RS->getRetValue() == Old) { RS->setRetValue(cast<Expr>(New)); return; }
    } else if (ExprWithCleanups *EWC = dyn_cast<ExprWithCleanups>(Parent)) {
      if (EWC->getSubExpr() == Old) { EWC->setSubExpr(cast<Expr>(New)); return; }
    } else if (CStyleCastExpr *CSE = dyn_cast<CStyleCastExpr>(Parent)) {
      if (CSE->getSubExpr() == Old) { CSE->setSubExpr(cast<Expr>(New)); return; }
    }
  }


  std::vector<Expr*> expandFString(CallExpr *FCall) {
    std::vector<Expr*> Result;
    if (FCall->getNumArgs() < 1) return Result;
    const Expr *StrArg = FCall->getArg(0)->IgnoreParenCasts();
    const StringLiteral *SL = dyn_cast<StringLiteral>(StrArg);
    if (!SL) return Result;

    FStringParser Parser(CI.getDiagnostics());
    std::vector<FStringToken> Tokens = Parser.parse(SL->getString().str().c_str(), SL->getBeginLoc());

    std::map<const Type*, Expr*> type_handlers;
    QualType CommonHandlerTy;
    for (unsigned j = 1; j + 1 < FCall->getNumArgs(); j += 2) {
      Expr *TypePtr = FCall->getArg(j); 
      Expr *Handler = FCall->getArg(j + 1); // Do not ignore casts, we need the pointer
      QualType QT = TypePtr->getType();


      if (const PointerType *PT = QT->getAs<PointerType>()) {
        QualType PointeeTy = PT->getPointeeType().getUnqualifiedType().getCanonicalType();
        const Type* TKey = PointeeTy.getTypePtr();
        if (type_handlers.count(TKey)) {
          report(TypePtr->getBeginLoc(), "duplicate handler for type %0", PointeeTy.getAsString());
        }
        type_handlers[TKey] = Handler;
        QualType HTy = Handler->getType().getCanonicalType();
        if (CommonHandlerTy.isNull()) CommonHandlerTy = HTy;
        else if (CommonHandlerTy != HTy) {
          report(Handler->getBeginLoc(), "all handlers must have the same type");
        }
      }
      }

      std::string new_fmt;
      std::vector<Expr*> vars;
      for (const auto &tok : Tokens) {
      if (tok.is_text) new_fmt += tok.text;
      else {
        new_fmt += "{";
        if (Expr *E = resolveExpression(tok.expr, SL->getBeginLoc())) {
          vars.push_back(E);
          validateTypeSpec(E->getType(), tok, SL->getBeginLoc());
          if (!tok.spec.empty()) { new_fmt += ":"; new_fmt += tok.spec; }
        } else {
          report(SL->getBeginLoc(), "variable or literal %0 not found", tok.expr);
        }
        new_fmt += "}";
      }
      }

      if (CommonHandlerTy.isNull()) CommonHandlerTy = Context.getPointerType(Context.VoidTy);

      std::vector<Expr*> handlers;
      for (Expr *v : vars) {
        QualType VT = v->getType().getUnqualifiedType().getCanonicalType();
        const Type* TKey = VT.getTypePtr();
        if (type_handlers.count(TKey)) handlers.push_back(type_handlers[TKey]);
        else {
          report(SL->getBeginLoc(), "no handler found for type %0", VT.getAsString());
          handlers.push_back(createNullPtr(Context, CommonHandlerTy, SL->getBeginLoc()));
        }
      }

      handlers.push_back(createNullPtr(Context, CommonHandlerTy, SL->getBeginLoc()));
      Expr *HandlerArray = createHandlerArray(Context, CommonHandlerTy, handlers, SL->getBeginLoc());
      Result.push_back(ImplicitCastExpr::Create(Context, Context.getPointerType(CommonHandlerTy), CK_ArrayToPointerDecay, HandlerArray, nullptr, VK_PRValue, FPOptionsOverride()));


    QualType CharTy = Context.CharTy.withConst();
    QualType StrTy = Context.getConstantArrayType(CharTy, llvm::APInt(32, new_fmt.length() + 1), nullptr, ArraySizeModifier::Normal, 0);

    StringLiteral *SLNode = StringLiteral::Create(Context, new_fmt, StringLiteralKind::Ordinary, false, StrTy, SL->getBeginLoc());
    Result.push_back(ImplicitCastExpr::Create(Context, Context.getPointerType(CharTy), CK_ArrayToPointerDecay, SLNode, nullptr, VK_PRValue, FPOptionsOverride()));
    
    for (Expr *v : vars) Result.push_back(v);
    return Result;



  }

  Expr *resolveExpression(const std::string &name, SourceLocation Loc) {
    if (name.empty()) return nullptr;
    char *endptr;
    long val = strtol(name.c_str(), &endptr, 0);
    if (*endptr == '\0') return IntegerLiteral::Create(Context, llvm::APInt(32, val), Context.IntTy, Loc);
    if (CurrentFD) {
      for (auto *P : CurrentFD->parameters()) 
        if (P->getNameAsString() == name) 
          return DeclRefExpr::Create(Context, NestedNameSpecifierLoc(), SourceLocation(), P, false, Loc, P->getType(), VK_LValue);
      
      for (auto *D : CurrentFD->decls()) {
        if (auto *VD = dyn_cast<VarDecl>(D)) {
          if (VD->getNameAsString() == name) {
            return DeclRefExpr::Create(Context, NestedNameSpecifierLoc(), SourceLocation(), VD, false, Loc, VD->getType(), VK_LValue);
          }
        }
      }
    }
    IdentifierInfo &II = Context.Idents.get(name);
    auto R = Context.getTranslationUnitDecl()->lookup(&II);
    if (!R.empty()) {
      if (auto *VD = dyn_cast<ValueDecl>(R.front()))
        return DeclRefExpr::Create(Context, NestedNameSpecifierLoc(), SourceLocation(), VD, false, Loc, VD->getType(), VK_LValue);
    }
    return nullptr;
  }


  void validateFStringFormat(CallExpr *CE, int fmtIdx) {
    if (fmtIdx >= (int)CE->getNumArgs()) return;
    const Expr *Arg = CE->getArg(fmtIdx)->IgnoreParenCasts();
    const StringLiteral *SL = dyn_cast<StringLiteral>(Arg);
    if (!SL) return;
    FStringParser Parser(CI.getDiagnostics());
    std::vector<FStringToken> Tokens = Parser.parse(SL->getString().str().c_str(), SL->getBeginLoc());
    int maxIdx = -1, autoIdx = 0;
    for (const auto &Tok : Tokens) {
      if (Tok.is_text) continue;
      auto getIdx = [&](const std::string &expr, const char *field) {
        int idx;
        if (expr.empty()) idx = autoIdx++;
        else { char *endptr; idx = (int)strtol(expr.c_str(), &endptr, 10);
               if (*endptr != '\0') {
                 report(SL->getBeginLoc(), "invalid dynamic field index");
                 return -1;
               }
        }
        if (idx > maxIdx) maxIdx = idx;
        return idx;
      };
      int mainIdx = getIdx(Tok.expr, "replacement");
      if (mainIdx >= 0 && (fmtIdx + 1 + mainIdx) < (int)CE->getNumArgs()) {
        validateTypeSpec(CE->getArg(fmtIdx + 1 + mainIdx)->getType(), Tok, SL->getBeginLoc());
      }
      if (Tok.width) {
        int wIdx = getIdx(*Tok.width, "width");
        if (wIdx >= 0 && (fmtIdx + 1 + wIdx) < (int)CE->getNumArgs()) {
          Expr *WArg = CE->getArg(fmtIdx + 1 + wIdx)->IgnoreParenCasts();
          if (!WArg->getType()->isIntegerType()) {
            report(WArg->getBeginLoc(), "dynamic width argument must be an integer");
          }
        }
      }
      if (Tok.precision) {
        int pIdx = getIdx(*Tok.precision, "precision");
        if (pIdx >= 0 && (fmtIdx + 1 + pIdx) < (int)CE->getNumArgs()) {
          Expr *PArg = CE->getArg(fmtIdx + 1 + pIdx)->IgnoreParenCasts();
          if (!PArg->getType()->isIntegerType()) {
            report(PArg->getBeginLoc(), "dynamic precision argument must be an integer");
          }
        }
      }
    }
    int placeholders = maxIdx + 1, actualArgs = CE->getNumArgs() - (fmtIdx + 1);
    if (actualArgs < placeholders) {
      report(CE->getBeginLoc(), "too few arguments for format string (expected %0, got %1)", std::to_string(placeholders), std::to_string(actualArgs));
    } else if (actualArgs > placeholders) {
      report(CE->getBeginLoc(), "too many arguments for format string (expected %0, got %1)", std::to_string(placeholders), std::to_string(actualArgs));
    }
  }
};

class FStringConsumer : public ASTConsumer {
  CompilerInstance &CI;
public:
  FStringConsumer(CompilerInstance &CI) : CI(CI) {}
  bool HandleTopLevelDecl(DeclGroupRef D) override {
    FStringTransformer Transformer(CI, CI.getASTContext());
    for (auto *Decl : D) {
      Transformer.TraverseDecl(Decl);
    }
    return true;
  }
};




class FStringAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef) override {
    return std::make_unique<FStringConsumer>(CI);
  }
  bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &args) override { 
    for (const auto &arg : args) {
      if (arg == "Wfstring-format=0") Wfstring_format = false;
      else if (arg == "Wfstring-format=1") Wfstring_format = true;
      else if (arg == "Werror-fstring-format=0") Werror_fstring_format = false;
      else if (arg == "Werror-fstring-format=1") Werror_fstring_format = true;
      else if (arg == "ffstring-optimizations=0") ffstring_optimizations = false;
      else if (arg == "ffstring-optimizations=1") ffstring_optimizations = true;
    }
    return true; 
  }
  ActionType getActionType() override { return AddBeforeMainAction; }
};

static FrontendPluginRegistry::Add<FStringAction> X("fstring_plugin", "yio f-string diagnostic plugin");
