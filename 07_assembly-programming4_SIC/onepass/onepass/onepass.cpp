// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 October 26
//
// onepass.cpp

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <list>
#include <unordered_map>

using namespace std;

class Instruction {
private:
	int address;
	string label;
	string opcode;
	string operand;
public:
	void setAddress(int a) { address = a; ; }
	void setLabel(string l) { label = l; };
	void setOpcode(string c) { opcode = c; };
	void setOperand(string o) { operand = o; };
	int getAddress() { return address; };
	string getLabel() { return label; };
	string getOpcode() { return opcode; };
	string getOperand() { return operand; };
};

int main(int argc, char** argv) {
	string program;
	string SRCFILE = "SRCFILE"; // default input source file name
	string INTFILE = "INTFILE"; // default output intermediate file name
	string SYMTABLE = "SYMTABLE"; // default output symbol table name
	string key, code;
	string label, opcode, operand;
	int curloc, proglen;
	int startingAddress = 0;
	int locctr = 0;
	bool firstline = true;

	ifstream fin;
	ofstream fout;
	string strline, str;

	// create symtable and optable
	unordered_map< string, string > optable;
	unordered_map< string, int > symtable;

	// create a list for intfile
	list<Instruction*> intfile;

	// ===== CHECK ARGUMENT & INITIALIZE INPUT/OUTPUT FILES =====

	// get a program name that currently running
	program = argv[0];
	program = program.substr(program.find_last_of("\\") + 1, program.find_last_of(".") - program.find_last_of("\\") - 1);

	// if the program name is "test", get the arguments to input/output file name
	if (program == "test") { SRCFILE = argv[1]; INTFILE = argv[2]; }

	// ===== CREATE OPTABLE =====================================

	fin.open("Optab.txt"); // open file
	while (!fin.eof()) {
		getline(fin, strline);
		key = strline.substr(0, strline.find(",")); // split
		code = strline.substr(strline.find(",") + 1);
		optable.insert(make_pair(key, code)); // insert key and code
	}
	fin.close();

	// ===== ONE PASS ===========================================

	fin.open(SRCFILE);  // open file
	while (opcode != "end") { // loop till opcode is end

		getline(fin, strline);
		str = strline;

		curloc = locctr; // current location
		label = str.substr(0, str.find(" ")); // label
		str = str.substr(9);
		opcode = str.substr(0, str.find(" ")); // opcode
		str = str.substr(8);
		operand = str.substr(0, str.find(" ")); // operand

		if (firstline) { // if strline is from firstline
			if (opcode == "start") {
				startingAddress = stoi(operand, 0, 16);
				locctr = startingAddress;
			}
			else {
				cout << "couldn't find opcode 'start'" << endl;
				locctr = 0;
			}
			curloc = locctr;
			firstline = false;
		}
		else {
			// if this is not a comment line
			if (label != ".") { 
				// if there is a symbol in the label field
				if (label.length()) { 
					// if label is not in symtable
					if (!symtable.count(label)) { 
						symtable.insert(make_pair(label, curloc));
					}
					else {
						// set error flag (duplicate symbol)
						cout << "duplicate symbol: " << label << endl;
					}
				}
				// if opcode is in optable
				if (optable.count(opcode)) { 
					// add 3(instruction length) to locctr
					locctr += 3;
				}
				else if (opcode == "word") {
					// add 3 to locctr
					locctr += 3;
				}
				else if (opcode == "resw") {
					// add 3 * operand to locctr
					locctr += (3 * stoi(operand));
				}
				else if (opcode == "resb") {
					// add operand to locctr
					locctr += stoi(operand);
				}
				else if (opcode == "byte") {
					// add length of constant in bytes to locctr
					if (operand.at(0) == 'c')
						locctr += operand.length() - 3;
					else if (operand.at(0) == 'x')
						locctr += (operand.length() - 3) / 2;
				}
				else {
					if (opcode != "end") {
						// set error flag (invalid operation code)
						cout << "invalid opcode: " << opcode << endl;
					}
				}
			}
			else { // if this is a comment line
				opcode = strline.substr(9);
				operand = "";
			}
		}

		// push new instclass object to infile list
		Instruction* instclass = new Instruction;
		instclass->setAddress(curloc);
		instclass->setLabel(label);
		instclass->setOpcode(opcode);
		instclass->setOperand(operand);
		intfile.push_back(instclass);
	}
	fin.close();

	// ===== PROGRAM LENGTH =====================================
	proglen = locctr - startingAddress;

	// ===== FOUT TO INTFILE ====================================
	fout.open(INTFILE);
	for (const auto& i : intfile) {
		fout << setw(5) << left << std::hex << i->getAddress()
			<< setw(9) << left << i->getLabel()
			<< setw(8) << left << i->getOpcode()
			<< left << i->getOperand() << endl;
	}
	fout.close();

	// ===== FOUT TO SYMTABLE ====================================
	fout.open(SYMTABLE);
	for (auto i = symtable.begin(); i != symtable.end(); i++) {
		fout << setw(10) << left << i->first
			<< setw(5) << left << i->second << endl;
	}
	fout.close();

	return 0;
}
