#include <iostream>
#include <fstream>
#include <pm.h>
#include "Shape.h"
#include "shapelist.h"

#define OCCUPENCYMIN 0.05
#define OCCUPENCYMAX 0.1
#define SHAPELISTLENGTH 1
#define ROOMWIDTH 200
#define ROOMHEIGHT 200
#define ROBOTSIZE 3
#define BOUND 1000

int room[ROOMWIDTH][ROOMHEIGHT] = {0};
Shape *shapeList[2];
/*
static int shapeArray0[5][10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                            2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                            2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                            2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                            2, 2, 2, 2, 2, 0, 0, 0, 0, 2};
static int shapeArray1[10][20] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2};
*/


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
    //std::cout << "Pass size check" << std::endl;
    // check if the occupancy are beyond max
    if ((current_occupancy * ROOMHEIGHT * ROOMWIDTH + shapeList[shape_index]->size) / (ROOMWIDTH * ROOMHEIGHT) >
        OCCUPENCYMAX)
        return false;
    //std::cout << "Pass occupancy check" << std::endl;
    // put it inside and check for intersect
    for (int i = 0; i < shapeList[shape_index]->height; i++) {
        for (int j = 0; j < shapeList[shape_index]->width; j++) {
            if (a[x + i][y + j] != 0)
                return false;
            else
                a[x + i][y + j] = shapeList[shape_index]->index(i, j);
        }
    }
    //std::cout << "Pass intersection check" << std::endl;

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
    //std::cout << "Pass roadway check" << std::endl;
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
    std::fstream myfile("/home/hiwi03/Roomgenerator/generatelist.txt");
    std::cout<<myfile.is_open()<<std::endl;
    double occupancy = 0;
    // in order to bound the search time as improper setting could face no valid map
    int search = 0;
    while (occupancy <= OCCUPENCYMIN && search<BOUND) {
        search++;
        float x = rand() % ROOMHEIGHT;
        float y = rand() % ROOMWIDTH;
        int s = rand() % SHAPELISTLENGTH;
        if (checkValidation(x, y, s, occupancy)) {
            occupancy = updateRoom(x, y, s, occupancy);
            std::cout << x<<' '<<y<<' '<<s<<' '<<occupancy << std::endl;
            // in order to fit into gazebo we change the coordinate
            float x_g = (y+float(shapeList[s]->width)/2)/10-10 ;
            float y_g = 10-(x+float(shapeList[s]->height)/2)/10 ;
            myfile <<x_g<<' '<<y_g<<' '<<s<<' '<<occupancy << std::endl;
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
    myfile.close();
    delete shapeList[0];
    return 0;
}