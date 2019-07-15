//
//  Sampler.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 12/07/2019.
//

#include "Sampler.hpp"


void Sampler::setup(){
    
    
    bDrawThumbs     = false;
    
    configJson      = ofLoadJson("config-sampler.json");
    outputWidth     = configJson["output-width"];
    outputHeight    = configJson["output-height"];
    
    //ofSetFrameRate(configJson["desired-frame-rate"]);
    
#ifdef BLACKMAGIC
    
    camWidth    = 1920;
    camHeight   = 1080;
    
#ifdef VIDEO_TEST
    videoPlayer.load("video.mp4");
    videoPlayer.play();
#else
    
    cam.setup(camWidth, camHeight, 50);
    
#endif
    
#else
    
    camWidth        = 640;
    camHeight       = 480;
    
    cam.listDevices();
    int deviceId = configJson["camera-device-id"];
    cam.setDeviceID(deviceId);
    cam.setup(camWidth, camHeight, 25);
    
#endif
    
    currentSampler = -1;
    
    for( int i=0; i<4; i++) {
        
        VideoSampler sampler;
        sampler.id = ofToString(i);
        sampler.setup(outputWidth, outputHeight);
        samplers.push_back(sampler);
    }
    
    ofFboSettings settings;
    settings.depthStencilAsTexture = false;
    settings.internalformat = GL_RGBA;
    settings.width = outputWidth;
    settings.height = outputHeight;
    settings.useDepth = false;
    settings.useStencil = false;
    camFbo.allocate(settings);
    mainFbo.allocate(settings);
    
    // clear main fbo
    mainFbo.begin();
    ofClear(0,0,0,0);
    mainFbo.end();
    
    // setup blending shaders to ouput & deinterlace shader
    psBlend.setup(outputWidth, outputHeight);
    blendId = psBlend.getBlendId( "BlendLightenf");
    
    deinterlaceShader.load("shaders/deinterlace");
    
    // set syphon
    //syphonLayer.setName("Yris-OF-Layer");
    
    // osc
    int port = configJson["osc-in-port"];
    receiver.setup(port);
    
    // we start fading camera to 1.
    cameraFadePct.reset(1);
    cameraFadePct.setCurve(EASE_IN);
    cameraFadePct.setRepeatType(PLAY_ONCE);
    cameraFadePct.setDuration(0);
    
    
}



//--------------------------------------------------------------
void Sampler::update(){
    
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
        videoPlayer.draw(0.0,0.0, outputWidth, outputHeight);
#else
        cam.getColorTexture().draw(0,0, outputWidth, outputHeight);
#endif
                
#else
        cam.draw(0, 0, outputWidth, outputHeight);
#endif
                
        deinterlaceShader.end();
        camFbo.end();
                
        for(int i=0; i < samplers.size(); i++) {
                
#ifdef BLACKMAGIC
                    
            if( samplers[i].bIsRecording ) {
                samplers[i].grabFrame(camFbo.getTexture());
            }
                    
#else
            if( samplers[i].bIsRecording && cam.getPixels().isAllocated()){
                samplers[i].grabFrame(camFbo.getTexture());
            }
#endif
                
            samplers[i].update();
            
            
        }
        
    }
        
        

    // redraw with opacity
    /*
     camFbo.begin();
     ofClear(0,0,0,0);
     ofEnableAlphaBlending();
     deinterlaceShader.begin();
     deinterlaceShader.setUniform1f("opacity", cameraFadePct.getCurrentValue());
     
     #ifdef BLACKMAGIC
     
     #ifdef VIDEO_TEST
     videoPlayer.draw(0.0,0.0, outputWidth, outputHeight);
     #else
     cam.getColorTexture().draw(0,0, outputWidth, outputHeight);
     #endif
     
     #else
     cam.draw(0, 0, outputWidth, outputHeight);
     #endif
     
     deinterlaceShader.end();
     camFbo.end();
     */
        
    // draw into main fbo
    ofSetColor(255, 255);
    mainFbo.begin();
    ofClear(0,0);
    ofEnableAlphaBlending();
    camFbo.draw(0,0);
    mainFbo.end();
    
    
    for(int i=0; i < samplers.size(); i++) {
        
        //mainFbo.getTexture().getTextureData().bFlipTexture = true;
        
        // get blend
        psBlend.begin();
        mainFbo.draw(0.0,0.0);
        psBlend.end();
        
        // draw back into main fbo
        mainFbo.begin();
        ofEnableAlphaBlending();
        ofPushMatrix();
       // ofScale(1, -1, 1);
        //ofTranslate(0, -samplers[i].getTextureFbo().getHeight());
        ofSetColor(255, 255 * samplers[i].fadePct.getCurrentValue());
        psBlend.draw(samplers[i].getTextureFbo().getTexture(), blendId);
        ofPopMatrix();
        mainFbo.end();
        
    }
    
    cameraFadePct.update(1.0 / 60.0f);
    
}

//--------------------------------------------------------------
void Sampler::draw(){
    
    
    ofEnableAlphaBlending();
    ofBackground(0);
    
#ifdef BLACKMAGIC
    
    float scale = 18;
#else
    float scale = 10;
    
#endif
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
    
    // draw rectangle following camera fade
    ofSetColor(255 * cameraFadePct.getCurrentValue());
    ofDrawRectangle(xOffset -2 , y -2, w + 4, h + 4);
    ofSetColor(255,255);
    
    // draw camera thumb
    camTexture.draw(xOffset, y, w, h);
    ofDrawBitmapString("Camera", xOffset, y + h + 16);
    
    // draw thumbnails
    if(bDrawThumbs) {
        for(int i=0; i< samplers.size(); i++) {
            
            x = xOffset + ( i + 1 ) * (xOffset + w);
            samplers[i].drawThumb(x, y, w, h );
            
            ofSetColor(126,255);
            ofDrawRectangle(x-2, y + h +2, samplers[i].getPosition() * w+4, 18);
            ofSetColor(255,255);
            
            //string str = "Track " + ofToString(i) + " ("+ofToString(samplers[i].getNFramesInMemory()) + "/)" + ofToString(MAX_FRAMES);
            string str = "Track " + ofToString(i);
            ofDrawBitmapString(str, x, y + h + 16);
            
        }
        
    }
    
    
    scale = 3;
    
    // draw main fbo
    ofPushMatrix();
    w = outputWidth / scale;
    h = outputHeight / scale;
    ofTranslate(ofGetWidth() * .5  - w * .5, ofGetHeight() * .5 - h * .5);
    mainFbo.draw(0,0, w,h);
    ofPopMatrix();
    
    // publish fbo
    //syphonLayer.publishTexture(&mainFbo.getTexture());
    
    
}

void Sampler::fadeIn(int sec) {
    
    cameraFadePct.setDuration(sec);
    
}

void Sampler::fadeOut(int sec) {
    cameraFadePct.setDuration(sec);
    
}

void Sampler::parseOsc() {
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        ofLogNotice("message" ) << m.getAddress();
        
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


