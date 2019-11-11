//
// Created by guan on 24.10.19.
//

#include "Shape.h"

Shape::Shape(int h, int w)
{
    width = w;
    height = h;
    size = w*h;
    shapeArray = new int[h*w];
}

int& Shape::index(int x, int y)
{
    return shapeArray[x*width+y];
}

Shape::~Shape()
{
    delete shapeArray;
}
