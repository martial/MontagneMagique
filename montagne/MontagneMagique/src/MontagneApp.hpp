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
#include "VideoPlayer.hpp"

#define TRACKING_MODE 0
#define SCENE_MODE 1
#define HAP_MODE 2
#define DRAW_MODE 3

class MontagneApp {
  
public:
    
    void setup();
    void setupTrackers(int trackerInputWidth, int trackerInputHeight);
    void setupFbos();
    void updateTrackers(ofBaseHasPixels & input);
    void updateScene();
    void drawScene(bool bDraw = true);
    void drawOffScreen();

    void processDebugDraw();
    void debugDrawTrackers();
    
    void setMode(int mode);
    void setPastMode();
    int  getMode();

    ArToolKitManager  & getArToolKitManager(){return arToolKitManager;};
    
    ofFbo debugFboLayer, fboLayer;
    
    // TODO move this into JSON config in ofApp*
    int videoOutputWidth, videoOutputHeight;
    
    ArSceneManager      arSceneManager;
    VideoPlayer         hapPlayer;

    // from OSC
    string currentSceneName, currentSubSceneName;

    
private:
    
    int trackerInputWidth, trackerInputHeight;
    
    ArToolKitManager    arToolKitManager;
    
    int mode, pastMode;
    
    // Mask FBO
    ofShader shader;
    ofImage backgroundImage;
    ofImage foregroundImage;
    ofImage brushImage;
    ofFbo  * maskFbo;
    ofFbo tempFbo;
    
    
    
};

#endif /* MontagneApp_hpp */
