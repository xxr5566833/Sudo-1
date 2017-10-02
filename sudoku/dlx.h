#pragma once
class dlx
{
public:
    typedef cross_link::Cross Cross;
    dlx();
    ~dlx();
    void addRestrict(int n, int(*rstr)[3]);
    void clearRestrict() { _restrict_point = 0; }
    bool find(int N, void ResDealing(const int *Res));
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