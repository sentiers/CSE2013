// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 November 1
//
// objectfile-conversion.cpp

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

int main() {

	string tag, progname, addr, size, length, codes, others;
	string strline, str;
	char a;
	ifstream fin;

	fin.open("OBJFILE"); // open file
	while (!fin.eof()) {
		getline(fin, strline);
		tag = strline.substr(0, 1); // tag

		if (tag == "H") {
			progname = strline.substr(1, 6);
			addr = strline.substr(7, 6);
			size = strline.substr(13, 6);
			cout << "Tag = " << tag
				<< ", ProgName = " << progname
				<< ", Addr = " << addr
				<< ", Size = " << size << endl;
		}
		else if (tag == "T") {
			addr = strline.substr(1, 6);
			length = strline.substr(7, 2);
			codes = strline.substr(9);
			cout << "Tag = " << tag
				<< ", Addr = " << addr
				<< ", Length = " << length
				<< ", Codes = " << codes << endl;
			cout << "-> ";
			for (int i = 0; i < codes.length(); i++) {
				a = codes.at(i);
				if (!isdigit(a)) { // if it's not a digit, subtract 55
					cout << a - 55;
				}
				else {
					cout << a - 48; // if it's a digit, subtract 48
				}
				if (i != codes.length() - 1) { // print comma
					cout << ", ";
				}
			}
			cout << endl;
		}
		else if (tag == "M" || tag == "E") {
			addr = strline.substr(1, 6);
			others = strline.substr(7);
			cout << "Tag = " << tag
				<< ", Addr = " << addr
				<< ", Others = " << others << endl;
		}
	}
	fin.close();
	return 0;
}
