#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "helper.h"
#include "obstacles.h"
#include <ev3.h>

#define MAX_OBSTACLES 25
#define MIN_TILE_LENGTH 0.305
#define MAX_LENGTH 4.88
#define MAX_WIDTH 3.05
#define MAX_NODES 160
#define RESOLUTION_MULTIPLIER 2
#define TILE_LENGTH 32
#define TILE_WIDTH 20
#define STANDARD_MOVE_TIME 6160
#define WHEEL_DIAMETER 2.2
#define PI 3.1415

double start[2] = {3,3};
double goal[2] = {15,4};

int co_ordinate_plane[TILE_LENGTH][TILE_WIDTH];

enum Direction internal_direction = East;

double convert_to_coord(double value, int is_x);
void initialize_matrix(int matrix[TILE_LENGTH][TILE_WIDTH]);
void populate_obstacles(int matrix[TILE_LENGTH][TILE_WIDTH], double obstacles[MAX_OBSTACLES][2]);
void print_map();
struct Node *create_node(int x, int y);
struct Node *delete_node(struct Node *node);
void greedy_dfs(struct Node *head);
void move_forward(short);
void turn(enum TurnDirection);
enum TurnDirection find_direction(struct Node *head);
double distance(int x, int y);
void traverse_path(struct Node *head);

int main(){
    initialize_matrix(co_ordinate_plane);
    start[0] = start[0] * RESOLUTION_MULTIPLIER;
    start[1] = start[1] * RESOLUTION_MULTIPLIER;
    goal[0] = goal[0] * RESOLUTION_MULTIPLIER;
    goal[1] = goal[1] * RESOLUTION_MULTIPLIER;

    co_ordinate_plane[(int)start[0]][(int)start[1]] = 5;
    co_ordinate_plane[(int)goal[0]][(int)goal[1]] = 8;

    populate_obstacles(co_ordinate_plane, obstacle);
    print_map();
    printf("\n");
    struct Node *head = create_node((int)start[0], (int)start[1]);

    greedy_dfs(head);
    print_map();

    struct Node *tmp = head;

    traverse_path(tmp);

    return 0;
}

// This initializes the entire map
void initialize_matrix(int matrix[TILE_LENGTH][TILE_WIDTH]){
    int i, j;
    for(i = 0 ; i < TILE_LENGTH ; i++){
        for(j = TILE_WIDTH ; j  >= 0 ;j--){
            matrix[i][j] = 0;
        }
    }
}

void populate_obstacles(int matrix[TILE_LENGTH][TILE_WIDTH], double obstacles[MAX_OBSTACLES][2]){
    int i;
    for(i = 0; i < MAX_OBSTACLES; i++){
        int x = (int)obstacles[i][0] * RESOLUTION_MULTIPLIER;
        int y = (int)obstacles[i][1] * RESOLUTION_MULTIPLIER;
        int i = 1; int j = 2;
        if(x > 0 && x < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x][y] = -1;
        if(x+i > 0 && x+i < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x+i][y] = -1;
        if(x > 0 && x < TILE_LENGTH && y+i > 0 && y+i < TILE_WIDTH) matrix[x][y+i] = -1;
        if(x-i > 0 && x-i < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x-i][y] = -1;
        if(x > 0 && x < TILE_LENGTH && y-i > 0 && y-i < TILE_WIDTH) matrix[x][y-i] = -1;

        if(x > 0 && x < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x][y] = -1;
        if(x+j > 0 && x+j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x+j][y] = -1;
        if(x > 0 && x < TILE_LENGTH && y+j > 0 && y+j < TILE_WIDTH) matrix[x][y+j] = -1;
        if(x-j > 0 && x-j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x-j][y] = -1;
        if(x > 0 && x < TILE_LENGTH && y-j > 0 && y-j < TILE_WIDTH) matrix[x][y-j] = -1;

        if(x > 0 && x < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x][y] = -1;
        if(x+j > 0 && x+j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x+j][y+j] = -1;
        if(x+j > 0 && x+j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x+j][y-j] = -1;
        if(x+j > 0 && x+j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x+j][y+i] = -1;
        if(x+j > 0 && x+j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x+j][y-i] = -1;
        if(x > 0 && x < TILE_LENGTH && y+j > 0 && y+j < TILE_WIDTH) matrix[x-i][y+j] = -1;
        if(x > 0 && x < TILE_LENGTH && y+j > 0 && y+j < TILE_WIDTH) matrix[x+i][y+j] = -1;
        if(x-j > 0 && x-j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x-j][y+j] = -1;
        if(x-j > 0 && x-j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x-j][y-j] = -1;
        if(x-j > 0 && x-j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x-j][y+i] = -1;
        if(x-j > 0 && x-j < TILE_LENGTH && y > 0 && y < TILE_WIDTH) matrix[x-j][y-i] = -1;
        if(x > 0 && x < TILE_LENGTH && y-j > 0 && y-j < TILE_WIDTH) matrix[x-i][y-j] = -1;
        if(x > 0 && x < TILE_LENGTH && y-j > 0 && y-j < TILE_WIDTH) matrix[x+i][y-j] = -1;
        if(x+i > 0 && x+i < TILE_LENGTH && y+1 > 0 && y+i < TILE_WIDTH) matrix[x+i][y+i] = -1;
        if(x-i > 0 && x-i < TILE_LENGTH && y+1 > 0 && y+i < TILE_WIDTH) matrix[x-i][y+i] = -1;
        if(x-i > 0 && x-i < TILE_LENGTH && y-1 > 0 && y-i < TILE_WIDTH) matrix[x-i][y-i] = -1;
        if(x+i > 0 && x+i < TILE_LENGTH && y-1 > 0 && y-i < TILE_WIDTH) matrix[x+i][y-i] = -1;
    }
}

double convert_to_coord(double value, int is_x){
    if(is_x){
        return round(value/MAX_LENGTH * TILE_LENGTH);
    }
    return round(value/MAX_WIDTH * TILE_WIDTH);
}

void print_map(){
	int j, i;
    for(j = TILE_WIDTH; j>=0;j--){
        for(i = 0;i<TILE_LENGTH;i++){
            printf("\t%d",co_ordinate_plane[i][j]);
        }
        printf("\n");
    }
}

double distance(int x, int y){
    return sqrt(pow(goal[0]-x,2)+pow(goal[1]-y,2))+fabs(goal[0]-x)+fabs(goal[1]-y);
}

struct Node *create_node(int x, int y){
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->x = x;
    node->y = y;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

struct Node *delete_node(struct Node *node){
    struct Node *tmp = node->prev;
    tmp->next = NULL;
    free(node);
    return tmp;
}

void greedy_dfs(struct Node *head){
    double min_value = 9999;
    int x = 9999, y = 9999;

    if(!(head->x == (int)goal[0] && head->y == (int)goal[1]) && head->x != 0 && head->y != 0){
        if(distance(head->x + 1, head->y) < min_value && head->x+1 > 0 && head->x+1 < TILE_LENGTH){
            if(co_ordinate_plane[head->x + 1][head->y] == 0 || co_ordinate_plane[head->x + 1][head->y] == 1 || co_ordinate_plane[head->x + 1][head->y] == 8){
                min_value = distance(head->x + 1, head->y);
                x = head->x + 1;
                y = head->y;
            }
        }
        if(distance(head->x - 1, head->y) < min_value && head->x-1 > 0 && head->x-1 < TILE_LENGTH){
            if(co_ordinate_plane[head->x - 1][head->y] == 0 || co_ordinate_plane[head->x - 1][head->y] == 1 || co_ordinate_plane[head->x - 1][head->y] == 8){
                min_value = distance(head->x - 1, head->y);
                x = head->x - 1;
                y = head->y;
            }
        }
        if(distance(head->x, head->y + 1) < min_value && head->y + 1 > 0 && head->y + 1 < TILE_WIDTH){
            if(co_ordinate_plane[head->x][head->y + 1] == 0 || co_ordinate_plane[head->x][head->y + 1] == 1 || co_ordinate_plane[head->x][head->y + 1] == 8){
                min_value = distance(head->x, head->y + 1);
                x = head->x;
                y = head->y + 1;
            }
        }
        if(distance(head->x, head->y - 1) < min_value && head->y - 1 > 0 && head->y - 1 < TILE_WIDTH){
            if(co_ordinate_plane[head->x][head->y - 1] == 0 || co_ordinate_plane[head->x][head->y - 1] == 1 || co_ordinate_plane[head->x][head->y - 1] == 8){
                min_value = distance(head->x, head->y - 1);
                x = head->x;
                y = head->y - 1;
            }
        }

        if(min_value == 9999){
            co_ordinate_plane[head->x][head->y] = -1;
            head = delete_node(head);
            greedy_dfs(head);
        }
        else{
            struct Node *next_point = create_node(x, y);
            co_ordinate_plane[next_point->x][next_point->y] += 2;
            head->next = next_point;
            next_point->prev = head;

            next_point->next = NULL;
            greedy_dfs(next_point);
        }

    }
}

void move_forward(short time){
    OnRevSync(OUT_AD, 10);
    Wait(time);
    Off(OUT_AD);
}

void turn(enum TurnDirection dir){
	int value = rand() % (101) - 50;

    switch(dir){
        case Right:
            OnRevReg(OUT_D, 10);
            OnFwdReg(OUT_A, 10);
            Wait(2400+value);
            Off(OUT_AD);
            break;
        case Left:
            OnRevReg(OUT_A, 10);
            OnFwdReg(OUT_D, 10);
            Wait(2450+value);
            Off(OUT_AD);
            break;
        case Around:
            turn(Right);
            turn(Right);
            break;
        case NoDir:
            break;
    }
}

enum TurnDirection find_direction(struct Node *head){
    // find if up or down
    if(head->next->y > head->y){
        if(internal_direction == East){
            internal_direction = North;
            return Left;
        }else if(internal_direction == West){
            internal_direction = North;
            return Right;
        }
        else if(internal_direction == South){
            internal_direction = North;
            return Around;
        }
    }
    else if(head->next->y < head->y){
        if(internal_direction == East){
            internal_direction = South;
            return Right;
        }
        else if(internal_direction == West){
            internal_direction = South;
            return Left;
        }
        else if(internal_direction == North){
            internal_direction = South;
            return Around;
        }
    }
    else if(head->next->x > head->x){
        if(internal_direction == North){
            internal_direction = East;
            return Right;
        }
        else if(internal_direction == South){
            internal_direction = East;
            return Left;
        }
        else if(internal_direction == West){
            internal_direction = East;
            return Around;
        }
    }
    else if(head->next->x < head->x){
        if(internal_direction == North){
            internal_direction = West;
            return Left;
        }
        else if(internal_direction == South){
            internal_direction = West;
            return Right;
        }
        else if(internal_direction == East){
            internal_direction = West;
            return Around;
        }
    }
    return NoDir;
}

void traverse_path(struct Node *head){
    while(head != NULL){
        turn(find_direction(head));
        move_forward(STANDARD_MOVE_TIME/RESOLUTION_MULTIPLIER);
        head = head->next;
    }
}
