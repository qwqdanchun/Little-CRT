#include "crt.h"

#pragma optimize("", off)
#define _FILE_TEXT		0x0001
#define _FILE_EOF		0x0002
#define _FILE_ERROR		0x0004

#define EOF    (-1)

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
    HeapFree(GetProcessHeap(), 0, ptr);
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

const char* xxstrstr(const char* str, const char* substr)
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

FILE* __cdecl xfopen(const char* path, const char* attrs)
{
    FILE* file;
    HANDLE hFile;
    DWORD access, disp;
    if (xstrchr(attrs, 'w'))
    {
        access = GENERIC_WRITE;
        disp = CREATE_ALWAYS;
    }
    else
    {
        access = GENERIC_READ;
        disp = OPEN_EXISTING;
    }

    hFile = CreateFileA(path, access, 0, 0, disp, 0, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return 0;

    file = (FILE*)xmalloc(sizeof(FILE));
    __stosb((PBYTE)file, 0, sizeof(FILE));
    file->_base = (char*)hFile;

    if (xstrchr(attrs, 't')) {
        file->_flag |= _FILE_TEXT;
    }

    return file;
}

FILE* __cdecl _xwfopen(const wchar_t* path, const wchar_t* attrs)
{
    HANDLE hFile;
    DWORD access, disp;
    FILE* file;

    if (xwcschr(attrs, L'w')) {
        access = GENERIC_WRITE;
        disp = CREATE_ALWAYS;
    }
    else
    {
        access = GENERIC_READ;
        disp = OPEN_EXISTING;
    }

    hFile = CreateFileW(path, access, 0, 0, disp, 0, 0);
    if (hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    file = (FILE*)xmalloc(sizeof(FILE));
    __stosb((PBYTE)file, 0, sizeof(FILE));
    file->_base = (char*)hFile;

    if (xwcschr(attrs, L't')) {
        file->_flag |= _FILE_TEXT;
    }

    return file;
}

size_t __cdecl xfwrite(const void* buffer, size_t size, size_t count, FILE* str)
{
    DWORD bw = 0, bw2 = 0;
    HANDLE hFile;
    int textMode;

    if (size * count == 0)
        return 0;

    hFile = str->_base;
    textMode = str->_flag & _FILE_TEXT;

    // Text-mode translation is always ANSI!
    if (textMode)			// text mode -> translate LF -> CRLF
    {
        const char* src = (const char*)buffer;
        size_t startpos = 0, i = 0;
        for (i = 0; i < size * count; i++) {
            const char* crlf;

            if (src[i] != '\n')
                continue;
            if (i > 0 && src[i - 1] == '\r')		// don't translate CRLF
                continue;

            if (i > startpos)
            {
                WriteFile(hFile, &src[startpos], i - startpos, &bw2, 0);
                bw += bw2;
            }

            crlf = "\r\n";
            WriteFile(hFile, crlf, 2, &bw2, 0);
            bw++;		// one '\n' written

            startpos = i + 1;
        }

        if (i > startpos)
        {
            WriteFile(hFile, &src[startpos], i - startpos, &bw2, 0);
            bw += bw2;
        }
    }
    else
        WriteFile(hFile, buffer, (DWORD)(size * count), &bw, 0);
    return bw / size;
}

int __cdecl xfprintf(FILE* fp, const char* s, ...)
{
    va_list args;
    char bfr[1024];
    int len;

    va_start(args, s);

    len = wvsprintfA(bfr, s, args);
    va_end(args);

    xfwrite(bfr, len + 1, sizeof(char), fp);
    return len;
}

int __cdecl vfprintf(FILE* fp, const char* s, va_list args)
{
    char bfr[1024];
    int len;

    len = wvsprintfA(bfr, s, args);

    xfwrite(bfr, len + 1, sizeof(char), fp);
    return len;
}

int __cdecl fwprintf(FILE* fp, const wchar_t* s, ...)
{
    va_list args;
    wchar_t bfr[1024];
    int len;
    char ansibfr[1024];

    va_start(args, s);

    len = wvsprintfW(bfr, s, args);

    va_end(args);

    WideCharToMultiByte(CP_ACP, 0, bfr, -1, ansibfr, sizeof(ansibfr), 0, 0);

    xfwrite(ansibfr, len + 1, sizeof(char), fp);
    return len;
}

int __cdecl xfclose(FILE* fp)
{
    CloseHandle(fp->_base);
    xfree(fp);
    return 0;
}

int __cdecl xfeof(FILE* fp)
{
    return (fp->_flag & _FILE_EOF) ? 1 : 0;
}

int __cdecl xfseek(FILE* str, long offset, int origin)
{
    DWORD meth = FILE_BEGIN;
    if (origin == SEEK_CUR)
        meth = FILE_CURRENT;
    else if (origin == SEEK_END)
        meth = FILE_END;
    SetFilePointer(str->_base, offset, 0, meth);
    str->_flag &= ~_FILE_EOF;
    return 0;
}

long __cdecl xftell(FILE* fp)
{
    return SetFilePointer(fp->_base, 0, 0, FILE_CURRENT);
}

size_t __cdecl xfread(void* buffer, size_t size, size_t count, FILE* str)
{
    HANDLE hFile;
    int textMode;
    char* src;
    DWORD br;

    if (size * count == 0)
        return 0;
    if (xfeof(str))
        return 0;

    hFile = str->_base;
    textMode = str->_flag & _FILE_TEXT;

    if (textMode)
        src = (char*)xmalloc(size * count);
    else
        src = (char*)buffer;

    if (!ReadFile(hFile, src, (DWORD)(size * count), &br, 0))
        str->_flag |= _FILE_ERROR;
    else if (!br)		// nonzero return value and no bytes read = EOF
        str->_flag |= _FILE_EOF;

    if (!br)
        return 0;

    // Text-mode translation is always ANSI
    if (textMode) { // text mode: must translate CR -> LF
        char* dst = (char*)buffer;
        DWORD i;

        for (i = 0; i < br; i++) {
            if (src[i] != '\r')
            {
                *dst++ = src[i];
                continue;
            }

            // If next char is LF -> convert CR to LF
            if (i + 1 < br)
            {
                if (src[i + 1] == '\n')
                {
                    *dst++ = '\n';
                    i++;
                }
                else
                    *dst++ = src[i];
            }
            else if (br > 1)
            {
                // This is the hard part: must peek ahead one byte
                DWORD br2 = 0;
                char peekChar = 0;
                ReadFile(hFile, &peekChar, 1, &br2, 0);
                if (!br2)
                    *dst++ = src[i];
                else if (peekChar == '\n')
                    *dst++ = '\n';
                else
                {
                    xfseek(str, -1, SEEK_CUR);
                    *dst++ = src[i];
                }
            }
            else
                *dst++ = src[i];
        }

        xfree(src);
    }

    return br / size;
}

char* __cdecl xfgets(char* str, int n, FILE* s)
{
    int i;

    if (xfeof(s))
        return 0;

    for (i = 0; i < n - 1; i++)
    {
        if (!xfread(&str[i], 1, sizeof(char), s))
            break;
        if (str[i] == '\r')
        {
            i--;
            continue;
        }
        if (str[i] == '\n')
        {
            i++;
            break;
        }
    }

    str[i] = 0;
    return str;
}

wchar_t* __cdecl xfgetws(wchar_t* str, int n, FILE* s)
{
    int i;
    // Text-mode fgetws converts MBCS->Unicode
    if (s->_flag & _FILE_TEXT)
    {
        char* bfr = (char*)xmalloc(n);
        xfgets(bfr, n, s);
        MultiByteToWideChar(CP_ACP, 0, bfr, -1, str, n);
        xfree(bfr);
        return str;
    }

    // Binary fgetws reads as Unicode

    if (xfeof(s))
        return 0;

    for (i = 0; i < n - 1; i++)
    {
        if (!xfread(&str[i], 1, sizeof(wchar_t), s))
            break;
        if (str[i] == L'\r')
        {
            i--;
            continue;	// does i++
        }
        if (str[i] == L'\n')
        {
            i++;
            break;
        }
    }

    str[i] = 0;
    return str;
}

int __cdecl xfgetc(FILE* s)
{
    char c;

    if (s == 0 || xfeof(s))
        return EOF;

    xfread(&c, 1, sizeof(char), s);

    return (int)c;
}

int __cdecl xfputc(int ch, FILE* s)
{
    char c;

    if (s == 0 || xfeof(s))
        return EOF;

    xfwrite(&c, 1, sizeof(char), s);

    return (int)c;
}

wint_t __cdecl xfgetwc(FILE* s)
{
    wint_t c;

    if (s == 0 || xfeof(s))
        return (wint_t)EOF;

    // text-mode fgetwc reads and converts MBCS
    if (s->_flag & _FILE_TEXT)
    {
        char ch = (char)xfgetc(s);
        wint_t wch;

        MultiByteToWideChar(CP_ACP, 0, &ch, 1, (LPWSTR)&wch, 1);
        return wch;
    }

    // binary fgetwc reads unicode

    xfread(&c, 1, sizeof(wint_t), s);

    return c;
}

int __cdecl xfflush(FILE* stream)
{
    return (FlushFileBuffers(stream->_base) ? 0 : 1);
}

int __cdecl xfileno(FILE* stream)
{
    //_VALIDATE_RETURN((stream != NULL), EINVAL, -1);
    return stream->_file;
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

int xsnprintf(char *s, size_t n, const char *format, ...)
{
    size_t r;
    int calculated_size;
    va_list vl;

    va_start(vl, format);
    calculated_size = x_vscprintf(format, vl);
    va_end(vl);

    va_start(vl, format);
    r = _vsnprintf(s, n, format, vl);
    va_end(vl);

    if (r >= n)
    {
        if (n > 0) s[n - 1] = '\0';
    }
    
    if (r < 0) return r;

    return calculated_size;
}
#pragma optimize("", on)