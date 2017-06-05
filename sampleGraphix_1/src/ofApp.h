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
    float width, height, quarterWidth, quarterHeight;
    ofVec3f center;
    
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
    particleMode currentMode;
    string currentModeStr;
    vector <Particle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    int numParticles, randomParticle;;
    bool isAttractPointMoving, changeOnce, changeParticleOnce, timeToDrawLogo, timeToDrawBigNumber;
    ofPoint attractPointPos;
    float attractPointSpeed;
    int timePerAttractionPhase, timeToChangeParticles, counter;
};
