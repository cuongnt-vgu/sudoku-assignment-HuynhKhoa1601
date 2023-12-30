#include "naked_triples.h"

int naked_triples(SudokuBoard *p_board) {
    int numberNakedTriples = 0;

    // Iterate over all units (rows, columns, boxes)
    for (int unit = 0; unit < BOARD_SIZE; unit++) {
        // Check each type of unit: rows, columns, and boxes
        Cell **units[3] = {p_board->p_rows[unit], p_board->p_cols[unit], p_board->p_boxes[unit]};
        
        for (int type = 0; type < 3; type++) {
            // Look for naked triples in the current unit
            for (int first = 0; first < BOARD_SIZE - 2; first++) {
                for (int second = first + 1; second < BOARD_SIZE - 1; second++) {
                    for (int third = second + 1; third < BOARD_SIZE; third++) {
                        Cell *cell1 = units[type][first];
                        Cell *cell2 = units[type][second];
                        Cell *cell3 = units[type][third];

                        // Combine candidates from cell1, cell2, and cell3
                        int combinedCandidates[BOARD_SIZE] = {0};
                        int numCombined = 0;
                        for (int i = 0; i < BOARD_SIZE; i++) {
                            if (cell1->candidates[i] || cell2->candidates[i] || cell3->candidates[i]) {
                                combinedCandidates[i] = 1;
                                numCombined += (cell1->candidates[i] + cell2->candidates[i] + cell3->candidates[i]) > 0 ? 1 : 0;
                            }
                        }

                        // Check if the combined candidates form a naked triple
                        if (numCombined == 3) {
                            numberNakedTriples++; // Increment the number of naked triples found
                            
                            // Found a naked triple, remove these candidates from other cells in the unit
                            for (int other = 0; other < BOARD_SIZE; other++) {
                                if (other != first && other != second && other != third) {
                                    Cell *otherCell = units[type][other];
                                    for (int candidateIndex = 0; candidateIndex < BOARD_SIZE; candidateIndex++) {
                                        if (combinedCandidates[candidateIndex] == 1 && is_candidate(otherCell, candidateIndex + 1)) {
                                            unset_candidate(otherCell, candidateIndex + 1);
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

    return numberNakedTriples;
}
