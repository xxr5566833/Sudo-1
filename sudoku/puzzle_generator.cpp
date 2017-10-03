#include "puzzle_generator.h"
#include "stdafx.h"
#include "dlx.h"
#include <random>
#include <assert.h>
#include <iostream>

using namespace std;

int **generateMatrix;
static dlx DLX;

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
    for (int i = 0; i < 81; i++)
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

void raiseDegree(int *Matrix, int pick, int &degree) { // 先置零再计算
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
        if (M[i])cout << M[i] << ' ';
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
    DLX.find(number, ResDealing_toMatrix);
    for (int i = 0; i < number; i++) {
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
        /*
        // print result
        int spaceCount = 0;
        for (int j = 0; j < 81; j++)if (!generateMatrix[i][j])spaceCount++;
        cout << "mode: " << mode + 1 << " degree: " << degree << " space count: " << spaceCount << endl;
        showM(generateMatrix[i]);
        */
        for (int j = 0; j < 81; j++)
            result[i][j] = generateMatrix[i][j];
    }
    for (int i = 0; i < number; i++)
        delete[] generateMatrix[i];
    delete[] generateMatrix;
}

bool solve(int *puzzle, int *solution) {
    int rstr[81][3];
    int rstr_p = 0;
    for(int i = 0; i < 81; i++)
        if (puzzle[i]) {
            rstr[rstr_p][0] = puzzle[i];
            rstr[rstr_p][1] = i / 9 + 1;
            rstr[rstr_p][2] = i % 9 + 1;
            rstr_p++;
        }
    DLX.addRestrict(rstr_p, rstr);
    generateMatrix = new int*;
    //*generateMatrix = new int[81];
    bool findout = DLX.find(1, ResDealing_toMatrix);
    DLX.clearRestrict();
    if (findout)
        for (int i = 0; i < 81; i++)
            solution[i] = (*generateMatrix)[i];
    delete[] * generateMatrix;
    delete generateMatrix;
    return findout;
}
/*
require: (number 1-10000) && (lower>=20 && upper<=55 &&	upper>=lower) && (result!=NULL)
*/
void generate(int number, int lower, int upper, bool unique, int  result[][81])
{
	//1.generate number sudo save to generateMatrix
	generateMatrix = new int*[number];
	DLX.find(number, ResDealing_toMatrix);
	//2.for every matrix,generate sudo puzzle 
	for (int i = 0; i < number; i++)
	{
		int *puzzle = generateMatrix[i];
		bool flag[81];
		//random notemptynum between 81-upper to 81-lower
		int notemptynum = 81 - (rand() % (upper - lower) + lower);		
		do {
			//clear last find's some set
			DLX.clearRestrict();								
			int rstr[81][3] = { 0 };
			int rstr_p = 0;
			for (int i = 0; i < 81; i++)
			{
				flag[i] = false;
			}
			while(rstr_p<notemptynum)
			{
				int pick = rand() % 81;							
				//random get a position not empty
				if (!flag[pick]) {
					rstr[rstr_p][0] = puzzle[pick];
					rstr[rstr_p][1] = pick / 9 + 1;
					rstr[rstr_p][2] = pick % 9 + 1;
					++rstr_p;
					flag[pick] = true;
				}
			}
			DLX.addRestrict(rstr_p, rstr);
		} while (unique && !DLX.find(2, NULL));
		for (int j = 0; j < 81; j++)
		{
			result[i][j] = flag[j]? puzzle[j]:0;
		}

	}
	for (int i = 0; i < number; i++) {
		delete[]generateMatrix[i];
	}
	delete[]generateMatrix;



}