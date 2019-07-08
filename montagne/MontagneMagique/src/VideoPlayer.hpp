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
    
    void setVideo(string name, bool loop = false);
    
    ofxHapPlayer player;

    
private:
    
    ofJson configJson;
    
};

#endif /* VideoPlayer_hpp */
