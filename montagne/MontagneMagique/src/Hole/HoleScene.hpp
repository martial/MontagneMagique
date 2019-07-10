//
//  HoleScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 11/04/2019.
//

#ifndef HoleScene_hpp
#define HoleScene_hpp

#include <stdio.h>

#include "AbstractARScene.hpp"

class HoleScene : public AbstractARScene {
    
public:
    
    HoleScene() {};
    void setup(string name);
    void update();
    void draw();
    
private:
    
    
};


#endif /* HoleScene_hpp */
