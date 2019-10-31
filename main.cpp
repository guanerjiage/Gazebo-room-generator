#include <iostream>
#include <pm.h>
#include "Shape.h"

#define OCCUPENCYMIN 0.2
#define OCCUPENCYMAX 0.8
#define SHAPELISTLENGTH 2
#define ROOMWIDTH 50
#define ROOMHEIGHT 50
#define ROBOTSIZE 3
#define BOUND 1000

int room[ROOMWIDTH][ROOMHEIGHT] = {0};
Shape *shapeList[2];
int shapeArray0[5][10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                          2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                          2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                          2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                          2, 2, 2, 0, 0, 0, 0, 2, 2, 2};
int shapeArray1[7][7] = {0, 0, 2, 2, 2, 0, 0,
                         0, 2, 0, 0, 0, 2, 0,
                         2, 0, 0, 0, 0, 0, 2,
                         2, 0, 0, 0, 0, 0, 2,
                         2, 0, 0, 0, 0, 0, 2,
                         0, 2, 0, 0, 0, 2, 0,
                         0, 0, 0, 0, 2, 0, 0};

// init hard code shape list and outside room wall
void init() {
    for (int i = 0; i < ROOMHEIGHT; i++) {
        room[i][0] = 1;
        room[i][ROOMWIDTH - 1] = 1;
    }
    for (int i = 0; i < ROOMWIDTH; i++) {
        room[0][i] = 1;
        room[ROOMHEIGHT - 1][i] = 1;
    }

    shapeList[0] = new Shape(5, 10);
    for (int i = 0; i < shapeList[0]->height; i++) {
        for (int j = 0; j < shapeList[0]->width; j++) {
            shapeList[0]->index(i, j) = shapeArray0[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeList[1] = new Shape(7, 7);
    for (int i = 0; i < shapeList[1]->height; i++) {
        for (int j = 0; j < shapeList[1]->width; j++) {
            shapeList[1]->index(i, j) = shapeArray1[i][j];
            //std::cout<<shapeList[1]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeList[1]->size = 37;
    return;
}

// check if it is still valid after inserting a new shape, both occupancy and reachability
// the x and y indicating the upper left point of one shape
bool checkValidation(int x, int y, int shape_index, double current_occupancy) {
    // new new space for room
    int **a = new int *[ROOMHEIGHT];
    for (int i = 0; i < ROOMHEIGHT; ++i)
        a[i] = new int[ROOMWIDTH];
    // copy the current room
    for (int i = 0; i < ROOMHEIGHT; i++) {
        for (int j = 0; j < ROOMWIDTH; j++) {
            a[i][j] = room[i][j];
        }
    }
    // check if the shape size is fit and check for the upper left corner already
    if (x < ROBOTSIZE || y < ROBOTSIZE || x + shapeList[shape_index]->height > ROOMHEIGHT ||
        y + shapeList[shape_index]->width > ROOMWIDTH)
        return false;
    std::cout << "Pass size check" << std::endl;
    // check if the occupancy are beyond max
    if ((current_occupancy * ROOMHEIGHT * ROOMWIDTH + shapeList[shape_index]->size) / (ROOMWIDTH * ROOMHEIGHT) >
        OCCUPENCYMAX)
        return false;
    std::cout << "Pass occupancy check" << std::endl;
    // put it inside and check for intersect
    for (int i = 0; i < shapeList[shape_index]->height; i++) {
        for (int j = 0; j < shapeList[shape_index]->width; j++) {
            if (a[x + i][y + j] != 0)
                return false;
            else
                a[x + i][y + j] = shapeList[shape_index]->index(i, j);
        }
    }
    std::cout << "Pass intersection check" << std::endl;

    // check if roadway for robot still exist
    // for simplification we over approximate at the corner cases
    for (int i = x - ROBOTSIZE; i < min(x + shapeList[shape_index]->height + ROBOTSIZE, ROOMHEIGHT); i++) {
        for (int j = 1; j <= ROBOTSIZE; j++) {
            //std::cout<<'('<<i<<','<<y-j<<") ("<<i<<','<<y + shapeList[shape_index]->width - 1 + j<<')'<<std::endl;
            if (a[i][y - j] != 0 || (y + shapeList[shape_index]->width - 1 + j < ROOMWIDTH &&
                                     a[i][y + shapeList[shape_index]->width - 1 + j] != 0))
                return false;
        }
    }
    for (int i = y - ROBOTSIZE; i < min(y + shapeList[shape_index]->width + ROBOTSIZE, ROOMWIDTH); i++) {
        for (int j = 1; j <= ROBOTSIZE; j++) {
            //std::cout<<'('<<x-j<<','<<i<<") ("<<x + shapeList[shape_index]->height - 1 + j<<','<<i<<')'<<std::endl;
            if (a[x - j][i] != 0  || (x + shapeList[shape_index]->height - 1 + j < ROOMHEIGHT &&
                                     a[x + shapeList[shape_index]->height - 1 + j][i] != 0))
                return false;
        }
    }
    std::cout << "Pass roadway check" << std::endl;
    // delete the space
    for (int i = 0; i < ROOMHEIGHT; ++i)
        delete[] a[i];
    delete[] a;

    return true;
}

// update the real room and return the new occupancy
double updateRoom(int x, int y, int shape_index, double current_occupancy) {
    double occ = (current_occupancy * ROOMHEIGHT * ROOMWIDTH + shapeList[shape_index]->size) / (ROOMWIDTH * ROOMHEIGHT);
    for (int i = 0; i < shapeList[shape_index]->height; i++) {
        for (int j = 0; j < shapeList[shape_index]->width; j++) {
            if (shapeList[shape_index]->index(i, j) != 0)
                room[x + i][y + j] = 1;
            else
                room[x + i][y + j] = 0;
        }
    }
    return occ;
}

int main() {

    init();
    double occupancy = 0;
    // in order to bound the search time as improper setting could face no valid map
    int search = 0;
    while (occupancy <= OCCUPENCYMIN && search<BOUND) {
        search++;
        int x = rand() % ROOMHEIGHT;
        int y = rand() % ROOMWIDTH;
        int s = rand() % SHAPELISTLENGTH;
        if (checkValidation(x, y, s, occupancy)) {
            occupancy = updateRoom(x, y, s, occupancy);
            std::cout << x<<' '<<y<<' '<<occupancy << std::endl;
        }


    }
    // print the room
    for (int i = 0; i < ROOMHEIGHT; i++) {
        for (int j = 0; j < ROOMWIDTH; j++) {
            if (room[i][j] == 0)
                std::cout << ' ';
            else
                std::cout << '*';
        }
        std::cout << std::endl;
    }

    delete shapeList[0];
    delete shapeList[1];
    return 0;
}