#include "ofApp.h"
#include "ofxImgSizeUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    bDrawPreview        = true;
    bDrawMessages       = true;
    bDrawGui            = false;
    bDebugTrackers      = false;
    trackedVideoInput   = NULL;
    
    currentFrameRate    = 60;
    
    ofSetCircleResolution(128);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
   // ofEnableSmoothing();
   // ofSetVerticalSync(true);
    
    ofLogNotice("Gl shading version ") <<  glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    // some config
    ofFile file;
    file.open("config.json");
    
    if(file.exists())
        file >> configJson;
    else
        exit();
    
    intputMode              = configJson["auto-start-mode"];
    videoInputWidth         = configJson["input-width"];
    videoInputHeight        = configJson["input-height"];
    videoOutputWidth        = configJson["output-width"];;
    videoOutputHeight       = configJson["output-height"];;
    
    syphonDir.setup();
    
    app.videoOutputWidth    = videoOutputWidth;
    app.videoOutputHeight   = videoOutputHeight;

    int startMode           = configJson.value("start-mode", 0);
    app.setup();
    app.setMode(startMode);
    
    int frameDrop           = configJson.value("ar-frame-drop", 0);
    app.getArToolKitManager().setDelays(configJson["marker-found-delay"], configJson["marker-lost-delay"]);
    app.getArToolKitManager().frameDrop = frameDrop;
    setInputMode(intputMode);
    
    ofAddListener(syphonDir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(syphonDir.events.serverRetired, this, &ofApp::serverRetired);
    syphonLayer.setName("MM-OF-Layer");
    
    oscManager.setup(configJson["osc-in-port"], configJson["osc-out-port"], configJson["osc-out-ip"]);
    oscManager.setSceneManager(&app.arSceneManager);
    oscManager.setMontagneApp(&app);
    
    messageString = "";
    
    // -- gui
    parameters.setName("Settings");
    parameters.add(debugMode.set("debugMode",true));
    
    gui.setup(parameters);
    
    gui.add(bigBangDampingMin.setup("BigBangDmpMin", 0.1, 0.0, 1.0));
    gui.add(bigBangDampingMax.setup("BigBangDmpMax", 0.25, 0.0, 1.0));
    gui.add(bigBangScaleMin.setup("BigBangScaleMin", 3, 0.0, 50.0));
    gui.add(bigBangScaleMax.setup("BigBangScaleMax", 6, 0.0, 50.0));
    gui.add(bigBangScaleDampingScale.setup("bigBangScaleDampingScale", .65, 0.0, 1.0));
    gui.add(bigBangRepulsionFactor.setup("bigBangRepulsionFactor", 1, 0.0, 3));
    
    addMessage("Welcome.");
    addMessage("Press d to activate debug mode");
    addMessage("Press p to draw preview");
    addMessage("Press m to show hide messages");
    addMessage("Press t to draw time measurements");
    addMessage("Press f to go full screen");

    addMessage("Press o and send an OSC test message on ip " + ofToString(configJson["osc-out-ip"]) + " and port " + ofToString(configJson["osc-out-port"]));
    addMessage("Your current OSC input port is " + ofToString(configJson["osc-in-port"]));
    
    if (frameDrop > 0 ) {
        addMessage("AR tracking runs every " + ofToString(frameDrop) + " frames");

    }
    TIME_SAMPLE_SET_FRAMERATE(60.0f); //specify a target framerate
    ofxTimeMeasurements::instance()->setEnabled(false);
    
   // oscManager.sendMessage("scene/printemps/ours", 0);
}

void ofApp::setInputMode(int mode) {
    
    intputMode = mode;
    
    int oldVideoInputWidth  = videoInputWidth;
    int oldVideoInputHeight = videoInputHeight;
    
    int deviceId            = configJson["camera-device-id"];
    string filename         = configJson["video-filename"];
    string videoUrl         = "videos/" + filename;
    
    ofFboSettings settings;
    settings.depthStencilAsTexture  = false;
    settings.internalformat         = GL_RGB;
    settings.width                  = videoInputWidth;
    settings.height                 = videoInputHeight;
    settings.useDepth               = false;
    settings.useStencil             = false;
    settings.numSamples             = 0;
    
    switch (intputMode) {
            
        case INPUT_VIDEO:
            
            videoInput.load(videoUrl);
            videoInputWidth    = videoInput.getWidth();
            videoInputHeight   = videoInput.getHeight();
            videoInput.play();
            videoInput.setLoopState(OF_LOOP_NORMAL);
            
            trackedVideoInput = &videoInput;
            
            resizedFbo.allocate(settings);
            trackedVideoInput = &resizedInputImg;

            break;
        
        case INPUT_CAMERA:
            
#ifdef USE_SAMPLER
            
            sampler.setup();
            
            resizedFbo.allocate(videoInputWidth, videoInputHeight, GL_RGB);
            trackedVideoInput = &resizedInputImg;
            
            bDrawSampler = true;
#else
            
            cameraInput.listDevices();
            cameraInput.setDeviceID(deviceId);
            cameraInput.setup(videoInputWidth, videoInputHeight);
            trackedVideoInput = &cameraInput;
            
#endif
            
            break;
            
        case INPUT_IMAGE:
            
            
            cameraInput.listDevices();
            cameraInput.setDeviceID(deviceId);
            cameraInput.setup(videoInputWidth, videoInputHeight);
            trackedVideoInput = &cameraInput;
            
            break;
            
        case INPUT_SYPHON:
            
            syphonInput.setup();
            syphonInput.set(configJson["syphon-input-name"], configJson["syphon-input-app"]);
            resizedFbo.allocate(settings);
            
            trackedVideoInput = &resizedInputImg;
            
            break;
            
    }
    
    // reset trackers if size is different
   // if(oldVideoInputWidth != videoInputWidth ||  oldVideoInputHeight != videoInputHeight ) {
        
        ofSetWindowShape(videoInputWidth, videoInputHeight*2);
        app.getArToolKitManager().clean();
        app.setupTrackers(videoInputWidth, videoInputHeight);
        app.setupFbos();
        
   // }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef USE_SAMPLER
    TS_START("sampler");
    sampler.update();
    TS_STOP("sampler");
#endif
    
    
    TS_START("Window-title");
    // frame rate & windows title
    currentFrameRate = 0.95 * currentFrameRate +  (1.0f - 0.95) * ceil(ofGetFrameRate());

    string sceneName = app.currentSceneName + " / " + app.currentSubSceneName;
    ofSetWindowTitle("Montagne Magique – " + sceneName + " ["+  ofToString(floor(currentFrameRate)) + " fps ]");
    TS_STOP("Window-title");

    TS_START("OSC");
    oscManager.update();
    TS_STOP("OSC");

    messageString = "";

    switch (intputMode) {
            
        case INPUT_VIDEO:
            
            TS_START("UPDATE FBO");
            videoInput.update();
            resizedFbo.begin();
            ofEnableAlphaBlending();
            ofClear(0, 0, 0, 0);
            videoInput.draw(0.0,0.0, videoInputWidth, videoInputHeight);
            ofDisableAlphaBlending();
            resizedFbo.end();
            resizedFbo.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
            TS_STOP("UPDATE FBO");

            TS_START("READ PIXELS");
            reader.readToPixels(resizedFbo, resizedInputPixels);
            TS_STOP("READ PIXELS");
            
            TS_START("SET PIXELS");
            resizedInputImg.setFromPixels(resizedInputPixels);
            TS_STOP("SET PIXELS");

            TS_START("UPDATE TRACKER");

            if(resizedInputPixels.getWidth() == videoInputWidth && resizedInputPixels.getHeight() == videoInputHeight )
                app.updateTrackers(resizedInputImg);
            else
                addMessage("Syphon stream is not " + ofToString(videoInputWidth) + " x " + ofToString(videoInputHeight) + " (tracking disabled)");
            TS_STOP("UPDATE TRACKER");


            break;
            
        case INPUT_CAMERA:
            
#ifdef USE_SAMPLER
            
            sampler.update();
            
            TS_START("main-fbo");
            resizedFbo.begin();
            ofClear(0, 0, 0, 0);
            sampler.cam.getColorTexture().draw(0,0,videoInputWidth, videoInputHeight);
            //sampler.mainFbo.draw(0.0,0.0, videoInputWidth, videoInputHeight);
            resizedFbo.end();
            TS_STOP("main-fbo");
            
            resizedFbo.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
            TS_START("resize-fbo");
            resizedFbo.getTexture().readToPixels(resizedInputPixels);
            TS_STOP("resize-fbo");
            
            TS_START("setpixels-fbo");
            resizedInputImg.setFromPixels(resizedInputPixels);
            TS_STOP("setpixels-fbo");
            
            app.updateTrackers(resizedInputImg);
            
#else
            
            cameraInput.update();
            
            if(cameraInput.isFrameNew()) {
                app.updateTrackers(cameraInput);
            }
            
            resizedInputImg.setFromPixels(cameraInput.getPixels());
            resizedInputImg.resize(videoInputWidth, videoInputHeight);
            
#endif
            
            
            
            break;
            
        case INPUT_IMAGE:
            app.updateTrackers(imageInput);
            
            break;
            
        case INPUT_SYPHON:

            TS_START("main-fbo");
            resizedFbo.begin();
            ofClear(0, 0, 0, 0);
            syphonInput.draw(0.0,0.0, videoInputWidth, videoInputHeight);
            resizedFbo.end();
            TS_STOP("main-fbo");
            
            //resizedFbo.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
            TS_START("readpixels-fbo");
            resizedFbo.getTexture().readToPixels(resizedInputPixels);
            TS_STOP("readpixels-fbo");
            
            TS_START("setpixels-fbo");
            resizedInputImg.setFromPixels(resizedInputPixels);
            TS_STOP("setpixels-fbo");
            
            
            /*
            // test
            TS_START("alternate-fbo");

            ofTextureData &texData = resizedFbo.getTexture().getTextureData();
            // reallocate if the incoming texture size is different from our fbo & image
            // ie. we've connected to a different Syphon server with a different texture size
            if((texData.width != 0 && texData.height != 0) &&
               (resizedInputImg.getWidth() != texData.width || resizedFbo.getHeight() != texData.height)) {
                resizedInputImg.allocate(texData.width, texData.height, OF_IMAGE_COLOR);
            }
            // grab pixel data from the FBO, note pixel data pointer as destination
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, resizedFbo.getId());
            glReadPixels(0, 0, texData.width, texData.height, texData.glInternalFormat, GL_UNSIGNED_BYTE, resizedInputImg.getPixels().getData());
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            // we manually loaded pixel data into the image, so update the image texture here
            //resizedInputImg.update();
            TS_STOP("alternate-fbo");
             
             */

            TS_START("tracker");

            if(resizedInputPixels.getWidth() == videoInputWidth && resizedInputPixels.getHeight() == videoInputHeight )
                app.updateTrackers(resizedInputImg);
            else
                messageString = "Syphon stream is not " + ofToString(videoInputWidth) + " x " + ofToString(videoInputHeight) + " (tracking disabled)";
 
            TS_STOP("tracker");
            break;
            
    }
    
    TS_START("update-scene");
    app.updateScene();
    TS_STOP("update-scene");

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    TS_START("draw-camera");

    
    ofBackground(0);
    ofSetColor(255);
    
    if(bDrawPreview ) {
        
        cameraRectCanvas = ofxImgSizeUtils::getCenteredRect(ofGetWidth(), (ofGetHeight() == videoInputHeight * 2.0f) ? ofGetHeight() * 0.5f : ofGetHeight(), videoInputWidth, videoInputHeight, false);

        switch (intputMode) {
                
            case INPUT_VIDEO:
                resizedInputImg.draw(cameraRectCanvas.x, cameraRectCanvas.y, cameraRectCanvas.getWidth(), cameraRectCanvas.getHeight());

                break;
                
            case INPUT_CAMERA:
                resizedInputImg.draw(cameraRectCanvas);

                break;
                
            case INPUT_IMAGE:
                resizedInputImg.draw(cameraRectCanvas);
                break;
                
            case INPUT_SYPHON:
                syphonInput.draw(cameraRectCanvas.x, cameraRectCanvas.y, cameraRectCanvas.getWidth(), cameraRectCanvas.getHeight());
                break;
                
        }
        
    }
    
    TS_STOP("draw-camera");

    TS_START("draw-scene");
    ofPushMatrix();
    ofTranslate(cameraRectCanvas.x, cameraRectCanvas.y);
     if(!bDebugTrackers) {
         
        app.drawScene();
         
     } else {
         app.processDebugDraw();
         app.debugDrawTrackers();
     }
    ofPopMatrix();
    TS_STOP("draw-scene");

    TS_START("publish-syphon");
    syphonLayer.publishTexture(&app.fboLayer.getTexture());
    TS_STOP("publish-syphon");

    TS_START("draw-offscreen");
    ofSetColor(255);
    ofPushMatrix();
    if(ofGetWidth() == videoInputWidth && ofGetHeight() == videoInputHeight*2) {
        ofTranslate(0.0, videoInputHeight);
    } else {
        ofTranslate(0.0, 0.0);
    }

    if(bDrawPreview ) {
        app.drawOffScreen();
    }
    
    TS_STOP("draw-offscreen");

    TS_START("draw-messages");

    ofSetColor(255,0,0);
    ofDrawBitmapStringHighlight("Montagne Magique | " + ofToString(floor(currentFrameRate)) + " fps", 20, 20, ofColor::white, ofColor::red);
    ofSetColor(255, 255);
    
    if(bDrawMessages) {
        ofSetColor(255, 255);
        for(int i=0; i<messages.size(); i++) {
            ofDrawBitmapStringHighlight(messages[i], 20, 40 + i * 20);
        }
    }
    
    ofPopMatrix();
    ofSetColor(255,255);
    if(bDrawGui)
        gui.draw();
    
    TS_STOP("draw-messages");

   // if(bDrawSampler)
     //   sampler.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'd') {
        
        app.arSceneManager.setDebugMode(!app.arSceneManager.bDebugMode);
        
        if(app.arSceneManager.bDebugMode)
            addMessage("Debug mode is on");
        else
            addMessage("Debug mode is off");
        
    }
    
    if(key == 'p')
        bDrawPreview =!bDrawPreview;
    
    
    if(key == 'm')
        bDrawMessages =!bDrawMessages;
    
    if(key == 'o')
        oscManager.sendMessage("/OF/test", "This is a test");
    
    
    if(key == 't')
        ofxTimeMeasurements::instance()->setEnabled(!ofxTimeMeasurements::instance()->getEnabled());
        
    bool keyboardLock = configJson.value("keyboard-lock", true);
    if(keyboardLock)
        return;
    
    if(key == '1')
        setInputMode(INPUT_CAMERA);
    
    if(key == '0')
        setInputMode(INPUT_VIDEO);

    if(key == '2')
        setInputMode(INPUT_SYPHON);
    
    if(key == 'g')
        bDrawGui = !bDrawGui;
 
    if(key == 'f')
        ofToggleFullscreen();
  
    
    // send random video
    if(key == 'v') {
        
        ofDirectory dir("videos");
        dir.allowExt("mov");
        dir.allowExt("mp4");
        
        dir.listDir();
        dir.sort();
        int nFiles = dir.size();
        int rdmIndex = floor(ofRandom(nFiles));
        string vid = dir.getName(rdmIndex);
        oscManager.sendMessage("/video/"+vid, "This is a test");
        
    }
    
   // if(key == 's')
     //   bDrawSampler = !bDrawSampler;
    
    
    oscManager.keyPressed(key);

}

void ofApp::addMessage(string message) {
    
        
    if(messages.size() > 15) {
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
void ofApp::dragEvent(ofDragInfo dragInfo){
    
    string filename = dragInfo.files[0].substr( dragInfo.files[0].find_last_of("/") + 1 );
    configJson["video-filename"] = filename;
    ofSaveJson("config.json", configJson);
    
    ofFile f;
    f.open(dragInfo.files[0]);
    f.copyTo("videos/"+ filename);

    setInputMode(INPUT_VIDEO);
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    
#ifdef USE_SAMPLER
    sampler.cam.close();
#endif
   // ofLogNotice("exit OF");
}

//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        
        //ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        
        if(configJson["auto-link-syphon"] && dir.serverName != "MM-OF-Layer") {
            syphonInput.set(dir.serverName, dir.appName);
            configJson["syphon-input-name"] = dir.serverName;
            configJson["syphon-input-app"] = dir.appName;
            ofSaveJson("config.json", configJson);

        }
        
        if(dir.serverName != "MM-OF-Layer")
            addMessage("Server Name: " + dir.serverName + " | App Name: " + dir.appName + " " + ofToString(syphonInput.getWidth()) + "x" +  ofToString(syphonInput.getHeight()));
        
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
