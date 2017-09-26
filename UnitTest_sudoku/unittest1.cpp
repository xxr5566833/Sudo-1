#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku/cross_link.h"
#include "../sudoku/dlx.h"
#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_sudoku
{
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestCrossLink_insert)
		{
			cross_link A(10, 10);
			A.insert(3, 4);
			Assert::AreEqual(1, A.rows[4]->count);
			Assert::AreEqual(1, A.cols[5]->count);
			A.insert(4, 3);
			A.insert(4, 4);
			A.insert(3, 3);
			int count = 0;
			for (int i = 1; i <= 10; i++) {
				for (cross_link::Cross p = A.rows[i]->right; p; p = p->right)
					count++;
			}
			Assert::AreEqual(count, 4);
		}

		TEST_METHOD(TestCrossLink_delAndRecover)
		{
			cross_link A(10, 10);
			A.insert(3, 4);
			A.insert(4, 3);
			A.insert(4, 4);
			A.insert(3, 3);
			{
				int count = 0;
				for (cross_link::Cross p = A.head->right; p; p = p->right)
					for (cross_link::Cross q = p->down; q; q = q->down)
						count++;
				Assert::AreEqual(4, count);
			}
			A.delcol(4);
			{
				int count = 0;
				for (cross_link::Cross p = A.head->right; p; p = p->right)
					for (cross_link::Cross q = p->down; q; q = q->down)
						count++;
				Assert::AreEqual(2, count);
			}
			A.recovercol(4);
			{
				int count = 0;
				for (cross_link::Cross p = A.head->right; p; p = p->right)
					for (cross_link::Cross q = p->down; q; q = q->down)
						count++;
				Assert::AreEqual(4, count);
			}
		}

		bool eval(int a[9][9]) {
			for (int l = 0; l < 9; l++) {
				for (int r = 0; r < 9; r++) {
					int key = a[l][r];
					for (int i = 0; i < 9; i++)
						if (a[l][i] == key&&i != r) return false;
					for (int j = 0; j < 9; j++)
						if (a[j][r] == key&&j != l) return false;
					int x = (l / 3) * 3, y = (r / 3) * 3;
					for (int i = x; i < x + 3; i++)
						for (int j = y; j < y + 3; j++)
							if (a[i][j] == key && (i != l&&r != j)) return false;
					return true;

				}
			}
		}

		TEST_METHOD(TestDlx_find_one)
		{
			dlx DLX;
			int Res[9][9];
			int rstr[81][3];
			int rstr_p = 0;
			/*
			int Q[9][9] = {
			{ 0, 0, 0, 0, 0, 0, 0, 3, 9 },
			{ 0, 0, 0, 0, 0, 1, 0, 0, 5 },
			{ 0, 0, 3, 0, 5, 0, 8, 0, 0 },
			{ 0, 0, 8, 0, 9, 0, 0, 0, 6 },
			{ 0, 7, 0, 0, 0, 2, 0, 0, 0 },
			{ 1, 0, 0, 4, 0, 0, 0, 0, 0 },
			{ 0, 0, 9, 0, 8, 0, 0, 5, 0 },
			{ 0, 2, 0, 0, 0, 0, 6, 0, 0 },
			{ 4, 0, 0, 7, 0, 0, 0, 0, 0 },
			};
			int Q[9][9] = {
			{0, 0, 0, 0, 0, 6, 0, 0, 0},
			{4, 5, 0, 0, 0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0, 2, 5, 0, 0},
			{0, 7, 0, 0, 1, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 6},
			{8, 0, 0, 0, 0, 0, 0, 7, 2},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 4, 8, 9, 0, 0},
			{0, 1, 2, 0, 0, 0, 0, 0, 0},
			};
			int Q[9][9] = {
			{ 1, 2, 0, 4, 0, 0, 3, 0, 0 },
			{ 3, 0, 0, 0, 1, 0, 0, 5, 0 },
			{ 0, 0, 6, 0, 0, 0, 1, 0, 0 },
			{ 7, 0, 0, 0, 9, 0, 0, 0, 0 },
			{ 0, 4, 0, 6, 0, 3, 0, 0, 0 },
			{ 0, 0, 3, 0, 0, 2, 0, 0, 0 },
			{ 5, 0, 0, 0, 8, 0, 7, 0, 0 },
			{ 0, 0, 7, 0, 0, 0, 0, 0, 5 },
			{ 0, 0, 0, 0, 0, 0, 0, 9, 8 },
			};
			int Q[9][9] = {
			{ 0, 0, 1, 2, 0, 0, 3, 0, 0 },
			{ 2, 0, 0, 4, 0, 0, 0, 0, 0 },
			{ 0, 0, 5, 6, 1, 0, 0, 7, 0 },
			{ 6, 8, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 2, 0, 4, 0, 9, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 5, 3 },
			{ 0, 3, 0, 0, 5, 2, 1, 0, 0 },
			{ 0, 0, 0, 0, 0, 4, 0, 0, 8 },
			{ 0, 0, 9, 0, 0, 1, 5, 0, 0 },
			};
			int Q[9][9] = {
			{ 0, 0, 1, 2, 0, 0, 3, 0, 0 },
			{ 2, 0, 0, 4, 0, 0, 0, 0, 0 },
			{ 0, 0, 5, 6, 1, 0, 0, 7, 0 },
			{ 6, 8, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 5, 2, 0, 4, 0, 9, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 5, 3 },
			{ 0, 3, 0, 0, 5, 2, 1, 0, 0 },
			{ 0, 0, 0, 0, 0, 4, 0, 0, 8 },
			{ 0, 0, 9, 0, 0, 1, 5, 0, 0 },
			};
			int Q[9][9] = {
			{ 8, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 3, 6, 0, 0, 0, 0, 0 },
			{ 0, 7, 0, 0, 9, 0, 2, 0, 0 },
			{ 0, 5, 0, 0, 0, 7, 0, 0, 0 },
			{ 0, 0, 0, 0, 4, 5, 7, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0, 3, 0 },
			{ 0, 0, 1, 0, 0, 0, 0, 6, 8 },
			{ 0, 0, 8, 5, 0, 0, 0, 1, 0 },
			{ 0, 9, 0, 0, 0, 0, 4, 0, 0 },
			};
			int Q[9][9] = {
			{ 9, 8, 0, 7, 0, 0, 6, 0, 0 },
			{ 5, 0, 0, 0, 9, 0, 0, 7, 0 },
			{ 0, 0, 7, 0, 0, 4, 0, 0, 0 },
			{ 3, 0, 0, 0, 0, 6, 0, 0, 0 },
			{ 0, 0, 8, 5, 0, 0, 0, 6, 0 },
			{ 0, 0, 0, 0, 0, 0, 3, 0, 2 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 5, 4, 0, 0, 0, 8, 0 },
			{ 0, 0, 0, 0, 2, 1, 9, 0, 0 },
			};
			int Q[9][9] = {
			{ 0, 0, 5, 0, 0, 0, 0, 0, 7 },
			{ 0, 0, 2, 5, 3, 6, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 4, 0, 0, 0, 0, 7, 0 },
			{ 8, 0, 3, 0, 2, 0, 0, 0, 0 },
			{ 0, 0, 0, 4, 0, 0, 0, 1, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 8 },
			{ 0, 0, 9, 0, 0, 0, 0, 0, 0 },
			};
			*/
			int Q[9][9] = {
				{ 0, 0, 0, 0, 0, 6, 0, 0, 0 },
				{ 4, 5, 0, 0, 0, 0, 1, 0, 0 },
				{ 0, 0, 0, 0, 0, 2, 5, 0, 0 },
				{ 0, 7, 0, 0, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 6 },
				{ 8, 0, 0, 0, 0, 0, 0, 7, 2 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 4, 8, 9, 0, 0 },
				{ 0, 1, 2, 0, 0, 0, 0, 0, 0 },
			};
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
					if (Q[i][j]) {
						rstr[rstr_p][0] = Q[i][j];
						rstr[rstr_p][1] = i + 1;
						rstr[rstr_p][2] = j + 1;
						rstr_p++;
					}
			DLX.addRestrict(rstr_p, rstr);
			DLX.find_one(Res);
			Assert::AreEqual(eval(Res), true);
		}

		TEST_METHOD(TestDlx_find_many)
		{
			dlx DLX;
			int rstr[1][3] = { { 3, 1, 1 } };
			DLX.addRestrict(1, rstr);
			DLX.find_many(100);
			int line_count = 0;
			int matrix[9][9];
			char s[19];
			std::ifstream fin("sudoku.txt");
			while (fin.getline(s, 19)) {
				for (int i = 0; i < 18; i += 2)
					matrix[line_count][i / 2] = s[i] - '0';
				if (++line_count == 9) {
					line_count = 0;
					Assert::AreEqual(matrix[0][0], 3);
					Assert::AreEqual(eval(matrix), true);
					fin.getline(s, 19);
				}
			}
		}

	};
}