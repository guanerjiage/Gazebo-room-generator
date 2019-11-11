//
// Created by guan on 24.10.19.
//

#ifndef CLIONPROJECTS_SHAPE_H
#define CLIONPROJECTS_SHAPE_H


class Shape {
public:
    int height;
    int width;
    int *shapeArray;
    int size;

public:
    Shape(int h = 0, int w = 0);
    ~Shape();
    int& index(int, int);
    
};



#endif //CLIONPROJECTS_SHAPE_H
