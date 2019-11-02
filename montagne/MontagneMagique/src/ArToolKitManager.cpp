//
//  ArToolKitManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 13/12/2018.
//

#include "ArToolKitManager.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include "ofApp.h"

void ArToolKitManager::setup(int width, int height) {
    
    inputWidth  = width;
    inputHeight = height;
    
    //trackers.reserve(10);
    loadTrackers();
    
}

void ArToolKitManager::setDelays( int markerLostDelay, int markerFoundDelay) {
    
    this->markerLostDelay   = markerLostDelay;
    this->markerFoundDelay  = markerFoundDelay;
    
    for(int i=0; i<trackers.size(); i++) {
        
        trackers[i]->timeFoundDelay = markerFoundDelay;
        trackers[i]->timeLostDelay  = markerLostDelay;

    }

}


void ArToolKitManager::loadTrackers() {
    
    ofDirectory dir("markers");
    dir.listDir();
    dir.sort();
    ofDisableDataPath();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getFile(i).isDirectory()) {
            
            string dirName = dir.getFile(i).getAbsolutePath();
            ofDirectory subdir(dirName);
            subdir.listDir();
            subdir.sort();
            
            for(int j = 0; j < subdir.size(); j++){
                subdir[j].copyTo("../Resources/"+subdir[j].getFileName(), false, true);
                
                if(subdir[j].getExtension() == "jpg" ) {
                    
                    ofImage img;
                    img.load(subdir[j].getAbsolutePath());
                    images.push_back(img);
                    
                }
                
            }
            
            ofDisableDataPath();
            string name = dir.getFile(i).getBaseName();
            // create marker file
            string fileName ="../Resources/m_"+name+".dat";
            
            ofFile myTextFile;
            myTextFile.open(fileName,ofFile::WriteOnly);
            myTextFile << "# Number of markers\n";
            myTextFile << "1\n";
            myTextFile << name+"\n";
            myTextFile << "NFT\n";
            myTextFile << "FILTER 5.0\n";
            
            myTextFile.close();
            
            subdir.close();
            
        }
        
    }
    
    dir.close();
    
    ofDirectory markersFolder;
    markersFolder.open("markers");
    ofDisableDataPath();
    markersFolder.copyTo("../Resources/markers", false, true);
    ofEnableDataPath();
    
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getFile(i).isDirectory()) {
            
            string name = dir.getFile(i).getBaseName();
            std::shared_ptr<MagiqueMarker> tracker = std::make_shared<MagiqueMarker>();
            
            tracker->setup(ofVec2f(inputWidth,inputHeight),ofVec2f(inputWidth,inputHeight), OF_PIXELS_BGR, "../Resources/camera_para.dat", "../Resources/m_"+name+".dat");
            tracker->markerid           = name;
            tracker->timeFoundDelay     = markerFoundDelay;
            tracker->timeLostDelay      = markerLostDelay;
            tracker->setImage(&images[i]);
            
            ofAddListener(tracker->evNewMarker, this, &ArToolKitManager::onNewMarker);
            ofAddListener(tracker->evLostMarker, this, &ArToolKitManager::onLostMarker);
            ofAddListener(tracker->solidFoundEvent, this, &ArToolKitManager::onSolidFoundEvent);
            ofAddListener(tracker->solidLostEvent, this, &ArToolKitManager::onSolidLostEvent);
            
            trackers.push_back(tracker);
            
        }
    }
    
    markersFolder.close();
}

void ArToolKitManager::update(ofBaseHasPixels & input){
    
    for(int i=0; i<trackers.size(); i++) {
        
        if(trackers[i]->bIsActive) {
            
            trackers[i]->update(input);
            trackers[i]->updateTimes();
                        
        }

    }
    
}

void ArToolKitManager::debugDraw() {
    
    vector<ofPoint> originPnts;
    
    for(int i=0; i<trackers.size(); i++) {
        
        if(trackers[i]->isFound()){
            
            trackers[i]->beginAR();
            
            
            if( i < images.size() && images[i].isAllocated() ) {
                
                ofPushMatrix();
                ofScale(1, -1.0);
                ofTranslate(0.0, -trackers[i]->height);
                ofSetColor(255, 125);
                trackers[i]->image->draw(0.0, 0.0, trackers[i]->width, trackers[i]->height);
                ofSetColor(255, 255);
                ofPopMatrix();

            }
    
            trackers[i]->endAR();
        }
        
    }
    ofSetColor(255);

    for(int i=0; i<originPnts.size(); i++) {
        if(i>0) {
            ofDrawLine(originPnts[i], originPnts[i-1]);
        }
    }
    
}

void ArToolKitManager::clean() {
    
    for(int i=0; i<trackers.size(); i++) {
        trackers[i]->cleanup();
    }
    
    trackers.clear();
    
}



void ArToolKitManager::onSolidFoundEvent(string & markerid) {
    

    ofApp* app = (ofApp*) ofGetAppPtr();
    app->oscManager.sendMessage("/marker/"+markerid, 1);
    app->addMessage("Marker found : " + markerid );
    
    int sceneIndex = app->app.arSceneManager.getSceneIndexForMarkerID(markerid);
    app->app.arSceneManager.scenes[sceneIndex]->onMarkerTracked();

   

}

void ArToolKitManager::onSolidLostEvent(string & markerid) {
    
    ofApp* app = (ofApp*) ofGetAppPtr();    
    app->oscManager.sendMessage("/marker/" + markerid, 0);
    app->addMessage("Marker lost : " + markerid );
    
    int sceneIndex = app->app.arSceneManager.getSceneIndexForMarkerID(markerid);
    app->app.arSceneManager.scenes[sceneIndex]->onMarkerLost();
    
}

//--------------------------------------------------------------
void ArToolKitManager::onNewMarker(int & mId){
    //cout<<"New Marker found!"<<endl;
    
    
    
}

void ArToolKitManager::onLostMarker(int & mId){
   // cout<<"Marker lost!"<<endl;
}

