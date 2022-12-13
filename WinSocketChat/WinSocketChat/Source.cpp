//Main.cpp
#include <iostream>
#include "Socket.h"

using namespace std;

int main()
{
	int price_smartphone = 500, price_laptop = 1000, price_tablet = 800, summa = 0;
	int nChoice;
	int port = 24242;  //выбираем порт
	string ipAddress = "127.0.0.1"; //Адрес сервера

	char receiveMessage[MAXSTRLEN];
	char sendMessage[MAXSTRLEN];

	cout << "1) Start server" << endl;
	cout << "2) Connect to server" << endl;
	cout << "3) Exit" << endl;
	cin >> nChoice;
	if (nChoice == 1)
	{
		ServerSocket server;
		cout << "Starting server..." << endl;
		//Запускаем сервер
		server.StartHosting(port);
		while (true)
		{
			cout << "\tWaiting..." << endl;
			//Получаем данные от клиента
			//и сохраняем в переменной receiveMessage
			server.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Count of smartphones -> " << receiveMessage << endl;
			summa = summa + price_smartphone * atoi(receiveMessage);
			server.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Count of laptop -> " << receiveMessage << endl;
			summa = summa + price_laptop * atoi(receiveMessage);
			server.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Count of tablet -> " << receiveMessage << endl;
			summa = summa + price_tablet * atoi(receiveMessage);
			cout << "Total -> " << summa << endl;
			//Отправляем данные клиенту
			server.SendDataMessageServer(summa);
			//Если есть сообщение "end", завершаем работу
			if (strcmp(receiveMessage, "end") == 0 ||
				strcmp(sendMessage, "end") == 0)
				break;
		}
	}
	else if (nChoice == 2)
	{
		//cout << "Enter an IP address: " << endl;
		//Запрашиваем IP сервера
		//cin >> ipAddress;
		ClientSocket client;
		//подключаемся к серверу
		client.ConnectToServer(ipAddress.c_str(), port);
		cout << "Smartphone -> 500" << endl;
		cout << "Laptop -> 1000" << endl;
		cout << "Tablet -> 800" << endl;
		while (true)
		{
			//отправляем сообщение
			client.SendDataMessage();
			cout << "\tWaiting" << endl;
			//получаем ответ
			client.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Received: " << receiveMessage << endl;
			if (strcmp(receiveMessage, "end") == 0 ||
				strcmp(sendMessage, "end") == 0)
				break;
		}
		//Закрываем соединение
		client.CloseConnection();
	}
	else if (nChoice == 3)
		return 0;
}










