#include<Windows.h>
#include<iostream>
SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning;
void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
void WINAPI HandlerFunc(DWORD code);
int main() {
	WCHAR Servicename[] = L"ServiceA";
	SERVICE_TABLE_ENTRY Table[] = { {Servicename,ServiceMain},{NULL,NULL} };
	StartServiceCtrlDispatcher(Table);
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv) {


	m_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_ServiceStatus.dwWin32ExitCode = 0;
	m_ServiceStatus.dwServiceSpecificExitCode = 0;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;

	m_ServiceStatusHandle = RegisterServiceCtrlHandler(L"ServiceA", HandlerFunc);
	m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;
	SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus);
	bRunning = true;
	while (bRunning) {
	LPCWSTR PipeName = L"\\\\.\\pipe\\testpipe";
	HANDLE PipeHandle=NULL;
	BOOL PipeInstance;
	WCHAR message[512] = { 0 };
	DWORD bytesWritten = 0;
	BOOL Flag = true;
	wchar_t message2[] = L"HELL";
	DWORD messageLength = lstrlen(message2) * 2;
	do {
		PipeHandle = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	} while (PipeHandle == INVALID_HANDLE_VALUE);

	WriteFile(PipeHandle, &message2, messageLength, &bytesWritten, NULL);

	Flag = ReadFile(PipeHandle, &message, 512, &bytesWritten, NULL);
	std::cout << "Message:" << message << std::endl;
	}
}

void WINAPI HandlerFunc(DWORD code) {
	switch (code) {
	case SERVICE_CONTROL_PAUSE:
		m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:
		m_ServiceStatus.dwWin32ExitCode = 0;
		m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		m_ServiceStatus.dwCheckPoint = 0;
		m_ServiceStatus.dwWaitHint = 0;

		SetServiceStatus(m_ServiceStatusHandle, &m_ServiceStatus);
		bRunning = false;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}
}