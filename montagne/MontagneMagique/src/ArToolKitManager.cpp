//
//  ArToolKitManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 13/12/2018.
//

#include "ArToolKitManager.hpp"
#include <glm/gtx/matrix_decompose.hpp>

void ArToolKitManager::setup(int width, int height) {
    
    inputWidth  = width;
    inputHeight = height;
    
    trackers.reserve(10);
    loadTrackers();
    
}

void ArToolKitManager::loadTrackers() {
    
    ofDirectory dir("markers");
    dir.listDir();
    ofDisableDataPath();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getFile(i).isDirectory()) {
            
            string dirName = dir.getFile(i).getAbsolutePath();
            ofDirectory subdir(dirName);
            subdir.listDir();
            
            for(int j = 0; j < subdir.size(); j++){
                subdir[j].copyTo("../Resources/"+subdir[j].getFileName(), false, true);
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
            myTextFile << "FILTER 15.0\n";
            
            
            
        }
        
    }
    
    ofDirectory markersFolder;
    markersFolder.open("markers");
    ofDisableDataPath();
    // markersFolder.copyTo("../Resources/markers", false, true);
    ofEnableDataPath();
    
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getFile(i).isDirectory()) {
            
            string name = dir.getFile(i).getBaseName();
            std::shared_ptr<ofxArtool5::NftTracker> tracker = std::make_shared<ofxArtool5::NftTracker>();
            tracker->setup(ofVec2f(inputWidth,inputHeight),ofVec2f(inputWidth,inputHeight), OF_PIXELS_BGR, "../Resources/camera_para.dat", "../Resources/m_"+name+".dat");
            ofAddListener(tracker->evNewMarker, this, &ArToolKitManager::onNewMarker);
            ofAddListener(tracker->evLostMarker, this, &ArToolKitManager::onLostMarker);
            trackers.push_back(tracker);
            
        }
    }
}

void ArToolKitManager::update(ofBaseHasPixels & input){
    
    for(int i=0; i<trackers.size(); i++) {
        trackers[i]->update(input);
    }
    
}

void ArToolKitManager::debugDraw() {
    
    vector<ofPoint> originPnts;
    
    for(int i=0; i<trackers.size(); i++) {
        
        if(trackers[i]->isFound()){
            
            trackers[i]->beginAR();
            
            /*
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW), scale, rotation, translation, skew, perspective);
            
            ofPoint origin;
            origin.x =  ofGetWidth() - (ofGetWidth()*.5 -  translation.x);
            origin.y =  ofGetHeight() *.5 - translation.y;
            //origin.z =  -translation.z;
            
            originPnts.push_back(origin);
             
             */
          
             ofDrawLine(-1000, 0, 1000, 0);
             ofDrawLine(0, -1000, 0, 1000);
             ofDrawBox(0, 0, 0.5, 21, 21, 21);
            
            ofDrawRectangle(0.0, 0.0, 300, 300);
            
            
            trackers[i]->endAR();
        }
        
    }
    
    for(int i=0; i<originPnts.size(); i++) {
        if(i>0) {
            ofDrawLine(originPnts[i], originPnts[i-1]);
        }
    }
    
}


//--------------------------------------------------------------
void ArToolKitManager::onNewMarker(int & mId){
    cout<<"New Marker found!"<<endl;
}

void ArToolKitManager::onLostMarker(int & mId){
    cout<<"Marker lost!"<<endl;
}