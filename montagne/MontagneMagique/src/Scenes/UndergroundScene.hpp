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
#include "SimpleParticle.hpp"

class UndergroundScene : public AbstractARScene {
    
public:
    
    UndergroundScene() {};
    void setup(string name);
    void update();
    void draw(int markerIndex, int markerWidth, int markerHeight);
    
private:
    
    ofImage bug0, bug1;
    vector<SimpleParticle*> particles;
    
};

#endif /* UndergroundScene_hpp */
