#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    
    ofGLWindowSettings s;
    s.setSize(1280, 720);
    s.windowMode = OF_WINDOW;
    ofCreateWindow(s);
    
	ofRunApp(new ofApp());

}
