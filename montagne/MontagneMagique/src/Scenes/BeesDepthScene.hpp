//
//  BeesDepthScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#ifndef BeesDepthScene_hpp
#define BeesDepthScene_hpp

#include "AbstractARScene.hpp"

class BeesDepthScene : public AbstractARScene {
    
public:
    
    BeesDepthScene() {};
    void setup(string name);
    void update();
    void draw();
    
private:
    
    ofImage bee0, bee1, bee2;
    
    vector<ofVec2f> positions;
    vector<float> scales;

    
};




#endif /* BeesDepthScene_hpp */
