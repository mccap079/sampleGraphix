#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //    ofSetFullscreen(true);
    ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofDisableSmoothing();
    ofSetSphereResolution(5);
    ofSetVerticalSync(true);
    
    viewPort.set((ofGetWidth()/2) - (1024/2), (ofGetHeight()/2) - (768/2) - 30, 1024, 768);
    
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
    numParticles = 15000;
    p.assign(numParticles, Particle());
    currentMode = PARTICLE_MODE_ATTRACT;
    currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
    resetParticles();
    
    isAttractPointMoving = true;
    changeOnce = false;
    attractPointPos.set(center.x/2,height/2);
    attractPointSpeed = 10;
    timePerAttractionPhase = 20;
    timeToChangeParticles = 5;
    changeParticleOnce = false;
    randomParticle = 0;
    timeToDrawLogo = false;
    counter = 0;
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
    }
    
    attractPointsWithMovement = attractPoints;
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
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
        p[i].setMode(currentMode);
        p[i].update(attractPointPos);
    }
    
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
    }
    
    if(ofToInt(ofToString(ofGetElapsedTimef()))%timePerAttractionPhase == 0 && !changeOnce){
        isAttractPointMoving = !isAttractPointMoving;
        changeOnce = true;
    } else if(ofToInt(ofToString(ofGetElapsedTimef()))%timePerAttractionPhase != 0){
        changeOnce = false;
    }
    
    //timer
    if(ofToInt(ofToString(ofGetElapsedTimef()))%timeToChangeParticles == 0 && !changeParticleOnce){
        randomParticle = ofRandom(numParticles);
        changeParticleOnce = true;
        counter++;
        if(counter == 3){
            timeToDrawLogo = true;
            counter = 0;
        } else if(timeToDrawLogo){
            timeToDrawLogo = false;
        }
        if(counter == 1){
            timeToDrawBigNumber = true;
        } else {
            timeToDrawBigNumber = false;
        }
    } else if(ofToInt(ofToString(ofGetElapsedTimef()))%timeToChangeParticles != 0){
        changeParticleOnce = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(colorScheme[0]);
    
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -1000, 1000);
    int margin = 20;
    
    //particles area
    ofSetColor(colorScheme[1]);
    ofDrawRectangle(viewPort.getLeft() + margin, viewPort.getTop() + margin, center.z - 10, ((width - viewPort.getLeft())/2) - (margin*2), (height - viewPort.getTop()) - (margin*2));
    
    ofSetColor(colorScheme[0]);
    ofPushMatrix();{
        if(ofGetFrameNum()%2 == 0){
            for(unsigned int i = 0; i < p.size(); i++){
                p[i].draw();
            }
        }
    }ofPopMatrix();
    
    if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
        for(unsigned int i = 0; i < attractPoints.size(); i++){
            ofNoFill();
            ofDrawCircle(attractPointsWithMovement[i], 10);
            ofFill();
            ofDrawCircle(attractPointsWithMovement[i], 4);
        }
    }
    
    //logo
    int padding = 20;
    ofRectangle logoBox = ofRectangle(center.x + margin, center.y + margin, (width/2) - (margin*2), (height/2) - (margin*2));
    
    if(timeToDrawLogo){
        fbo.begin();{
            ofSetColor(colorScheme[1]);
            logo.draw(logoBox.getLeft(), logoBox.getCenter().y - (logo.getHeight()/2));
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
    if(!timeToDrawLogo && !timeToDrawBigNumber){
        font.drawString("#" + ofToString(randomParticle) + "\n" + ofToString(p[randomParticle].pos.x) + "\n" + ofToString(p[randomParticle].pos.y), center.x, viewPort.getTop() + txtHeight + padding);
    } else if(timeToDrawBigNumber){
        string charr;
        charr = ofToString(ofGetFrameRate()).substr(ofToString(ofGetFrameRate()).size() - 1,1);
        ofRectangle box = bigFont.getStringBoundingBox(charr, 0, 0);
        bigFont.drawString(charr, center.x + (center.x - quarterWidth) - (box.width/2), viewPort.getBottom() - margin);
    }
    
    //head
    bool showInnerCircle = false;
    ofPushMatrix();{
        ofEnableDepthTest();{
            if(timeToDrawLogo){
                ofTranslate(center.x + (center.x - quarterWidth),(quarterHeight) - margin, center.z);
                ofScale(headWarpAmount,1,1);
                showInnerCircle = true;
            } else {
                ofTranslate(quarterWidth,center.y,center.z);
                ofScale(headWarpAmount/2,0.5,0.5);
                showInnerCircle = false;
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
    ofSetColor(255,200,0);
    ofDrawBitmapString(ofGetFrameRate(), ofGetMouseX() + 10, ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == '1'){
        currentMode = PARTICLE_MODE_ATTRACT;
        currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
    }
    if( key == ' ' ){
        resetParticles();
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
    if(key == '1'){
        cout << "1 pressed" << endl;
    }
    
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
