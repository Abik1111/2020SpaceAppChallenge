#include <iostream>
#include "matter.h"

using namespace std;

int main()
{
    Matter Earth;
    Earth.setMass(6e24);
    cout<<Earth.getGravitationField(Vector4::getVector(0,1,1,1));
    
    return 0;
}
