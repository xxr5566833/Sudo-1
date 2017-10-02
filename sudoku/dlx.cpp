#include "stdafx.h"
#include "cross_link.h"
#include "dlx.h"
#include <iostream>
#include <fstream>


dlx::dlx()
{
    A = new cross_link(rownum, colnum);
    rows = A->rows;
    cols = A->cols;
    head = A->head;
    for (int i = 0; i < rownum; i++) {
        int val = i % 9 + 1;
        int pos = i / 9;
        int row = pos / 9;
        int col = pos % 9;
        int block = (row / 3) * 3 + (col / 3);
        A->insert(i, pos);
        A->insert(i, 81 + row * 9 + val - 1);
        A->insert(i, 81 * 2 + col * 9 + val - 1);
        A->insert(i, 81 * 3 + block * 9 + val - 1);
    }
}

dlx::~dlx()
{
    delete A;
}

void dlx::addRestrict(int n, int(*rstr)[3])
{
    int val, pos_row, pos_col;
    for (int i = 0; i < n; i++) {
        val = rstr[i][0];
        pos_row = rstr[i][1];
        pos_col = rstr[i][2];
        result[_restrict_point++] = ((pos_row - 1) * 9 + pos_col - 1) * 9 + val;
    }
}

void dlx::_deleteRestrict(int & stack_top, int & result_pos)
{
    for (int i = 0; i < result_pos; i++) {
        for (Cross p = rows[result[i]]->right; p != NULL; p = p->right) {
            for (Cross q = cols[p->col]->down; q != NULL; q = q->down) {
                A->delrow(q->row);
                stack[stack_top++] = q->row;
            }
            A->delcol(p->col);
            stack[stack_top++] = -p->col;
        }
    }
}

bool dlx::find(int N, void ResDealing(const int *Res))
{
    int stack_top = 0;
    int result_pos = _restrict_point;
    _deleteRestrict(stack_top, result_pos);
    int n = 0;
    return _find(stack_top, result_pos, N, n, ResDealing);
}

bool dlx::_find(int stack_top, int result_pos, int N, int & n, void ResDealing(const int *Res)) {
    if (!head->right) {
        if (ResDealing) {
            ResDealing(result);
        }
        if (++n == N) {
            for (int i = stack_top - 1; i >= 0; --i) {
                if (stack[i] > 0)
                    A->recoverrow(stack[i]);
                else
                    A->recovercol(-stack[i]);
            }
            return true;
        }
        else
            return false;
    }
    int min_col_count = 100;
    int min_col_index = -1;
    for (Cross p = head->right; p != NULL; p = p->right) {
        if (min_col_count > p->count) {
            min_col_count = p->count;
            min_col_index = p->col;
        }
    }
    for (Cross a = cols[min_col_index]->down; a != NULL; a = a->down) {
        result[result_pos++] = a->row;
        int new_stack_top = stack_top;
        for (Cross b = rows[a->row]->right; b != NULL; b = b->right) {
            for (Cross c = cols[b->col]->down; c != NULL; c = c->down) {
                A->delrow(c->row);
                stack[new_stack_top++] = c->row;
            }
            A->delcol(b->col);
            stack[new_stack_top++] = -b->col;
        }
        if (_find(new_stack_top, result_pos, N, n, ResDealing))
            return true;
        for (int i = new_stack_top - 1; i >= stack_top; i--) {
            if (stack[i] > 0)
                A->recoverrow(stack[i]);
            else
                A->recovercol(-stack[i]);
        }
        result_pos--;
    }
    return false;

}
