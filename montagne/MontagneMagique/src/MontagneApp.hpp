//
//  MontagneApp.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 14/12/2018.
//

#ifndef MontagneApp_hpp
#define MontagneApp_hpp

#include "ofMain.h"
#include "ArToolKitManager.hpp"
#include "ArSceneManager.hpp"

class MontagneApp {
  
public:
    
    void setup();
    void setupTrackers(int trackerInputWidth, int trackerInputHeight);
    void setupFbos();
    void updateTrackers(ofBaseHasPixels & input);
    
    
    void drawScene();

    void processDebugDraw();
    void debugDrawTrackers();
    
    ArToolKitManager  & getArToolKitManager(){return arToolKitManager;};
    
    ofFbo debugFboLayer, fboLayer;
    
    
private:
    
    int trackerInputWidth, trackerInputHeight;
    
    ArToolKitManager    arToolKitManager;
    ArSceneManager      arSceneManager;
    
    
};

#endif /* MontagneApp_hpp */
