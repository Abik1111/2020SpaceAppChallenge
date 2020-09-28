/**
**This is four vector class
**/
#include <iostream>

//#ifndef HEADER_FILE
//#define HEADER_FILE

using namespace std;

class Vector4{
private:
    double v[4];
public:
    Vector4(){
        this->setValue(0,0,0,0);
    }
    Vector4(const Vector4 &v){
        this->v[0]=v.v[0];
        this->v[1]=v.v[1];
        this->v[2]=v.v[2];
        this->v[3]=v.v[3];
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
    void setValue(Vector4 v){
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
    Vector4 getVector(){
        Vector4 temp;
        temp.setValue(v);
        return temp;
    }
    void operator = (const Vector4 &v ) {
        this->v[0]=v.v[0];
        this->v[1]=v.v[1];
        this->v[2]=v.v[2];
        this->v[3]=v.v[3];
    }
    static Vector4 getVector(double v0,double v1,double v2,double v3){
        Vector4 temp;
        temp.setValue(v0,v1,v2,v3);
        return temp;
    }
    static Vector4 getVector(double v[4]){
        Vector4 temp;
        temp.setValue(v);
        return temp;
    }

    //mathematics
    Vector4 operator+(const Vector4 &v){
        Vector4 temp;
        temp.v[0]=this->v[0]+v.v[0];
        temp.v[1]=this->v[1]+v.v[1];
        temp.v[2]=this->v[2]+v.v[2];
        temp.v[3]=this->v[3]+v.v[3];
        return temp;
    }
    Vector4 operator-(const Vector4 &v){
        Vector4 temp;
        temp.v[0]=this->v[0]-v.v[0];
        temp.v[1]=this->v[1]-v.v[1];
        temp.v[2]=this->v[2]-v.v[2];
        temp.v[3]=this->v[3]-v.v[3];
        return temp;
    }

    //Print Value
    void print(){
        cout<<"("<<v[0]<<","<<v[1]<<","<<v[2]<<","<<v[3]<<")"<<endl;
    }
    friend ostream& operator <<(ostream &out,Vector4 v){
         out<<"("<<v.v[0]<<","<<v.v[1]<<","<<v.v[2]<<","<<v.v[3]<<")";
         return out;
    }
};
//#endif

