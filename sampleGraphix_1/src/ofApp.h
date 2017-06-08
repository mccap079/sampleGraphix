#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Particle.hpp"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void resetParticles();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    float focusColorGlow_a(float angle);
    float focusColorGlow_s(float sine, float angle);
    
    ofRectangle viewPort;
    float width, height, quarterWidth, quarterHeight, threeQuarterWidth;
    ofVec3f center, leftCenter, rightCenter;
    int margin;
    ofPoint particlesPos, littleNumbersPos, bigNumberPos, logoPos;
    vector <bool> switchOnce;
    
    ofxAssimpModelLoader head;
    float headWarpAmount;
    float headOrientation;
    
    vector<ofColor>colorScheme;
    float focusColorGlow_sine, focusColorGlow_angle;
    
    ofTrueTypeFont font, bigFont;
    float txtHeight;
    ofImage logo;
    ofShader shader;
    ofFbo fbo;
    float angle;
    
    //particles
    vector <Particle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    int numParticles, randomParticle;;
    bool isAttractPointMoving, changeParticleModeOnce, changeParticleOnce, drawLogo, drawBigNumber, drawLittleNumbers, drawParticles;
    ofPoint attractPointPos;
    float attractPointSpeed;
    int timePerAttractionPhase, changeParticles;
    
    bool isRightOn, isLeftOn;
};
