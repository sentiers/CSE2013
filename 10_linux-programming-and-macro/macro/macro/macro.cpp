// CSE2013-01 System Programming and Practice
// 
// Name: Hyunji Cho
// StudentID: 2019113581
// Email: sentiers@naver.com
// Date: 2021 November 14
//
// macro.cpp

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <list>
#include <map>
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

int main() {
	string SRCFILE = "SRCFILE"; // default input source file name
	string label, opcode, operand;

	ifstream fin;
	string strline, str;
	bool macroend = false;

	list<Instruction*> INTFILE;
	list<Instruction*> MACRO;
	unordered_map< string, string > ARGS;
	unordered_map< string, int > LABELS; // for unique labels

	unordered_map< string, int > NAMTAB;
	list<unordered_map< string, string >> ARGTAB;
	list<list<Instruction*>> DEFTAB;

	fin.open(SRCFILE);  // open file
	while (getline(fin, strline)) { // loop till opcode is end
		str = strline;
		label = str.substr(0, str.find(" ")); // label
		str = str.substr(9);
		opcode = str.substr(0, str.find(" ")); // opcode
		str = str.substr(8);
		operand = str.substr(0, str.find_first_of(" ")); // operand

		Instruction* instclass = new Instruction;
		instclass->setLabel(label);
		instclass->setOpcode(opcode);
		instclass->setOperand(operand);

		if (opcode == "MACRO") { // when opcode is macro
			MACRO.clear(); // starting macro instruction list
			macroend = false;

			string opstr = operand;
			while (opstr.find(",") != string::npos) {
				string arg = opstr.substr(0, opstr.find(","));
				ARGS.insert(make_pair(arg, arg));
				opstr = opstr.substr(opstr.find(",") + 1);
			}
			string arg = opstr.substr(0, opstr.find(" "));
			ARGS.insert(make_pair(arg, arg));
			ARGTAB.push_back(ARGS); // ARGTAB

			NAMTAB.insert(make_pair(label, NAMTAB.size())); // NAMETAB

			INTFILE.push_back(instclass);
			MACRO.push_back(instclass);
		}
		else if (NAMTAB.count(opcode)) { // when macro is detected
			string newlabel; // for unique labels
			int index = NAMTAB[opcode];
			auto deftab = DEFTAB.begin();
			std::advance(deftab, index);
			auto argtab = ARGTAB.begin();
			std::advance(argtab, index);

			for (const auto& d : *deftab) {
				if (d->getOpcode() == "MACRO") { // setting up the argtab
					string opstr = operand;
					for (auto a = argtab->begin(); a != argtab->end(); a++) {
						if (opstr.find(",") != string::npos) {
							string arg = opstr.substr(0, opstr.find(","));
							a->second = arg;
							opstr = opstr.substr(opstr.find(",") + 1);
						}
						else {
							string arg = opstr.substr(0, opstr.find(" "));
							a->second = arg;
						}
					}
				}
				else if (d->getOpcode() == "MEND") {
					break;
				}
				else {
					Instruction* newinst = new Instruction; // new instruction
					newinst->setLabel(d->getLabel());
					newinst->setOpcode(d->getOpcode());
					newinst->setOperand(d->getOperand());
					int pos, len;

					for (auto a = argtab->begin(); a != argtab->end(); a++) { // check operand and replace
						if (d->getOperand().find(a->first) != string::npos) {
							pos = d->getOperand().find(a->first);
							len = a->first.length();
							if (d->getOperand().find("->") != string::npos) { // for concatenation
								len = len + 2;
							}
							newinst->setOperand(d->getOperand().replace(pos, len, a->second));
						}
					}
					if (d->getLabel().substr(0, 1) == "$") { // for unique label
						if (LABELS.count(d->getLabel())) {
							char first = 'A';
							char second = 'A';
							int num = LABELS[d->getLabel()];
							if (num > 26) {
								first += (num - 26);
							}
							else {
								second += num;
							}
							newlabel.push_back(first);
							newlabel.push_back(second);
							newinst->setLabel(d->getLabel().replace(1, 0, newlabel));
							LABELS[d->getLabel()] += 1;
						}
						else {
							LABELS.insert(make_pair(d->getLabel(), 0));
						}
					}
					if (LABELS.count(d->getOperand())) {
						newinst->setOperand(d->getOperand().replace(1, 0, newlabel));
					}
					INTFILE.push_back(newinst);
				}
			}
		}
		else if (opcode == "MEND") { // when opcode is mend
			macroend = true;
			INTFILE.push_back(instclass);
			MACRO.push_back(instclass);
			DEFTAB.push_back(MACRO);
		}
		else {
			if (!macroend) { // if the instruction is macro instruction
				INTFILE.push_back(instclass);
				MACRO.push_back(instclass);
			}
			else {
				INTFILE.push_back(instclass);
			}
		}
	}
	fin.close();

	for (const auto& i : INTFILE) { // print instruction list
		cout << setw(9) << left << i->getLabel()
			<< setw(8) << left << i->getOpcode()
			<< left << i->getOperand() << endl;
	}

	return 0;
}
