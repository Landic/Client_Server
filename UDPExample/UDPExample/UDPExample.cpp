#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;
    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    while (true) {
        cout << "Receiving data...." << endl;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }
        char menu[][36] = { "burger", "sprite", "free potato" };
        receiveBuf[bytesReceived] = '\0';
        cout << "Received from " << senderAddr.sin_addr.s_host << endl;
        cout << "Data: " << receiveBuf << endl;
        const size_t sendBufSize = 1024;
        char Start[sendBufSize] = "Wait";
        int sendResult = sendto(udpSocket, Start, strlen(Start), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (strstr(receiveBuf, menu[0]))
        {
            Sleep(9000);
        }
        if (strstr(receiveBuf, menu[1]))
        {
            Sleep(1000);
        }
        if (strstr(receiveBuf, menu[2]))
        {
            Sleep(3000);
        }
        char End[sendBufSize] = "Your delivered ready!";
        sendResult = sendto(udpSocket, End, strlen(End), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        //int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 4;
        }
    }

    closesocket(udpSocket);
    WSACleanup();
}

