#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(60);
    contextWasUpdated = true;
    
    camWidth        = 640;
    camHeight       = 480;
    
#ifdef USE_BUFFER
    camera.setup(camWidth, camHeight);
    vconf = "-module=Buffer -width=640 -height=480 -format=RGB";
#else
    vconf = "-module=AVFoundation -width=640 -height=480";
    
    pixelBuffer = new uint32_t[camWidth * camHeight * 3];
    tex.allocate(camWidth, camHeight, GL_RGBA);
    
#endif
    cpara = NULL;

    arLogLevel = AR_LOG_LEVEL_INFO;

    arController = new ARController();
    if (!arController->initialiseBase()) {
        ARLOGe("Error initialising ARController.\n");
    }
    
    
    int markerModelIDs[markerCount];
    char *resourcesDir = arUtilGetResourcesDirectoryPath(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_BEST);
    for (int i = 0; i < markerCount; i++) {
        std::string markerConfig = "2d;" + std::string(resourcesDir) + '/' + markers[i].name + ';' + std::to_string(markers[i].height);
        markerIDs[i] = arController->addTrackable(markerConfig);
        if (markerIDs[i] == -1) {
            ofLogNotice("Error adding marker ") << markerIDs[i];
        } else {
            ofLogNotice("Added Marker ") << markerIDs[i];

        }
    }
    
    arController->startRunning(vconf, NULL, NULL, 0);
    

    bool drawVideoInitSuccess = arController->drawVideoInit(0);
    if (!drawVideoInitSuccess)
    {
        ofLogNotice("Error with video init ");

    }
    
    bool inited = arController->isInited();
    if(!inited)
    {
        ofLogNotice("Error with init");

    }
    
    int w = 0, h = 0;
    AR_PIXEL_FORMAT pixF = AR_PIXEL_FORMAT::AR_PIXEL_FORMAT_RGB;
    arController->videoParameters(0, &w, &h, &pixF);
   
    

}

//--------------------------------------------------------------
void ofApp::update(){

  
#ifdef USE_BUFFER
    
    camera.update();
    
    if(camera.isFrameNew()) {
        
        
        pixels.setFromPixels(camera.getPixels().getData(), camera.getWidth(), camera.getHeight(), 3);
        //pixels.mirror(false, true);
        
        //cv::Mat grabberCv;
        ofImage img;
        img.setFromPixels(pixels.getData(), 640, 480, OF_IMAGE_COLOR);
        //grabberCv = toCv(img);
        cv::Mat grabberCv = cv::Mat(480, 640, CV_8U, &pixelBuffer);
        cv::Mat converted;
        grabberCv.convertTo(converted, CV_32S);
        
        buffer.time.sec            = ofGetElapsedTimef();
        buffer.time.usec           = ofGetElapsedTimeMicros();
        buffer.buff                = converted.data;
        buffer.fillFlag            = 1;
        
        if (!arController->update(&buffer))
            ARLOGe("Error in ARController::update().\n");
        
        ARTrackable *marker = arController->findTrackable(0);
        if (marker->visible) {
            ofLogNotice("Grosse teuf");
        }
        
    }
    
    
#else
    
    if(arController->capture())
        if (!arController->update())
            ARLOGe("Error in ARController::update().\n");
    
#endif
    
   
    
  
            
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofBackground(255);
#ifdef USE_BUFFER

    if(pixels.isAllocated()) {
        
        ofImage img;
        img.setFromPixels(pixels.getData(), 640, 480, OF_IMAGE_COLOR);
        img.draw(0.0, 0.0);
        
        
        if(buffer.buffLuma) {
            
            ofPixels pixs;
            pixs.setFromPixels(buffer.buffLuma, camWidth, camHeight, 1);
            img.setFromPixels(pixs.getData(), 640, 480, OF_IMAGE_GRAYSCALE);
            img.draw(640, 0.0);
            
        }
        
        if(buffer.buff) {
            
            ofPixels pixs;
            pixs.clear();
            pixs.setFromPixels(buffer.buff, camWidth, camHeight, 3);
            img.setFromPixels(pixs.getData(), 640, 480, OF_IMAGE_COLOR);
            img.draw(640, 480);
            
        }
        
        
    }
    
#else
    
    //cv::Mat pixelMat(640, 480,CV_32F,0);
    arController->updateTextureRGBA32(0, pixelBuffer);
    cv::Mat m = cv::Mat(camHeight, camWidth, CV_32S, &pixelBuffer);
    cv::Mat converted;
    m.convertTo(converted, CV_8U);
    ofImage _img;
    toOf(converted, _img);
    
    ofSetColor(255, 255);
    _img.draw(0.0,0.0);
    
#endif
    
    bool bHasFound = false;
    
    ARdouble projectionARD[16];
    if(arController->projectionMatrix(0, 10.0f, 10000.0f, projectionARD)) {
        
        
        
        
        ofPushView();
        // ofViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        
        //mtxLoadIdentityf(projectionARD);
        //mtxLoadIdentityf(gView);
        
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(projectionARD);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(gView);
        glLoadIdentity();

        // model
        

        // Look for trackables, and draw on each found one.
        for (int i = 0; i < markerCount; i++) {
            
            // Find the trackable for the given trackable ID.
            ARTrackable *marker = arController->findTrackable(markerIDs[i]);
            
            
            float view[16];
            if (marker->visible) {
                
                
                
                ofLogNotice("something is here..");
                //arUtilPrintMtx16(marker->transformationMatrix);
                for (int i = 0; i < 16; i++) view[i] = (float)marker->transformationMatrix[i];
                
                glPushMatrix(); // Save world coordinate system.
                //ofScale(-1.0, 1.0, 1.0);
                glMultMatrixf(view);
                
                ofSetColor(255, 0, 0);
                ofDrawRectangle(0.0,0.0, 100, 100);
                glPopMatrix();
                
                bHasFound = true;
                
                
            }
            // drawSetModel(markerModelIDs[i], marker->visible, view);
        }
        
        
        ofPopView();
        
    }
    
    
    if(bHasFound) {
        
        ofSetColor(255,0,0);
        ofDrawRectangle(0.0, 0.0, 100, 100);
        
    }
     
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    contextWasUpdated = true;
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
