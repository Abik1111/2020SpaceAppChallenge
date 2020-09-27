/**
**This is four vector class
**/
#include <iostream>

//#ifndef HEADER_FILE
//#define HEADER_FILE

using namespace std;

class Vector{
private:
    double v[4];
public:
    Vector(){
        this->setValue(0,0,0,0);
    }

    //Setting vector's component
    void setValue(double v0,double v1,double v2,double v3){
        this->v[0]=v0;
        this->v[1]=v1;
        this->v[2]=v2;
        this->v[3]=v3;
    }
    void setValue(double v[4]){
        this->v[0]=v[0];
        this->v[1]=v[1];
        this->v[2]=v[2];
        this->v[3]=v[3];
    }
    void setValue(Vector v){
        this->v[0]=v.v[0];
        this->v[1]=v.v[1];
        this->v[2]=v.v[2];
        this->v[3]=v.v[3];
    }

    //Getting vector's Component
    void getValue(double v[4]){
        v[0]=this->v[0];
        v[1]=this->v[1];
        v[2]=this->v[2];
        v[3]=this->v[3];
    }
    double getValue0(){
        return v[0];
    }
    double getValue1(){
        return v[1];
    }
    double getValue2(){
        return v[2];
    }
    double getValue3(){
        return v[3];
    }
    Vector getVector(){
        Vector temp;
        temp.setValue(v);
        return temp;
    }

    //Print Value
    void print(){
        cout<<"("<<v[0]<<","<<v[1]<<","<<v[2]<<","<<v[3]<<")"<<endl;
    }
};
//
//#endif

