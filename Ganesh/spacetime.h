/**
    according to frame of reference of ship
**/
#include "matter.h"
#include <map>

class Spacetime{
private:
    map <int,Matter> matters;
    double dt=1;

    double t;
    Vector3 position;
    Vector3 velocity;
public:
    Spacetime(){
        t=0;
    }

    void addMatter(int id,Matter matter){
        matters[id]=matter;
    }

    void update(){
        t=t+dt;
        Vector3 intensity;
        //acceleration update
        for (auto& m: matters) {
            intensity = this->getGravitationalField(m.second.getPosition(),m.first);
            m.second.updateAcceleration(intensity);
        }

        //velocity update
        for (auto& m: matters) {
            intensity = this->getGravitationalField(m.second.getPosition(),m.first);
            m.second.updateVelocity(dt);
        }

        //position update
        for (auto& m: matters) {
            m.second.updatePosition(dt);
        }
    }

    //Getting
    Vector3 getGravitationalField(Vector3 effectPosition,int id=-1){
        Vector3 intensity;
        for (auto& m: matters) {
            if(id!=m.first){
                intensity=intensity+m.second.getGravitationalField(effectPosition);
            }
        }
        return intensity;
    }

    Matter getMatter(int id){
        Matter m = matters.at(id);
        return m;
    }

};
