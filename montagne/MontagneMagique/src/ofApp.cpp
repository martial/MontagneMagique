#include "ofApp.h"
#include "ofxImgSizeUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    bDebugTrackers = false;
    trackedVideoInput = NULL;

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
    
    syphonDir.setup();
    ofAddListener(syphonDir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(syphonDir.events.serverRetired, this, &ofApp::serverRetired);
    syphonLayer.setName("MM-OF-Layer");
    
    oscManager.setup(configJson["osc-in-port"],configJson["osc-out-port"],configJson["osc-out-ip"]);
    oscManager.setSceneManager(&app.arSceneManager);
    
    messageString = "";
    
    app.setup();
    
    // -- gui
    
    parameters.setName("Settings");
    parameters.add(debugMode.set("debugMode",true));
    
    
    gui.setup(parameters);
    
    gui.add(bigBangDampingMin.setup("BigBangDmpMin", 0.1, 0.0, 1.0));
    gui.add(bigBangDampingMax.setup("BigBangDmpMax", 0.25, 0.0, 1.0));
    gui.add(bigBangScaleMin.setup("BigBangScaleMin", 2, 0.0, 50.0));
    gui.add(bigBangScaleMax.setup("BigBangScaleMax", 4, 0.0, 50.0));
    gui.add(bigBangScaleDampingScale.setup("bigBangScaleDampingScale", .35, 0.0, 1.0));
    gui.add(bigBangRepulsionFactor.setup("bigBangRepulsionFactor", 3, 0.0, 10));
    gui.add(bigBangParticleIntensity.setup("bigBangParticleIntensity", 1, 1, 2000));

    
    
    
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
            trackedVideoInput = &videoInput;

            break;
        
        case INPUT_CAMERA:
            
            videoInputWidth    = 1280 ;
            videoInputHeight   = 720 ;
            
            cameraInput.listDevices();
            cameraInput.setDeviceID(deviceId);
            cameraInput.setup(videoInputWidth, videoInputHeight);
            trackedVideoInput = &cameraInput;

            break;
            
        case INPUT_SYPHON:
            
            syphonInput.setup();
            syphonInput.set(configJson["syphon-input-name"], configJson["syphon-input-app"]);
            videoInputWidth    = 1280 ;
            videoInputHeight   = 720 ;
            syphonFbo.allocate(videoInputWidth, videoInputHeight, GL_RGB);
            
            trackedVideoInput = &syphonInputImg;
            
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
    
    oscManager.update();
    
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
            ofEnableAlphaBlending();
            ofClear(255);
            syphonInput.draw(0.0,0.0, 1280, 720);
            syphonFbo.end();
            
            syphonFbo.readToPixels(syphonInputPixels);
            syphonInputImg.setFromPixels(syphonInputPixels);
            
            if(syphonInputImg.getWidth() == videoInputWidth && syphonInputImg.getHeight() == videoInputHeight )
                app.updateTrackers(syphonInputImg);
            else
                messageString = "Syphon stream is not " + ofToString(videoInputWidth) + " x " + ofToString(videoInputHeight) + " (tracking disabled)";
 
            break;
            
    }
    
   
    
    app.updateScene();

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    ofEnableAlphaBlending();
    
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
    
     ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(cameraRectCanvas.x, cameraRectCanvas.y);
    
     if(!bDebugTrackers) {
         app.drawScene();
     } else {
         app.processDebugDraw();
         app.debugDrawTrackers();
     }
             
    ofPopMatrix();
    ofTexture & tex = app.fboLayer.getTexture();
    if(tex.isAllocated()) {
        syphonLayer.publishTexture(&app.fboLayer.getTexture());
    }
    ofSetColor(255);
    ofDrawBitmapString(messageString, 20, 20);
    
    gui.draw();

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

//--------------------------------------------------------------
void ofApp::exit() {
    
    ofLogNotice("exit OF");
}

//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        if(configJson["auto-link-syphon"])
            syphonInput.set(dir.serverName, dir.appName);
        
        
    }
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
}
