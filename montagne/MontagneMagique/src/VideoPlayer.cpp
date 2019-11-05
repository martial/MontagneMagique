#include "VideoPlayer.hpp"
#include "ofxImgSizeUtils.h"
#include "ofApp.h"

void VideoPlayer::setup() {

    bPreloadVideos = true;
    
    dir.open("videos");
    dir.allowExt("mov");
    dir.allowExt("mp4");
    dir.listDir();
    dir.sort();
    
    if(bPreloadVideos) {
        preloadVideos();
        currentPlayerIndex = -1;
    }
    
}

void VideoPlayer::update() {
    
    player.update();
    
}

void VideoPlayer::draw(float maxWidth, float maxHeight) {
    
    if(!bPreloadVideos) {

        if(player.isLoaded() && !player.getIsMovieDone()) {
        
            ofRectangle rect = ofxImgSizeUtils::getCenteredRect(maxWidth, maxHeight, player.getWidth(), player.getHeight(), false);
            player.draw(rect.x, rect.y, rect.width, rect.height);
            
        }
        
    } else {
                
        if(currentPlayerIndex >= 0 && players[currentPlayerIndex]->isLoaded() && !players[currentPlayerIndex]->getIsMovieDone()) {
            
            ofRectangle rect = ofxImgSizeUtils::getCenteredRect(maxWidth, maxHeight, players[currentPlayerIndex]->getWidth(), players[currentPlayerIndex]->getHeight(), false);
            players[currentPlayerIndex]->draw(rect.x, rect.y, rect.width, rect.height);
            
        }
        
    }

}

void VideoPlayer::stop() {
    
     if(!bPreloadVideos) {
         
         player.close();
         
     } else {
         
         if(currentPlayerIndex >= 0 )
             players[currentPlayerIndex]->stop();
     }
    
}

void VideoPlayer::preloadVideos() {
    
    int nFiles = dir.size();
    
    for(int i=0; i<nFiles; i++) {
        
        ofxHapPlayer * player = new ofxHapPlayer();
        bool bHasLoaded = player->load(dir.getPath(i));
        
        if(bHasLoaded ) {
            players.push_back(player);
        }
        
    }
    
}

void VideoPlayer::setVideo(string name, bool loop) {
    
    int nFiles = dir.size();
    
    currentPlayerIndex = -1;
    
    for(int i=0; i<nFiles; i++) {
        
        if(dir.getName(i) == name ) {
            
            if(!bPreloadVideos) {
                
                player.close();
                player.load(dir.getPath(i));
                
                if(!loop)
                    player.setLoopState(OF_LOOP_NONE);
                else
                    player.setLoopState(OF_LOOP_NORMAL);
                
                player.play();
                
            } else {
                
                currentPlayerIndex = i;
                
                if(!loop)
                    players[i]->setLoopState(OF_LOOP_NONE);
                else
                    players[i]->setLoopState(OF_LOOP_NORMAL);
                
                players[i]->setPosition(0.0);
                players[i]->play();
                break;
                
            }
            
        }
        
    }
    
}
