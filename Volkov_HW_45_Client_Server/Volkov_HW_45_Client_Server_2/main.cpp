#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include "ctime"
using namespace std;

int main() {
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        cout << "WSAStartup ошибка: " << err << endl;
        return 1;
    }
    char hostname[255] = "api.privatbank.ua";
    addrinfo* result = NULL;
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    int iResult = getaddrinfo(hostname, "http", &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo error: " << iResult << endl;
        WSACleanup();
        return 3;
    }
    SOCKET connectSocket = INVALID_SOCKET;
    addrinfo* ptr = NULL;
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {   
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            printf("Ошибка сокета: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);  
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    string uri = "/p24api/pubinfo?json&exchange&coursid=5";
    string request = "GET " + uri + " HTTP/1.1\n";
    request += "Host: " + string(hostname) + "\n";
    request += "Accept: */*\n";
    request += "Accept-Encoding: gzip, deflate, br\n";
    request += "Connection: close\n";
    request += "\n";
    if (send(connectSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) { 
        cout << "Error sending: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 5;
    }
    cout << "Send information..." << endl;
    //5. HTTP Response
    string response;
    const size_t BUFFERSIZE = 1024;
    char resBuf[BUFFERSIZE];
    int respLength;
    do {
        respLength = recv(connectSocket, resBuf, BUFFERSIZE, 0);
        if (respLength > 0) {
            response += string(resBuf).substr(0, respLength) + "\n";
        }
        else {
            cout << "Error accept: " << WSAGetLastError() << endl;
            closesocket(connectSocket);
            WSACleanup();
            return 6;
        }
    } while (respLength == BUFFERSIZE);
    cout << response << endl;
    iResult = shutdown(connectSocket, SD_BOTH); //отключает отправку и получение сообщений сокетом
    if (iResult == SOCKET_ERROR) {
        cout << "Error disconaction: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 7;
    }
    closesocket(connectSocket);
    WSACleanup();
}