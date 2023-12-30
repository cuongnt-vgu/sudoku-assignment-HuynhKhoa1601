#include "hidden_pairs.h"
#include <stdio.h>
#include <stddef.h>

int hidden_pairs(SudokuBoard *p_board)
{
    int pair_found = 0;

    // Search for hidden pairs in all rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        pair_found += find_hidden_pairs_in_unit(p_board->p_rows[i]);
        pair_found += find_hidden_pairs_in_unit(p_board->p_cols[i]);
        pair_found += find_hidden_pairs_in_unit(p_board->p_boxes[i]);
    }
    return pair_found;
}

// Helper function to find hidden pairs in a unit (row, col, or box)
int find_hidden_pairs_in_unit(Cell **unit)
{
    for (int num1 = 1; num1 <= BOARD_SIZE; ++num1)
    {
        for (int num2 = num1 + 1; num2 <= BOARD_SIZE; ++num2)
        {
            Cell *pair_cells[2] = {NULL, NULL};
            int pair_count = 0;

            // Check for the occurrence of num1 and num2 in the candidates of the unit
            for (int i = 0; i < BOARD_SIZE; ++i)
            {
                if (is_candidate(unit[i], num1) || is_candidate(unit[i], num2))
                {
                    if (pair_count < 2)
                    {
                        pair_cells[pair_count] = unit[i];
                        pair_count++;
                    }
                    else
                    {
                        // More than two cells with num1 and num2 as candidates, not a hidden pair
                        pair_count++;
                        break;
                    }
                }
            }

            // If exactly two cells can have num1 and num2, it's a hidden pair
            if (pair_count == 2)
            {
                // Verify that both numbers are candidates in both cells
                if (is_candidate(pair_cells[0], num1) && is_candidate(pair_cells[0], num2) &&
                    is_candidate(pair_cells[1], num1) && is_candidate(pair_cells[1], num2))
                {
                    // Remove other candidates from the two cells
                    for (int num = 1; num <= BOARD_SIZE; ++num)
                    {
                        if (num != num1 && num != num2)
                        {
                            unset_candidate(pair_cells[0], num);
                            unset_candidate(pair_cells[1], num);
                        }
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}
