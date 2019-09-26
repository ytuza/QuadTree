#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point(){
        x = 0.0;
        y = 0.0;
    }
    Point(float a,float b){
        x=a;
        y=b;
    }

    float x,y;
};

#endif // POINT_H
