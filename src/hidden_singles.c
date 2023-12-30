#include "hidden_singles.h"
#include "sudoku.h"
#include <stdio.h>
#include <string.h>
#define MAX_CELLS (BOARD_SIZE * BOARD_SIZE)

int hidden_singles(SudokuBoard *p_board)
{
  int hiddenSinglesCount = 0;
  HiddenSingle hiddenSingles[MAX_CELLS];
  Cell **p_solved_cells = p_board->solved_cells;

  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      Cell *cell = &p_board->data[i][j];
      if (cell->num_candidates == 1)
      {
        int *value_to_set = get_candidates(cell);
        change_possible_value_for_cells(p_board, value_to_set[0], cell->row_index, cell->col_index, cell->box_index);
      };
    }
  }

  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      Cell *cell = &p_board->data[i][j];
      if (cell->num_candidates != 1)
      {
        int *p_hidden_singles = get_candidates(cell);
        find_hidden_single_cell(p_board, cell, p_hidden_singles, hiddenSingles, &hiddenSinglesCount);
      }
    }
  }

  for (int i = 0; i < hiddenSinglesCount; i++)
  {
    Cell *cell = hiddenSingles[i].p_cell;
    int value = hiddenSingles[i].value;
    // Set all candidates to 0
    memset(cell->candidates, 0, sizeof(cell->candidates));
    // Set the hidden single as the only candidate
    cell->candidates[value - 1] = value;
  }

  check_solved_cells(p_board, &p_solved_cells);
  return hiddenSinglesCount;
}

void find_hidden_single_cell(SudokuBoard *p_board, Cell *p_cell, int *p_hidden_singles, HiddenSingle *hiddenSingles, int *p_counter)
{
  int number_of_possible_candidates = p_cell->num_candidates;
  for (int i = 0; i < number_of_possible_candidates; i++)
  {
    bool is_unique_row = true;
    bool is_unique_col = true;
    bool is_unique_box = true;
    int value_to_check = p_hidden_singles[i];
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      if (p_cell->col_index != j && is_candidate(&p_board->data[p_cell->row_index][j], value_to_check))
      {
        is_unique_row = false;
      }

      if (p_cell->row_index != j && is_candidate(&p_board->data[j][p_cell->col_index], value_to_check))
      {
        is_unique_col = false;
      }

      if (!is_unique_col && !is_unique_row)
      {
        break;
      }
    }

    if (!is_unique_col && !is_unique_row)
    {
      int startRow = (p_cell->box_index / 3) * 3;
      int startCol = (p_cell->box_index % 3) * 3;
      for (int row = startRow; row < startRow + 3; row++)
      {
        for (int col = startCol; col < startCol + 3; col++)
        {
          if ((row != p_cell->row_index || col != p_cell->col_index) && is_candidate(&p_board->data[row][col], value_to_check))
          {
            is_unique_box = false;
            break;
          }
        }
      }
    }

    if (is_unique_row || is_unique_col || is_unique_box)
    {
      p_cell->num_candidates = 1;
      hiddenSingles[*p_counter].p_cell = p_cell;
      hiddenSingles[*p_counter].value = value_to_check;
      (*p_counter)++;
      break;
    }
    else
    {
      p_cell->value = 0;
    }
  }
};

void change_possible_value_for_cells(SudokuBoard *p_board, int value_to_set, int row_index, int col_index, int box_index)
{
  int index_candidate_need_reduced = value_to_set - 1;

  // row & column
  for (int i = 0; i < BOARD_SIZE; i++)
  {

    if (i != col_index && index_candidate_need_reduced >= 0)
    {
      Cell *cell_to_unset = &p_board->data[row_index][i];
      if (cell_to_unset->num_candidates > 1 && cell_to_unset->candidates[index_candidate_need_reduced] == 1)
      {
        unset_candidate(cell_to_unset, value_to_set);
      }
    }

    if (i != row_index && index_candidate_need_reduced >= 0)
    {
      Cell *cell_to_unset = &p_board->data[i][col_index];
      if (cell_to_unset->num_candidates > 1 && cell_to_unset->candidates[index_candidate_need_reduced] == 1)
      {
        unset_candidate(cell_to_unset, value_to_set);
      }
    }
  }

  // box
  int startRow = (box_index / 3) * 3;
  int startCol = (box_index % 3) * 3;
  for (int row = startRow; row < startRow + 3; row++)
  {
    for (int col = startCol; col < startCol + 3; col++)
    {
      if (row != row_index && col != col_index && index_candidate_need_reduced >= 0)
      {
        Cell *cell_to_unset = &p_board->data[row][col];
        if (cell_to_unset->num_candidates > 1 && cell_to_unset->candidates[index_candidate_need_reduced] == 1)
        {
          unset_candidate(cell_to_unset, value_to_set);
        }
      }
    }
  }
}