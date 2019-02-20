//
//  Ants.hpp
//  2DSceneApp
//
//  Created by Martial Geoffre-Rouland on 16/01/2019.
//

#ifndef Ants_hpp
#define Ants_hpp

#include <stdio.h>
#include "ofMain.h"
#include "SimpleParticle.hpp"
#include "ImageSequencePlayer.hpp"

class Ants {

public:
    
    void setup(string dataPath = "");
    void update();
    void draw();
    
private:
    
    vector<SimpleParticle*>         particles;
    vector<ImageSequencePlayer>    images;

    
};

#endif /* Ants_hpp */
