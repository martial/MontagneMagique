#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    imgPlayer.setup();
    imgPlayer.loadFolder("fourmis_1_sprites");
    imgPlayer.setFps(31);
    imgPlayer.setLoop(true);
    imgPlayer.play();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    imgPlayer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableAlphaBlending();
    
    ofBackground(255);
    ofSetColor(255);

    ofPushMatrix();
    ofTranslate(ofGetWidth() * .5 - imgPlayer.width * .5, ofGetHeight() * .5 - imgPlayer.height * .5);
    imgPlayer.draw(0.0, 0.0);
    ofPopMatrix();
    
    ofSetColor(0);
    ofDrawBitmapString(ofToString(imgPlayer.fps), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    float fps = ofMap(x, 0, ofGetWidth(), 0, 240);
    imgPlayer.setFps(fps);

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
