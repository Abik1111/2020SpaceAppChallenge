#include <iostream>
#include "matter.h"

using namespace std;

int main()
{
    Vector3 v;
    v.setValue(1,2,3);
    Vector3 c;
    c=v+c;
    cout<<Vector3::getVector(1,3,4).getMagnitude();





    return 0;
}
