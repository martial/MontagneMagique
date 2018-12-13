#include "ofApp.h"
#include <glm/gtx/matrix_decompose.hpp>


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_NOTICE);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    cam.initGrabber(640, 480);
    
    loadTrackers();
    
    trackers.reserve(2);
    
}

void ofApp::loadTrackers() {
    
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
            tracker->setup(ofVec2f(640,480),ofVec2f(640,480), OF_PIXELS_BGR, "../Resources/camera_para.dat", "../Resources/m_"+name+".dat");
            ofAddListener(tracker->evNewMarker, this, &ofApp::onNewMarker);
            ofAddListener(tracker->evLostMarker, this, &ofApp::onLostMarker);
            trackers.push_back(tracker);
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    
    if(cam.isFrameNew()){
        
        for(int i=0; i<trackers.size(); i++) {
            trackers[i]->update(cam);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::onNewMarker(int & mId){
    cout<<"New Marker found!"<<endl;
}

void ofApp::onLostMarker(int & mId){
    cout<<"Marker lost!"<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    cam.draw(0, 0);
    
    vector<ofPoint> originPnts;
    
    for(int i=0; i<trackers.size(); i++) {
        //trackers[i]->update(cam);
        
        if(trackers[i]->isFound()){
            trackers[i]->beginAR();
            
            
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW), scale, rotation, translation, skew, perspective);
            
            //ofLogNotice("translation x ") << ofGetWidth() - (ofGetWidth()*.5 -  translation.x )<< " " << ofGetHeight()*.5 - translation.y << " "  << translation.z ;
            
            ofPoint origin;
            origin.x =  ofGetWidth() - (ofGetWidth()*.5 -  translation.x);
            origin.y =  ofGetHeight() *.5 - translation.y;
            //origin.z =  -translation.z;

            originPnts.push_back(origin);
            
           // ofDrawLine(-1000, 0, 1000, 0);
            //ofDrawLine(0, -1000, 0, 1000);
           // ofDrawBox(0, 0, 0.5, 21, 21, 21);
            
            //cam.draw(0, 0, 592 * .4,600 * .4 );
            
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
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
