
#include <iostream>
#include "spacetime.h"

using namespace std;

int physicsMain(){
    Spacetime spacetime;

    Matter Sun;
    Sun.setMass(1.9891e30);

    Matter Earth;
    Earth.setMass(5.97237e24);
    Earth.setPosition(Vector3::getVector(149.598e9,0,0));

    Matter Mars;
    Mars.setMass(6.4171e23);
    Mars.setPosition(Vector3::getVector(227.939e9,0,0));

    spacetime.addMatter(0,Sun);
    spacetime.addMatter(3,Earth);
    spacetime.addMatter(4,Mars);

    for(int i=0;i<30;i++){
        spacetime.update();
        cout<<spacetime.getMatter(4).getPosition()<<endl;
    }


    return 0;
}
