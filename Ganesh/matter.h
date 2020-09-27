#include <iostream>
#include "vector.h"

using namespace std;

class Matter{
private:
    double mass;// in kg
    Vector position;// in position 0 second others meter
    Vector velocity;// in position 0 unit less other m/s
    Vector acceleration;// in position 0 1/s other m/s2
    double gamma;// Lorentz factor unit less
public:
    Matter(){
        this->setMass();
    }

    //Setting values
    void setMass(double mass=1){
        this->mass=mass;
    }
    void setPosition(Vector position){
       this->position.setValue(position);
    }
    void setVelocity(Vector velocity){
        this->velocity.setValue(velocity);
    }

    //update
    void update(){
        //update acceleration
        //update position in space time
    }

    Vector getGravitationField(Vector position){
        //return gravity given by this matter to given point in space time
    }



};

