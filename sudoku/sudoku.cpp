#include "stdafx.h"
#include "cross_link.h"
#include "dlx.h"
#include <iostream>
#include <fstream>
#include <random>
#include <assert.h>

using namespace std;

int **generateMatrix;

void ResDealing_toMatrix(const int *Res) {
    static int count = 0;
    generateMatrix[count] = new int[81];
    for (int i = 0; i < 81; i++) {
        int row = (Res[i] - 1) / 9 / 9;
        int col = (Res[i] - 1) / 9 % 9;
        int val = (Res[i] - 1) % 9 + 1;
        generateMatrix[count][row * 9 + col] = val;
    }
    ++count;
}

int computeDegree(int *Matrix) {
    int degree = 0;
    int row_zero_count[9] = { 0 };
    int col_zero_count[9] = { 0 };
    for(int i = 0; i < 81; i++)
        if (!Matrix[i]) {
            row_zero_count[i / 9]++;
            col_zero_count[i % 9]++;
        }
    for (int i = 0; i < 81; i++) {
        if (!Matrix[i]) {
            degree += row_zero_count[i / 9] + col_zero_count[i % 9] - 2;
            for (int j = i / 9 / 3 * 3 * 9 + i % 9 / 3 * 3; j < (i / 9 / 3 + 1) * 3 * 9 + i % 9 / 3 * 3; j += 9)
                for (int k = j; k < j + 3; ++k)
                    if (i / 9 != k / 9 && i % 9 != k % 9 && !Matrix[k])
                        ++degree;
        }
    }
    return degree;
}

void raiseDegree(int *Matrix, int pick, int &degree) { // ÏÈÖÃÁãÔÙ¼ÆËã
    for (int j = pick / 9 * 9; j / 9 == pick / 9; ++j)
        if (!Matrix[j])degree += 2;
    for (int j = pick % 9; j < 81; j += 9)
        if (!Matrix[j])degree += 2;
    degree -= 4;
    for (int j = pick / 9 / 3 * 3 * 9 + pick % 9 / 3 * 3; j < (pick / 9 / 3 + 1) * 3 * 9 + pick % 9 / 3 * 3; j += 9)
        for (int k = j; k < j + 3; ++k)
            if (pick / 9 != k / 9 && pick % 9 != k % 9 && !Matrix[k])
                degree += 2;
}

void showM(int *M) {
    for (int i = 0; i < 81; i++) {
        if(M[i])cout << M[i] << ' ';
        else cout << "_ ";
        if ((i + 1) % 9 == 0)cout << endl;
    }
    cout << endl;
}

void generate(int number, int mode, int result[][81]) {
    assert(mode > 0 && mode < 4);
    --mode;
    random_device rand;
    const int START[3] = { 20, 30, 40 };
    const int DEGREE[3] = { rand() % 200 + 100, rand() % 200 + 300, rand() % 200 + 500 };
    generateMatrix = new int*[number];
    dlx DLX;
    DLX.find(number, ResDealing_toMatrix);
    for (int i = 0; i < number; i++) {
        mode = rand() % 3;
        int x = 0;
        while (x < START[mode]) {
            int pick = rand() % 81;
            if (generateMatrix[i][pick]) {
                generateMatrix[i][pick] = 0;
                x++;
            }
        }
        int degree = computeDegree(generateMatrix[i]);
        while (degree < DEGREE[mode]) {
            int pick = 0;
            do pick = rand() % 81;
            while (!generateMatrix[i][pick]);
            generateMatrix[i][pick] = 0;
            raiseDegree(generateMatrix[i], pick, degree);
            // assert(degree == computeDegree(generateMatrix[i]));
        }

        // print result
        int spaceCount = 0;
        for (int j = 0; j < 81; j++)if (!generateMatrix[i][j])spaceCount++;
        cout << "mode: " << mode+1 << " degree: " << degree << " space count: " << spaceCount << endl;
        showM(generateMatrix[i]);


        for (int j = 0; j < 81; j++)
            result[i][j] = generateMatrix[i][j];
    }
}


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
    int result[20][81];
    generate(20, 2, result);
    return 0;
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

