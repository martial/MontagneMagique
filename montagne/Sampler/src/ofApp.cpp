#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(25);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
#ifdef BLACKMAGIC
    camWidth    = 1920;
    camHeight   = 1080;
    
    #ifdef VIDEO_TEST
        videoPlayer.load("video.mp4");
        videoPlayer.play();
    #else
        cam.setup(camWidth, camHeight, 25);

    #endif
    
#else
    
    camWidth    = 640;
    camHeight   = 480;
    cam.listDevices();
    cam.setDeviceID(0);
    cam.setup(camWidth, camHeight, 25);

#endif
    
    currentSampler = -1;
    
    for( int i=0; i<4; i++) {
        
        VideoSampler sampler;
        sampler.setup(camWidth, camHeight);
        samplers.push_back(sampler);
    }
    
    camFbo.allocate(camWidth, camHeight, GL_RGBA);
    mainFbo.allocate(camWidth, camHeight, GL_RGBA);
    tempFbo.allocate(camWidth, camHeight, GL_RGBA);
    
    mainFbo.begin();
    ofClear(0,0,0,0);
    mainFbo.end();
    
    psBlend.setup(camWidth, camHeight);
    syphonLayer.setName("Yris-OF-Layer");
    
    deinterlaceShader.load("deinterlace");
    
    receiver.setup(PORT);
    
    // fadePct
    cameraFadePct.reset(1);
    cameraFadePct.setCurve(EASE_IN);
    cameraFadePct.setRepeatType(PLAY_ONCE);
    cameraFadePct.setDuration(0);
    
    blendId = psBlend.getBlendId( "BlendLightenf");
    
    bDrawThumbs = true;

}



//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    parseOsc();
    
    // if frame new..
#ifdef BLACKMAGIC
    
#ifdef VIDEO_TEST
    videoPlayer.update();
    if(videoPlayer.isFrameNew()) {
#else
    if(cam.update()) {
    
#endif
    
#else
    cam.update();
    if(cam.isFrameNew()) {
#endif
        
        camFbo.begin();
        ofClear(0,0,0,0);
        ofEnableAlphaBlending();
        deinterlaceShader.begin();
        deinterlaceShader.setUniform1f("opacity", 1.0);

#ifdef BLACKMAGIC
        
        #ifdef VIDEO_TEST
        videoPlayer.draw(0.0,0.0);
            #else
            cam.drawColor();
        #endif
        
    #else
        cam.draw(0, 0);
#endif

        deinterlaceShader.end();
        camFbo.end();
        
        //camFbo.readToPixels(pixs);
        
        for(int i=0; i < samplers.size(); i++) {
            
#ifdef BLACKMAGIC
            
            //if( samplers[i].bIsRecording && cam.getColorPixels().isAllocated())
            if( samplers[i].bIsRecording ) {
#else
                if( samplers[i].bIsRecording && cam.getPixels().isAllocated()){
#endif
                
                
                samplers[i].grabFrame(camFbo);
                    
                }
            
            
            samplers[i].update();
            
        }
    }
        
        // redraw with opacity
   
        camFbo.begin();
        ofClear(0,0,0,0);
        ofEnableAlphaBlending();
        deinterlaceShader.begin();
        deinterlaceShader.setUniform1f("opacity", cameraFadePct.getCurrentValue());
        
#ifdef BLACKMAGIC
        
#ifdef VIDEO_TEST
        videoPlayer.draw(0.0,0.0);
#else
        cam.drawColor();
#endif
        
#else
        cam.draw(0, 0);
#endif
        
        deinterlaceShader.end();
        camFbo.end();
        
        cameraFadePct.update(1.0 / 60.0f);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
   
   // update fbos
    ofEnableAlphaBlending();
    
    //camFbo.getTexture().getTextureData().bFlipTexture = false;
    mainFbo.getTexture().getTextureData().bFlipTexture = false;

    psBlend.begin();
    ofEnableAlphaBlending();
    camFbo.draw(0,0);
    psBlend.end();
    
    ofSetColor(255, 255);
    mainFbo.begin();
    ofClear(0,0);
    ofEnableAlphaBlending();
    psBlend.draw(camFbo.getTexture(), blendId);
    mainFbo.end();

    
    for(int i=0; i < samplers.size(); i++) {
        
        //mainFbo.getTexture().getTextureData().bFlipTexture = true;

        
        psBlend.begin();
        mainFbo.draw(0.0,0.0);
        psBlend.end();
        
        mainFbo.begin();
        ofEnableAlphaBlending();
        ofPushMatrix();
        ofScale(1, -1, 1);
        ofTranslate(0, -samplers[i].getTextureFbo().getHeight());
        ofSetColor(255, 255 * samplers[i].fadePct.getCurrentValue());
        psBlend.draw(samplers[i].getTextureFbo().getTexture(), blendId);
        //ofSetRectMode( OF_RECTMODE_CORNER );
        ofPopMatrix();
        mainFbo.end();
        
    }
  
    ofBackground(0);
    
    float scale = 12;
  
    float w = camWidth / scale;
    float h = camHeight / scale;
    
    float spaceLeft = ofGetWidth() - ((samplers.size() + 1) * w );
    float xOffset = spaceLeft / float(samplers.size() + 2);
    
    float x = xOffset;
    float y = 25;
    
    // draw back
    ofFill();
    ofSetColor(96);
    ofDrawRectangle(0.0, 0.0, ofGetWidth(), h + 10);
    ofSetColor(255);

    // draw cam
#ifdef BLACKMAGIC
    
    #ifdef VIDEO_TEST
    ofTexture & camTexture = videoPlayer.getTexture();
    #else
    ofTexture & camTexture = cam.getColorTexture();
    #endif
    
#else
    ofTexture & camTexture = cam.getTexture();

#endif
    
    ofSetColor(255 * cameraFadePct.getCurrentValue());
    ofDrawRectangle(xOffset -2 , y -2, w + 4, h + 4);
    ofSetColor(255,255);

    camTexture.draw(xOffset, y, w, h);
    
    ofDrawBitmapString("Camera", xOffset, y + h + 16);
    
    for(int i=0; i< samplers.size(); i++) {
        
        x = xOffset + ( i + 1 ) * (xOffset + w);
        
        if(bDrawThumbs)
            samplers[i].drawThumb(x, y, w, h );
        
        ofSetColor(126,255);
        ofDrawRectangle(x-2, y + h +2, samplers[i].getPosition() * w+4, 18);
        ofSetColor(255,255);

        string str = "Track " + ofToString(i) + " ("+ofToString(samplers[i].getNFramesInMemory()) + "/300)";
        ofDrawBitmapString(str, x, y + h + 16);

    }
    
    scale = 3;
    ofPushMatrix();
    
    w = camWidth / scale;
    h = camHeight / scale;

    ofTranslate(ofGetWidth() * .5  - w * .5, ofGetHeight() * .5 - h * .5);
    
    mainFbo.draw(0,0, w,h);
    ofPopMatrix();
    
    syphonLayer.publishTexture(&mainFbo.getTexture());

}
    
void ofApp::fadeIn(int sec) {
    
    cameraFadePct.setDuration(sec);

}
    
void ofApp::fadeOut(int sec) {
    cameraFadePct.setDuration(sec);

}
    
void ofApp::parseOsc() {
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        //ofLogNotice("message" ) << m.getAddress();
        
        // check for mouse moved message
        if(m.getAddress() == "/yris/camera/play"){
            
            int status  = m.getArgAsInt32(0);
            
            if (status == 0)
                cameraFadePct.animateTo(0.0);

            else
                cameraFadePct.animateTo(1.0);


        }
        
        if(m.getAddress() == "/yris/camera/fadein"){
            
            int sec = m.getArgAsInt32(0);
            fadeIn(sec);
        }
        
        if(m.getAddress() == "/yris/camera/fadeout"){
            
            int sec = m.getArgAsInt32(0);
            fadeOut(sec);
            
        }
        
        
        if(m.getAddress() == "/yris/track/play"){
            
            int id = m.getArgAsInt32(0);
            int status = m.getArgAsInt32(1);

            if(id == 0) {
                
                for ( int i=0; i<samplers.size(); i++) {
                    
                     if (status == 1)
                         samplers[i].play();
                    else
                        samplers[i].stop();

                }
                
            } else {
                
                if (status == 1)
                    samplers[id - 1].play();
                else
                    samplers[id - 1].stop();
                
            }
            
        }
        
        if(m.getAddress() == "/yris/track/record"){
            
            int id      = m.getArgAsInt32(0);
            int status  = m.getArgAsInt32(1);
            
            if(id == 0) {
                
                for ( int i=0; i<samplers.size(); i++) {
                    
                    
                    if (status == 1)
                        samplers[i].startRecord();
                    else
                        samplers[i].stopRecord();
                    
                }
                
            } else {
                
                if (status == 1)
                    samplers[id - 1].startRecord();
                else
                    samplers[id - 1].stopRecord();
                
            }

        }
        
        if(m.getAddress() == "/yris/track/fadein"){
            
            int id = m.getArgAsInt32(0);
            int sec = m.getArgAsInt32(1);
            
            if(id == 0) {
                
                for ( int i=0; i<samplers.size(); i++) {
                    samplers[i].fadeIn(sec);
                    
                }
                
            } else {
                samplers[id - 1].fadeIn(sec);

            }
            
        }
        
        if(m.getAddress() == "/yris/track/fadeout"){
            
            int id = m.getArgAsInt32(0);
            int sec = m.getArgAsInt32(1);
            
            if(id == 0) {
                
                for ( int i=0; i<samplers.size(); i++) {
                    samplers[i].fadeOut(sec);
                }
                
            } else {
                
                samplers[id - 1].fadeOut(sec);
                
            }
            
        }
        
        if(m.getAddress() == "/yris/track/playmode"){
            string loop = m.getArgAsString(1);
            for ( int i=0; i<samplers.size(); i++) {
                
                if(loop == "loop") {
                    
                    samplers[i].setLoopMode(OF_LOOP_NORMAL);
                } else {
                    samplers[i].setLoopMode(OF_LOOP_PALINDROME);
                    
                }
            }
        }
        
        if(m.getAddress() == "/yris/track/recordmode"){
            
            string recordmode = m.getArgAsString(1);
            
            for ( int i=0; i<samplers.size(); i++) {
                if(recordmode == "fade") {
                    samplers[i].recordFadePct.setDuration(2.0);
                } else {
                    samplers[i].recordFadePct.setDuration(0.0);

                }
            }
            
            
            
        }
        
        if(m.getAddress() == "/yris/track/afterrecord"){
            
            string afterRecord = m.getArgAsString(1);
            for ( int i=0; i<samplers.size(); i++) {
                if(afterRecord == "none") {
                    samplers[i].bPlayOnstop = false;
                } else {
                    samplers[i].bPlayOnstop = true;
                    
                }
            }
            
        }
        
        if(m.getAddress() == "/yris/track/speed"){
            
            int id = m.getArgAsInt32(0);
            float speed = m.getArgAsFloat(1);
            
            if(id == 0) {
                
                for ( int i=0; i<samplers.size(); i++) {
                    samplers[i].setSpeed(speed);
                }
                
            } else {
                
                samplers[id - 1].setSpeed(speed);

            }
            
        }
        
        if(m.getAddress() == "/yris/setblend"){
            
            blendId = m.getArgAsInt32(0);

        }
        
        if(m.getAddress() == "/yris/setspeedsmooth"){
            
        }
        
      
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
 
 
    if(key == ' ') {
        
        bDrawThumbs = !bDrawThumbs;
            
        
        
    }
    
    
    
    
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

void ofApp::exit() {
        
        cam.close();
}
