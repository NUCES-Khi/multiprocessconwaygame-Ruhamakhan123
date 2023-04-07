#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROWS 10
#define COLS 10

void initialize_grid(int grid[ROWS][COLS]) {
    for(int row=0; row<ROWS; row++) {
        for(int col=0; col<COLS; col++) {
            grid[row][col] = rand() % 2;
        }
    }
}

void print_grid(int grid[ROWS][COLS]) {
    printf("\033[2J"); 
    printf("\033[%d;%dH", 0, 0); 
    for(int row=0; row<ROWS; row++) {
        for(int col=0; col<COLS; col++) {
            if(grid[row][col] == 1) {
                printf("# ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int count_neighbors(int grid[ROWS][COLS], int row, int col) {
    int count = 0;
    for(int i=row-1; i<=row+1; i++) {
        for(int j=col-1; j<=col+1; j++) {
            if(i < 0 || i >= ROWS || j < 0 || j >= COLS) {
                continue;
            } else if(i == row && j == col) {
                continue;
            } else if(grid[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void update_grid(int grid[ROWS][COLS], int new_grid[ROWS][COLS]) {
    for(int row=0; row<ROWS; row++) {
        for(int col=0; col<COLS; col++) {
            int neighbors = count_neighbors(grid, row, col);
            if(grid[row][col] == 1 && (neighbors < 2 || neighbors > 3)) {
                new_grid[row][col] = 0;
            } else if(grid[row][col] == 0 && neighbors == 3) {
                new_grid[row][col] = 1;
            } else {
                new_grid[row][col] = grid[row][col];
            }
        }
    }
}

int main() {
    int grid[ROWS][COLS];
    int new_grid[ROWS][COLS];

    srand(time(NULL));
    initialize_grid(grid);
    initialize_grid(new_grid);

    while(1) {
        update_grid(grid, new_grid);
        print_grid(new_grid);
        int pid = fork();
        if(pid == 0){
            for(int row=0; row<ROWS; row++) {
                for(int col=0; col<COLS; col++) {
                    grid[row][col] = new_grid[row][col];
                }
            }
            sleep(1);
            exit(0);
        }else{
            for(int row=0; row<ROWS; row++) {
                for(int col=0; col<COLS; col++) {
                    new_grid[row][col] = grid[row][col];
                }
            }
            wait(NULL);
        }
    }
    return 0;
} 
