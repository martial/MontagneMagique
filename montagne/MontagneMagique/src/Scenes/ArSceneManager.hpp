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
    
    void setup(vector<std::shared_ptr<MagiqueMarker>> & trackers);
    void update();
    void drawScene(int sceneIndex);
    void draw(int markerIndex, MagiqueMarker & marker);
    
private:
    
    vector<AbstractARScene*> scenes;
    int getSceneIndexForMarkerID(string markerID);
};

#endif /* ArSceneManager_hpp */
