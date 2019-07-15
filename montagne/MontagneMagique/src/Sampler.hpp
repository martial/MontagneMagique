//
//  Sampler.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 12/07/2019.
//

#ifndef Sampler_hpp
#define Sampler_hpp

#include "ofMain.h"
#include "VideoSampler.hpp"
#include "ofxPSBlend.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxAnimatableFloat.h"

#define BLACKMAGIC
//#define VIDEO_TEST

#ifdef BLACKMAGIC
#include "ofxBlackMagic.h"
#endif
class Sampler {
  
public:
    
    void setup();
    void update();
    void draw();
    
    void parseOsc();
    void fadeIn(int sec);
    void fadeOut(int sec);
    
    
#ifdef BLACKMAGIC
    ofxBlackMagic cam;
    
#ifdef VIDEO_TEST
    ofVideoPlayer videoPlayer;
#endif
    
#else
    ofVideoGrabber cam;
#endif
    
    int camWidth, camHeight;
    int outputWidth, outputHeight;
    
    vector<VideoSampler> samplers;
    ofImage camImage;
    
    int currentSampler;
    
    ofPixels pixs;
    
    ofFbo mainFbo;
    ofFbo camFbo;
    ofxPSBlend psBlend;
    
    ofFbo syphonFbo;
    //ofxSyphonServer             syphonLayer;
    ofShader deinterlaceShader;
    ofxOscReceiver receiver;
    ofxAnimatableFloat cameraFadePct;
    
    int blendId;
    
    bool bDrawThumbs;
    
    ofJson configJson;
    
};

#endif /* Sampler_hpp */
