#pragma once

#include "sudoku.h"

typedef struct CellPosition_impl
{
    int row;
    int column;
} CellPosition;


int naked_pairs(SudokuBoard *p_board);