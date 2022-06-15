#include <yio.h>
#if YIO_HAS_WCHAR_H
#include <ywio.h>
#endif
#include <yio_test.h>
#include <locale.h>

#ifndef YYIO_HAS_UNISTRING
#error YYIO_HAS_UNISTRING
#endif
#ifndef YIO_HAS_WCHAR_H
#error YIO_HAS_WCHAR_H
#endif

int main() {
	// we need UTF-8 locale to work here
	char *ret = setlocale(LC_ALL, "en_US.UTF-8");
	if (!ret) {
		ret = setlocale(LC_ALL, "C.UTF-8");
		if (!ret) {
			//// SKIP_RETURN_CODE 100
			return 100;
		}
	}

#if YYIO_HAS_UNISTRING
	// This needs unistring support from u8_width
	char *str = yformatf("{:5s}", "Ω");
	YIO_TESTEXPR(strcmp(str, "Ω    ") == 0, "'%s'", str);
	YIO_TESTEXPR(strlen(str) == strlen("Ω    "));
	free(str);
#endif

#if YIO_HAS_WCHAR_H
	wchar_t *wstr = ywformatf(L"{:5s}", ywpwstring(L"Ω"));
	//fprintf(stderr, "AAAAAAAAAAAAAAAAAAAAAA %ls\n", wstr);
	//for (size_t i = 0; i < wcslen(wstr); i++) {
		//fprintf(stderr, "BBBBB %x\n", wstr[i]);
	//}
	YIO_TESTEXPR(wcscmp(wstr, L"Ω    ") == 0, "'%ls'", wstr);
	YIO_TESTEXPR(wcslen(wstr) == wcslen(L"Ω    "));
	free(wstr);
#endif
}

