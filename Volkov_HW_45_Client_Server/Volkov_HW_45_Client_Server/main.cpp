#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;

int main() {
	string found[8] = { "\"ccy\"", "\"base_ccy\"", "\"buy\"", "\"sale\"", "\"ccy\"", "\"base_ccy\"", "\"buy\"", "\"sale\"" };
	string srcURL = "https://api.privatbank.ua:443/p24api/pubinfo?json&exchange&coursid=5";
	string File = "Save.txt";
	if (S_OK == URLDownloadToFileA(NULL, srcURL.c_str(), File.c_str(), 0, NULL)) {
		cout << "Save " << File << endl;
	}
	else cout << "Error saving" << endl;
	string buffer;
	fstream in("Save.txt", ios::binary);
	if (!in.is_open()) {
		cout << "Error file opening";
	}
	in >> buffer;
	in.close();
	string temp;
	for (short i = 0; i < 8; i++) {
		short index = buffer.find(found[i]);
		if (index == -1) {
			temp += "Not found\n";
		}
		else {
			short j = index;
			while (buffer != ",") {
				temp += buffer[j];
			}
		}
	}
	cout << endl << temp << endl;
}