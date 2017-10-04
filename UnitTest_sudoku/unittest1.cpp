#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku/cross_link.h"
#include "../sudoku/dlx.h"
#include "../sudoku/puzzle_generator.h"
#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
extern int **generateMatrix;
extern  dlx DLX;
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
		TEST_METHOD(TestGenerate5)
		{
			int uppers[10] = { 20,55,55,55,55,55,20,20,46,50 };
			int lowers[10] = { 20,55,20,20,46,46,20,20,40,40 };
			for(int j=0;j<10;j++){
				bool unique = j%2==0;
				int upper = uppers[j];
				int lower = lowers[j];
				int number = 3000;
				int result[3000][81] = { 0 };

			
				generate(number,lower,upper,unique,result);
				for (int i = 0; i < number; i++) {
					int rstr[81][3] = { 0 };
					int rstr_p = 0;
					for (int j = 0; j < 81; j++) {
						if (result[i][j])
						{
							rstr[rstr_p][0] = result[i][j];
							//bug 2 下标都写错为了0
							rstr[rstr_p][1] = j / 9 + 1;
							rstr[rstr_p][2] = j % 9 + 1;
							++rstr_p;
						}
					}
					DLX.addRestrict(rstr_p, rstr);
					if (!unique) {
						Assert::AreEqual(DLX.find(1, NULL), true);
					}
					else
					{
						Assert::AreEqual(DLX.find(2, NULL), false);
					}
					DLX.clearRestrict();
				}
			}
		}
    };
}