#pragma once
#include "Header.h"
class ClientDLG
{
public:
	static ClientDLG* ptr;
	static const int MAXSTRLEN = 255;
	TCHAR buffer[MAXSTRLEN];
	char IP[26] = "127.0.0.1";
	HWND hDialog, hEdit1, hEdit2, hEdit3, hButSend, hButStart, hButStop, hButIp;
	WSADATA wData;
	SOCKET ClientSocket;
	sockaddr_in SockAddr;
	ClientDLG();
	~ClientDLG();
	void CloseDialog(HWND hWnd);
	BOOL InitDialog(HWND hWnd, HWND hFocus, LPARAM lParam);
	void WmCommand(HWND hWnd, int id, HWND hCtl, UINT CodeNotify);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

