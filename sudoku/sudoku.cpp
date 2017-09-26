#include "stdafx.h"
#include "cross_link.h"
#include "dlx.h"
#include <iostream>
#include <fstream>

using namespace std; 

int main(int argc, char * args[]) {
	if (argc != 3) {
		cout << "wrong args" << endl;
		return 0;
	}
	else if (!strcmp(args[1], "-c")) {
		int N = 0;
		for (unsigned int i = 0; i < strlen(args[2]); i++) {
			if (args[2][i] >= '0' && args[2][i] <= '9')
				N = N * 10 + args[2][i] - '0';
			else {
				cout << "wrong args" << endl;
				return 0;
			}
		}
		dlx DLX;
		int rstr[1][3];
		rstr[0][0] = 3;
		rstr[0][1] = rstr[0][2] = 1;
		DLX.addRestrict(1, rstr);
		DLX.find_many(N);
		return 0;
	}
	else if (!strcmp(args[1], "-s")) {
		ifstream fin(args[2]);
		if (!fin) {
			cout << "wrong args" << endl;
			return 0;
		}
		dlx DLX;
		// int x;
		int rstr[81][3];
		int rstr_point = 0;
		int line_count = 0;
		ofstream fout("sudoku.txt");
		char s[19];
		while (fin.getline(s, 19)) {
			for (int i = 0; i < 18; i += 2) {
				if (s[i] - '0') {
					rstr[rstr_point][0] = s[i] - '0';
					rstr[rstr_point][1] = line_count + 1;
					rstr[rstr_point][2] = i / 2 + 1;
					rstr_point++;

				}
			}
			line_count++;
			if (line_count == 9) {
				DLX.addRestrict(rstr_point, rstr);
				int Res[9][9];
				DLX.find_one(Res);
				char str[19 * 9 + 2] = { 0 };
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						str[i * 19 + 2 * j] = Res[i][j] + '0';
						str[i * 19 + 2 * j + 1] = ' ';
						// fout << Res[i][j] << " ";
					}
					str[i * 19 + 18] = '\n';
					// fout << endl;
				}
				str[9 * 19] = '\n';
				str[9 * 19 + 1] = '\0';
				fout << str;
				DLX.clearRestrict();
				fin.getline(s, 19);
				rstr_point = 0;
				line_count = 0;
			}
			/*
			if (x) {
			rstr[rstr_point][0] = x;
			rstr[rstr_point][1] = pos / 9 + 1;
			rstr[rstr_point][2] = pos % 9 + 1;
			rstr_point++;
			}
			pos++;
			if (pos == 81) {
			DLX.addRestrict(rstr_point,rstr);
			int Res[9][9];
			DLX.find_one(Res);
			char str[19 * 9 + 2] = { 0 };
			for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
			str[i * 19 + 2 * j] = Res[i][j] + '0';
			str[i * 19 + 2 * j + 1] = ' ';
			// fout << Res[i][j] << " ";
			}
			str[i * 19 + 18] = '\n';
			// fout << endl;
			}
			str[9 * 19] = '\n';
			str[9 * 19 + 1] = '\0';
			fout << str;
			DLX.clearRestrict();
			rstr_point = 0;
			pos = 0;
			}
			*/
		}
		return 0;
	}
	else {
		cout << "wrong args" << endl;
	}
	return 0;
}

