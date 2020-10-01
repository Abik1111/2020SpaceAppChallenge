#include <iostream>
#include "vector3.h"
#include <math.h>
#include "constant.h"
using namespace std;

class Matter{
private:
    double mass;
    double time;
	double temperature;
	double emissivity;
	double radius;
	bool blackBody;
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    double gamma;
public:
    Matter(){
        this->setMass();
        gamma=1.0;
        time=0.0;
		this->setEmissivity(1);
		blackBody = false;
		this->setTemperature(273);
    }

    void operator = (const Matter &m) {
        this->mass=m.mass;
        this->time=m.time;
        this->position=m.position;
        this->velocity=m.velocity;
        this->acceleration=m.acceleration;
        this->gamma=m.gamma;
		this->radius = m.radius;
		this->temperature = m.temperature;
		this->emissivity = m.emissivity;
		this->blackBody = m.blackBody;
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
	void setRadius(double radius) {
		this->radius = radius;
	}
	void setEmissivity(double emissivity) {
		this->emissivity = emissivity;
	}
	void setTemperature(double temp) {
		this->temperature = temp;
	}
	void setBlackBody(bool isBlackBody) {
		this->blackBody = isBlackBody;
	}

    //updating
    void updatePosition(double dt){
        time=time+dt;
        position=position+velocity.scale(dt);
    }

    void updateVelocity(double dt){
        velocity=velocity+acceleration.scale(dt);
		double vecMag = velocity.getMagnitude();
		this->gamma = 1 / (sqrt(1 - (vecMag*vecMag) / (C*C)));
    }

    void updateAcceleration(Vector3 intensity){
        acceleration=intensity;
    }
	void updateTemperature(double temperature) {
		this->temperature = temperature;
		
	}

    //Getting values
    Vector3 getPosition(){
        return position;
    }

	double getRadius() {
		return radius;
	}
	double getEmissivity() {
		return this->emissivity;
	}
	double getTemperature() {
		return temperature;
	}
	bool isBlackBody() {
		return blackBody;
	}

	double calculateTemperature(Vector3 effectPosition) {
		double tPower4=0;

		double a = (1 - (velocity.getValue1()*velocity.getValue1() + velocity.getValue2()*velocity.getValue2() +
			velocity.getValue3()*velocity.getValue3()) / (C*C));
		double b = -2.0*(time - (velocity.getValue1()*effectPosition.getValue1() + velocity.getValue2()*effectPosition.getValue2() +
			velocity.getValue3()*effectPosition.getValue3()) / (C*C));
		double c = (time*time - (effectPosition.getValue1()*effectPosition.getValue1() + effectPosition.getValue2()*effectPosition.getValue2()
			+ effectPosition.getValue3()*effectPosition.getValue3()) / (C*C));

		double temp = sqrt(b*b - 4.0*a*c);

		if (temp < 0.0) {
			return tPower4;
		}
		double t = (-b - temp) / (2.0*a);

		if (t < 0.0) {
			t = (-b + temp) / (2.0*a);
		}
		if (t > time) {
			return tPower4;
		}

		double dt = time - t;

		Vector3 causePosition = position - Vector3::getVector(velocity.getValue1()*dt, velocity.getValue2()*dt, velocity.getValue3()*dt);
		Vector3 dr = effectPosition - causePosition;
		double drMag = dr.getMagnitude();
		//cout << temperature << endl;
		tPower4 = emissivity*(radius*radius*temperature*temperature*temperature*temperature)/(4*drMag*drMag) ;
		//cout << tPower4 << endl;

		return tPower4;
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
        intensity = dr.scale(-GM/(r_mag*r_mag*r_mag));

        return intensity;
    }



};

