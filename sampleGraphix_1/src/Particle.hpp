//
//  Particle.hpp
//  20170516_someFaces
//
//  Created by Andrew McCausland on 5/17/17.
//
//

#pragma once
#include "ofMain.h"

enum particleMode{
    PARTICLE_MODE_ATTRACT = 0,
    PARTICLE_MODE_REPEL,
    PARTICLE_MODE_NEAREST_POINTS,
    PARTICLE_MODE_NOISE
};

class Particle{
    
public:
    Particle();
    
    void setMode(particleMode newMode);
    void setAttractPoints( vector <ofPoint> * attract );
    
    void reset(ofRectangle _viewPort);
    void update(ofPoint attractPointPos);
    void draw();
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    
    float drag;
    float scale;
    
    particleMode mode;
    
    vector <ofPoint> * attractPoints;
};
