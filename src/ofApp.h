#pragma once

#include "ofMain.h"

class Particle {
    
public:
    Particle();
    void setup(); // start particle
    void update( float dt); // recalculate physics
    void draw(); // draw particle
    
    ofPoint pos; // position
    ofPoint vel; // velocity
    float time; // time of living
    float lifeTime; // allowed lifetime
    bool live; // is particle alive
    
};

class Params {
public:
    void setup();
    ofPoint eCenter; // emitter center
    float eRad; // emitter radius
    float velRad; // initial velocity limit
    float lifeTime; // lifetime in seconds
    
    float rotate; // direction rotation speed in angles per second
    
};

extern Params param; // declaration of a global variable


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
   vector<Particle> p; // particles array
    ofFbo fbo; // offscreen buffer for trails
    
    float history; // control parameter for trails
    float time0; // time value for computing dt
    
    float bornRate; // particles born rate per second
    float bornCount; // integrated number particles to be born

};
