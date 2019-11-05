//
//  VideoPlayer.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 20/05/2019.
//

#ifndef VideoPlayer_hpp
#define VideoPlayer_hpp

#include "ofMain.h"
#include "ofxHapPlayer.h"


class VideoPlayer {
  
public:
    
    void setup();
    void update();
    void draw(float maxWidth, float maxHeight);
    void stop();
    void setVideo(string name, bool loop = false);
    
    void preloadVideos();
    
    ofxHapPlayer player;
    vector<ofxHapPlayer*> players;
    
private:
    
    bool    bPreloadVideos;
    int     currentPlayerIndex;
    
    ofJson  configJson;
    ofDirectory dir;
    
};

#endif /* VideoPlayer_hpp */
