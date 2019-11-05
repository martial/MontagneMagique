#ifndef GenericScene_hpp
#define GenericScene_hpp

#include <stdio.h>
#include "AbstractARScene.hpp"
#include "ofxHapPlayer.h"

class GenericScene : public AbstractARScene {
    
public:
    
    GenericScene() {};
    
    void loadVideos();
    void setup(string name);
    void update();
    void draw();
    
    void onMarkerTracked();
    void onMarkerLost();

    void setLoopMode(ofLoopType loopMode);
    
private:
    
    int numOfVideos;
    vector<ofxHapPlayer*> videoPlayers;
    
};


#endif /* GenericScene_hpp */
