#include <iostream>
#include "vector4.h"
#include "vector3.h"

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

    Vector4 getGravitationField(Vector4 position){
        //return gravity given by this matter to given point in space time
    }



};

