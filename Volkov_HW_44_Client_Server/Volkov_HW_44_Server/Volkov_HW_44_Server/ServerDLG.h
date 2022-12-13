#pragma once
#include "Header.h"
class ServerDLG
{
public:
	static ServerDLG* ptr;
	static const int MAXSTRLEN = 255;
	TCHAR buffer[MAXSTRLEN];
	HWND hDialog, hEdit1, hEdit2, hBut1, hBut2, hBut3;
	WSADATA wData;
	SOCKET ServerSocket, ClientSocket;
	sockaddr_in SockAddr;
	ServerDLG();
	~ServerDLG();
	void CloseDialog(HWND hWnd);
	BOOL InitDialog(HWND hWnd, HWND hFocus, LPARAM lParam);
	void WMCommand(HWND hWnd, int id, HWND hCtl, UINT CodeNotify);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

