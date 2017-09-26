#include<fstream>

#pragma once
class dlx
{
public:
	typedef cross_link::Cross Cross;
	dlx();
	~dlx();
	void addRestrict(int n, int(*rstr)[3]);
	void clearRestrict() { _restrict_point = 0; }
	bool find_one(int result[9][9]);
	bool find_many(int N);

private:
	cross_link *A;
	const int rownum = 81 * 9;
	const int colnum = 81 * 4;
	Cross  head;
	Cross *rows;
	Cross *cols;

	int result[81] = { 0 };
	int stack[81 * 9 + 81 * 4] = { 0 };

	char str_buffer[1000 * (19 * 9 + 1) + 1];
	int str_p = 0;

	int _restrict_point = 0;

	void _deleteRestrict(int &stack_top, int &result_pos);
	bool _find_one(int stack_top, int result_pos);
	bool _find_many(int stack_top, int result_pos, int N, int &n, std::ofstream &fout);
};