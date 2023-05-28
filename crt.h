#ifndef Little_CRT
#define Little_CRT

#include <stdint.h>
#include <windows.h>

void* xmalloc(size_t size);
void* xrealloc(void* ptr, size_t size);
void xfree(void* ptr);

void* xmemcpy(void* dest, const void* src, size_t n);
void* xmemset(void* dst, int val, size_t size);
int xmemcmp(const void* str1, const void* str2, size_t n);
void* xmemmove(void* dst, const void* src, size_t count);

extern uint32_t rand_seed;
void xsrand(uint32_t seed);
uint32_t xrand(void);
int xabs(int n);

size_t xstrlen(const char* str);
int xstrcmpi(const char* s1, const char* s2);
int xstricmp(const char* s1, const char* s2);
int xstrcmp(const char* s1, const char* s2);
int xstrncmp(const char* s1, const char* s2, size_t n);
int xstrnicmp(const char* s1, const char* s2, size_t n);
char* xstrcpy(char* dest, const char* src);
char* xstrdup(const char* src);
char* xstrncpy(char* dest, const char* src, size_t n);
const char* xstrchr(const char* str, int ch);
const char* xstrrchr(const char* str, int ch);
char* xstrcat(char* dst, const char* src);
const char* xstrstr(const char* str, const char* substr);

size_t xwcslen(const wchar_t* str);
wchar_t* xwcscpy(wchar_t* dest, const wchar_t* src);
int xwcsicmp(const wchar_t* s1, const wchar_t* s2);
int xwcscmp(const wchar_t* s1, const wchar_t* s2);
int xwcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n);
int xwcsnicmp(const wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* xwcsdup(const wchar_t* src);
wchar_t* xwcsncpy(wchar_t* dest, const wchar_t* src, size_t n);
const wchar_t* xwcschr(const wchar_t* str, wchar_t ch);
const wchar_t* xwcsrchr(const wchar_t* str, wchar_t ch);
wchar_t* xwcscat(wchar_t* dst, const wchar_t* src);
const wchar_t* xwcsstr(const wchar_t* str, const wchar_t* substr);

char* xstrupr(char* s);
wchar_t* xwcsupr(wchar_t* s);
char* xstrlwr(char* s);
wchar_t* xwcslwr(wchar_t* s);
int xtoupper(int c);
wint_t xtowupper(wint_t c);
int xtolower(int c);
wint_t xtowlower(wint_t c);

int32_t xputs(const char* str);

int32_t xprintf(const char* format, ...);
int32_t xwprintf(const wchar_t* format, ...);

#endif