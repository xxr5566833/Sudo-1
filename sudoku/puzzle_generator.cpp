#include "puzzle_generator.h"
#include "stdafx.h"
#include "dlx.h"
#include <random>
#include <assert.h>
#include <iostream>
#include <string>
// add time.h
#include <time.h>

using namespace std;



dlx DLX;
int **generateMatrix;
 //为了能在测试中使用，这里把static去掉了，不知道有什么影响。。
static int matrixcount = 0;
//为了能修改count  这里把它放在外面了，否则下一次调用时count不是从0开始了

void print(bool flag[], int puzzle[])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			std::cout << " " << (flag[i * 9 + j] ? puzzle[i * 9 + j] : 0);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void ResDealing_toMatrix(const int *Res) {
    generateMatrix[matrixcount] = new int[81];
    for (int i = 0; i < 81; i++) {
        int row = (Res[i] - 1) / 9 / 9;
        int col = (Res[i] - 1) / 9 % 9;
        int val = (Res[i] - 1) % 9 + 1;
        generateMatrix[matrixcount][row * 9 + col] = val;
    }
    ++matrixcount;
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
    matrixcount = 0;
    for (int i = 0; i < number; i++) {
        DLX.find(1, true, ResDealing_toMatrix);
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
        }
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
    bool findout = DLX.find(1, false, ResDealing_toMatrix);
    DLX.clearRestrict();
    if (findout)
        for (int i = 0; i < 81; i++)
            solution[i] = (*generateMatrix)[i];
    delete[] * generateMatrix;
    delete generateMatrix;
    return findout;
}

void getRandomArray(int num, int array[])
{
	int i = 0;
	int temp[9] = { 1,2,3,4,5,6,7,8,9 };
    std::random_device rand;
	while (num > 0)
	{
		//srand(time(0));
		int index = rand() % (9-i);
		array[i++] = temp[index];
		for (int j = index; j < 9 - i; j++) 
		{
			temp[j] = temp[j + 1];
		}
		--num;
	}
}
bool judgeResult(int  puzzle[], bool flag[], bool unique)
{
	int rstr[81][3] = { 0 };
	int rstr_p = 0;
	bool result = true;
	for (int j = 0; j < 81; j++) {
		if (flag[j]) {
			rstr[rstr_p][0] = puzzle[j];
			rstr[rstr_p][1] = j / 9 + 1;
			rstr[rstr_p][2] = j % 9 + 1;
			++rstr_p;
		}
	}
	DLX.addRestrict(rstr_p, rstr);
	if (unique)
	{
		result = !DLX.find(2, false, NULL);
	}
	DLX.clearRestrict();
	return result;
}

bool choose(bool flag[], int puzzle[81], bool unique, int emptynum)
//这个函数是根据emptynum和unique，产生符合要求的数独游戏
{
	//不要求唯一解，那么设置好相应数量的flag就好
	if (!unique) {
		for (int i = 0; i < 9; i++) {
			int empty_line = i != 8 ? emptynum / (9 - i) : emptynum;
			emptynum -= empty_line;
			int *array = new int[empty_line];
			getRandomArray(empty_line, array);
			for (int j = 0; j < empty_line; j++) {
				int pick = i * 9 + array[j] - 1;
				flag[pick] = false;
			}
			delete []array;
		}
		return true;
	}
	/*
	这里简要说明一下当要求唯一解时的思路：
	1.一行一行挖空，每挖一行检测一下，如果挖完满足唯一解，那么在栈里存好①挖了哪些空②挖了几个空，然后继续挖下一行
	2.如果发现这一行挖完不满足唯一解了，那么恢复栈，重新随机挖这一行
	3.但是在实践中发现，前面几行挖的情况可能导致现在这一行不管怎么挖都无法满足唯一解，或者很难找到那种满足唯一解的情况，所以这里
		就不能光重挖这一行了，当这一行重挖次数大于kmax_try，那么就把这一行和上一行都恢复了，然后重新挖上一行
	4.但是这样还不行，现在是随机生成的数独终局，所以发现对于有的终局，很容易就能挖出一个唯一解的，但是对于有的终局，死活挖不出
		唯一解的，所以这里又设置了count_iterate，每一次恢复这一行和上一行的栈，那么这个count就加1，当这个count比kmax_iterate
		大时，就结束这次寻找，放弃这个终局（返回false），根据下一个随机的终局继续上述步骤。。

	还有就是这里kmax_iterate和kmax_try这两个参数的调整影响着生成数独的速度，我觉得这里可以研究一个最优的组合
	*/
	else {
		const int kmax_iterate = 5;
		const int kmax_try = 5;
		int *stack_array[9] = { 0 };
		int stack_emptynum[9] = { 0 };
		int line = 0;
		int count_iterate = 0;
		while (line < 9)
		{
			if (count_iterate > kmax_iterate)
				return false;
			int try_num = 0;
			do {
				try_num += 1;
				int empty_line = line != 8 ? emptynum / (9 - line) : emptynum;
				//std::cout << empty_line << std::endl;
				emptynum -= empty_line;
				int *array = new int[empty_line];
				if (stack_array[line])
					delete[]stack_array[line];
				getRandomArray(empty_line, array);
				//push
				stack_emptynum[line] = empty_line;
				stack_array[line] = array;
				for (int j = 0; j < empty_line; j++) {
					int pick = line * 9 + array[j] - 1;
					flag[pick] = false;
				}
				if (judgeResult(puzzle, flag, unique)) {
					break;
				}
				if(try_num>kmax_try) {
					count_iterate++;
					for (int j = 0; j < empty_line; j++) {
						int pick = line * 9 + array[j] - 1;
						flag[pick] = true;
					}
					emptynum += empty_line;
					--line;
					for (int j = 0; j < stack_emptynum[line ]; j++)
					{
						int pick = line * 9 + stack_array[line][j] - 1;
						flag[pick] = true;
					}
					emptynum += stack_emptynum[line];
					--line;
					break;
				}
				else
				{
					for (int j = 0; j < empty_line; j++) {
						int pick = line * 9 + array[j] - 1;
						flag[pick] = true;
					}
					emptynum += empty_line;
				}
			} while (1);
			++line;
			//print(flag, puzzle);
		}
		return true;
	}
}

/*
require: (number 1-10000) && (lower>=20 && upper<=55 &&	upper>=lower) && (result!=NULL)
*/
void generate(int number, int lower, int upper, bool unique, int  **result)
{
	//1.generate number sudo save to generateMatrix
	generateMatrix = new int*[number];
	matrixcount = 0;
	
	//2.for every matrix,generate sudo puzzle 
    std::random_device rand;
	for (int i = 0; i < number; i++)
	{
        DLX.find(1, true, ResDealing_toMatrix);
		int *puzzle = generateMatrix[i];
		bool flag[81];
		for (int i = 0; i < 81; i++)
		{
			flag[i] = true;
		}
		int emptynum = rand() % (upper - lower + 1) + lower;
		//如果这个终局一直挖不出来，那么就选择下一个终局来挖，这里如果失败的话，需要恢复i和matrixcount
		if (!choose(flag, puzzle, unique, emptynum))
		{
			--i;
			--matrixcount;
			continue;
		}
		//print(flag, puzzle);
		//std::cout << i << std::endl;
		for (int j = 0; j < 81; j++)
		{
            result[i][j] = flag[j] ? puzzle[j] : 0;
		}

	}
	for (int i = 0; i < number; i++) {
		delete[]generateMatrix[i];
	}
	delete[]generateMatrix;
}
