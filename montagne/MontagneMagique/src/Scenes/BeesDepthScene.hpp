//
//  BeesDepthScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#ifndef BeesDepthScene_hpp
#define BeesDepthScene_hpp

#include "AbstractARScene.hpp"
#include "Bee.hpp"

/*
struct SortByX
{
    bool operator() const (MyClass const & L, MyClass const & R) { return L.x < R.x; }
};
*/
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
