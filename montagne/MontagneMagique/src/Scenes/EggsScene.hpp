//
//  EggsScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 23/01/2019.
//

#ifndef EggsScene_hpp
#define EggsScene_hpp

#include "AbstractARScene.hpp"
#include "Eggs.hpp"

class EggsScene : public AbstractARScene {
    
public:
    
    EggsScene() {};
    void setup(string name);
    void update();
    void draw(int markerIndex, int markerWidth, int markerHeight);
    
private:
    
    Eggs eggs;
  
    
};

#endif /* EggsScene_hpp */
