#include "ofApp.h"
#include "ofxImgSizeUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
    
    
    
    camWidth = 1920 ;
    camHeight = 1080 ;
    
    ofSetWindowShape(camWidth, camHeight);
    
    fullScreenSize = ofxImgSizeUtils::getSizeRatio(ofGetWidth(), ofGetHeight(), camWidth, camHeight, false);
    
    camera.setDeviceID(3);
    camera.setup(camWidth, camHeight);
    camera.listDevices();
    
    arToolKitManager.setup(fullScreenSize.x, fullScreenSize.y);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    camera.update();
    if(camera.isFrameNew()) {
        
        // resize image
        ofImage img;
        img.setFromPixels(camera.getPixels());
        img.resize(fullScreenSize.x, fullScreenSize.y);
        arToolKitManager.update(img);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    camera.draw(0.0,0.0, fullScreenSize.x, fullScreenSize.y);
    arToolKitManager.debugDraw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'f')
        ofToggleFullscreen();
    
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
