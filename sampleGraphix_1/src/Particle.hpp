//
//  Particle.hpp
//  20170516_someFaces
//
//  Created by Andrew McCausland on 5/17/17.
//
//

#pragma once
#include "ofMain.h"

class Particle{
    
public:
    Particle();
    
    void setAttractPoints( vector <ofPoint> * attract );
    
    void reset(ofRectangle _viewPort);
    void update(ofPoint attractPointPos);
    void draw();
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    
    float drag;
    float scale;
    
    vector <ofPoint> * attractPoints;
};
