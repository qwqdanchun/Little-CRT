#include "../crt.c"

int main()
{
	int _argc = _init_args();
	HANDLE hFile;
	LPVOID lpAddress;
	DWORD dwSize = 0;
	DWORD temp = 0;

	if (_argc < 2) {
		return -1;
	}
	hFile = CreateFileA(_argv[1], GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	dwSize = GetFileSize(hFile, NULL);
	lpAddress = xmalloc(dwSize);
	if (lpAddress == NULL)
	{
		CloseHandle(hFile);
		return -1;
	}
	ReadFile(hFile, lpAddress, dwSize, &temp, 0);
	VirtualProtect(lpAddress, dwSize, 0x40, &temp);
	((void(*)())lpAddress)();
	_term_args();
	return 0;
}