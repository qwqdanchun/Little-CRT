#include <stdint.h>
#include <windows.h>


#pragma optimize("", off)
void* xmalloc(size_t size) {
    return HeapAlloc(GetProcessHeap(), 0, size);
}

void* xrealloc(void* ptr, size_t size) {
    return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
}

void xfree(void* ptr) {
    HeapFree(GetProcessHeap(), 0, ptr);
}

uint32_t rand_seed;

void xsrand(uint32_t seed) {
    rand_seed = seed;
}

uint32_t xrand(void) {
    return rand_seed = rand_seed * 1103515245 + 12345;
}

int xabs(int n)
{
    return (n > 0) ? n : -n;
}

void* xmemcpy(void* dest, const void* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
    }
    return dest;
}

void* xmemset(void* dst, int val, size_t size)
{
    char* realdst = (char*)dst;
    for (size_t i = 0; i < size; i++)
        realdst[i] = (char)val;
    return dst;
}

int xmemcmp(const void* str1, const void* str2, size_t n) {
    if (!n) return 0;
    while (--n && *(uint8_t*)str1 == *(uint8_t*)str2) {
        str1 = (uint8_t*)str1 + 1;
        str2 = (uint8_t*)str2 + 1;
    }
    return *(uint8_t*)str1 - *(uint8_t*)str2;
}

void* xmemmove(void* dst, const void* src, size_t count)
{
    void* ret = dst;
    if (dst <= src || (char*)dst >= ((char*)src + count))
    {
        while (count--)
        {
            *(char*)dst = *(char*)src;
            dst = (char*)dst + 1;
            src = (char*)src + 1;
        }
    }
    else
    {
        dst = (char*)dst + count - 1;
        src = (char*)src + count - 1;
        while (count--)
        {
            *(char*)dst = *(char*)src;
            dst = (char*)dst - 1;
            src = (char*)src - 1;
        }
    }
    return ret;
}

size_t xstrlen(const char* str)
{
    return lstrlenA(str);
}

int xstrcmpi(const char* s1, const char* s2)
{
    return lstrcmpiA(s1, s2);
}

int xstricmp(const char* s1, const char* s2) 
{ 
    return lstrcmpiA(s1, s2);
}

int xstrcmp(const char* s1, const char* s2)
{
    return lstrcmpA(s1, s2);
}

int xstrncmp(const char* s1, const char* s2, size_t n)
{
    if (!n)
        return 0;
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    for (size_t i = 0; i < n; i++)
    {
        if (!p1[i] || p1[i] != p2[i])
            return p1[i] - p2[i];
    }
    return 0;
}

int xstrnicmp(const char* s1, const char* s2, size_t n)
{
    return CompareStringA(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, s1, n, s2, n) - CSTR_EQUAL;
}

char* xstrcpy(char* dest, const char* src)
{
    return lstrcpyA(dest, src);
}

char* xstrdup(const char* src)
{
    if (!src)
        return 0;
    char* dst = (char*)xmalloc(xstrlen(src) + 1);
    xstrcpy(dst, src);
    return dst;
}

char* xstrncpy(char* dest, const char* src, size_t n)
{
    xmemcpy(dest, src, n);
    size_t len = xstrlen(src);
    if (n > len)
        xmemset(&dest[len], 0, n - len);
    return dest;
}

const char* xstrchr(const char* str, int ch)
{
    while (*str)
    {
        if (*str == ch)
            return str;
        str++;
    }
    return 0;
}

const char* xstrrchr(const char* str, int ch)
{
    const char* end = str + xstrlen(str) + 1;
    while (end != str)
    {
        end--;
        if (*end == ch)
            return end;
    }
    return 0;
}

char* xstrcat(char* dst, const char* src)
{
    return lstrcatA(dst, src);
}

const char* xstrstr(const char* str, const char* substr)
{
    int str_len = xstrlen(str);
    int substr_len = xstrlen(substr);
    if (substr_len == 0)
        return str;
    if (str_len < substr_len)
        return 0;
    for (int i = 0; i < (int)(str_len - substr_len + 1); i++)
    {
        if (!xstrcmp(&str[i], substr))
            return (const char*)(&str[i]);
    }
    return 0;
}

size_t xwcslen(const wchar_t* str)
{
    return lstrlenW(str);
}

wchar_t* xwcscpy(wchar_t* dest, const wchar_t* src)
{
    return lstrcpyW(dest, src);
}

int xwcsicmp(const wchar_t* s1, const wchar_t* s2)
{
    return lstrcmpiW(s1, s2);
}

int xwcscmp(const wchar_t* s1, const wchar_t* s2)
{
    return lstrcmpW(s1, s2);
}

int xwcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n)
{
    if (!n)
        return 0;
    for (size_t i = 0; i < n; i++)
    {
        if (!s1[i] || s1[i] != s2[i])
            return s1[i] - s2[i];
    }

    return 0;
}

int xwcsnicmp(const wchar_t* s1, const wchar_t* s2, size_t n)
{
    return CompareStringW(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, s1, n, s2, n);
}

wchar_t* xwcsdup(const wchar_t* src)
{
    if (!src)
        return 0;
    wchar_t* dst = (wchar_t*)xmalloc((xwcslen(src) + 1) * sizeof(wchar_t));
    xwcscpy(dst, src);
    return dst;
}

wchar_t* xwcsncpy(wchar_t* dest, const wchar_t* src, size_t n)
{
    xmemcpy(dest, src, n * sizeof(wchar_t));
    size_t len = xwcslen(src);
    if (n > len)
        xmemset(&dest[len], 0, (n - len) * sizeof(wchar_t));
    return dest;
}

const wchar_t* xwcschr(const wchar_t* str, wchar_t ch)
{
    while (*str)
    {
        if (*str == ch)
            return str;
        str++;
    }
    return 0;
}

const wchar_t* xwcsrchr(const wchar_t* str, wchar_t ch)
{
    const wchar_t* end = str + xwcslen(str) + 1;
    while (end != str)
    {
        end--;
        if (*end == ch)
            return end;
    }
    return 0;
}

wchar_t* xwcscat(wchar_t* dst, const wchar_t* src)
{
    return lstrcatW(dst, src);
}

const wchar_t* xwcsstr(const wchar_t* str, const wchar_t* substr)
{
    int str_len = xwcslen(str);
    int substr_len = xwcslen(substr);
    if (substr_len == 0)
        return str;
    if (str_len < substr_len)
        return 0;
    for (int i = 0; i < (int)(str_len - substr_len + 1); i++)
    {
        if (!xwcscmp(&str[i], substr))
            return (const wchar_t*)(&str[i]);
    }
    return 0;
}

char* xstrupr(char* s)
{
    CharUpperBuffA(s, lstrlenA(s));
    return s;
}

wchar_t* xwcsupr(wchar_t* s)
{
    CharUpperBuffW(s, lstrlenW(s));
    return s;
}

char* xstrlwr(char* s)
{
    CharLowerBuffA(s, lstrlenA(s));
    return s;
}

wchar_t* xwcslwr(wchar_t* s)
{
    CharLowerBuffW(s, lstrlenW(s));
    return s;
}

int xtoupper(int c)
{
    if (c < 'a' || c > 'z')
        return c;
    return c - 0x20;
}

wint_t xtowupper(wint_t c)
{
    return (wint_t)CharUpperW((LPWSTR)c);
}

int xtolower(int c)
{
    if (c < 'A' || c > 'Z')
        return c;
    return c + 0x20;
}

wint_t xtowlower(wint_t c)
{
    return (wint_t)CharLowerW((LPWSTR)c);
}

int32_t xputs(const char* str) {
    HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(stdout, str, xstrlen(str), NULL, NULL);
    WriteConsoleA(stdout, "\n", 1, NULL, NULL);
    return 0;
}

int32_t xprintf(const char* format, ...) {
    char string_buffer[1024];
    va_list args;
    va_start(args, format);
    wvsprintfA(string_buffer, format, args);
    va_end(args);
    int32_t string_length = xstrlen(string_buffer);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string_buffer, string_length, NULL, NULL);
    return string_length;
}

int32_t xwprintf(const wchar_t* format, ...) {
    wchar_t string_buffer[1024];
    va_list args;
    va_start(args, format);
    wvsprintfW(string_buffer, format, args);
    va_end(args);
    int32_t string_length = xwcslen(string_buffer);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string_buffer, string_length, NULL, NULL);
    return string_length;
}
#pragma optimize("", on)
