#ifndef DLX_H_
#define DLX_H_

#include "cross_link.h"
#pragma once
class dlx
{
/**************************
*首先调用addRestrict函数添加对矩阵的限制，即填入已知数字。
*然后调用find函数寻找N个在当前限制条件下的数独解，解的数量能达到N时返回true，否则返回false。
*最后调用clearRestrict函数将当前限制条件清空。
**************************/
public:
    typedef cross_link::Cross Cross;
    dlx();
    ~dlx();
    void addRestrict(int n, int(*rstr)[3]);
    // n表示添加的限制的数量。
    // rstr二维数组中，rstr[i][0]表示第i个数的值，rstr[i][1]表示第i个数的行号（1-9），rstr[i][2]表示第i个数的列号（1-9）。
    void clearRestrict() { _restrict_point = 0; }
    // 清空限制条件
    bool find(int N, bool random, void ResDealing(const int *Res));
    // N表示想要得到的解的个数。
    // 布尔值random表示是否加入随机性。加入随机性会极小地影响性能。
    // ResDealing函数在每找到一个解时都会被调用，传入的是dlx十字链表的行号数组，可转换为数独矩阵元素，共81个数。当传入的参数为NULL时表示不做操作。
    // 转换方式：对于行号i，对应数独行号(0-8)为(i - 1) / 9 / 9，对应数独列号(0-8)为(i - 1) / 9 % 9，对应数值为(i - 1) % 9 + 1。
    // 当解的数量能够达到N时返回true，否则返回false。

private:
    cross_link *A;
    const int rownum = 81 * 9;
    const int colnum = 81 * 4;
    Cross  head;
    Cross *rows;
    Cross *cols;

    int result[81] = { 0 };
    int stack[81 * 9 + 81 * 4] = { 0 };

    int _restrict_point = 0;

    void _deleteRestrict(int &stack_top, int &result_pos);
    bool _find(int stack_top, int result_pos, int N, int &n, void ResDealing(const int *Res));
};
#endif // !DLX_H_