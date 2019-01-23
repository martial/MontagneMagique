//
//  ArSceneManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#include "ArSceneManager.hpp"
#include "UndergroundScene.hpp"
#include "EggsScene.hpp"

void ArSceneManager::setup() {
    
    UndergroundScene * undergroundScene = new UndergroundScene();
    undergroundScene->setup("underground");
    
    EggsScene * eggsScene = new EggsScene();
    eggsScene->setup("eggs");

    scenes.push_back(eggsScene);
    scenes.push_back(undergroundScene);

    
    
}

void ArSceneManager::update() {
    
    scenes[0]->update();

    
}

void ArSceneManager::draw(int markerIndex, int markerWidth, int markerHeight) {
    
    scenes[0]->draw(markerIndex, markerWidth, markerHeight);
    
}
