//
//  ImageSequencePlayer.cpp
//  5GTunnel
//
//  Created by Martial Geoffre-Rouland on 24/11/2017.
//

#include "ImageSequencePlayer.hpp"

ImageSequencePlayer::ImageSequencePlayer() {
    
    this->fps                   = 25;
    this->currentFrame          = 0;
    this->savedTime             = 0;
    this->bIsPlaying            = false;
    this->durationMillis        = 0;
    this->bLoop                 = false;
    this->customEventMillis     = 0;
    this->bHasShotCustomEvent   = false;
    
    this->width                 = 0;
    this->height                = 0;
}

void ImageSequencePlayer::setup() {
    
}
void ImageSequencePlayer::update(){
    
    // try to reach that fps
     if(bIsPlaying)
         updateFrame();
    
}

void ImageSequencePlayer::draw(float x, float y) {
    
    if(bIsPlaying && currentFrame <= images.size() -1)
        images[currentFrame].draw(x,y);
}


void ImageSequencePlayer::draw(float x, float y, float w, float h){
    
    if(bIsPlaying && currentFrame <= images.size() -1)
        images[currentFrame].draw(x,y,w,h);
    
}

void ImageSequencePlayer::updateFrame() {
    
    if(!bIsPlaying || this->fps == 0)
        return;
    
    int currentTime = ofGetElapsedTimeMillis();
    
    // diff between each frame should be 1000ms / fps
    
    int diff = currentTime - savedTime;
    int totalFrame      = images.size();
    int totalTime       = totalFrame * ( 1000.0 / (float)this->fps );
    currentFrame        = (floor)(diff * totalFrame / (float) totalTime);
    
    if(customEventMillis > 0 && durationMillis * ( 1.0 - getPosition() ) < customEventMillis && !bHasShotCustomEvent) {

        ofNotifyEvent(videoCustomTimingEvent, currentFrame);
        bHasShotCustomEvent = true;
    }
    
    if(currentFrame >= images.size() - 1) {
        
        currentFrame = 0;
        
        if(!bLoop)
            stop();
        else
            play();
        

        ofNotifyEvent(videoCompleteEvent, currentFrame);
    }
    
}


void ImageSequencePlayer::loadFolder(string path){
    
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");

    dir.listDir();
    dir.sort();
    int nFiles = dir.size();
    
    for(int i=0; i<nFiles; i++) {
        
        
        ofImage img;
        //ofLogNotice("load image") << dir.getPath(i);
        img.load(dir.getPath(i));
        
        /*
        img.mirror(true, true);

        // flip
        ofImage cropped;
        cropped = img;
        cropped.crop(0.0, 0.0, cropped.getWidth() * .5, cropped.getHeight());
        cropped.mirror(false, true);
        
        ofFbo fbo;
        fbo.allocate(img.getWidth(), img.getHeight());
        fbo.begin();
        ofClear(0);
        img.draw(0.0,0.0);
        cropped.draw(0.0, 0.0);
        fbo.end();
        
        ofPixels pixels;
        fbo.readToPixels(pixels);
        img.setFromPixels(pixels);
         
         */
        
        images.push_back(img);
        
        width   = MAX(width, img.getWidth());
        height  = MAX(height, img.getHeight());
        
    }
    
    computeDuration();
    
}
void ImageSequencePlayer::setFps(int fps){
    
    this->fps = fps;
    computeDuration();
}

void ImageSequencePlayer::play(){
    
    if(images.size() > 0) {
        bIsPlaying = true;
        resetCustomEvent();
        savedTime = ofGetElapsedTimeMillis();
       // ofLogNotice("savedTime on play") << savedTime;

    }
    
    

}

void ImageSequencePlayer::stop(){
    
    if(bIsPlaying) {
        bIsPlaying = false;
        resetCustomEvent();
    }
}

void ImageSequencePlayer::setLoop(bool bLoop) {
    
    this->bLoop = bLoop;
    
}


void ImageSequencePlayer::goToFrame(int frameID){
    
    currentFrame = frameID;
    
    // todo – check if frame position is before custom event millis
    // We'll not need that for the project I guess
    resetCustomEvent();

}

float ImageSequencePlayer::getPosition() {
    return currentFrame / (float)images.size();
    
}


int ImageSequencePlayer::getNumFrames(){
    return images.size();
}
void ImageSequencePlayer::computeDuration() {
    
    int nImages     = images.size();
    float seconds   = (float)nImages / fps;
    durationMillis  = seconds * 1000.0;
    
}


float   ImageSequencePlayer::getDurationInMillis(){
    return durationMillis;
}

bool    ImageSequencePlayer::isPlaying(){
    
    return bIsPlaying;
    
}

void    ImageSequencePlayer::setCustomTimingMillisEvent(int millis) {
    
    customEventMillis = millis;
}

void    ImageSequencePlayer::resetCustomEvent() {
    bHasShotCustomEvent = false;
}

void   ImageSequencePlayer::nextFrame() {
    
    currentFrame++;
    if (currentFrame > images.size() - 1 )
        currentFrame = 0;
}

