/**
    according to frame of reference of ship
**/
#include "matter.h"
#include <map>

class Spacetime{
private:
    map <int,Matter> matters;
    double dt=1;//4*60*60;
    double gravDilFactor;
    double time;
    Vector3 position;
    Vector3 velocity;
public:
    Spacetime(){
        time=0;
        position.setValue(-2.81622223e3,0,0);//81622223e3
        //velocity.setValue(0.999*C,0,0);
        gravDilFactor=1;
    }

    void addMatter(int id,Matter matter){
        matters[id]=matter;
    }

    void update(){
        time=time+dt;
        Vector3 intensity;

        double v_mag_ship=velocity.getMagnitude();
        //acceleration update
        for (auto& m: matters) {
            intensity = this->getGravitationalField(m.second.getPosition(),m.first);
            m.second.updateAcceleration(intensity);
        }

        gravDilFactor=0;
        for (auto& m: matters) {
            double a = (1-(velocity.getValue1()*velocity.getValue1()+velocity.getValue2()*velocity.getValue2()+
                velocity.getValue3()*velocity.getValue3())/(C*C));
            double b = -2.0*(time-(velocity.getValue1()*position.getValue1()+velocity.getValue2()*position.getValue2()+
                velocity.getValue3()*position.getValue3())/(C*C));
            double c = (time*time-(position.getValue1()*position.getValue1()+position.getValue2()*position.getValue2()
				+position.getValue3()*position.getValue3())/(C*C));

            double temp=sqrt(b*b-4.0*a*c);

            if(temp<0.0) {
                continue;
            }
            double t =  (-b-temp)/(2.0*a);

            if(t<0.0) {
                t=(-b+temp)/(2.0*a);
            }
            if(t>time) {
                continue;
            }

            double dt = time-t;

            Vector3 causePosition = m.second.getPosition()-Vector3::getVector(velocity.getValue1()*dt,velocity.getValue2()*dt,velocity.getValue3()*dt);
            Vector3 dr = position-causePosition;
            double GM = G*m.second.getMass();
            double r_mag=dr.getMagnitude();

            gravDilFactor+=GM/r_mag;
        }

        for (auto& m: matters) {
            double a = (1-(velocity.getValue1()*velocity.getValue1()+velocity.getValue2()*velocity.getValue2()+
                velocity.getValue3()*velocity.getValue3())/(C*C));
            double b = -2.0*(time-(velocity.getValue1()*position.getValue1()+velocity.getValue2()*position.getValue2()+
                velocity.getValue3()*position.getValue3())/(C*C));
            double c = (time*time-(position.getValue1()*position.getValue1()+position.getValue2()*position.getValue2()
				+position.getValue3()*position.getValue3())/(C*C));

            double temp=sqrt(b*b-4.0*a*c);

            if(temp<0.0) {
                continue;
            }
            double t =  (-b-temp)/(2.0*a);

            if(t<0.0) {
                t=(-b+temp)/(2.0*a);
            }
            if(t>time) {
                continue;
            }

            double dt = time-t;

            Vector3 causePosition = m.second.getPosition()-Vector3::getVector(velocity.getValue1()*dt,velocity.getValue2()*dt,velocity.getValue3()*dt);
            Vector3 dr = position-causePosition;
            double GM = G*m.second.getMass();
            double r_mag=dr.getMagnitude();

            double netGravDilFactor = gravDilFactor-GM/r_mag;
            netGravDilFactor=1/sqrt(1-2*netGravDilFactor/(C*C));

            //cout<<netGravDilFactor<<endl;

            m.second.updateVelocity(this->dt*netGravDilFactor,v_mag_ship);
        }

        //position update
        for (auto& m: matters) {
            m.second.updatePosition();
        }
    }

    //Getting
    Vector3 getGravitationalField(Vector3 effectPosition,int id=-1){
        Vector3 intensity;
        gravDilFactor=0;
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
