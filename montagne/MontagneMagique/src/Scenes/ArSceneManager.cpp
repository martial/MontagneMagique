//
//  ArSceneManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#include "ArSceneManager.hpp"
#include "UndergroundScene.hpp"
#include "EggsScene.hpp"

void ArSceneManager::setup(vector<std::shared_ptr<MagiqueMarker>> & trackers) {
    
    UndergroundScene * undergroundScene = new UndergroundScene();
    undergroundScene->setup("underground");
    
    EggsScene * eggsScene = new EggsScene();
    eggsScene->setup("eggs");

    scenes.push_back(eggsScene);
    scenes.push_back(undergroundScene);
    
    for(int i=0; i<trackers.size(); i++) {
        
        int sceneIndex = getSceneIndexForMarkerID(trackers[i]->markerid);
        scenes[sceneIndex]->marker = trackers[i];
        
    }


    
}

void ArSceneManager::update() {
    
    //scenes[0]->update();

    
}

void ArSceneManager::draw(int markerIndex, MagiqueMarker & marker) {
    
    int sceneIndex = getSceneIndexForMarkerID(marker.markerid);
    
    if(sceneIndex >= 0) {
        
        scenes[sceneIndex]->update();
        scenes[sceneIndex]->draw();

    }
    
}


int ArSceneManager::getSceneIndexForMarkerID(string markerID) {
    
    for(int i=0; i<scenes.size(); i++) {
        
        if(scenes[i]->markerID == markerID)
            return i;
        
    }
    
    return -1;
    
}
