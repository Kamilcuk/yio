#include <yio.h>
#if _yIO_HAS_WCHAR_H
#include <ywio.h>
#endif
#include <yio_test.h>
#include <locale.h>

#ifndef _yIO_HAS_UNISTRING
#error _yIO_HAS_UNISTRING
#endif
#ifndef _yIO_HAS_WCHAR_H
#error _yIO_HAS_WCHAR_H
#endif

int main() {
	// we need UTF-8 locale to work here
	char *ret = setlocale(LC_ALL, "en_US.UTF-8");
	if (!ret) {
		ret = setlocale(LC_ALL, "C.UTF-8");
		if (!ret) {
			return 0;
		}
	}

#if !_yIO_HAS_UNISTRING
	// This needs unistring support from u8_width
	char *str = yformatf("{:5s}", "Ω");
	_yIO_TEST(strcmp(str, "    Ω") == 0, "'%s'", str);
	_yIO_TEST(strlen(str) == strlen("    Ω"));
	free(str);
#endif

#if _yIO_HAS_WCHAR_H
	wchar_t *wstr = ywformatf(L"{:5s}", ywpwstring(L"Ω"));
	//fprintf(stderr, "AAAAAAAAAAAAAAAAAAAAAA %ls\n", wstr);
	//for (size_t i = 0; i < wcslen(wstr); i++) {
		//fprintf(stderr, "BBBBB %x\n", wstr[i]);
	//}
	_yIO_TEST(wcscmp(wstr, L"    Ω") == 0, "'%ls'", wstr);
	_yIO_TEST(wcslen(wstr) == wcslen(L"    Ω"));
	free(wstr);
#endif
}

