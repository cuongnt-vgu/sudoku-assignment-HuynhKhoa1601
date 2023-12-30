#include "naked_pairs.h"
#include "hidden_singles.h"
#include <stdio.h>

int naked_pairs(SudokuBoard *p_board)
{
    int numberSolvedCells = 0;

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

    CellPosition previous_naked_pairs_cell1_position;
    CellPosition previous_naked_pairs_cell2_position;
    // Iterate over all units (rows, columns, boxes)
    for (int unit = 0; unit < BOARD_SIZE; unit++)
    {
        // Check each type of unit: rows, columns, and boxes
        Cell **units[3] = {p_board->p_rows[unit], p_board->p_cols[unit], p_board->p_boxes[unit]};

        for (int type = 0; type < 3; type++)
        {
            // Look for naked pairs in the current unit
            for (int first = 0; first < BOARD_SIZE - 1; first++)
            {
                Cell *cell1 = units[type][first];
                if (cell1->num_candidates == 2)
                {
                    bool check_common = true;
                    for (int second = first + 1; second < BOARD_SIZE; second++)
                    {
                        Cell *cell2 = units[type][second];
                        if (cell2->num_candidates == 2)
                        {
                            for (int index = 0; index < BOARD_SIZE; index++)
                            {
                                if (cell1->candidates[index] != cell2->candidates[index])
                                {
                                    check_common = false;
                                    break;
                                }
                            }
         
                            // Check if cell1 and cell2 form a naked pair
                            if (check_common)
                            {
                                if (cell1->row_index != previous_naked_pairs_cell1_position.row && cell1->col_index != previous_naked_pairs_cell1_position.column && cell2->row_index != previous_naked_pairs_cell2_position.row && cell2->col_index != previous_naked_pairs_cell2_position.column)
                                {
                                    numberSolvedCells++;
                                }

                                if (cell1->box_index == cell2->box_index)
                                {
                                    previous_naked_pairs_cell1_position.row = cell1->row_index;
                                    previous_naked_pairs_cell1_position.column = cell1->col_index;
                                    previous_naked_pairs_cell2_position.row = cell2->row_index;
                                    previous_naked_pairs_cell2_position.column = cell2->row_index;
                                }
                                // Found a naked pair, remove these candidates from other cells in the unit
                                for (int other = 0; other < BOARD_SIZE; other++)
                                {
                                    if (other != first && other != second)
                                    {
                                        Cell *otherCell = units[type][other];
                                        for (int candidate = 0; candidate < BOARD_SIZE; candidate++)
                                        {
                                            if (cell1->candidates[candidate] == 1)
                                            {
                                                int value = candidate + 1;
                                                if (is_candidate(otherCell, value))
                                                {
                                                    unset_candidate(otherCell, value);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return numberSolvedCells;
}
