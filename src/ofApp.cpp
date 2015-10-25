
#include "ofApp.h"

Params param; //Definition of global variable

void Params::setup() {
    
	eCenter = ofPoint( ofGetWidth() / 2, ofGetHeight() / 2 ); // the starting point for the particles
	eRad = 80; // emitter radius
	velRad = 250; // initial velocity limit
	lifeTime = 2.0; // how long the particles will live in seconds
    
	rotate = 120; // how much the particles should rotate
}

Particle::Particle() {
    
	live = false; // setting the particles to false = not alive
}

ofPoint randomPointInCircle( float maxRad ) { // this function creates a random pathway for each particle
    
	ofPoint pnt;
	float rad = ofRandom( 0, maxRad ); // setting the radians and the angle to a random value
	float angle = ofRandom( 0, M_TWO_PI );
	pnt.x = cos( angle ) * rad;
	pnt.y = sin( angle ) * rad;
    
	return pnt; // returning an ofPoint
}

void Particle::setup() {
    
	pos = param.eCenter + randomPointInCircle( param.eRad );
	vel = randomPointInCircle( param.velRad );
	time = 0;
	lifeTime = param.lifeTime;
	live = true;
}

void Particle::update( float dt ) {
    
	if ( live ) { // if they are alive then update
        
		
		vel.rotate( 0, 0, param.rotate * dt ); // This rotates the particles
        
		pos += vel * dt;    // update the position (Euler method)
        
		time += dt;
		if ( time >= lifeTime ) { // Checking if time is greater or equal to lifeTime
			live = false;   // if that's the case the particle then dies
		}
	}
}

void Particle::draw() {
    
	if ( live ) { // if the particles are alive then call ..
        
		float size = ofMap(
                fabs(time - lifeTime/2), 0, lifeTime/2, 2, 0.5 ); // mapping the size of the particles
        
		//Compute color
		ofColor color = ofColor::white; // setting the color of the particles
		float bright = ofMap( time, 0, lifeTime, 128, 60 ); // changing the brightness
		color.setBrightness( bright ); // setting the brightness to the mapped value
		ofSetColor( color );
        
		ofCircle( pos, size );  //Draw particle
	}
}

void ofApp::setup() {
    
	ofSetFrameRate( 60 );	//Set screen frame rate
    
	int w = ofGetWidth(); // setting w to the width of the sketch and h to be the height
	int h = ofGetHeight();
	fbo.allocate( w, h, GL_RGB32F_ARB ); // Before you use fbo you need to allocate it
	fbo.begin();
	ofBackground(0, 0, 0); //Fill buffer with black color
	fbo.end();
    
    param.setup();		//Global parameters
    
	history = 0.80;
	bornRate = 2000; // number of particles
    
	bornCount = 0;
	time0 = ofGetElapsedTimef(); // Returns the elapsed time since the application started in seconds as a float
}

void ofApp::update(){
    
	
	float time = ofGetElapsedTimef(); // storing the time since the application started in the variable time
	float dt = ofClamp( time - time0, 0, 0.1 ); // Restricts a value to be within a specified range defined by values min and max
	time0 = time;

	int i = 0;
    
	while ( i < p.size() ) { // looping through the array
        
		if ( !p[i].live ) {
			p.erase( p.begin() + i ); // if the particle is not active then erase it, else increase i
            
		}else {
            
			i++; // increse i for every frame
		}
	}

	bornCount += dt * bornRate; //Update bornCount value
    
	if ( bornCount >= 1 ) {
        
		int bornN = int( bornCount );// How many are born
		bornCount -= bornN;          // Correct bornCount value
        
		for (int i = 0; i < bornN; i++) {
			Particle newP;
			newP.setup();            // Start a new particle
			p.push_back( newP );     // Adding the particles to array
		}
	}
	
	for (int i = 0; i < p.size(); i++) { // looping through the array p.size()
		p[i].update( dt ); //Update the particles
	}
}

void ofApp::draw() {
	   
	//1. Drawing to buffer
	fbo.begin();
    
    ofEnableAlphaBlending();    // turn on alpha blending
    ofSetColor(0,0,0,70);    // black background with transparency creating a trail effect
    
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() ); // drawing a black rectangle for every frame
    
	ofDisableAlphaBlending();  //Disable transparency
    
	for (int i = 0; i < p.size(); i++) { // looping through the array p.size()
		p[i].draw(); // Drawing the particles
	}
    
	fbo.end();
    
	//2. Draw buffer on the screen
	
	fbo.draw( 0, 0 );
}

