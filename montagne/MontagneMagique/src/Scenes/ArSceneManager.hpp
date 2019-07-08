//
//  ArSceneManager.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#ifndef ArSceneManager_hpp
#define ArSceneManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "AbstractARScene.hpp"
#include "MagiqueMarker.hpp"

class ArSceneManager {
    
public:
    
    void loadAutoScenes();
    void setup(vector<std::shared_ptr<MagiqueMarker>> & trackers);
    void update();
    void drawScene(int sceneIndex);
    void draw(int markerIndex, MagiqueMarker & marker);
    
    void setDebugMode(bool debugMode);
    
    void setGenericSceneLoopMode(ofLoopType loopState);
    
    void activateAllMarkers();
    void activateMarkersFromScene();
    
    AbstractARScene * getSceneIndexForIndex(int index, MagiqueMarker & marker);
    AbstractARScene * getSceneIndexForPath(string path);
    int getSceneIndexForMarkerID(string markerID);
    vector<AbstractARScene*> scenes;

    
    bool bDebugMode;

private:
    

};

#endif /* ArSceneManager_hpp */
