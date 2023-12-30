#pragma once

#include "sudoku.h"

int hidden_pairs(SudokuBoard *p_board);

int find_hidden_pairs_in_unit(Cell **unit);
