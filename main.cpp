#include <iostream>
#include <fstream>
#include <pm.h>
#include "Shape.h"
#include "shapelist.h"

#define OCCUPENCYMIN 0.1
#define OCCUPENCYMAX 0.4
#define SHAPELISTLENGTH 9
#define ROOMWIDTH 200
#define ROOMHEIGHT 200
#define ROBOTSIZE 3
#define BOUND 1000

int room[ROOMWIDTH][ROOMHEIGHT] = {0};
Shape *shapeList[SHAPELISTLENGTH];

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
    // init the rectangle shapes
    int shapeIndex = 0;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray0[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray1[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray2[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray3[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray4[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray5[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray6[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray7[i][j];
            //std::cout<<shapeList[0]->index(i,j);
        }
        //std::cout<<std::endl;
    }
    shapeIndex++;
    shapeList[shapeIndex] = new Shape(shapeSize[shapeIndex][0],shapeSize[shapeIndex][1]);
    for (int i = 0; i < shapeList[shapeIndex]->height; i++) {
        for (int j = 0; j < shapeList[shapeIndex]->width; j++) {
            shapeList[shapeIndex]->index(i, j) = shapeArray8[i][j];
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
    // check if the shape size is fit 
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
    for (int i = x - ROBOTSIZE; i < (x + shapeList[shape_index]->height + ROBOTSIZE); i++) {
        for (int j = 1; j <= ROBOTSIZE; j++) {
            //std::cout<<'('<<i<<','<<y-j<<") ("<<i<<','<<y + shapeList[shape_index]->width - 1 + j<<')'<<std::endl;
            if (a[i][y - j] != 0 || (y + shapeList[shape_index]->width - 1 + j < ROOMWIDTH &&
                                     a[i][y + shapeList[shape_index]->width - 1 + j] != 0))
                return false;
        }
    }
    for (int i = y - ROBOTSIZE; i <(y + shapeList[shape_index]->width + ROBOTSIZE); i++) {
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
        }
    }
    return occ;
}

int main() {

    init();
    std::fstream generateListF("/home/hiwi03/Roomgenerator/generatelist.txt", std::fstream::in | std::fstream::out |std::fstream::trunc);
    std::fstream mapArrayF("/home/hiwi03/Roomgenerator/maparray.txt", std::fstream::in | std::fstream::out |std::fstream::trunc);
    double occupancy = 0;
    srand(time(0));
    
    // in order to bound the search time as improper setting could face no valid map
    int search = 0;
    while (occupancy <= OCCUPENCYMIN && search<BOUND) {
        search++;
        float x = rand() % ROOMHEIGHT;
        float y = rand() % ROOMWIDTH;
        int s = rand() % SHAPELISTLENGTH;
        if (checkValidation(x, y, s, occupancy)) {
            occupancy = updateRoom(x, y, s, occupancy);
            // std::cout << x<<' '<<y<<' '<<s<<' '<<occupancy << std::endl;
            // in order to fit into gazebo we change the coordinate
            float x_g = (y+float(shapeList[s]->width)/2)/10-10 ;
            float y_g = 10-(x+float(shapeList[s]->height)/2)/10 ;
            generateListF <<x_g<<' '<<y_g<<' '<<s<<' '<<occupancy << std::endl;
        }
    }

    // print the room
    for (int i = 0; i < ROOMHEIGHT; i++) {
        for (int j = 0; j < ROOMWIDTH; j++) {
            if (room[i][j] == 0)
                mapArrayF << 0<<' ';
            else
                mapArrayF << 100<<' ';
        }
    }


    generateListF.close();
    mapArrayF.close();
    for(int i = 0; i<SHAPELISTLENGTH; i++)
        delete shapeList[i];
    return 0;
}