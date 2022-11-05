/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Hassan Aamer 1235857
   Dated:     12/09/2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3 /* sudoku dimension, size of each inner square */
#define NDIG (NDIM * NDIM)
/* total number of values in each row */
#define NGRP 3 /* number of sets each cell is a member of */
#define NSET (NGRP * NDIG)
/* total number of sets in the sudoku */
#define NCLL (NDIG * NDIG)
/* total number of cells in the sudoku */

#define ERROR (-1) /* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM * NDIM] = {
    /* the first group of nine sets describe the sudoku's rows */
    {0, 1, 2, 3, 4, 5, 6, 7, 8},
    {9, 10, 11, 12, 13, 14, 15, 16, 17},
    {18, 19, 20, 21, 22, 23, 24, 25, 26},
    {27, 28, 29, 30, 31, 32, 33, 34, 35},
    {36, 37, 38, 39, 40, 41, 42, 43, 44},
    {45, 46, 47, 48, 49, 50, 51, 52, 53},
    {54, 55, 56, 57, 58, 59, 60, 61, 62},
    {63, 64, 65, 66, 67, 68, 69, 70, 71},
    {72, 73, 74, 75, 76, 77, 78, 79, 80},
    /* the second group of nine sets describes the sudoku's columns */
    {0, 9, 18, 27, 36, 45, 54, 63, 72},
    {1, 10, 19, 28, 37, 46, 55, 64, 73},
    {2, 11, 20, 29, 38, 47, 56, 65, 74},
    {3, 12, 21, 30, 39, 48, 57, 66, 75},
    {4, 13, 22, 31, 40, 49, 58, 67, 76},
    {5, 14, 23, 32, 41, 50, 59, 68, 77},
    {6, 15, 24, 33, 42, 51, 60, 69, 78},
    {7, 16, 25, 34, 43, 52, 61, 70, 79},
    {8, 17, 26, 35, 44, 53, 62, 71, 80},
    /* the last group of nine sets describes the inner squares */
    {0, 1, 2, 9, 10, 11, 18, 19, 20},
    {3, 4, 5, 12, 13, 14, 21, 22, 23},
    {6, 7, 8, 15, 16, 17, 24, 25, 26},
    {27, 28, 29, 36, 37, 38, 45, 46, 47},
    {30, 31, 32, 39, 40, 41, 48, 49, 50},
    {33, 34, 35, 42, 43, 44, 51, 52, 53},
    {54, 55, 56, 63, 64, 65, 72, 73, 74},
    {57, 58, 59, 66, 67, 68, 75, 76, 77},
    {60, 61, 62, 69, 70, 71, 78, 79, 80},
};

/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function 
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void fill_c2s()
{
    int s = 0, d = 0, c;
    for (; s < NSET; s++)
    {
        /* record the first set number each cell is part of */
        for (c = 0; c < NDIM * NDIM; c++)
        {
            c2s[s2c[s][c]][d] = s;
        }
        if ((s + 1) % (NGRP * NDIM) == 0)
        {
            d++;
        }
    }
    if (0)
    {
        /* this code available here if you want to see the array
	   cs2[][] that gets created, just change that 0 two lines back
	   to a 1 and recompile */
        for (c = 0; c < NCLL; c++)
        {
            printf("cell %2d: sets ", c);
            for (s = 0; s < NGRP; s++)
            {
                printf("%3d", c2s[c][s]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return;
}

/* find the row number a cell is in, counting from 1
*/
int rownum(int c)
{
    return 1 + (c / (NDIM * NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int colnum(int c)
{
    return 1 + (c % (NDIM * NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int sqrnum(int c)
{
    return 1 + 3 * (c / NSET) + (c / NDIM) % NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/

#define SYMBOL_WIDTH 2
/* the space each symbol (number or otherwise) takes on the output console */
#define LINE_LENGTH (((NDIG + NDIM - 1) * SYMBOL_WIDTH) - 1)
/* the total length of each line of the board on the output console
-- minus 1 due to left justification*/
#define NRCS 9 /* Number of rows/col/sqrs*/

/*----Booleans for whether a cell should be left_justified when printing----*/
#define LEFT_JUST_YES 1
#define LEFT_JUST_NO 0
/* -------------------------------------------------- */

void read_board(int board[]);
void print_board(int board[]);
void print_cell(int board[], int i, int left_justified_flag);
void print_dashed_line(int length, int sub_divisions);
int check_errors(int board[]);
void print_violation(int set, int value, int count);
int strategy1(int board[]);
int find_valid_label(int board[], int i);
void output_change(int i, int label);
int count_unknown_cells(int board[]);
void update_board(int temp_board[], int board[]);
void reset_board(int temp_board[]);
/****************************************************************/
/****************************************************************/

/* main program controls all the action
*/
int main(int argc, char *argv[])
{
    int board[NCLL];
    int solved_flag = 0;
    fill_c2s();
    read_board(board);
    print_board(board);
    if (!(check_errors(board)))
    {
        solved_flag = strategy1(board);
        print_board(board);
    }

    if (solved_flag)
    {
        printf("\n\nta daa!!!");
    }
    /* all done, so pack up bat and ball and head home */
    /* ALGORITHMS ARE FUN!*/
    return 0;
}

/* Reads values into the array*/

void read_board(int board[])
{
    int i = 0;
    while (scanf("%d", &board[i++]) != EOF)
    {
        /*Read 81 values*/
    }
}

/* Counts unknown cells in the array*/
int count_unknown_cells(int board[])
{
    int count = 0;
    for (int i = 0; i < NCLL; i++)
    {
        if (board[i])
        {
            continue;
        }
        count++;
    }
    return count;
}

/* Prints values of board on the console*/

void print_board(int board[])
{
    printf("\n");
    /*Print the first cell out (required for the if statements to behave)*/
    print_cell(board, 0, LEFT_JUST_YES);
    /*For each subsequent cell on the board*/
    for (int i = 1; i < NCLL; i++)
    {
        /* Add a dashed line after every third row*/
        if (i % (NDIM * NDIG) == 0)
        {
            print_dashed_line(LINE_LENGTH, NDIM);
            print_cell(board, i, LEFT_JUST_YES);
            continue;
        }
        /* Else just add a new-line after every row*/
        else if (i % NDIG == 0)
        {
            printf("\n");
            print_cell(board, i, LEFT_JUST_YES);
            continue;
        }

        /* If it's the last cell in a row*/
        if (i % NDIG == NDIG - 1)
        {
            /* Print the cell that is not left justified*/
            print_cell(board, i, LEFT_JUST_NO);
            continue;
        }

        /* Add a | char after each small square*/
        if (i % NDIM == 0)
        {
            /* Always be Left justified*/
            printf("| ");
        }
        print_cell(board, i, LEFT_JUST_YES);
    }
    printf("\n\n%2d cells are unknown", count_unknown_cells(board));
}

/* Prints out a cell at a particular index*/
void print_cell(int board[], int i, int left_justified_flag)
{
    if (left_justified_flag)
    {
        /* The cell is left justified*/
        if (board[i] == 0)
        {
            printf(". ");
        }
        else
        {
            printf("%-2d", board[i]);
        }
    }
    else
    {
        /*The cell is not left justified*/
        if (board[i] == 0)
        {
            printf(".");
        }
        else
        {
            printf("%d", board[i]);
        }
    }
    return;
}

/* Prints out a dashed line with a length [length]
and with a + after [sub_divisions] cells*/
void print_dashed_line(int length, int sub_divisions)
{
    printf("\n");
    for (int j = 1; j <= length; j++)
    {
        if ((j == ((sub_divisions * SYMBOL_WIDTH) + 1)) ||
            /*If sub_division (3) cells have been printed,
            then print a '+' next*/
            (j == (((2 * (sub_divisions)) + 1) * SYMBOL_WIDTH) + 1))
        /*OR if 2 sub_division (6) cells have been printed,
        then print a '+' accounting for justification and other symbols*/
        {
            printf("+");
        }
        else
        {
            printf("-");
        }
    }
    printf("\n");
}

/* Implementation of Grid violation error checking*/
/* Returns 1 if errors found, returns 0 if OK*/
int check_errors(int board[])
{
    int c1, c2;
    int v1, v2;
    int set_vio_flag, errors_found_flag = 0;
    int count = 0, total_vio = 0, set_vio = 0;
    printf("\n");
    for (int s = 0; s < NSET; s++)
    {
        /* For each set in the board*/
        set_vio_flag = 0;
        for (int i = 0; i < NDIG; i++)
        {
            /* For each cell in the set*/
            count = 0;
            c1 = s2c[s][i];
            v1 = board[c1];
            if (v1 == 0)
            {
                /* Do not consider empty spaces as duplicates*/
                continue;
            }
            for (int j = 0; j < NDIG; j++)
            {
                /* Check all other cells in the set for duplicates*/
                c2 = s2c[s][j];
                v2 = board[c2];
                if ((i > j) && (v1 == v2))
                {
                    /* Move on to finding duplicates of the next cell if
                    we have already checked for duplicates of this value*/
                    break;
                }

                if (v1 == v2)
                {
                    count++;
                }
            }
            if (count > 1)
            {
                if (!(errors_found_flag))
                {
                    /* If this is the first error found to be printed,
                    then add a new line*/
                    printf("\n");
                }
                errors_found_flag = 1;
                total_vio++;
                set_vio_flag = 1;
                print_violation(s, v1, count);
            }
        }

        if (set_vio_flag)
        {
            /* If there has been a violation in this set*/
            set_vio++;
        }
    }

    if (errors_found_flag)
    {
        /*If there are any errors*/
        printf("\n%d different sets have violations\n", set_vio);
        printf("%d violations in total", total_vio);
        return 1;
    }
    return 0;
}

/* Formats the output for
- Ordinal Number of Violation set
- Conventional Name of violation set
- Qty of Duplicate instances
- Value of duplicate instances*/
void print_violation(int set, int value, int count)
{
    printf("set %2d ", set);
    if (set < NRCS)
    {
        printf("(row ");
    }
    else if (set < (2 * NRCS)) /* Cols begin after the 18th or
                                [NRCS * 2]th set.*/
    {
        printf("(col ");
    }
    else
    {
        printf("(sqr ");
    }
    printf("%d): ", ((set % NRCS) + 1));
    printf("%d instances of %d\n", count, value);
}

/* Tries to solve the board using S1, returns 1 if successful.*/
int strategy1(int board[])
{
    int count = 0;
    int temp_board[NCLL];

    while (1)
    {
        /* Fill temp board with zeros*/
        reset_board(temp_board);
        count = 0;

        for (int i = 0; i < NCLL; i++)
        {
            /* For every unknown cell in the board*/
            if (board[i] == 0)
            {
                /* Assign the label to temp_board[i] while also recording
                that activity in "count"*/
                count += (temp_board[i] = find_valid_label(board, i));
            }
        }
        if (count == 0)
        {
            /* No changes done in one complete pass through*/
            break;
        }
        else
        {
            printf("\nstrategy one\n");
            update_board(temp_board, board);
        }
    }
    if (count_unknown_cells(board) == 0)
    {
        return 1;
    }
    return 0;
}

/* Returns the integer value (1 - 9) of a valid label for the cell.
Returns 0 if no valid label found -- no changes */
int find_valid_label(int board[], int i)
{
    int set, value;
    int found_flag = 0;

    int label = 0;
    for (int n = 1; n <= NDIG; n++)
    {
        /* For each label (1 - 9)*/
        found_flag = 0;
        for (int j = 0; j < NGRP; j++)
        {
            /* For eeach set the cell [i] is a part of*/
            set = c2s[i][j];
            for (int c = 0; c < NDIG; c++)
            {
                /* For every other [non i] cell in the set*/
                value = board[s2c[set][c]];
                if (value == n)
                {
                    /* If a matching value for the label has been found*/
                    found_flag = 1;
                    break;
                }
            }
            if (found_flag)
            {

                /* If a label has been found already,
                no need to search other sets*/
                break;
            }
        }
        if (!(found_flag))
        {
            /* If no cell in all 3 sets has that label assigned*/
            if (!(label))
            {
                /* If we have not assigned a label for [i] already then*/
                /* This current label is the interim label for the cell*/
                label = n;
            }
            else
            {
                /* If there is already an interim label [one that also doesnt
                exist in all 3 sets], then we have 2 options, we lack certainty,
                exit and return 0*/
                label = 0;
                break;
            }
        }
    }
    return label;
}

/* For every non-zero cell in temp_board, overwrite the coress. cell in board
with the non zero value*/
void update_board(int temp_board[], int board[])
{
    for (int i = 0; i < NCLL; i++)
    {
        if (temp_board[i])
        {
            /* If change made to that cell*/
            board[i] = temp_board[i];
            output_change(i, temp_board[i]);
        }
    }
}

/* Prints a change in label for a cell i to a label*/
void output_change(int i, int label)
{
    int row, col;
    row = rownum(i);
    col = colnum(i);

    printf("row %d col %d must be %d\n", row, col, label);
}

/* Sets all cells in temp_board to 0*/
void reset_board(int temp_board[])
{
    for (int i = 0; i < NCLL; i++)
    {
        temp_board[i] = 0;
    }
}

/****************************************************************/
