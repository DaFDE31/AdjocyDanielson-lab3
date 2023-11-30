#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
//int* worker_validation;

int* valid;

int**board;

int** read_board_from_file(char* filename){
    FILE *fp = fopen(filename, "r");
    sudoku_board = (int**)malloc(9*sizeof(int*));
    for (int col = 0; col < 9; col++)
    {
        sudoku_board[col] = (int*)malloc(9*sizeof(int));
    }
    if(fp == NULL){
        fprintf(stderr, "No file.");
        return NULL;
        }

    int row = 0;
    while(row < 9){
        int col = 0;
        while(col < 9){
            fscanf(fp,"%d%*c",&sudoku_board[row][col]);
            col++;
        }
        row++;
    }
    fclose(fp);
    return sudoku_board;
}

void* board_piece(void* piece){
    param_struct* tester = (param_struct*) piece;
    int spot = tester->id;
    int sr = tester->starting_row;
    int er = tester->ending_row;
    int sc = tester->starting_col;
    int ec = tester->ending_col;
    
    valid[spot] = 1;
    

    int checker[9] ={0,0,0,0,0,0,0,0,0};
    for (int row = sr; row <= er; row++){
        //printf("Row #%d\n",row);

        for (int col = sc; col <= ec; col++){
            //printf("Row #%d Col#%d\n",row, col);

            //printf("%d",board[row][col]);
            checker[ sudoku_board[row][col] -1] = 1;
        }
    } 

    for (int value =0; value < 9; value++){
        //printf("%d ", checker[value]); BRING THIS AND THE NEXT BACK
        if(checker[value] != 1){
            valid[spot] = 0;
        }
    }
    printf("SPOT: %d VALID: %d Starting row:%d Ending row:%d Starting column: %d Ending column: %d\n",spot, valid[spot], sr,er,sc,ec);
    //printf("%d ", valid[spot]);
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    // replace this comment with your code
    tid = (pthread_t*) malloc(sizeof(int*) * NUM_OF_THREADS);   
    parameter = (param_struct*) malloc(sizeof(param_struct) * NUM_OF_THREADS);
    valid = (int*)malloc(27*sizeof(int));
    int identity = 0;// id variable, while spot is used to denote rows and such.
    for (int spot = 0; spot < ROW_SIZE; spot++){
        parameter[identity].id = identity;
        parameter[identity].starting_row = spot;
        parameter[identity].starting_col = 0;
        parameter[identity].ending_col = COL_SIZE-1;
        parameter[identity].ending_row = spot;
        /*int check [9];
        int index = 0;
        //for (int row = parameter[spot].starting_row; row <= parameter[spot].ending_row; row++){
            for (int col = parameter[spot].starting_col; col < parameter[spot].ending_row; col++){
                check[index] = sudoku_board[spot][col];
                index++;
            }
        //}
        */
        printf("IDENTITY: %d, ID %d\n", identity, parameter[identity].id);
        pthread_create(&(tid[identity]), NULL, board_piece, &parameter[identity]);
        identity++;
    }

    for (int spot = 0; spot < COL_SIZE; spot++){
        parameter[identity].id = identity;
        parameter[identity].starting_row = 0;
        parameter[identity].starting_col = spot;
        parameter[identity].ending_col = spot;
        parameter[identity].ending_row = ROW_SIZE-1;
        /*int check [9];
        int index = 0;
        for (int row = parameter[spot].starting_row; row < parameter[spot].ending_row; row++){
            //for (int col = parameter[spot].starting_col; col <= parameter[spot].ending_row; col++){
                check[index] = sudoku_board[row][spot];
                index++;
            //}
        }
        */
        printf("IDENTITY: %d, ID %d\n", identity, parameter[identity].id);
        pthread_create(&(tid[identity]), NULL, board_piece, &parameter[identity]);
        identity++;
    }
    
    int horz = 0;
    int vert = 0;
    for (int spot = 0; spot < NUM_OF_SUBGRIDS; spot++){
        parameter[identity].id = identity;
        parameter[identity].starting_row = horz;
        parameter[identity].starting_col = vert;
        parameter[identity].ending_col = vert+2;
        parameter[identity].ending_row = horz+2;
        /*int check [9];
        int index = 0;
        for (int row = parameter[spot].starting_row; row < parameter[spot].ending_row; row++){
            for (int col = parameter[spot].starting_col; col < parameter[spot].ending_row; col++){
                check[index] = sudoku_board[row][col];
                index++;
            }
        }
        */
        printf("IDENTITY: %d, ID %d\n", identity, parameter[identity].id);
        pthread_create(&(tid[identity]), NULL, board_piece, &parameter[identity]);
        identity++;
        if(vert == 6){
            horz+=3;
            vert = 0;
        }
        else{ 
            vert+=3;
        }
    }
    for(int i =0; i< 27; i++){
        //printf("%s", (char*)tid[i]);
        pthread_join(tid[i], NULL);
    }

    for (int v = 0; v < 27; v++){
        printf("VALIDATOR: %d ", valid[v]);// BRING THIS BACK
        if (valid[v] != 1){  
            return 0; 
        }
    }
    free(valid);
    free(tid);
   return 1;
}
/*
int main(int argc, char *argv[]){
    



    board = read_board_from_file(argv[1]);
    for(int i = 0; i<9; i++ ){
        for (int e = 0; e<9; e++){
            printf("%d",board[i][e]);
        }
        printf("\n");
    }
    
    
   
    if (is_board_valid()){
        printf("The board is valid.\n");
        printf("RAHHHHHHHH: 1");
    }
    else{
        printf("The board is not valid.\n");
        printf("RAHHHHHHHH: 0");
    }
    
    for(int row = 0; row < ROW_SIZE; row++){
        free(board[row]);
    }
    free(board);
    return 0;
}
*/