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
