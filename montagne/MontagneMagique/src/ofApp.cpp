#include "ofApp.h"
#include "ofxImgSizeUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    bDrawPreview        = true;
    bDrawGui            = false;
    bDebugTrackers      = false;
    trackedVideoInput   = NULL;

    //ofSetCircleResolution(256);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
    
    // some config
    ofFile file;
    file.open("config.json");
    
    if(file.exists())
        file >> configJson;
    else
        exit();
    
    syphonDir.setup();
    
    
    intputMode          = configJson["auto-start-mode"];
    videoInputWidth     = 0;
    videoInputHeight    = 0;
    setInputMode(intputMode);
    
    ofAddListener(syphonDir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(syphonDir.events.serverRetired, this, &ofApp::serverRetired);
    syphonLayer.setName("MM-OF-Layer");
    
    oscManager.setup(configJson["osc-in-port"],configJson["osc-out-port"],configJson["osc-out-ip"]);
    oscManager.setSceneManager(&app.arSceneManager);
    oscManager.setMontagneApp(&app);
    
    messageString = "";
    
    app.setup();
    
    // -- gui
    
    parameters.setName("Settings");
    parameters.add(debugMode.set("debugMode",true));
    
    gui.setup(parameters);
    
    gui.add(bigBangDampingMin.setup("BigBangDmpMin", 0.1, 0.0, 1.0));
    gui.add(bigBangDampingMax.setup("BigBangDmpMax", 0.25, 0.0, 1.0));
    gui.add(bigBangScaleMin.setup("BigBangScaleMin", 3, 0.0, 50.0));
    gui.add(bigBangScaleMax.setup("BigBangScaleMax", 6, 0.0, 50.0));
    gui.add(bigBangScaleDampingScale.setup("bigBangScaleDampingScale", .35, 0.0, 1.0));
    gui.add(bigBangRepulsionFactor.setup("bigBangRepulsionFactor", 1, 0.0, 3));
    
    addMessage("Welcome.");
    addMessage("Press g for GUI");
    addMessage("Press d to debug markers");
    addMessage("Press p to draw preview");
    addMessage("Press o and send an OSC test message on ip " + ofToString(configJson["osc-out-ip"]) + " and port " + ofToString(configJson["osc-out-port"]));
    addMessage("Your current OSC input port is " + ofToString(configJson["osc-in-port"]));

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
    
    if(bDrawPreview) {
        
        switch (intputMode) {
                
            case INPUT_VIDEO:
                videoInput.draw(cameraRectCanvas);
                break;
                
            case INPUT_CAMERA:
                cameraInput.draw(cameraRectCanvas);
                break;
                
            case INPUT_SYPHON:
                syphonFbo.draw(cameraRectCanvas.x, cameraRectCanvas.y, cameraRectCanvas.getWidth(), cameraRectCanvas.getHeight());
                break;
                
        }
        
    }
        
    ofPushMatrix();
    ofTranslate(cameraRectCanvas.x, cameraRectCanvas.y);
    
     if(!bDebugTrackers) {
         app.drawScene(bDrawPreview);
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
    
    if(bDrawGui)
        gui.draw();
    
    ofSetColor(255,0,0);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    
    if(bDrawPreview) {
        
        ofSetColor(255, 255);
        for(int i=0; i<messages.size(); i++) {
            ofDrawBitmapString(messages[i], 20, 40 + i * 20);
        }
        ofSetColor(255);
        
    }
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
    
    if(key == 'g')
        bDrawGui = !bDrawGui;
    
    if(key == 'o')
        oscManager.sendMessage("/OF/test", "This is a test");
    
    if(key == 'd')
        app.arSceneManager.setDebugMode(!app.arSceneManager.bDebugMode);
    
    if(key == 'p')
        bDrawPreview =!bDrawPreview;
    
    oscManager.keyPressed(key);

}

void ofApp::addMessage(string message) {
    
    
    if(!bDrawPreview)
        return;
    
    if(messages.size() > 10) {
        messages.erase(messages.begin());
    }
    
    if(message == lastMessage) {
        
        string m = messages[messages.size() - 1];
        ofStringReplace(m," [" + ofToString(duplicateCount-1) + "]"," [" + ofToString(duplicateCount+1) + "]");
        
        if(duplicateCount == 0)
            m += " [" + ofToString(duplicateCount) + "]";
        
        messages[messages.size() - 1] = m;
        duplicateCount++;
        
    } else {
        duplicateCount = 0;
        messages.push_back(message);

    }
    
    
    lastMessage = message;
    
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
        
        if(configJson["auto-link-syphon"] && dir.serverName != "MM-OF-Layer") {
            syphonInput.set(dir.serverName, dir.appName);
            configJson["syphon-input-name"] = dir.serverName;
            configJson["syphon-input-app"] = dir.appName;
            ofSaveJson("config.json", configJson);

        }
        
        if(dir.serverName != "MM-OF-Layer")
            addMessage("Server Name: " + dir.serverName + " | App Name: " + dir.appName);
        
        
        
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
