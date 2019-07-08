//
//  GenericScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 10/04/2019.
//

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
    
    void setLoopMode(ofLoopType loopMode);
    
private:
    
    int numOfVideos;
    vector<ofxHapPlayer*> videoPlayers;

    

};


#endif /* GenericScene_hpp */
