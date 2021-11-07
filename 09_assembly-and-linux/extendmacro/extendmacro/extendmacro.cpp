// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 November 7
//
// extendmacro.cpp

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
	string label;
	string opcode;
	string operand;
public:
	void setLabel(string l) { label = l; };
	void setOpcode(string c) { opcode = c; };
	void setOperand(string o) { operand = o; };
	string getLabel() { return label; };
	string getOpcode() { return opcode; };
	string getOperand() { return operand; };
};

class Macro { // macro
public:
	string macroname;
	string arg1, arg2;
	list<Instruction*> macroInst; // original macro list
};

int main(int argc, char** argv) {
	string program;
	string SRCFILE = "SRCFILE"; // default input source file name
	string label, opcode, operand;
	string arg1, arg2;

	ifstream fin;
	string strline, str;
	bool macroend = false;

	list<Instruction*> inst;
	Macro* macro = new Macro;

	fin.open(SRCFILE);  // open file
	while (getline(fin, strline)) { // loop till opcode is end
		str = strline;
		label = str.substr(0, str.find(" ")); // label
		str = str.substr(9);
		opcode = str.substr(0, str.find(" ")); // opcode
		str = str.substr(8);
		operand = str.substr(0, str.find_last_of(" ")); // operand

		if (opcode == "macro") { // when opcode is macro
			macro->macroname = label; // set macro name
			macro->arg1 = operand.substr(0, operand.find(",")); // set arg1
			macro->arg2 = operand.substr(operand.find(",") + 1, operand.find(" ") - operand.find(",") - 1); // set arg2
		}

		Instruction* instclass = new Instruction;
		instclass->setLabel(label);
		instclass->setOpcode(opcode);
		instclass->setOperand(operand);

		if (opcode != "macro" && !macroend) {
			if (opcode == "mend") {
				macroend = true;
			}
			else {
				macro->macroInst.push_back(instclass); // push into original macro list
			}
		}

		if (opcode == macro->macroname) { // when macro is detected
			arg1 = operand.substr(0, operand.find(",")); // new arg1
			arg2 = operand.substr(operand.find(",") + 1, operand.find(" ") - operand.find(",") - 1); // new arg2
			
			for (const auto& i : macro->macroInst) {
				Instruction* newinst = new Instruction; // new instruction
				newinst->setLabel(i->getLabel());
				newinst->setOpcode(i->getOpcode());
				newinst->setOperand(i->getOperand());
				int pos, len;

				if (i->getOperand().find(macro->arg1) != string::npos) {
					// replace macro arg1 to new arg1
					pos = i->getOperand().find(macro->arg1);
					len = macro->arg1.length();
					newinst->setOperand(i->getOperand().replace(pos, len, arg1));
				}
				if (i->getOperand().find(macro->arg2) != string::npos) {
					// replace macro arg2 to new arg2
					pos = i->getOperand().find(macro->arg2);
					len = macro->arg2.length();
					newinst->setOperand(i->getOperand().replace(pos, len, arg2)); 
				}
				inst.push_back(newinst); // push new instruction to instruction list
			}
		}
		else {
			inst.push_back(instclass);
		}
	}
	fin.close();

	for (const auto& i : inst) { // print instruction list
		cout << setw(9) << left << i->getLabel()
			<< setw(8) << left << i->getOpcode()
			<< left << i->getOperand() << endl;
	}

	return 0;
}
