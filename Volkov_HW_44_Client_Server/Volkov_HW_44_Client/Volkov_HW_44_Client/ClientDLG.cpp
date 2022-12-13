#include "ClientDLG.h"

ClientDLG* ClientDLG::ptr = NULL;
bool ServerActive = false;

ClientDLG::ClientDLG()
{
	ptr = this;
}

ClientDLG::~ClientDLG()
{
	closesocket(ClientSocket);
	WSACleanup();
	exit(1);
}

DWORD WINAPI ReceiveMessage(LPVOID lp) {
	ClientDLG* objpt = (ClientDLG*)lp;
	while (true) {
		int ClientMessage = recv(objpt->ClientSocket, (char*)objpt->buffer, objpt->MAXSTRLEN, 0);
		objpt->buffer[ClientMessage] = '\0';
		SendMessage(objpt->hEdit2, WM_SETTEXT, NULL, (LPARAM)objpt->buffer);
		Sleep(1000);
	}
	return 0;
}

void ClientDLG::CloseDialog(HWND hWnd)
{
	closesocket(ClientSocket);
	WSACleanup();
	exit(4);
	EndDialog(hWnd, 0);
}

BOOL ClientDLG::InitDialog(HWND hWnd, HWND hFocus, LPARAM lParam)
{
	hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hWnd, IDC_EDIT2);
	hEdit3 = GetDlgItem(hWnd, IDC_EDIT3);
	hDialog = hWnd;
	return TRUE;
}

void ClientDLG::WmCommand(HWND hWnd, int id, HWND hCtl, UINT CodeNotify)
{
	if (id == IDC_BUTTON1) {
		char temp[26];
		GetWindowText(hEdit1, (LPWSTR)temp, 26);
		if (!strcmp((char*)temp, IP)) {
			MessageBox(hWnd, TEXT("IP incorrect"), TEXT("Client"), MB_OK);
		}
	}
	else if (id == IDC_BUTTON3) {
		WSAStartup(MAKEWORD(2, 2), &wData);
		if (WSAStartup(MAKEWORD(2, 2), &wData) != NO_ERROR) {
			MessageBox(hWnd, TEXT("Error WSAStartup"), TEXT("Client"), MB_OK);
			WSACleanup();
			exit(1);
		}
		ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ClientSocket == INVALID_SOCKET) {
			MessageBox(hWnd, TEXT("Error start server"), TEXT("Cliend"), MB_OK);
			WSACleanup();
			exit(2);
		}
		SockAddr.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &SockAddr.sin_addr);
		SockAddr.sin_port = htons(24000);
		if (connect(ClientSocket, (SOCKADDR*)&SockAddr, sizeof(SockAddr)) == SOCKET_ERROR) {
			MessageBox(hWnd, TEXT("Error connect server"), TEXT("Cliend"), MB_OK);
			WSACleanup();
			exit(3);
		}
		CreateThread(NULL, 0, ReceiveMessage, this, 0, NULL);
		ServerActive = true;
		MessageBox(hWnd, TEXT("Server connect"), TEXT("Cliend"), MB_OK);
	}
	else if (id == IDC_BUTTON2) {
		if (ServerActive == false) {
			MessageBox(hWnd, TEXT("Server is not start"), TEXT("Cliend"), MB_OK);
		}
		TCHAR buffer[MAXSTRLEN];
		GetWindowText(hEdit3, buffer, MAXSTRLEN);
		send(ClientSocket, (char*)buffer, wcslen(buffer), 0);
	}
	else if (id == IDC_BUTTON4) {
		closesocket(ClientSocket);
		WSACleanup();
		MessageBox(hWnd, TEXT("Disconnect"), TEXT("Cliend"), MB_OK);
	}
}

BOOL ClientDLG::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_CLOSE, ptr->CloseDialog);
		HANDLE_MSG(hWnd, WM_INITDIALOG, ptr->InitDialog);
		HANDLE_MSG(hWnd, WM_COMMAND, ptr->WmCommand);
	}
	return FALSE;
}
