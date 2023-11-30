#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;
// ALSO IMPLEMENT GLOBAL ARRAY TO RETURN ANSWERS OF VALIDATION USING TID
int* valid;

int**board;

int** read_board_from_file(char* filename){
    FILE *fp = fopen(filename, "r");
    board = (int**)malloc(9*sizeof(int*));
    for (int col = 0; col < 9; col++)
    {
        board[col] = (int*)malloc(9*sizeof(int));
    }
    if(fp == NULL){
        fprintf(stderr, "No file.");
        return NULL;
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

void* board_piece(void* piece){
    param_struct* tester = (param_struct*) piece;
    int spot = tester->id;
    int sr = tester->starting_row;
    int er = tester->ending_row;
    int sc = tester->starting_col;
    int ec = tester->ending_col;

    int checker[] ={0,0,0,0,0,0,0,0,0};
    for (int row = sr; row <= er; row++){
        for (int col = sc; col <= ec; col++){
            checker[ board[row][col] -1] = 1;
        }
    }

    for (int value =0; value < 9; value++){
        if(checker[value] != 1){
            valid[spot] = 1;
        }
    }
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    // replace this comment with your code
    tid = (pthread_t*) malloc(sizeof(int*) * NUM_OF_THREADS);   
    parameter = (param_struct*) malloc(sizeof(param_struct) * NUM_OF_THREADS);
    valid = (int*)malloc(27*sizeof(int));


    for (int spot = 0; spot < ROW_SIZE; spot++){
        parameter[spot].starting_row = spot;
        parameter[spot].starting_col = 0;
        parameter[spot].ending_col = COL_SIZE-1;
        parameter[spot].ending_row = spot;
        /*int check [9];
        int index = 0;
        //for (int row = parameter[spot].starting_row; row <= parameter[spot].ending_row; row++){
            for (int col = parameter[spot].starting_col; col < parameter[spot].ending_row; col++){
                check[index] = sudoku_board[spot][col];
                index++;
            }
        //}
        */
        printf("%d\n", spot);
        pthread_create(&(tid[spot]), NULL, board_piece, &parameter[spot]);
    }

    for (int spot = 9; spot < COL_SIZE+ROW_SIZE; spot++){
        parameter[spot].starting_row = 0;
        parameter[spot].starting_col = spot;
        parameter[spot].ending_col = spot;
        parameter[spot].ending_row = ROW_SIZE-1;
        /*int check [9];
        int index = 0;
        for (int row = parameter[spot].starting_row; row < parameter[spot].ending_row; row++){
            //for (int col = parameter[spot].starting_col; col <= parameter[spot].ending_row; col++){
                check[index] = sudoku_board[row][spot];
                index++;
            //}
        }
        */
        printf("%d\n", spot);
        pthread_create(&(tid[spot]), NULL, board_piece, &parameter[spot]);
    }
    
    int horz = 0;
    int vert = 0;
    for (int spot = 18; spot < NUM_OF_THREADS; spot++){
        parameter[spot].starting_row = horz;
        parameter[spot].starting_col = vert;
        parameter[spot].ending_col = vert+2;
        parameter[spot].ending_row = horz+2;
        /*int check [9];
        int index = 0;
        for (int row = parameter[spot].starting_row; row < parameter[spot].ending_row; row++){
            for (int col = parameter[spot].starting_col; col < parameter[spot].ending_row; col++){
                check[index] = sudoku_board[row][col];
                index++;
            }
        }
        */
        printf("%d\n", spot);
        pthread_create(&(tid[spot]), NULL, board_piece, &parameter[spot]);
        if(vert == 6){
            horz+=3;
            vert = 0;
        }
        else{ 
            vert+=3;
        }
    }
    for(int i =0; i< 27; i++){
        pthread_join(tid[i], NULL);
    }

    for (int v = 0; v < 27; v++){
        printf("%d", 0);
        if (valid[v] != 1){  
            return 0; 
        }
    }
    free(valid);
    free(tid);
   return 1;
}

int main(int argc, char *argv[]){
    



    board = read_board_from_file(argv[1]);
    for(int i = 0; i<9; i++ ){
        for (int e = 0; e<9; e++){
            printf("%d",board[i][e]);
        }
        printf("\n");
    }
    
    int RAHHH = is_board_valid();
    /*
    if (is_board_valid()){
        printf("The board is valid.\n");
    } else {
        printf("The board is not valid.\n");
    }
    
    for(int row = 0; row < ROW_SIZE; row++){
        free(board[row]);
    }
    free(board);
    return 0;*/
}
