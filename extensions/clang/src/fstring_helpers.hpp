#ifndef FSTRING_HELPERS_HPP
#define FSTRING_HELPERS_HPP

#include "clang/AST/AST.h"
#include "clang/Frontend/CompilerInstance.h"

using namespace clang;

extern bool Wfstring_format;
extern bool Werror_fstring_format;

inline DiagnosticsEngine::Level getDiagLevel(DiagnosticsEngine &Diags) {
  if (!Wfstring_format) return DiagnosticsEngine::Ignored;
  if (Werror_fstring_format || Diags.getWarningsAsErrors()) return DiagnosticsEngine::Error;
  return DiagnosticsEngine::Warning;
}

inline void reportDiag(CompilerInstance &CI, SourceLocation Loc, const char *Msg, 
                       const std::string &Arg1 = "", const std::string &Arg2 = "") {
  DiagnosticsEngine &Diags = CI.getDiagnostics();
  unsigned ID = Diags.getDiagnosticIDs()->getCustomDiagID(
      (DiagnosticIDs::Level)getDiagLevel(Diags), StringRef(Msg));
  auto Builder = Diags.Report(Loc, ID);
  if (!Arg1.empty()) Builder << Arg1;
  if (!Arg2.empty()) Builder << Arg2;
}

inline Expr *createNullPtr(ASTContext &Context, QualType Ty, SourceLocation Loc) {
  return ImplicitCastExpr::Create(Context, Ty, CK_NullToPointer, 
                                  IntegerLiteral::Create(Context, llvm::APInt(32, 0), Context.IntTy, Loc), 
                                  nullptr, VK_PRValue, FPOptionsOverride());
}

inline Expr *createHandlerArray(ASTContext &Context, QualType ElemTy, const std::vector<Expr*> &handlers, SourceLocation Loc) {
  QualType ArrayTy = Context.getConstantArrayType(ElemTy, llvm::APInt(32, handlers.size()), nullptr, ArraySizeModifier::Normal, 0);
  InitListExpr *ILE = new (Context) InitListExpr(Context, Loc, handlers, Loc);
  ILE->setType(ArrayTy);
  return new (Context) CompoundLiteralExpr(Loc, Context.getTrivialTypeSourceInfo(ArrayTy), ArrayTy, VK_LValue, ILE, false);
}

#endif // FSTRING_HELPERS_HPP
