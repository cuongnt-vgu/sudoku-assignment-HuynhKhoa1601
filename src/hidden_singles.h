#pragma once

#include "sudoku.h"

typedef struct HiddenSingle_impl
{
    Cell *p_cell;
    int value;
} HiddenSingle;

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values);

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles,
                        int *p_counter);

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board);
void change_possible_value_for_cells(SudokuBoard *p_board,int value_to_set,  int row_index, int col_index, int box_index);
void find_hidden_single_cell(SudokuBoard *p_board, Cell *p_cell, int *p_hidden_singles, HiddenSingle *hiddenSingles, int *p_counter);

