#include "crt.h"

#pragma optimize("", off)

#define _MAX_CMD_LINE_ARGS  32
char* _argv[_MAX_CMD_LINE_ARGS + 1];
static char* _rawCmd = 0;

int _init_args()
{
    char* sysCmd = GetCommandLineA();
    int szSysCmd = lstrlenA(sysCmd);
    int argc = 1;

    // copy the system command line
    char* cmd = (char*)HeapAlloc(GetProcessHeap(), 0, sizeof(char) * (szSysCmd + 1));
    _argv[0] = 0;
    _rawCmd = cmd;
    if (!cmd)
        return 0;
    lstrcpyA(cmd, sysCmd);

    // Handle a quoted filename
    if (*cmd == '"')
    {
        cmd++;
        _argv[0] = cmd;						// argv[0] = exe name

        while (*cmd && *cmd != '"')
            cmd++;

        if (*cmd)
            *cmd++ = 0;
        else
            return 0;						// no end quote!
    }
    else
    {
        _argv[0] = cmd;						// argv[0] = exe name

        while (*cmd && !xisspace(*cmd))
            cmd++;

        if (*cmd)
            *cmd++ = 0;
    }

    for (;;)
    {
        while (*cmd && xisspace(*cmd))		// Skip over any whitespace
            cmd++;

        if (*cmd == 0)						// End of command line?
            return argc;

        if (*cmd == '"')					// Argument starting with a quote???
        {
            cmd++;

            _argv[argc++] = cmd;
            _argv[argc] = 0;

            while (*cmd && *cmd != '"')
                cmd++;

            if (*cmd == 0)
                return argc;

            if (*cmd)
                *cmd++ = 0;
        }
        else
        {
            _argv[argc++] = cmd;
            _argv[argc] = 0;

            while (*cmd && !xisspace(*cmd))
                cmd++;

            if (*cmd == 0)
                return argc;

            if (*cmd)
                *cmd++ = 0;
        }

        if (argc >= _MAX_CMD_LINE_ARGS)
            return argc;
    }
}

void _term_args()
{
    if (_rawCmd)
        HeapFree(GetProcessHeap(), 0, _rawCmd);
}

void* xmalloc(size_t size) {
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void* xrealloc(void* ptr, size_t size) {
    return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
}

void xfree(void* ptr) {
	HeapFree(GetProcessHeap(),0, ptr);
}

unsigned int rand_seed;

void xsrand(unsigned int seed) {
    rand_seed = seed;
}

unsigned int xrand(void) {
    return rand_seed = rand_seed * 1103515245 + 12345;
}

int xabs(int n)
{
    return (n > 0) ? n : -n;
}

void xmemcpy(void* dest, void* src, size_t n) {
    int i;
    char* src_char = (char*)src;
    char* dest_char = (char*)dest;
    for (i = 0; i < n; i++)
        dest_char[i] = src_char[i];
}

void* xmemset(void* blk, int c, size_t n)
{
    unsigned char* dst = (unsigned char*)blk;

    while (n-- > 0)
        *dst++ = (unsigned char)c;

    return blk;
}

int xmemcmp(const void* str1, const void* str2, size_t n) {
    if (!n) return 0;
    while (--n && *(unsigned char*)str1 == *(unsigned char*)str2) {
        str1 = (unsigned char*)str1 + 1;
        str2 = (unsigned char*)str2 + 1;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
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
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    size_t i;
    if (!n)
        return 0;
    for (i = 0; i < n; i++)
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
    else {
        char* dst = (char*)xmalloc(xstrlen(src) + 1);
        xstrcpy(dst, src);
        return dst;
    }
}

char* xstrncpy(char* dest, const char* src, size_t n)
{
    size_t len = xstrlen(src);
    xmemcpy(dest, (void *)src, n);
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

int xstrncat_s(char * dest, size_t num, const char * source, size_t count)
{
    size_t i, j;
    if (!dest || !source)
        return 22;

    for(i = 0; i < num; i++)
    {
        if(dest[i] == '\0')
        {
            for(j = 0; (j + i) < num; j++)
            {
                if(j == count || (dest[j + i] = source[j]) == '\0')
                {
                    dest[j + i] = '\0';
                    return 0;
                }
            }
        }
    }

    return 34;
}

char* xstrstr(char* str, const char* substr)
{
    int i;
    int str_len = xstrlen(str);
    int substr_len = xstrlen(substr);
    if (substr_len == 0)
        return str;
    if (str_len < substr_len)
        return 0;
    for (i = 0; i < (int)(str_len - substr_len + 1); i++)
    {
        if (!xstrcmp(&str[i], substr))
            return (char*)(&str[i]);
    }
    return 0;
}

char* xstrtok(char* s, const char* delm)
{
	HMODULE msvcrt = LoadLibraryA("msvcrt.dll");
    char* (__cdecl *func)(char* s, const char* delm) = NULL;
    func = (char* (__cdecl *)(char* s, const char* delm))GetProcAddress(msvcrt, "strtok");
    return (*func)(s, delm);
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
    size_t i;
    if (!n)
        return 0;
    for (i = 0; i < n; i++)
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
    else {
        wchar_t* dst = (wchar_t*)xmalloc((xwcslen(src) + 1) * sizeof(wchar_t));
        xwcscpy(dst, src);
        return dst;
    }
}

wchar_t* xwcsncpy(wchar_t* dest, const wchar_t* src, size_t n)
{
    size_t len = xwcslen(src);
    xmemcpy(dest, (void*)src, n * sizeof(wchar_t));
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
    int i;
    if (substr_len == 0)
        return str;
    if (str_len < substr_len)
        return 0;
    for (i = 0; i < (int)(str_len - substr_len + 1); i++)
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

int xputs(const char* str) {
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(std_out, str, xstrlen(str), NULL, NULL);
    WriteConsoleA(std_out, "\n", 1, NULL, NULL);
    return 0;
}

int xprintf(const char* format, ...) {
    char string_buffer[1024];
    va_list args;
    int string_length;
    va_start(args, format);
    wvsprintfA(string_buffer, format, args);
    va_end(args);
    string_length = xstrlen(string_buffer);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string_buffer, string_length, NULL, NULL);
    return string_length;
}

int xwprintf(const wchar_t* format, ...) {
    wchar_t string_buffer[1024];
    va_list args;
    int string_length;
    va_start(args, format);
    wvsprintfW(string_buffer, format, args);
    va_end(args);
    string_length = xwcslen(string_buffer);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string_buffer, string_length, NULL, NULL);
    return string_length;
}

int xiswctype(wint_t c, wctype_t type)
{
    WORD ret;
    GetStringTypeW(CT_CTYPE1, (LPCWSTR)&c, 1, &ret);
    if ((ret & type) != 0)
        return 1;
    return 0;
}

int xisspace(int c) { return ((c >= 0x09 && c <= 0x0D) || (c == 0x20)); }
int xiswspace(wint_t c) { return xiswctype(c, _BLANK); }

int xisupper(int c) { return (c >= 'A' && c <= 'Z'); }
int xiswupper(wint_t c) { return xiswctype(c, _UPPER); }

int xisalpha(int c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
int xiswalpha(wint_t c) { return xiswctype(c, _ALPHA); }

int xisdigit(int c) { return (c >= '0' && c <= '9'); }
int xiswdigit(wint_t c) { return xiswctype(c, _DIGIT); }

int xisxdigit(int c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
int xiswxdigit(wint_t c) { return xiswctype(c, _HEX); }

int xisalnum(int c) { return xisalpha(c) || xisdigit(c); }
int xiswalnum(wint_t c) { return xiswctype(c, _ALPHA | _DIGIT); }

int xisprint(int c) { return c >= ' '; }
int xiswprint(wint_t c) { return xiswctype(c, (wctype_t)(~_CONTROL)); }

long xatol(const char* str)
{
    int cur;
    int neg;
    long total = 0;
    while (xisspace(*str))
        ++str;
    cur = *str++;
    neg = cur;
    if (cur == '-' || cur == '+')
        cur = *str++;
    while (xisdigit(cur))
    {
        total = 10 * total + (cur - '0');
        cur = *str++;
    }
    if (neg == '-')
        return -total;
    else
        return total;
}

int xatoi(const char* str)
{
    return (int)xatol(str);
}

long xwtol(const wchar_t* str)
{
    wint_t cur;
    wint_t neg;
    long total = 0;
    while (xiswspace(*str))
        ++str;
    cur = *str++;
    neg = cur;
    if (cur == L'-' || cur == L'+')
        cur = *str++;
    while (xiswdigit(cur))
    {
        total = 10 * total + (cur - L'0');
        cur = *str++;
    }
    if (neg == L'-')
        return -total;
    else
        return total;
}

int xwtoi(const wchar_t* str)
{
    return (int)xwtol(str);
}

int x_vscprintf(const char *format, va_list argptr)
{
    HMODULE msvcrt = LoadLibraryA("msvcrt.dll");
    int (__cdecl *func)(const char * __restrict__, va_list) = NULL;
    func = (int (__cdecl *)(const char * __restrict__, va_list))GetProcAddress(msvcrt, "_vscprintf");
    return (*func)(format, argptr);
}

int x_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
    HMODULE msvcrt = LoadLibraryA("msvcrt.dll");
    int (__cdecl *func)(char *buffer, size_t count, const char *format, va_list argptr) = NULL;
    func = (int (__cdecl *)(char *buffer, size_t count, const char *format, va_list argptr))GetProcAddress(msvcrt, "_vsnprintf");
    return (*func)(buffer, count, format, argptr);
}

int xvsprintf_s(char *buffer, size_t numberOfElements, const char *format, va_list argptr)
{
	HMODULE msvcrt = LoadLibraryA("msvcrt.dll");
    int (__cdecl *func)(char *buffer, size_t numberOfElements, const char *format, va_list argptr) = NULL;
    func = (int (__cdecl *)(char *buffer, size_t numberOfElements, const char *format, va_list argptr))GetProcAddress(msvcrt, "vsprintf_s");
    return (*func)(buffer, numberOfElements, format, argptr);
}

int xsnprintf(char *s, size_t n, const char *format, ...)
{
    size_t r;
    int calculated_size;
    va_list vl;

    va_start(vl, format);
    calculated_size = x_vscprintf(format, vl);
    va_end(vl);

    va_start(vl, format);
    r = x_vsnprintf(s, n, format, vl);
    va_end(vl);

    if (r >= n)
    {
        if (n > 0) s[n - 1] = '\0';
    }
    
    if (r < 0) return r;

    return calculated_size;
}

int xputenv(const char *str)
{
 char *name, *value;
 char *dst;
 int ret;

 if (!str)
   return -1;
   
 name = xmalloc(strlen(str) + 1);
 if (!name)
   return -1;
 dst = name;
 while (*str && *str != '=')
  *dst++ = *str++;
 if (!*str++)
 {
   ret = -1;
   goto finish;
 }
 *dst++ = '\0';
 value = dst;
 while (*str)
  *dst++ = *str++;
 *dst = '\0';

 ret = SetEnvironmentVariableA(name, value[0] ? value : NULL) ? 0 : -1;

 /* _putenv returns success on deletion of nonexistent variable, unlike [Rtl]SetEnvironmentVariable */
 if ((ret == -1) && (GetLastError() == ERROR_ENVVAR_NOT_FOUND)) ret = 0;
   
finish:
 HeapFree(GetProcessHeap(), 0, name);
 return ret;
}

time_t xtime( time_t *destTime )
{
	HMODULE msvcrt = LoadLibraryA("msvcrt.dll");
    time_t (__cdecl *func)(time_t *destTime) = NULL;
    func = (time_t (__cdecl *)(time_t *destTime))GetProcAddress(msvcrt, "time");
    return (*func)(destTime);
}
#pragma optimize("", on)