// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 October 3
//
// SrcfileToIntfile.cpp

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

class Instruction {
private:
	string label;
	string opcode;
	string operand;
public:
	void setLabel(string l) { label = l.substr(0, l.find(" ")); };
	void setOpcode(string c) { opcode = c.substr(0, c.find(" ")); };
	void setOperand(string o) { operand = o.substr(0, o.find(" ")); };
	string getLabel() { return label; };
	string getOpcode() { return opcode; };
	string getOperand() { return operand; };
};

int main(int argc, char** argv) {
	string p;
	string srcfile = "SRCFILE"; // default input file name
	string intfile = "INTFILE"; // default output file name
	int instNum = 0;
	string str;
	string label;
	string opcode;
	string operand;

	// get a program name that currently running
	p = argv[0];
	p = p.substr(p.find_last_of("\\") + 1, p.find_last_of(".") - p.find_last_of("\\") - 1);

	// if the program name is "test", get the arguments to input/output file name
	if (p == "test") {
		srcfile = argv[1];
		intfile = argv[2];
	}

	ifstream fin;
	fin.open(srcfile);
	while (getline(fin, str))
		++instNum; // count the number of instructions
	fin.close();

	// create an array of the object dynamically
	Instruction* inst = new Instruction[instNum];

	fin.open(srcfile);
	for (int i = 0; i < instNum; i++) { // read from srcfile
		getline(fin, str);
		inst[i].setLabel(str.substr(0, 9)); // set label
		inst[i].setOpcode(str.substr(9, 8)); // set opcode
		inst[i].setOperand(str.substr(17)); // set operand
	}

	ofstream fout;
	fout.open(intfile);
	fout << "------------------------" << endl
		<< "LABLE    OPCODE  OPERAND" << endl
		<< "------------------------" << endl;
	for (int i = 0; i < instNum; i++) { // write to intfile
		fout << setw(9) << left << inst[i].getLabel()
			<< setw(8) << left << inst[i].getOpcode()
			<< left << inst[i].getOperand() << endl;
	}
	fout.close();
	delete[] inst;

	return 0;
}
