#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(30);
    
    status = "Waiting for files to be dropped";
    
    
    gui.setup(); // most of the time you don't need a name
    gui.add(scale.setup("scale", 1, 0, 2));
   
    gui.add(rows.setup("rows", 1, 1, 12));
    gui.add(cols.setup("cols", 1, 1, 12));
    gui.add(generate.setup("generate"));
    generate.addListener(this, &ofApp::generateMarkers);


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    ofSetColor(255);
    
    ofDrawBitmapString(status, 20, 20);
    
    if(draggedImages.size() > 0 && draggedImages[0].isAllocated()) {
        
        ofPushMatrix();
        
        ofTranslate(ofGetWidth() * .5, ofGetHeight() * .5);
        ofScale(scale,scale);
        ofTranslate(-draggedImages[0].getWidth() * .5, -draggedImages[0].getHeight() * .5);
        draggedImages[0].draw(0.0, 0.0);
        
        for(int i=0; i<cols; i++) {
            
            for(int j=0; j< rows; j++) {
                
                float w = draggedImages[0].getWidth() / rows;
                float h = draggedImages[0].getHeight() / cols;
                
                ofNoFill();
                ofDrawRectangle(w * j, h * i, w, h);
                
            }
            
        }
        
        ofPopMatrix();
        
        gui.draw();
    }
    
}

void ofApp::generateMarkers() {
    
    int count = 0;
    for(int i=0; i<cols; i++) {
        
        for(int j=0; j< rows; j++) {
            
            float w = draggedImages[0].getWidth() / rows;
            float h = draggedImages[0].getHeight() / cols;
            
            ofPoint pos;
            pos.x = j * w;
            pos.y = i * h;
            
            ofImage imgClone = draggedImages[0];
            imgClone.crop(pos.x, pos.y, w, h);
            string filename = ofToString(count) + ".jpg";
            imgClone.save(filename);
                        
            status = "¨Procesing";
            
            string path = ofToDataPath(filename, true);
            string command = "../../../../../Utils/genTexData " + path + " -level=4 -leveli=3 -dpi=72 -max_dpi=72 -min_dpi=4";
            string result = ofSystem(command);
            ofLogNotice("result ") << result;
            
            count++;
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
        for(unsigned int k = 0; k <1; k++){
            
            draggedImages[k].load(dragInfo.files[k]);
            return;
        }
    

    }
    
}
