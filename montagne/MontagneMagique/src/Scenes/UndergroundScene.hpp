//
//  UndergroundScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#ifndef UndergroundScene_hpp
#define UndergroundScene_hpp

#include <stdio.h>
#include "AbstractARScene.hpp"

class UndergroundScene : public AbstractARScene {
    
public:
    
    void setup();
    void update();
    void draw(int markerIndex, int markerWidth, int markerHeight);
    
    
};

#endif /* UndergroundScene_hpp */
