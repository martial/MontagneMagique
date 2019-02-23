//
//  ArSceneManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#include "ArSceneManager.hpp"
#include "UndergroundScene.hpp"
#include "EggsScene.hpp"
#include "VideoScene.hpp"
#include "BigBangScene.hpp"
#include "BeesDepthScene.hpp"
#include "BirdsScene.hpp"


void ArSceneManager::setup(vector<std::shared_ptr<MagiqueMarker>> & trackers) {
    
    bDebugMode = false;
    
    // UndergroundScene * undergroundScene = new UndergroundScene();
   // undergroundScene->setup("underground");
    
   // EggsScene * eggsScene = new EggsScene();
   // eggsScene->setup("eggs");
    
   // VideoScene * videoScene = new VideoScene();
   // videoScene->setup("video");
    
    BigBangScene * bigBangScene = new BigBangScene();
    bigBangScene->setup("bigbang");
    
    BirdsScene * birdsScene = new BirdsScene();
    birdsScene->setup("birds");
    
    BeesDepthScene * beesScene = new BeesDepthScene();
    beesScene->setup("beesDepth");

    scenes.push_back(bigBangScene);

    scenes.push_back(birdsScene);
    //scenes.push_back(videoScene);
   // scenes.push_back(eggsScene);
    //scenes.push_back(undergroundScene);
    scenes.push_back(beesScene);
    
    for(int i=0; i<trackers.size(); i++) {
        
        int sceneIndex = getSceneIndexForMarkerID(trackers[i]->markerid);
        if(sceneIndex >= 0)
            scenes[sceneIndex]->marker = trackers[i];
        
    }

}

void ArSceneManager::update() {
    
    //scenes[0]->update();

    
}

void ArSceneManager::drawScene(int sceneIndex) {
    
    scenes[sceneIndex]->update();
    scenes[sceneIndex]->draw();
    
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

AbstractARScene * ArSceneManager::getSceneIndexForPath(string path) {
    
    for(int i=0; i<scenes.size(); i++) {
        
        if(scenes[i]->name == path)
            return scenes[i];
        
    }
    
    return NULL;
    
}

void ArSceneManager::setDebugMode(bool debugMode) {
    
    for(int i=0; i<scenes.size(); i++) {
        
        scenes[i]->bDebugMarker = debugMode;
          
    }
    this->bDebugMode = debugMode;
}


