#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
int main()
{
    setlocale(0, "ru");
    //1. инициализация "Ws2_32.dll" для текущего процесса
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        cout << "WSAStartup failed with error: " << err << endl;
        return 1;
    }
    //инициализация структуры, для указания ip адреса и порта сервера с которым мы хотим соединиться

    char hostname[255] = "api.openweathermap.org";

    addrinfo* result = NULL;

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    int iResult = getaddrinfo(hostname, "http", &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        return 3;
    }
    SOCKET connectSocket = INVALID_SOCKET;
    addrinfo* ptr = NULL;
    //Пробуем присоединиться к полученному адресу
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        //2. создание клиентского сокета
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        //3. Соединяемся с сервером
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    //4. HTTP Request
    string city;
    cout << "Enter city" << endl;
    cout << "Enter: ";
    cin >> city;
    system("cls");
    string uri = "/data/2.5/weather?q=" + city + "&appid=75f6e64d49db78658d09cb5ab201e483&mode=JSON";
    string request = "GET " + uri + " HTTP/1.1\n";
    request += "Host: " + string(hostname) + "\n";
    request += "Accept: */*\n";
    request += "Accept-Encoding: gzip, deflate, br\n";
    request += "Connection: close\n";
    request += "\n";
    //отправка сообщения
    if (send(connectSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 5;
    }
    cout << "send data" << endl;
    //5. HTTP Response
    string response;
    const size_t BUFFERSIZE = 1024;
    char resBuf[BUFFERSIZE];
    int respLength;
    do {
        respLength = recv(connectSocket, resBuf, BUFFERSIZE, 0);
        if (respLength > 0) {
            response += string(resBuf).substr(0, respLength);
        }
        else {
            cout << "recv failed: " << WSAGetLastError() << endl;
            closesocket(connectSocket);
            WSACleanup();
            return 6;
        }
    } while (respLength == BUFFERSIZE);
    cout << response << endl;
    string str;
    cout << endl << endl;
    int pos;
    str += "ID -> ";
    pos = response.find("id");
    for (int i = pos + 4; response[i] != ','; i++)
    {
        str += response[i];
    }
    str += "\n";
    str += "Country -> ";
    pos = response.find("country");
    for (int i = pos + 10; response[i] != '\"'; i++)
    {
        str += response[i];
    }
    str += "\n";
    str += "City -> ";
    pos = response.find("name");
    for (int i = pos + 7; response[i] != '\"'; i++)
    {
        str += response[i];
    }
    str += "\n";
    str += "Coordinate -> ";
    str += "X: ";
    pos = response.find("lon");
    for (int i = pos + 5; response[i] != ','; i++)
    {
        str += response[i];
    }
    str += "\t";
    str += "Y: ";
    pos = response.find("lat");
    for (int i = pos + 5; response[i] != '}'; i++)
    {
        str += response[i];
    }
    str += "\n";
    string buffer;
    double temp;
    str += "Temp -> ";
    pos = response.find("temp");
    for (int i = pos + 6; response[i] != ','; i++)
    {
        buffer += response[i];
    }
    temp = atof(buffer.c_str());
    temp -= 273.15;
    buffer += temp;
    str += buffer;
    str += "\n";
    str += "Temp min max: min-> ";
    pos = response.find("temp_min");
    for (int i = pos; response[i] != ','; i++)
    {
        buffer += response[i];
    }
    temp = atof(buffer.c_str());
    temp -= 273.15;
    buffer += temp;
    str += buffer;
    str += "\n";
    str += "max-> ";
    pos = response.find("temp_max");
    for (int i = pos; response[i] != ','; i++)
    {
        buffer += response[i];
    }
    temp = atof(buffer.c_str());
    temp -= 273.15;
    buffer += temp;
    str += buffer;
    str += "\n";
    str += "Sunrise -> ";
    pos = response.find("sunrise");
    for (int i = pos; response[i] != ','; i++)
    {
        str += response[i];
    }
    str += "\n";
    str += "Sunset -> ";
    pos = response.find("sunset");
    for (int i = pos; response[i] != '}'; i++)
    {
        str += response[i];
    }
    str += "\n";


    fstream file("save.txt", ios::app);



    if (file.is_open()) {
        file << str;
    }
    else {
        cout << "file does not open" << endl;
    }
    file.close();
    cout << str << endl;
    //отключает отправку и получение сообщений сокетом
    iResult = shutdown(connectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 7;
    }
    closesocket(connectSocket);
    WSACleanup();
}