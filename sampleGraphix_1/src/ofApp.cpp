#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofDisableSmoothing();
    ofSetSphereResolution(5);
    ofSetVerticalSync(true);
    
    viewPort.set((ofGetWidth()/2) - (1024/2), (ofGetHeight()/2) - (768/2) - 30, 1024, 768);
    
    margin = 20;
    
    head.loadModel("3d_models/FaceModel.obj");
    head.setScale(0.4, 0.4, 0.4);
    headWarpAmount = 1.;
    
    colorScheme.resize(3);
    colorScheme[0].set(255); //bg // blue
    colorScheme[1].set(30,20,20); //fore // black
    colorScheme[2].set(255,150,0); //highlight // orange
    
    focusColorGlow_sine = focusColorGlow_angle = 0;
    
    font.load("fonts/DINEng", 150);
    font.setLetterSpacing(1.0);
    font.setSpaceSize(1);
    txtHeight = font.getLineHeight();
    
    bigFont.load("fonts/DINEng", 760);
    
    logo.load("img/20170523_sampleGraphixLogo1.png");
    logo.resize(450,200);
    
    shader.load("shaders/shaderVert.c", "shaders/shaderFrag.c");
    fbo.allocate( ofGetWidth(), ofGetHeight() );
    
    //particles
    numParticles = 20000;
    p.assign(numParticles, Particle());
    resetParticles();
    particlesPos.set(0,0,0);
    
    drawParticles = false;
    isAttractPointMoving = true;
    changeParticleModeOnce = false;
    attractPointPos.set(center.x/2,height/2);
    attractPointSpeed = 10;
    timePerAttractionPhase = 20;
    changeParticles = 5;
    changeParticleOnce = false;
    randomParticle = 0;
    drawLogo = false;
    
    isRightOn = isLeftOn = false;
    littleNumbersPos.set(0,0,0);
    bigNumberPos.set(0,0,0);
    logoPos.set(0,0,0);
    switchOnce.resize(4);
    for(int i = 0; i < switchOnce.size();i++){
        switchOnce[i] = false;
    }
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
    }
    
    attractPointsWithMovement = attractPoints;
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setAttractPoints(&attractPointsWithMovement);;
        p[i].reset(viewPort);
    }
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
    
    focusColorGlow_angle = focusColorGlow_a(focusColorGlow_angle);
    focusColorGlow_sine = focusColorGlow_s(focusColorGlow_sine, focusColorGlow_angle);
    if(focusColorGlow_sine <= 0){
        focusColorGlow_sine = 0;
    }
    
    
    colorScheme[0].setHsb(ofMap(focusColorGlow_sine, 0, 162, 150, 190), 50, 255);
    colorScheme[1].setHsb(ofMap(focusColorGlow_sine, 0, 162, 240, 25), 255, 30);
    //    colorScheme[2].g = ofMap(focusColorGlow_sine, 0, 162, 150, 50);
    
    if(headWarpAmount < 1){
        headWarpAmount += 0.01;
    } else if(headWarpAmount >= 1){
        headWarpAmount = 0;
    }
    
    if(angle >= TWO_PI){
        angle = 0;
    } else {
        angle += 0.1;
    }
    
    //particles
    if(!isAttractPointMoving){
        attractPointPos.set(quarterWidth,center.y);
    } else {
        attractPointPos.y += attractPointSpeed;
        if(attractPointPos.y > height){
            attractPointPos.y = 0;
        }
        if(attractPointPos.x <= viewPort.getLeft()){
            attractPointPos.x = quarterWidth;
        }
    }
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].update(attractPointPos);
    }
    
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
    }
    
    if(ofToInt(ofToString(ofGetElapsedTimef()))%timePerAttractionPhase == 0 && !changeParticleModeOnce){
        isAttractPointMoving = !isAttractPointMoving;
        changeParticleModeOnce = true;
    } else if(ofToInt(ofToString(ofGetElapsedTimef()))%timePerAttractionPhase != 0){
        changeParticleModeOnce = false;
    }
    
    //timer
    if(ofToInt(ofToString(ofGetElapsedTimef()))%changeParticles == 0 && !changeParticleOnce){
        randomParticle = ofRandom(numParticles);
        changeParticleOnce = true;
    } else if(ofToInt(ofToString(ofGetElapsedTimef()))%changeParticles != 0){
        changeParticleOnce = false;
    }
    
    if(drawParticles && !switchOnce[0]){
        //this needs to happen once
        if(!isLeftOn){
            particlesPos = leftCenter;
            isLeftOn = true;
        } else { // if !isRightOn
            particlesPos = rightCenter;
            isRightOn = true;
        }
        switchOnce[0] = true;
    }
    
    if(drawLittleNumbers && !switchOnce[1]){
        //this needs to happen once
        if(!isLeftOn){
            littleNumbersPos.set(viewPort.getLeft() + margin,0,0);
            isLeftOn = true;
        } else { // if !isRightOn
            littleNumbersPos.set(center.x,0,0);
            isRightOn = true;
        }
         switchOnce[1] = true;
    }
    
    if(drawBigNumber && !switchOnce[2]){
        if(!isLeftOn){
            bigNumberPos.set(quarterWidth,0,0);
            isLeftOn = true;
        } else {
            bigNumberPos.set(threeQuarterWidth,0,0);
            isRightOn = true;
        }
        switchOnce[2] = true;
    }
    
    if(drawLogo && !switchOnce[3]){
        if(!isLeftOn){
            logoPos.set(quarterWidth,0,0);
            isLeftOn = true;
        } else {
            logoPos.set(threeQuarterWidth,0,0);
            isRightOn = true;
        }
        switchOnce[3] = true;
        
        fbo.begin();
        ofClear(255,255,255, 0);
        fbo.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(colorScheme[0]);
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -1000, 1000);
    
    //particles area
    bool showInnerCircle = false;
    if(drawParticles){
        ofSetColor(colorScheme[1]);
        ofPushMatrix();{
            if(particlesPos == rightCenter){
            ofTranslate((threeQuarterWidth - quarterWidth), 0);
            }
            ofDrawRectangle(viewPort.getLeft() + margin, viewPort.getTop() + margin, center.z - 10, ((width - viewPort.getLeft())/2) - (margin*2), (height - viewPort.getTop()) - (margin*2));
        }ofPopMatrix();
    
        ofSetColor(colorScheme[0]);
        ofPushMatrix();{
            if(particlesPos == rightCenter){ //if we're on the right
                ofTranslate((threeQuarterWidth - quarterWidth), 0);
            }
            if(ofGetFrameNum()%2 == 0){
                for(unsigned int i = 0; i < p.size(); i++){
                    p[i].draw();
                }
            }
        }ofPopMatrix();
        if(!drawLogo){
            ofPushMatrix();{
                ofEnableDepthTest();{
                    if(particlesPos == rightCenter){
                        ofTranslate(threeQuarterWidth,center.y,center.z);
                    } else {
                        ofTranslate(quarterWidth,center.y,center.z);
                    }
                    ofScale(headWarpAmount/2,0.5,0.5);
                    ofRotateY(ofGetElapsedTimef()*100);
                    ofSetColor(colorScheme[1]);
                    head.drawFaces();
                    showInnerCircle = false;
                }ofDisableDepthTest();
            }ofPopMatrix();
        }
    }
    
    //logo
    int padding = 20;
    ofRectangle logoBox = ofRectangle(logoPos.x - (logo.getWidth()/2), center.y + margin, (width/2) - (margin*2), (height/2) - (margin*2));
    
    if(drawLogo){
        fbo.begin();{
            ofSetColor(colorScheme[1]);
            logo.draw(logoBox.getLeft(), logoBox.getCenter().y - (logo.getHeight()/2));
            ofSetColor(255,0,0);
            ofDrawBitmapString("logoBox.getLeft() = " + ofToString(logoBox.getLeft()), mouseX, mouseY + 60);
        }fbo.end();
        shader.begin();{
            float time = ofGetElapsedTimef()/2;
            shader.setUniform1f( "time", time );
            if(ofGetFrameNum()%2 == 0){
                fbo.draw(0,0);
            }
        }shader.end();
    }
    
    //dataTxt
    ofSetColor(colorScheme[1]);
    
    if(drawLittleNumbers){
        font.drawString("#" + ofToString(randomParticle) + "\n" + ofToString(p[randomParticle].pos.x) + "\n" + ofToString(p[randomParticle].pos.y), littleNumbersPos.x, viewPort.getTop() + txtHeight + padding);
    }
    
    if(drawBigNumber){
        string charr;
        charr = ofToString(ofGetFrameRate()).substr(ofToString(ofGetFrameRate()).size() - 1,1);
        ofRectangle box = bigFont.getStringBoundingBox(charr, 0, 0);
        bigFont.drawString(charr, bigNumberPos.x - (box.width/2), viewPort.getBottom() - margin);
    }
    
    //head
    ofPushMatrix();{
        ofEnableDepthTest();{
            if(drawLogo){
                ofTranslate(logoPos.x,(quarterHeight) - margin, center.z);
                ofScale(headWarpAmount,1,1);
                showInnerCircle = true;
            }
            ofRotateY(ofGetElapsedTimef()*100);
            ofSetColor(colorScheme[1]);
            head.drawFaces();
        }ofDisableDepthTest();
        ofNoFill();
        ofSetColor(colorScheme[0]);
        if(ofGetFrameNum()%2 == 0 && showInnerCircle){
            ofDrawCircle(0,0,20);
        }
        ofFill();
    }ofPopMatrix();
    
    //viewPort border
    ofSetColor(colorScheme[1]);
    ofFill();
    ofBeginShape();{
        ofVertex(0,0);
        ofVertex(ofGetWidth(),0);
        ofVertex(ofGetWidth(),ofGetHeight());
        ofVertex(0,ofGetHeight());
        
        ofNextContour(true);
        
        ofVertex(viewPort.getLeft(),viewPort.getTop());
        ofVertex(viewPort.getRight(),viewPort.getTop());
        ofVertex(viewPort.getRight(),viewPort.getBottom());
        ofVertex(viewPort.getLeft(),viewPort.getBottom());
    }ofEndShape(true);
    
    //debug info
//    ofSetColor(255,0,0);
//    ofDrawCircle(leftCenter, 10);
//    ofDrawCircle(rightCenter, 10);
//    ofDrawBitmapString(ofGetFrameRate(), ofGetMouseX() + 10, ofGetMouseY());
//    ofDrawBitmapString("isLeftOn = " + ofToString(isLeftOn), ofGetMouseX() + 10, ofGetMouseY() + 10);
//    ofDrawBitmapString("isRightOn = " + ofToString(isRightOn), ofGetMouseX() + 10, ofGetMouseY() + 20);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' ' ){
        resetParticles();
    }
    
    if(key == '1'){
        if(drawParticles){ // if this is already on
            drawParticles = false; // turn it off
            if(particlesPos == leftCenter){ //if it was on the left side
                isLeftOn = false; //turn the left switch back off
            } else {
                isRightOn = false; //otherwise it was on th right so turn the right switch back off
            }
        } else { // otherwise
            if(!isLeftOn || !isRightOn){ // if there's empty space
                drawParticles = true; // turn it on
            }
        }
        switchOnce[0] = false;
    }
    if(key == '2'){
        if(drawLittleNumbers){ // if this is already on
            drawLittleNumbers = false; // turn it off
            if(littleNumbersPos.x == viewPort.getLeft() + margin){ //if it was on the left side
                isLeftOn = false; //turn the left switch back off
            } else {
                isRightOn = false; //otherwise it was on th right so turn the right switch back off
            }
        } else { // otherwise
            if(!isLeftOn || !isRightOn){ // if there's empty space
                drawLittleNumbers = true; // turn it on
            }
        }
         switchOnce[1] = false;
    }
    if(key == '3'){
        if(drawLogo){ // if this is already on
            drawLogo = false; // turn it off
            if(logoPos.x == quarterWidth){ //if it was on the left side
                isLeftOn = false; //turn the left switch back off
            } else {
                isRightOn = false; //otherwise it was on th right so turn the right switch back off
            }
        } else { // otherwise
            if(!isLeftOn || !isRightOn){ // if there's empty space
                drawLogo = true; // turn it on
            }
        }
        switchOnce[3] = false;
    }
    if(key == '4'){
        if(drawBigNumber){ // if this is already on
            drawBigNumber = false; // turn it off
            if(bigNumberPos.x == quarterWidth){ //if it was on the left side
                isLeftOn = false; //turn the left switch back off
            } else {
                isRightOn = false; //otherwise it was on th right so turn the right switch back off
            }
        } else { // otherwise
            if(!isLeftOn || !isRightOn){ // if there's empty space
                drawBigNumber = true; // turn it on
            }
        }
        switchOnce[2] = false;
    }
    
}

//--------------------------------------------------------------

float ofApp::focusColorGlow_a(float angle){
    float speed = 0.005;
    float framerateMult = 60.0f/(1.0f/ofGetLastFrameTime());
    angle += (speed * framerateMult);
    if (angle >= TWO_PI) {
        angle = 0;
    }
    return angle;
}

float ofApp::focusColorGlow_s(float sine, float angle){
    float minAlpha = 75;
    float maxAlpha = 50;
    sine = (sin(angle)*100) + (maxAlpha - ((maxAlpha - minAlpha)/2));
    return sine;
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
