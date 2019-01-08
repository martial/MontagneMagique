#include "ofApp.h"
#include "ofxImgSizeUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
    
    // some config
    ofFile file;
    file.open("config.json");
    
    if(file.exists())
        file >> configJson;
    else
        exit();
    
    intputMode          = configJson["auto-start-mode"];
    videoInputWidth     = 0;
    videoInputHeight    = 0;
    setInputMode(intputMode);
    
    syphonLayer.setName("MM Layer");
    
    messageString = "";
    
    app.setup();
    
}

void ofApp::setInputMode(int mode) {
    
    intputMode = mode;
    
    int oldVideoInputWidth  = videoInputWidth;
    int oldVideoInputHeight = videoInputHeight;
    
    int deviceId            = configJson["camera-device-id"];
    string filename         = configJson["video-filename"];
    string videoUrl         = "videos/" + filename;
    
    switch (intputMode) {
            
        case INPUT_VIDEO:
            
            videoInput.load(videoUrl);
            videoInputWidth    = videoInput.getWidth();
            videoInputHeight   = videoInput.getHeight();
            videoInput.play();
            videoInput.setLoopState(OF_LOOP_NORMAL);

            break;
        
        case INPUT_CAMERA:
            
            videoInputWidth    = 640 ;
            videoInputHeight   = 480 ;
            
            cameraInput.listDevices();
            cameraInput.setDeviceID(deviceId);
            cameraInput.setup(videoInputWidth, videoInputHeight);
            
            break;
            
        case INPUT_SYPHON:
            
            syphonInput.setup();
            syphonInput.set("", configJson["syphon-input-name"]);

            videoInputWidth    = 640 ;
            videoInputHeight   = 480 ;
            
            syphonFbo.allocate(videoInputWidth, videoInputHeight, GL_RGB);
            
            break;
            
    }
    
    // reset trackers if size is different
    if(oldVideoInputWidth != videoInputWidth ||  oldVideoInputHeight != videoInputHeight ) {
        
        ofSetWindowShape(videoInputWidth, videoInputHeight);
        cameraRectCanvas = ofxImgSizeUtils::getCenteredRect(ofGetWidth(), ofGetHeight(), videoInputWidth, videoInputHeight, false);
        
        app.getArToolKitManager().clean();
        app.setupTrackers(videoInputWidth, videoInputHeight);
        app.setupFbos();
        
    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    messageString = "";
    
    switch (intputMode) {
            
        case INPUT_VIDEO:
            
            videoInput.update();
            app.updateTrackers(videoInput);
            
            break;
            
        case INPUT_CAMERA:
            
            cameraInput.update();
            if(cameraInput.isFrameNew()) {
                app.updateTrackers(cameraInput);
            }
            
            break;
            
        case INPUT_SYPHON:
            
            syphonFbo.begin();
            ofClear(255);
            syphonInput.draw(0.0,0.0);
            syphonFbo.end();
            
            syphonFbo.readToPixels(syphonInputPixels);
            syphonInputImg.setFromPixels(syphonInputPixels);
            
            if(syphonInputImg.getWidth() == videoInputWidth && syphonInputImg.getHeight() == videoInputHeight )
                app.updateTrackers(syphonInputImg);
            else
                messageString = "Syphon stream is not " + ofToString(videoInputWidth) + " x " + ofToString(videoInputHeight) + " (tracking disabled)";
 
            break;
            
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    
    switch (intputMode) {
            
        case INPUT_VIDEO:
            videoInput.draw(cameraRectCanvas);
            break;
            
        case INPUT_CAMERA:
            cameraInput.draw(cameraRectCanvas);
            break;
            
        case INPUT_SYPHON:
            syphonFbo.draw(cameraRectCanvas.x, cameraRectCanvas.y, cameraRectCanvas.getWidth(), cameraRectCanvas.getHeight());
            //syphonInputImg.draw(0.0,0.0, syphonInput.getWidth() * .5, syphonInput.getHeight() * .5);
            break;
            
    }
    
    /* for debug
    ofPushMatrix();
    ofTranslate(cameraRectCanvas.x, cameraRectCanvas.y);
    app.processDebugDraw();
    app.debugDrawTrackers();
    ofPopMatrix();
    
    ofTexture & tex = app.debugFboLayer.getTexture();
    
    if(tex.isAllocated()) {
        debugSyphonLayer.publishTexture(&app.debugFboLayer.getTexture());
    }
   */
    
    ofPushMatrix();
    ofTranslate(cameraRectCanvas.x, cameraRectCanvas.y);
    app.drawScene();
    ofPopMatrix();
    
    ofTexture & tex = app.fboLayer.getTexture();
    
    if(tex.isAllocated()) {
        syphonLayer.publishTexture(&app.fboLayer.getTexture());
    }
    
    ofSetColor(255);
    ofDrawBitmapString(messageString, 20, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == '1')
        setInputMode(INPUT_CAMERA);
    
    if(key == '0')
        setInputMode(INPUT_VIDEO);

    if(key == '2')
        setInputMode(INPUT_SYPHON);
    
    if(key == 'd')
        setInputMode(INPUT_SYPHON);

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
