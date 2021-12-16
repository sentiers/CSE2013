// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 December 16
//
// 8bit-simple-CPU.cpp

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <bitset>
#include <array>
#include <algorithm>

using namespace std;

class Register {
public:
	unsigned int value{};
	void setValue(unsigned int a) { value = a; };
	unsigned int getValue() { return value; };
};

class PC : public Register {};
class IR : public Register {};
class AC : public Register {};

class CU {
public:
	PC pc;
	IR ir;
};

class ALU {
public:
	AC ac;
	unsigned int add(unsigned int a, unsigned int b) { return a + b; };
	unsigned int mul(unsigned int a, unsigned int b) { return a * b; };
};

int main() {

	string PROGRAM = "PROGRAM";
	string OPTABLE = "OPTABLE";

	int mpt, init;
	string key, code;
	string opcode, operand;
	
	string str;
	ifstream fin;
	
	// ===== CREATE OPTABLE =====================================
	unordered_map< string, string > optable;
	fin.open(OPTABLE); // open file
	while (!fin.eof()) {
		getline(fin, str);
		key = str.substr(0, str.find(",")); // split
		code = str.substr(str.find(",") + 1);
		optable.insert(make_pair(key, code)); // insert key and code
	}
	fin.close();

	cout << "======================" << endl;
	cout << "== 8 bit simple CPU ==" << endl;
	cout << "======================" << endl << endl;
	cout << "Enter a program to load: ";
	cin >> PROGRAM;
	cout << "Enter a value for initializing memory: ";
	cin >> init;
	cout << "Enter a starting location: ";
	cin >> mpt;
	cout << endl;

	// ===== INITIALIZE MEMORY AND REGISTERS =====================
	unsigned int Memory[64]; // initialize Memory
	fill_n(Memory, 64, init); // initialize value for Memory
	CU cu; // initialize control unit
	ALU alu; // initialize arithmetic logic unit
	cu.pc.setValue(mpt); // initialize starting point

	// ===== LOAD PROGRAM =======================================
	fin.open(PROGRAM);  // open program
	while (!fin.eof()) { // loop till it is end
		getline(fin, str);

		opcode = str.substr(0, str.find(" ")); // opcode
		str = str.substr(8);
		operand = str.substr(0, str.find(" ")); // operand

		int value = stoi(optable[opcode], 0, 2); // get opcode value
		value = value << 6; // left shift
		value += stoi(operand, 0, 16); // add operand value
		Memory[mpt++] = value; // load value into memory
	}
	fin.close();

	// ===== SIMULATOR ==========================================
	int select;
	cout << "======================" << endl;
	cout << "Current Program: " + PROGRAM << endl;
	cout << "1: Start" << endl;
	cout << "0: Exit" << endl;
	cout << "======================" << endl;

	cout << "Select: ";
	cin >> select;
	cout << endl;

	while (select != 0) {
		cout << endl;
		cout << "---- 1: run ---- 2: register ---- 3: memory ---- 0: exit ----" << endl;
		cout << "Select: ";
		cin >> select;

		if (select == 1) { // run
			cu.ir.setValue(Memory[cu.pc.getValue()]);
			string inst = std::bitset<8>{ cu.ir.getValue() }.to_string();
			opcode = inst.substr(0, 2);
			operand = inst.substr(2, 6);
			int value = stoi(operand, 0, 2);

			if (opcode == "00") alu.ac.setValue(value);  // LDA
			else if (opcode == "10") alu.ac.setValue(alu.add(alu.ac.getValue(), value)); // ADD
			else if (opcode == "11") alu.ac.setValue(alu.mul(alu.ac.getValue(), value));  // MUL
			else if (opcode == "01") Memory[value] = alu.ac.getValue(); // STA

			cu.pc.setValue(cu.pc.getValue() + 1);
		}
		else if (select == 2) { // view register status
			cout << "------------------------------" << endl;
			cout << " PC: " << std::bitset<8>{cu.pc.getValue()} << setw(8) << cu.pc.getValue() << endl;
			cout << " IR: " << std::bitset<8>{cu.ir.getValue()} << setw(8) << cu.ir.getValue() << endl;
			cout << " AC: " << std::bitset<8>{alu.ac.getValue()} << setw(8) << alu.ac.getValue() << endl;
			cout << "------------------------------" << endl;
		}
		else if (select == 3) { // view memory status
			cout << "------------------------------" << endl;
			cout << "            Memory            " << endl;
			cout << "------------------------------" << endl;
			cout << " Address     Data" << endl;
			for (unsigned int i = 0; i < 64; i++) {
				cout << " " << std::bitset<8>{i}
				<< setw(12) << std::bitset<8>{Memory[i]}
				<< setw(8) << Memory[i] << endl;
			}
			cout << "------------------------------" << endl;
		}
	}
	cout << endl << ". . . Exit" << endl;
	return 0;
}