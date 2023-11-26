#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;
// ALSO IMPLEMENT GLOBAL ARRAY TO RETURN ANSWERS OF VALIDATION USING TID
int* valid[27];


int** read_board_from_file(char* filename){
    FILE *fp = fopen(filename, "r");
    int** board = NULL;


    // replace this comment with your code
    board = (int**)malloc(9*sizeof(int*));
    for (int col = 0; col < 9; col++)
    {
        board[col] = (int*)malloc(9*sizeof(int));
    }
    if(fp == NULL){
        fprintf(stderr, "No file.");
        return;
        }

    int row = 0;
    while(row < 9){
        int col = 0;
        while(col < 9){
            fscanf(fp,"%d",&board[row][col]);
            col++;
        }
        row++;
    }
    fclose(fp);
    return board;
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    // replace this comment with your code
    tid = (pthread_t*) malloc(sizeof(int*) * NUM_OF_THREADS);   
    parameter = (param_struct*) malloc(sizeof(param_struct) * NUM_OF_THREADS);

    //MIGHT WANT TO PUT ALL THIS IN THE VALIDATION METHOD

    for (int spot = 0; spot < ROW_SIZE; spot++){
        parameter[spot].starting_row = spot;
        parameter[spot].starting_col = 0;
        parameter[spot].ending_col = COL_SIZE;
        parameter[spot].ending_row = spot;
        int check [9];
        int index = 0;
        //for (int row = parameter[spot].starting_row; row <= parameter[spot].ending_row; row++){
            for (int col = parameter[spot].starting_col; col < parameter[spot].ending_row; col++){
                check[index] = sudoku_board[spot][col];
                index++;
            }
        //}
        pthread_create(&(tid[spot]), &attr, board_piece(check), &check);
    }

    for (int spot = 0; spot < COL_SIZE; spot++){
        parameter[spot].starting_row = 0;
        parameter[spot].starting_col = spot;
        parameter[spot].ending_col = spot;
        parameter[spot].ending_row = ROW_SIZE;
        int check [9];
        int index = 0;
        for (int row = parameter[spot].starting_row; row < parameter[spot].ending_row; row++){
            //for (int col = parameter[spot].starting_col; col <= parameter[spot].ending_row; col++){
                check[index] = sudoku_board[row][spot];
                index++;
            //}
        }
        pthread_create(&(tid[spot]), &attr, board_piece(check), &check);
    }
    
    int horz = 0;
    int vert = 0;
    for (int spot = 0; spot < COL_SIZE; spot++){
        parameter[spot].starting_row = horz;
        parameter[spot].starting_col = vert;
        parameter[spot].ending_col = vert+3;
        parameter[spot].ending_row = horz+3;
        int check [9];
        int index = 0;
        for (int row = parameter[spot].starting_row; row < parameter[spot].ending_row; row++){
            for (int col = parameter[spot].starting_col; col < parameter[spot].ending_row; col++){
                check[index] = sudoku_board[row][col];
                index++;
            }
        }
        pthread_create(&(tid[spot]), &attr, board_piece(check), &check);
        if(vert == 6){
            horz+=3;
            vert = 0;
        }
        else{
            vert+=3;
        }
    }


    
}

int board_piece(int* row){
    int checker[] ={0,0,0,0,0,0,0,0,0}; 
    int sum = 0;
    for (int value =0; value < 9; value++){
        checker[row[value]-1] = 1;
    }
    for (int value =0; value < 9; value++){
        if(checker[value] != 1){
            return 0;
        }
    }
    return 1;
}