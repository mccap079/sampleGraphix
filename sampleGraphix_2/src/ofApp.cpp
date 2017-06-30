#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofDisableSmoothing();
    ofSetSphereResolution(5);
    ofSetVerticalSync(true);
    
    viewPort.set((ofGetWidth()/2) - (1024/2), (ofGetHeight()/2) - (768/2) - 30, 1024, 768);

}

//--------------------------------------------------------------
void ofApp::update(){
    width = viewPort.getRight();
    height = viewPort.getBottom();
    quarterWidth = viewPort.getCenter().x - ((viewPort.getCenter().x - viewPort.getLeft())/2);
    quarterHeight = viewPort.getCenter().y - ((viewPort.getCenter().y - viewPort.getTop())/2);
    center.set(ofGetWidth()/2, ofGetHeight()/2, 0);
    threeQuarterWidth = center.x + (center.x - quarterWidth);
    leftCenter.set(quarterWidth, center.y, 0);
    rightCenter.set(threeQuarterWidth, center.y, 0);

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
