#ifndef PUZZLE_GENERATOR_H_
#define PUZZLE_GENERATOR_H_
#pragma once
void generate(int number, int mode, int result[][81]);
void generate(int number, int lower, int upper, bool unique, int result[][81]);
bool solve(int *puzzle, int *solution);

#endif // !PUZZLE_GENERATOR_H_