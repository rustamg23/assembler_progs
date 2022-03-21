#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


using namespace std;
//[bx] [si] [di] [bx+si] [bx+di]
//w=1 when ax cx dx bx sp bp si di
//w=0 when al cl dl bl ah ch dh bh
//add 000000dw mod rem r/m
//cmp 0011101w mod reg r/m    (rm, reg)
//cmp 0011100w mod reg r/m    (reg, rm)
//mod = 11 when (reg, reg)
//mod = 00 when (reg, rm)

void regAndWidth(string &rm, string& w) {
	if (rm == "ax" || rm == "[bx+si]") {
		w = "1";
		rm = "000";
	}
	else if (rm == "cx" || rm == "[bx+di]") {
		w = "1";
		rm = "001";
	}
	else if (rm == "dx" || rm == "[bp+si]") {
		w = "1";
		rm = "010";
	}
	else if (rm == "bx" || rm == "[bp+di]") {
		w = "1";
		rm = "011";
	}
	else if (rm == "sp" || rm == "[si]") {
		w = "1";
		rm = "100";
	}
	else if (rm == "bp" || rm == "[di]") {
		w = "1";
		rm = "101";
	}
	else if (rm == "si" || rm == "[bp]") {
		w = "1";
		rm = "110";
	}
	else if (rm == "di" || rm == "[bx]") {
		w = "0";
		rm = "111";
	}
	else if (rm == "al") {
		w = "0";
		rm = "000";
	}
	else if (rm == "cl") {
		w = "0";
		rm = "001";
	}
	else if (rm == "dl") {
		w = "0";
		rm = "010";
	}
	else if (rm == "bl") {
		w = "0";
		rm = "011";
	}
	else if (rm == "ah") {
		w = "0";
		rm = "100";
	}
	else if (rm == "ch") {
		w = "0";
		rm = "101";
	}
	else if (rm == "dh") {
		w = "0";
		rm = "110";
	}
	else if (rm == "bh") {
		w = "0";
		rm = "111";
	}
}


int main() {
	string mnema, reg, mem, _reg="", listing = "", temp1, temp2, d, mod, w;
	string sub = "101000", cmp1 = "0011100", cmp2 = "0011101";
	cin >> mnema >> temp1 >> temp2;

	if (temp1[0] == '[') {
		mem = temp1;
		regAndWidth(mem, w);
		mod = "00";
		d = "0";
		if (temp2[0] == '[') return 404;
		else {
			reg = temp2;
			regAndWidth(reg, w);
		}
	}
	else {
		reg = temp1;
		regAndWidth(reg, w);
		d = "1";
		if (temp2[0] == '[') {
			mod = "00";
			mem = temp2;
			regAndWidth(mem, w);
		}
		else {
			mod = "11";
			_reg = temp2;
			regAndWidth(_reg, w);
		}
	}

	if (mnema == "sub") {
		listing.append(sub);
	}
	else {
		if (d == "1")
			listing.append(cmp2);
		else listing.append(cmp1);
	}

	listing.append(d);
	listing.append(w);
	listing.append(mod);
	listing.append(reg);
	(_reg != "") ? listing.append(_reg) : listing.append(mem);

	cout <<listing;
	return 0;
}