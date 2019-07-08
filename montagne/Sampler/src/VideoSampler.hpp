//
//  VideoSampler.hpp
//  Sampler
//
//  Created by Martial Geoffre-Rouland on 12/04/2019.
//

#ifndef VideoSampler_hpp
#define VideoSampler_hpp

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

#define MAX_FRAMES 300

class VideoSampler {
  
public:
    
    void setup(int width, int height);
    void update();
    void draw();
    void drawThumb(float x, float y, float w, float h);

    void play();
    void stop();
    void clear();
    
    void startRecord();
    void stopRecord();
    void toggleRecord();

    //void grabFrame(ofPixels  pixels);
    void grabFrame(ofFbo  & texture);

    void setLoopMode(int mode);
    
    void fadeIn(int sec);
    void fadeOut(int sec);
    
    void setSpeed(float pct);
    
    float getPosition();
    float position;
    
    ofTexture & getTexture(int frame = - 1);
    ofFbo & getTextureFbo(bool bForceOpacity = false);

    bool bUseFbo;
    bool bIsPlaying, bIsRecording;
    bool bPlayOnstop;
    
    ofxAnimatableFloat fadePct, recordFadePct;
    
    int getNFramesInMemory();
    
private:
    
    
   // ofPixels    samples[600];
    //ofPixels    tempSamples[600];
    
    //ofFbo       samples[600];
    //ofFbo       tempSamples[600];
    
    //vector<ofFbo*> samples;
    //vector<ofFbo*> tempSamples;
    
    vector<shared_ptr<ofFbo>> samples;
    vector<shared_ptr<ofFbo>> tempSamples;


    
    void        copyTempPixels();

    int         nPixels, nTempPixels;
    int         currentFrame, previousFrame;
    float       currentFramef;
    float       fps, speedPct;
    
    ofTexture   texture;
    ofFbo       fbo;
    
    int         loopMode;
    bool        bReverse;
    bool        bFirstRecord;
    
    float       fadeDuration;
    
    void        onFadeRecordEndHandler(ofxAnimatable::AnimationEvent & e);
};

#endif /* VideoSampler_hpp */
