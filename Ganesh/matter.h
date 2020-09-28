#include <iostream>
#include "vector4.h"
#include "vector3.h"
#include <math.h>
#include "constant.h"
using namespace std;

class Matter{
private:
    double mass;// in kg
    Vector4 position;// in position 0 second others meter
    Vector4 velocity;// in position 0 unit less other m/s
    Vector4 acceleration;// in position 0 1/s other m/s2
    double gamma;// Lorentz factor unit less
public:
    Matter(){
        this->setMass();
        this->setVelocity(Vector4::getVector(1,0,0,0));
    }

    //Setting values
    void setMass(double mass=1){
        this->mass=mass;
    }
    void setPosition(Vector4 position){
       this->position.setValue(position);
    }
    void setVelocity(Vector4 velocity){
        this->velocity.setValue(velocity);
    }

    //update
    void update(){
        //update acceleration
        //update position in space time
    }

    Vector3 getGravitationField(Vector4 position){
        //return gravity given by this matter to given point in space time
        Vector4 r4;
        Vector3 r3;
        double L_ang;
        double rMag;
        double GM=G*mass;
        r4=position-this->position;
        r3=Vector3::getVector(r4.getValue1(),r4.getValue2(),r4.getValue3());
        rMag=r3.getMagnitude();
        //L_ang=sqrt((GM*rMag)/(1-(3*GM/(c*c*rMag))));
        Vector3 intensity;//acceleration of the celestialbody
        intensity=r3.scale(-GM/(rMag*rMag*rMag));
        return intensity;
    }



};

