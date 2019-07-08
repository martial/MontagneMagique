//
//  VideoPlayer.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 20/05/2019.
//

#include "VideoPlayer.hpp"
#include "ofxImgSizeUtils.h"

void VideoPlayer::setup() {
    
   
   
    
}

void VideoPlayer::update() {
    
    player.update();
    
    
}

void VideoPlayer::draw(float maxWidth, float maxHeight) {
    
    if(player.isLoaded() && !player.getIsMovieDone()) {
    
        ofRectangle rect = ofxImgSizeUtils::getCenteredRect(maxWidth, maxHeight, player.getWidth(), player.getHeight(), false);
        player.draw(rect.x, rect.y, rect.width, rect.height);
        
    }

}

void VideoPlayer::setVideo(string name, bool loop) {
    
    ofDirectory dir("videos");
    dir.allowExt("mov");
    dir.allowExt("mp4");
    
    dir.listDir();
    dir.sort();
    int nFiles = dir.size();
    
    for(int i=0; i<nFiles; i++) {
        
        
        if(dir.getName(i) == name ) {
            
            player.load(dir.getPath(i));
            
            ofLogNotice("is loop ? ") << loop;
            
            if(!loop)
                player.setLoopState(OF_LOOP_NONE);
            else
                player.setLoopState(OF_LOOP_NORMAL);
            
            player.play();
            
        }
        
    }
        
}
