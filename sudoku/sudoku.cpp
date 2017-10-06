#include "stdafx.h"
#include "cross_link.h"
#include "dlx.h"
#include "puzzle_generator.h"
#include "ExceptionHandler.h"
#include <iostream>
#include <fstream>

using namespace std;
extern dlx DLX;
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
	
	int number = 100;
	int **result = new int*[number]{ 0 };
	for (int i = 0; i < number; i++)
	{
		result[i] = new int[81]{0};
	}
    generate(number, 55, 55, true, result);
    for (int i = 0; i < number; i++) {
        for (int j = 0; j < 81; j++) {
            if (result[i][j]) cout << result[i][j] << ' ';
            else cout << "_ ";
            if ((j + 1) % 9 == 0)cout << endl;
        }
        cout << endl;
    }
    return 0;
}

