#include "stdafx.h"
#include "cross_link.h"
#include <iostream>

void cross_link::build() {
    head->left = head->up = NULL;
    head->count = 0;
    head->ishead = true;
    rows[0] = head;
    cols[0] = head;
    for (int i = 1; i < rownum; i++) {
        rows[i] = new cross_node;
        rows[i]->count = 0;
        rows[i]->row = i;
        rows[i]->col = 0;
        rows[i]->ishead = true;
        rows[i]->left = rows[i]->right = rows[i]->down = NULL;
        rows[i]->up = rows[i - 1];
        rows[i - 1]->down = rows[i];
    }
    for (int i = 1; i < colnum; i++) {
        cols[i] = new cross_node;
        cols[i]->count = 0;
        cols[i]->col = i;
        cols[i]->row = 0;
        cols[i]->ishead = true;
        cols[i]->up = cols[i]->down = cols[i]->right = NULL;
        cols[i]->left = cols[i - 1];
        cols[i - 1]->right = cols[i];
    }
}

cross_link::~cross_link() {
    for (int i = 0; i < rownum; i++) {
        Cross p = rows[i];
        Cross a;
        while (p) {
            a = p;
            p = p->right;
            delete a;
        }
    }
    delete[] rows;
    delete[] cols;
}

void cross_link::insert(int i, int j) {
    i++, j++;
    Cross r, c;
    rows[i]->count++;
    cols[j]->count++;
    for (r = rows[i]; r->right != NULL && r->right->col < j; r = r->right);
    for (c = cols[j]; c->down != NULL && c->down->row < i; c = c->down);
    Cross p = new cross_node;
    p->row = i;
    p->col = j;
    p->count = 0;
    p->ishead = false;
    if (r->right)
        r->right->left = p;
    p->left = r;
    p->right = r->right;
    r->right = p;

    if (c->down)
        c->down->up = p;
    p->up = c;
    p->down = c->down;
    c->down = p;
}

void cross_link::delrow(int r) {
    for (Cross i = rows[r]; i != NULL; i = i->right) {
        cols[i->col]->count--;
        if (i->up)
            i->up->down = i->down;
        if (i->down)
            i->down->up = i->up;
    }
}

void cross_link::recoverrow(int r) {
    for (Cross i = rows[r]; i != NULL; i = i->right) {
        cols[i->col]->count++;
        if (i->up)
            i->up->down = i;
        if (i->down)
            i->down->up = i;
    }
}

void cross_link::delcol(int c) {
    for (Cross i = cols[c]; i != NULL; i = i->down) {
        rows[i->row]->count--;
        if (i->left)
            i->left->right = i->right;
        if (i->right)
            i->right->left = i->left;
    }
}

void cross_link::recovercol(int c) {
    for (Cross i = cols[c]; i != NULL; i = i->down) {
        rows[i->row]->count++;
        if (i->left)
            i->left->right = i;
        if (i->right)
            i->right->left = i;
    }
}