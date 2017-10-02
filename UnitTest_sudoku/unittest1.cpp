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
    };
}