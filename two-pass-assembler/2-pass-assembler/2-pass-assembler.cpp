// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 November 19
//
// 2-pass-assembler.cpp

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
	string codestr;
	string label;
	string opcode;
	string operand;
public:
	void setAddress(int a) { address = a; ; }
	void setCodestr(string s) { codestr = s; };
	void setLabel(string l) { label = l; };
	void setOpcode(string c) { opcode = c; };
	void setOperand(string o) { operand = o; };
	int getAddress() { return address; };
	string getCodestr() { return codestr; };
	string getLabel() { return label; };
	string getOpcode() { return opcode; };
	string getOperand() { return operand; };
};

int main(int argc, char** argv) {
	string program;
	string SRCFILE = "SRCFILE"; // default source file name
	string INTFILE = "INTFILE"; // default intermediate file name
	string SYMTABLE = "SYMTABLE"; // default symbol table name
	string OPTABLE = "OPTABLE"; // default opcode table name
	string LISFILE = "LISFILE"; // default listing file name
	string OBJFILE = "OBJFILE"; // defaulf object file name
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

	// create a list for intfile and listfile
	list<Instruction*> intfile;
	list<Instruction*> lisfile;
	list<string> objfile;

	// ===== CHECK ARGUMENT & INITIALIZE INPUT/OUTPUT FILES =====

	// get a program name that currently running
	program = argv[0];
	program = program.substr(program.find_last_of("\\") + 1, program.find_last_of(".") - program.find_last_of("\\") - 1);

	// if the program name is "test", get the arguments to input/output file name
	if (program == "test") { SRCFILE = argv[1]; INTFILE = argv[2]; }

	// ===== CREATE OPTABLE =====================================

	fin.open(OPTABLE); // open file
	while (!fin.eof()) {
		getline(fin, strline);
		key = strline.substr(0, strline.find(",")); // split
		code = strline.substr(strline.find(",") + 1);
		optable.insert(make_pair(key, code)); // insert key and code
	}
	fin.close();

	// ===== PASS ONE ===========================================

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


	// ===== PASS TWO ===========================================
	firstline = true; // clear firstline
	opcode = ""; // clear opcode
	std::stringstream obj, lisobj;// for object file

	fin.open(INTFILE);  // open file
	while (opcode != "end") { // loop till opcode is end

		getline(fin, strline);
		str = strline;

		curloc = stoi(str.substr(0, str.find(" ")), 0, 16); //location
		str = str.substr(5);
		label = str.substr(0, str.find(" ")); // label
		str = str.substr(9);
		opcode = str.substr(0, str.find(" ")); // opcode
		str = str.substr(8);
		operand = str.substr(0, str.find(" ")); // operand

		if (firstline) { // if strline is from firstline
			obj << "H" << label << " " << setw(6) << setfill('0') << std::hex << curloc
				<< setw(6) << setfill('0') << std::hex << proglen;
			objfile.push_back(obj.str()); // push H line into objfile
			obj.str(std::string()); // clear
			obj << "T" << setw(6) << setfill('0') << std::hex << curloc << 30; // start T line
			firstline = false;
		}
		else {
			// if this is not a comment line
			if (label != ".") {
				// if opcode is in optable
				if (optable.count(opcode)) {
					lisobj << optable[opcode];
					// if there is a symbol in the operand field
					if (operand.length()) {
						// if label is not in symtable
						if (!symtable.count(operand)) {
							if (operand.find(",") != string::npos) { // operand that has , inside
								string operandtmp = str.substr(0, str.find(","));
								// because of x(1), plus binary 1000000000000000 (= dec 32768)
								lisobj << std::hex << symtable[operandtmp] + 32768;
							}
							else { // set error flag
								cout << "undefined symbol: " << operand << endl;
							}
						}
						else { // store symbol value as operand address
							lisobj << std::hex << symtable[operand];
						}
					}
					else { // operand is not exist when opcode is exist
						lisobj << setw(4) << setfill('0') << "0";
					}
				}
				else if (opcode == "word" || opcode == "byte") { // convert constant to object code
					lisobj << setw(6) << setfill('0') << std::hex << stoi(operand);
				}
				// if object code will not fit into the current Text record
				if (obj.str().length() + lisobj.str().length() > 69) {
					objfile.push_back(obj.str()); // push to objfile
					obj.str(std::string()); // clear
					obj << "T" << setw(6) << setfill('0') << std::hex << curloc << 30; // new T line
				}			
				obj << lisobj.str(); // add object code to Text record
			}
			else { // if this is a comment line
				opcode = strline.substr(9);
				operand = "";
			}
		}

		// push new instclass object to lisfile list
		Instruction* listinst = new Instruction;
		listinst->setAddress(curloc);
		listinst->setCodestr(lisobj.str());
		listinst->setLabel(label);
		listinst->setOpcode(opcode);
		listinst->setOperand(operand);
		lisfile.push_back(listinst);
		lisobj.str(std::string()); // clear
	}

	string lastTline = obj.str(); // last T line
	int len = (lastTline.length() - 9) / 2; // calculate length of T line
	stringstream tmplen;
	tmplen << setw(2) << setfill('0') << std::hex << len; // convert to hex
	lastTline.replace(7, 2, tmplen.str()); // put T line's length into T line

	objfile.push_back(lastTline); // push last T line into objfile
	obj.str(std::string()); // clear

	obj << "E" << setw(6) << setfill('0') << std::hex << startingAddress;
	objfile.push_back(obj.str()); // push E line into objfile
	obj.str(std::string()); // clear

	fin.close();

	// ===== FOUT TO LISFILE ====================================
	fout.open(LISFILE);
	for (const auto& i : lisfile) {
		fout << setw(5) << left << std::hex << i->getAddress()
			<< setw(7) << left << std::hex << i->getCodestr()
			<< setw(9) << left << i->getLabel()
			<< setw(8) << left << i->getOpcode()
			<< left << i->getOperand() << endl;
	}
	fout.close();

	// ===== FOUT TO OBJFILE ====================================
	fout.open(OBJFILE);
	for (const auto i : objfile) {
		fout << i << endl;
	}
	fout.close();

	return 0;
}
