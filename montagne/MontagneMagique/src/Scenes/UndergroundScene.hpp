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
#include "Ants.hpp"

class UndergroundScene : public AbstractARScene {
    
public:
    
    UndergroundScene() {};
    void setup(string name);
    void update();
    void draw();
    
private:
    
    Ants ants;
    
};

#endif /* UndergroundScene_hpp */
