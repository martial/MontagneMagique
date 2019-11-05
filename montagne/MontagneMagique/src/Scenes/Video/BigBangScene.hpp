//
//  BigBangScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#ifndef BigBangScene_hpp
#define BigBangScene_hpp

#include "AbstractARScene.hpp"
#include "BigBang.hpp"

class BigBangScene : public AbstractARScene {
    
public:
    
    BigBangScene() {};
    void setup(string name);
    void update();
    void draw();
    
    
    BigBang bigBang;
    
private:
    
    void setProps();
    
};

#endif /* BigBangScene_hpp */
