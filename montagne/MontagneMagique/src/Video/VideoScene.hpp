#ifndef VideoScene_hpp
#define VideoScene_hpp

#include "AbstractARScene.hpp"

class VideoScene : public AbstractARScene {
    
public:
    
    VideoScene() {};
    void setup(string name);
    void update();
    void draw();
    
private:
    
    ofVideoPlayer player;
    
};

#endif /* EggsScene_hpp */
