//
//  Particle.cpp
//  20170516_someFaces
//
//  Created by Andrew McCausland on 5/17/17.
//
//

#include "Particle.hpp"
//------------------------------------------------------------------
Particle::Particle(){
    attractPoints = NULL;
}

//------------------------------------------------------------------
void Particle::setAttractPoints( vector <ofPoint> * attract ){
    attractPoints = attract;
}

//------------------------------------------------------------------
void Particle::reset(ofRectangle _viewPort){
    
    pos.x = ofRandom(_viewPort.getLeft(), ofGetWidth()/2);
    pos.y = ofRandom(_viewPort.getTop(), _viewPort.getBottom());
    
    vel.x = ofRandom(-3.9, 3.9);
    vel.y = ofRandom(-3.9, 3.9);
    
    frc   = ofPoint(0,0,0);
    
    scale = ofRandom(0.5, 1.0);
    
    drag  = ofRandom(0.95, 0.998);
}

//------------------------------------------------------------------
void Particle::update(ofPoint attractPointPos){
    
    ofPoint attractPt = attractPointPos;
    frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
    frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point
    
    vel *= drag; //apply drag
    vel += frc * 0.6; //apply force // originally 0.6

    pos += vel;
    
    if( pos.x > ofGetWidth()*2){
        pos.x = ofGetWidth()*2;
        vel.x *= -1.0;
    }else if( pos.x < -ofGetWidth()){
        pos.x = -ofGetWidth();
        vel.x *= -1.0;
    }
    if( pos.y > ofGetHeight()*2 ){
        pos.y = ofGetHeight()*2;
        vel.y *= -1.0;
    }
    else if( pos.y < -ofGetHeight() ){
        pos.y = -ofGetHeight();
        vel.y *= -1.0;
    }
    
}

//------------------------------------------------------------------
void Particle::draw(){
    ofDrawCircle(pos.x, pos.y, scale);
}
