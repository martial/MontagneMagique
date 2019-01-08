//
//  ArSceneManager.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#ifndef ArSceneManager_hpp
#define ArSceneManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "AbstractARScene.hpp"

class ArSceneManager {
    
public:
    
    void setup();
    void update();
    void draw(int markerIndex, int markerWidth, int markerHeight);
    
private:
    
    vector<AbstractARScene*> scenes;
    
};

#endif /* ArSceneManager_hpp */
