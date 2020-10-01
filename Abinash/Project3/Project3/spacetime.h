/**
    according to frame of reference of ship
**/
#include "matter.h"
#include <map>

class Spacetime{
private:
    map <int,Matter> matters;
    double dt=24*60*60;

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
		double temperatureNew;
		double tPower4 = 0.0;
		double radius;
		double emissivity;
        //acceleration update
        for (auto& m: matters) {
            intensity = this->getGravitationalField(m.second.getPosition(),m.first);
            m.second.updateAcceleration(intensity);
			//m.second.updateVelocity(dt);
			//m.second.updatePosition(dt);

        }

        //velocity update
        for (auto& m: matters) {
            m.second.updateVelocity(dt);
        }

        //position update
        for (auto& m: matters) {
            m.second.updatePosition(dt);
        }

		for (auto& m : matters) {
			if (!m.second.isBlackBody()) {
				tPower4 = this->calculateTemperature(m.second.getPosition(), m.first);
				emissivity = m.second.getEmissivity();
				temperatureNew = sqrt(sqrt(emissivity*tPower4));
				m.second.updateTemperature(temperatureNew);
			}
		}
    }
	double calculateTemperature(Vector3 effectPosition, int id = -1) {
		double tPower4=0;
		for (auto& m : matters) {
			if (id != m.first) {
				tPower4 = tPower4 + m.second.calculateTemperature(effectPosition);
			}
		}
		return tPower4;
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
