#include <iostream>
#include "vector3.h"
#include <math.h>
#include "constant.h"
using namespace std;

class Matter{
private:
    double mass;
    double time;
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    double dt;
public:
    Matter(){
        this->setMass();
        time=0.0;
        dt=0;
    }

    void operator = (const Matter &m) {
        this->mass=m.mass;
        this->time=m.time;
        this->position=m.position;
        this->velocity=m.velocity;
        this->acceleration=m.acceleration;
    }

    //Setting values
    void setMass(double mass=1){
        this->mass=mass;
    }
    void setPosition(Vector3 position){
       this->position.setValue(position);
    }
    void setVelocity(Vector3 velocity){
        this->velocity.setValue(velocity);
    }

    //updating
    void updatePosition(){
        time=time+dt;
        position=position+velocity.scale(dt);
    }

    void updateVelocity(double dt,double v_mag_ship){
        double v_mag=velocity.getMagnitude();
        double dv=v_mag_ship-v_mag;
        if(dv>=0){
            dv=dv/(1-v_mag_ship*v_mag/(C*C));
            this->dt=dt/sqrt(1-dv*dv/(C*C));
        }else{
            dv=dv/(1-v_mag_ship*v_mag/(C*C));
            this->dt=dt*sqrt(1-dv*dv/(C*C));
        }
        velocity=velocity+acceleration.scale(this->dt);
    }

    void updateAcceleration(Vector3 intensity){
        acceleration=intensity;
    }

    //Getting values
    double getMass(){
        return mass;
    }
    Vector3 getPosition(){
        return position;
    }

    Vector3 getGravitationalField(Vector3 effectPosition){
        Vector3 intensity;

        double a = (1-(velocity.getValue1()*velocity.getValue1()+velocity.getValue2()*velocity.getValue2()+
                       velocity.getValue3()*velocity.getValue3())/(C*C));
		double b = -2.0*(time-(velocity.getValue1()*effectPosition.getValue1()+velocity.getValue2()*effectPosition.getValue2()+
                         velocity.getValue3()*effectPosition.getValue3())/(C*C));
		double c = (time*time-(effectPosition.getValue1()*effectPosition.getValue1()+effectPosition.getValue2()*effectPosition.getValue2()
				+effectPosition.getValue3()*effectPosition.getValue3())/(C*C));

        double temp=sqrt(b*b-4.0*a*c);

		if(temp<0.0) {
			return intensity;
		}
		double t =  (-b-temp)/(2.0*a);

		if(t<0.0) {
			t=(-b+temp)/(2.0*a);
		}
		if(t>time) {
			return intensity;
		}

		double dt = time-t;

        Vector3 causePosition = position-Vector3::getVector(velocity.getValue1()*dt,velocity.getValue2()*dt,velocity.getValue3()*dt);
        Vector3 dr = effectPosition-causePosition;
        double GM = G*mass;
        double r_mag=dr.getMagnitude();

        double L2=GM*r_mag/(1-3*GM/(C*C*r_mag));

        intensity = dr.scale(-(GM/(r_mag*r_mag*r_mag)+3*GM*L2/(C*C*r_mag*r_mag*r_mag*r_mag*r_mag)));

        return intensity;
    }



};

