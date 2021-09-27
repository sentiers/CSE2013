// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 September 27
//
// opcode-program.cpp

#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int main() {
	map< string, string > m;

	ifstream fin;
	fin.open("Optab.txt"); // open file
	while (!fin.eof()) {
		string str;
		string key;
		string code;
		getline(fin, str);
		key = str.substr(0, str.find(",")); // split
		code = str.substr(str.find(",") + 1, str.length());
		m.insert(make_pair(key, code)); // insert key and code
	}

	while (1) {
		string input;
		cout << "Key: ";
		cin >> input;
		if (m.count(input)) // if input is exist in key
			cout << "Code: " << m.find(input)->second << endl;
		else
			cout << "[Error] '" << input << "' is not in table." << endl;
	}

	return 0;
}
