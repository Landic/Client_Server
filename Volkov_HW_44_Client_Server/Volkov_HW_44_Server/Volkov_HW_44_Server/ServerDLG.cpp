#include "ServerDLG.h"

ServerDLG* ServerDLG::ptr = NULL;
bool serveractive = false;

ServerDLG::ServerDLG()
{
	ptr = this;
}

ServerDLG::~ServerDLG()
{
	closesocket(ClientSocket);
	closesocket(ServerSocket);
	WSACleanup();
	exit(6);
}

DWORD WINAPI AcceptMessage(LPVOID lp) {
	ServerDLG* obptr = (ServerDLG*)lp;
	while (true) {
		obptr->ClientSocket = accept(obptr->ServerSocket, NULL, NULL);
		int client_message = recv(obptr->ClientSocket, (char*)obptr->buffer, obptr->MAXSTRLEN, 0);
		obptr->buffer[client_message] = '\0';
		SendMessage(obptr->hEdit1, WM_SETTEXT, NULL, (LPARAM)obptr->buffer);
	}
	return 0;
}

void ServerDLG::CloseDialog(HWND hWnd)
{
	closesocket(ClientSocket);
	closesocket(ServerSocket);
	WSACleanup();
	exit(5);
	EndDialog(hWnd, 0);
}

BOOL ServerDLG::InitDialog(HWND hWnd, HWND hFocus, LPARAM lParam)
{
	hEdit1 = GetDlgItem(hWnd, IDC_EDIT1); // edit message
	hEdit2 = GetDlgItem(hWnd, IDC_EDIT2); // edit send message
	hBut1 = GetDlgItem(hWnd, IDC_BUTTON1); // button send
	hBut2 = GetDlgItem(hWnd, IDC_BUTTON2); // button start
	hBut3 = GetDlgItem(hWnd, IDC_BUTTON3); // button stop server
	hDialog = hWnd;
	return TRUE;
}

void ServerDLG::WMCommand(HWND hWnd, int id, HWND hCtl, UINT CodeNotify)
{
	if (id == IDC_BUTTON2) {
		WSAStartup(MAKEWORD(2, 2), &wData);
		if (WSAStartup(MAKEWORD(2, 2), &wData) != NO_ERROR) {
			MessageBox(hWnd, TEXT("Error WSAStartup"), TEXT("Server"), MB_OK);
			WSACleanup();
			exit(1);
		}
		ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ServerSocket == INVALID_SOCKET) {
			MessageBox(hWnd, TEXT("Error start server"), TEXT("Server"), MB_OK);
			WSACleanup();
			exit(2);
		}
		SockAddr.sin_family = AF_INET;
		inet_pton(AF_INET, "0.0.0.0", &SockAddr.sin_addr);
		SockAddr.sin_port = htons(24000);
		if (bind(ServerSocket, (SOCKADDR*)&SockAddr, sizeof(SockAddr)) == SOCKET_ERROR) {
			MessageBox(hWnd, TEXT("Error binding server"), TEXT("Server"), MB_OK);
			WSACleanup();
			exit(3);
		}
		listen(ServerSocket, 1);
		if (listen(ServerSocket, 1) == SOCKET_ERROR) {
			MessageBox(hWnd, TEXT("Error listening server"), TEXT("Server"), MB_OK);
			WSACleanup();
			exit(4);
		}
		CreateThread(NULL, 0, AcceptMessage, this, 0, NULL);
		serveractive = true;
		MessageBox(hWnd, TEXT("Server start!"), TEXT("Server"), MB_OK);
	}
	else if (id == IDC_BUTTON1) {
		if (serveractive == false) {
			MessageBox(hWnd, TEXT("Server is not started"), TEXT("Server"), MB_OK);
		}
		TCHAR buffer[MAXSTRLEN];
		GetWindowText(hEdit2, buffer, MAXSTRLEN);
		send(ClientSocket, (char*)buffer, wcslen(buffer), 0);
	}
	else if (id == IDC_BUTTON3 && serveractive == true) {
		closesocket(ClientSocket);
		closesocket(ServerSocket);
		WSACleanup();
		MessageBox(hWnd, TEXT("Server stop"), TEXT("Server"), MB_OK);
	}
}

BOOL CALLBACK ServerDLG::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_CLOSE, ptr->CloseDialog);
		HANDLE_MSG(hWnd, WM_INITDIALOG, ptr->InitDialog);
		HANDLE_MSG(hWnd, WM_COMMAND, ptr->WMCommand);
	}
	return 0;
}
