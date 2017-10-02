#include "stdafx.h"
#include "cross_link.h"
#include "dlx.h"
#include <iostream>
#include <fstream>

using namespace std;

void toFile(const int *Res) {
    static std::ofstream fout("sudoku.txt");
    char buf[19 * 9 + 2];
    for (int i = 0; i < 81; ++i) {
        int j = Res[i] - 1;
        int val = j % 9 + 1;
        int pos = j / 9;
        int row = pos / 9;
        int col = pos % 9;
        buf[row * 19 + col * 2] = val + '0';
        buf[row * 19 + col * 2 + 1] = ' ';
    }
    for (int i = 0; i < 9; ++i)
        buf[i * 19 + 18] = '\n';
    buf[9 * 19] = '\n';
    buf[9 * 19 + 1] = 0;
    fout << buf;
}

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
        DLX.find(N, toFile);
        return 0;
    }
    else if (!strcmp(args[1], "-s")) {
        ifstream fin(args[2]);
        if (!fin) {
            cout << "wrong args" << endl;
            return 0;
        }
        dlx DLX;
        int rstr[81][3];
        int rstr_point = 0;
        int line_count = 0;
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
                int **Res = new int*[9];
                for (int i = 0; i < 9; i++)
                    Res[i] = new int[9];
                DLX.find(1, toFile);
                DLX.clearRestrict();
                fin.getline(s, 19);
                rstr_point = 0;
                line_count = 0;
            }
        }
        return 0;
    }
    else {
        cout << "wrong args" << endl;
    }
    return 0;
}

