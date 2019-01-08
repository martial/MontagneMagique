#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(30);
    
    status = "Waiting for files to be dropped";
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    ofSetColor(255);
    
    ofDrawBitmapString(status, 20, 20);
    
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
    
    if( dragInfo.files.size() > 0 ){
        
        status = "Analyzing";

        dragPt = dragInfo.position;
        
        float subDivisions = 1;
        
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            draggedImages[k].load(dragInfo.files[k]);
            
            // cut and save
            
            int count = 0;
            for(int i=0; i<subDivisions; i++) {
                
                for(int j=0; j< subDivisions; j++) {
                    
                    float w = draggedImages[k].getWidth() / subDivisions;
                    float h = draggedImages[k].getHeight() / subDivisions;
                    
                    ofPoint pos;
                    pos.x = i * w;
                    pos.y = j * h;
                    
                    ofImage imgClone = draggedImages[k];
                    imgClone.crop(pos.x, pos.y, w, h);
                    string filename = ofToString(count) + ".jpg";
                    imgClone.save(filename);
                    
                    status = "¨Procesing";

                    string path = ofToDataPath(filename, true);
                    
                    string command = "../../../../../Utils/genTexData " +path + " -level=4 -leveli=3 -dpi=72 -max_dpi=72 -min_dpi=4";
                    string result = ofSystem(command);
                    ofLogNotice("result ") << result;
                    
                    count++;
                }
                
            }
            
        }
        
        // copy everything into the main app
        
        string path = "";
        ofDirectory dir(path);
        dir.listDir();
        
        //go through and print out all the paths
        for(int i = 0; i < dir.size(); i++){
            dir.getFile(i).moveTo("../../../MontagneMagique/bin/data/markers/"+dir.getFile(i).getBaseName()+"/"+dir.getFile(i).getFileName(), true);
        }
        
        status = "¨All done. ";

    }
    
}
