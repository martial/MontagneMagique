//
//  VideoSampler.cpp
//  Sampler
//
//  Created by Martial Geoffre-Rouland on 12/04/2019.
//

#include "VideoSampler.hpp"

void VideoSampler::setup(int width, int height) {
    
    bFirstRecord    = true;
    bIsPlaying      = false;
    bIsRecording    = false;
    bPlayOnstop     = false;
    bUseFbo         = true;
    fps             = 25.0f;
    nPixels         = 0;
    nTempPixels     = 0; // for record fade
    bReverse        = false;
    loopMode        = OF_LOOP_PALINDROME;
    
    currentFrame    = 0;
    currentFramef   = 0.0;
    previousFrame   = 0;
    
    fadeDuration    = 1;
    
    speedPct        = 1;
    
    // fadePct
    fadePct.reset(0);
    fadePct.setCurve(EASE_IN);
    fadePct.setRepeatType(PLAY_ONCE);
    fadePct.setDuration(fadeDuration);
    
    recordFadePct.reset(0);
    recordFadePct.setCurve(EASE_IN);
    recordFadePct.setRepeatType(PLAY_ONCE);
    recordFadePct.setDuration(1);
    
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
    
    // set black texture
    ofPixels pixs;
    pixs.allocate(width, height, OF_PIXELS_RGB);
    
    for(int i=0; i<width*height; i++)
        pixs.setColor(i, ofColor(0));
    
    texture.loadData(pixs);
    
    for (int i=0; i<MAX_FRAMES; i++) {
        
        shared_ptr<ofFbo> t(new ofFbo);
        
        t->allocate(1920, 1080, GL_RGB);
        t->begin();
        ofClear(255,0);
        t->end();
        samples.push_back(t);
        
        
    
        tempSamples.push_back(t);
        
    }
}

void VideoSampler::update(){
    
    fadePct.update(1.0f / 30.0f);
    recordFadePct.update(1.0f / 30.0f);
        
    if(nPixels > 0) {
        
        if(!bIsPlaying) {
            currentFrame = 0;
            currentFramef = 0.0;
            return;
        }
        
        if( loopMode == OF_LOOP_NORMAL) {
            
            bReverse = false;
            
            if(currentFrame >= nPixels - 1) {
                currentFrame = 0;
                currentFramef = 0.0;
            }
            
                        
        }
        
        if( loopMode == OF_LOOP_PALINDROME) {
            
            if(currentFrame >= nPixels - 1)
                bReverse = true;
            
            if(currentFrame <= 0)
                bReverse = false;
       
        }

        currentFramef += (bReverse) ? -speedPct : speedPct;
        currentFrame = round(currentFramef);
        // we store the frame only in one direction ( before inverting it )
        previousFrame = currentFrame;
        currentFrame = ofClamp(currentFrame, 0, nPixels - 1);
        
    }
    
}
void VideoSampler::draw(){
    
  
}

void VideoSampler::drawThumb(float x, float y, float w, float h) {
    
    ofEnableAlphaBlending();

    ofPushMatrix();
    ofTranslate(x,y);
    
    ofFill();

    if(bIsRecording) {
        
        ofSetColor(255, 0, 0);
        ofDrawRectangle(-2, -2, w + 4, h + 4);
        
    } else {
        
        ofSetColor(255 * fadePct.getCurrentValue());
        ofDrawRectangle(-2, -2, w + 4, h + 4);

        
    }
    ofSetColor(255,255);
    
    getTextureFbo(true);
    fbo.draw(0,0, w, h);
    ofPopMatrix();
    
}

ofTexture & VideoSampler::getTexture(int frame) {
    
    int frameToLoad = (frame >= 0) ? frame : currentFrame;
    if(nPixels > 0 ) {
        texture.clear();
       // texture.loadData(samples[currentFrame]);
    }
    
    return texture;
    
}

ofFbo & VideoSampler::getTextureFbo(bool bForceOpacity) {
    
    fbo.begin();
    ofClear(0,0,0,0);
    ofEnableAlphaBlending();
    
    ofSetColor(255, 255);
    if(!bForceOpacity) {
        ofSetColor(255, 255 * fadePct.getCurrentValue());
    }
    
    
    bool bIsRecordFading = (recordFadePct.getCurrentValue() > 0);
    
    if( bIsRecording && bIsRecordFading) {
        
        // if recording, and we are still faded
        // animation is stored in temp samples.
        // we need to see the realtime in the back, so let's grab a frame at nTempPixels - 1
        
        int lastFrame = nTempPixels - 1;
        //ofTexture tempTexture;
       // tempTexture.loadData(tempSamples[lastFrame]);
       // tempTexture.draw(0,0);
        
        tempSamples[lastFrame]->draw(0,0);
        
        //ofLogNotice("Drawing in back real time at ") << lastFrame << " for " << nTempPixels;
        
    }
    
    
    
    if(nPixels > 0) {
        
        // for front, we have various options
        // if we are recording, but fade is done – we just set the currentFrame in last position
        
        shared_ptr<ofFbo> texture;
        if(bIsRecording && !bIsRecordFading) {
            
            int frame = nPixels - 1;
            //getTexture(frame);
            //texture.clear();
            //texture.loadData(samples[frame]);
            texture = samples[frame];
            position = frame / (float)nPixels;

            //ofLogNotice("Drawing in front real time at ") << frame << " for " << nPixels;

        } else {
            
            //getTexture();
           // texture.clear();
            //texture.loadData(samples[currentFrame]);
            texture = samples[currentFrame];
            position = currentFrame / (float)nPixels;

            //ofLogNotice("Drawing in front still loop at ") << currentFrame;

        }
        
        
        // we load the texture with current frame.
        if(texture && texture->isAllocated()) {
            
            if( !bFirstRecord && bIsRecordFading ) {
                
                // if we are recording and fading, we fade out the texture in front to let appear back
                // we set out the color to match recordFadePct value
                // ofLogNotice("opacity ? ") << recordFadePct.getCurrentValue();

                ofSetColor(255,recordFadePct.getCurrentValue() * 255);
                
            } else {
                
                // at this point, no back is drawn and we have a perfect copy
                ofSetColor(255,255);
                if(!bForceOpacity) {
                    ofSetColor(255, 255 * fadePct.getCurrentValue());
                    
                }
                
            }
            texture->draw(0,0);
        }
        
    }

    
    fbo.end();
    
    return fbo;
    
}


float VideoSampler::getPosition() {
    
    if(nPixels == 0 )
        return 0.0;
    
    return position;
    
}



void VideoSampler::startRecord(){
    
    //clear();

    nTempPixels = 0;
    
    //stop();
    bIsRecording = true;
    
    // start fade for recording
    recordFadePct.reset(1);
    recordFadePct.animateTo(0);

    ofAddListener(recordFadePct.animFinished, this, &VideoSampler::onFadeRecordEndHandler);
    
}

void VideoSampler::stopRecord(){
        
    ofRemoveListener(recordFadePct.animFinished, this, &VideoSampler::onFadeRecordEndHandler);
    
    bIsRecording = false;
    
    if(nTempPixels > 0 ) {
        
        recordFadePct.reset(0);
        recordFadePct.pause();
        copyTempPixels();
        
    }

    if(bPlayOnstop) {
        play();
    }

}

void VideoSampler::toggleRecord() {
    
    if (bIsRecording)
        stopRecord();
    else
        startRecord();
}

int VideoSampler::getNFramesInMemory() {
    
    if(nPixels == 0 )
        return nTempPixels;
    
    return nPixels;
    
}
    

/*
void VideoSampler::grabFrame(ofPixels  pixels) {
    
    // if we have a recording fade, we need to store pixels to temp buffer
    
    bool bIsRecordFading = (recordFadePct.getCurrentValue() > 0);

    if( bIsRecordFading ) {

        tempSamples[nTempPixels].clear();
        tempSamples[nTempPixels] = pixels;
        nTempPixels++;
        
    
    } else {
    
        // store to new
        if(nPixels <= 600) {
            samples[nPixels].clear();
            samples[nPixels] = pixels;
            nPixels++;
            
        }
        
    }
    
    
}
 
 */

void VideoSampler::grabFrame(ofFbo & texture) {
    
    // if we have a recording fade, we need to store pixels to temp buffer
    
    bool bIsRecordFading = (recordFadePct.getCurrentValue() > 0);
    
    if( bIsRecordFading ) {
        
        //tempSamples[nTempPixels]->clear();
        
        // Draw into FBO
        tempSamples[nTempPixels]->begin();
        ofClear(255,0);
        texture.draw(0.0,0.0);
        tempSamples[nTempPixels]->end();
        nTempPixels++;
        
        
    } else {
        
        // store to new
        if(nPixels < MAX_FRAMES) {
            //samples[nPixels]->clear();
            samples[nPixels]->begin();
            texture.draw(0.0,0.0);
            samples[nPixels]->end();
            
            nPixels++;
            
        }
        
    }
    
    
}

void VideoSampler::copyTempPixels() {
    
    if(nTempPixels > 0 ) {
        
        nPixels = nTempPixels;
        samples = tempSamples;
        nTempPixels = 0;
        currentFrame = nPixels - 1;

         /*
        //memcpy(samples, tempSamples, 600 * sizeof * samples );
        for(int i=0; i<600; i++) {
            
            samples[i] = tempSamples[i];
            if(i >= nTempPixels)
                samples[i]->clear();
        }
         */
       
        
    }
    bFirstRecord = false;
    
}

void VideoSampler::onFadeRecordEndHandler(ofxAnimatable::AnimationEvent & e) {
    
    
    copyTempPixels();
}



void VideoSampler::play(){
    
    currentFramef   = 0;
    currentFrame    = 0;
    bIsPlaying      = true;
    fadePct.animateTo(1.0);
    
    
}

void VideoSampler::stop(){
    
    currentFramef   = 0;
    currentFrame    = 0;
    bIsPlaying = false;
    fadePct.animateTo(0.0);

}

void VideoSampler::setLoopMode(int mode) {
    
    loopMode = mode;
    
    if(loopMode == OF_LOOP_NORMAL)
        bReverse = false;
    
}

void VideoSampler::fadeIn(int sec) {
    
    fadePct.setDuration(sec);
}

void VideoSampler::fadeOut(int sec) {
    
     fadePct.setDuration(sec);
}

void VideoSampler::setSpeed(float pct) {
    
    speedPct = pct;
    
}


void VideoSampler::clear(){
    
    nPixels = 0;
}
