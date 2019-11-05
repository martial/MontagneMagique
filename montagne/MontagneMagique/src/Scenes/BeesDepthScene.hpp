#ifndef BeesDepthScene_hpp
#define BeesDepthScene_hpp

#include "AbstractARScene.hpp"
#include "Bee.hpp"

class BeesDepthScene : public AbstractARScene {
    
public:
    
    BeesDepthScene() {};
    void setup(string name);
    void update();
    void draw();
    
private:
    
    ofImage bee0, bee1, bee2, beeShadow;
    
    vector<Bee> bees;
    
    vector<ofVec2f> positions, targetPositions;
    vector<float> scales, targetScales;
    
};

#endif /* BeesDepthScene_hpp */
