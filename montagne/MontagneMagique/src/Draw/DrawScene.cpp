//
//  DrawScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 10/07/2019.
//

#include "DrawScene.hpp"
#include "ofApp.h"

void DrawScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    shader.load("shaders/mask");
    
    for(int i=0; i<configJson["colors"].size(); i++) {
        
        ofColor c;
        c.r = configJson["colors"][i]["r"];
        c.g = configJson["colors"][i]["g"];
        c.b = configJson["colors"][i]["b"];
        c.a = configJson["colors"][i]["a"];
        
        colors.push_back(c);
        
    }
    
    // check if config.json has a load folder parameter
    string shapesFolder = configJson.value("auto-load-folder", "");
    if (!shapesFolder.empty()) {
        loadFolder(shapesFolder);
    }
    
    ofApp * app = (ofApp*)ofGetAppPtr();
    post.init(app->videoOutputWidth, app->videoOutputHeight);
    dofPass = post.createPass<DofPass>();
    dofPass->setEnabled(true);
    post.setFlip(true);
    
    bird.setup();
   

}

void DrawScene::setMarker(shared_ptr<MagiqueMarker> marker) {
    
    AbstractARScene::setMarker(marker);
    
    
}


void DrawScene::setTreshold(float thresold) {
    
    configJson["thresold"] = thresold;
    saveConfigJson();
}
void DrawScene::setAperture(float aperture) {
    
    configJson["aperture"] = aperture;
    saveConfigJson();
    
}
void DrawScene::loadFolder(string folder) {
    
    ofLogNotice("loadFolder ") << folder;
  
    // first load shapes
    ofJson shapesJson;
    shapesJson = ofLoadJson(folder + "/shapes.json");
    
    for(int i=0; i<shapesJson["shapes"].size(); i++) {
        
        ofPolyline line;
        for(int j=0; j<shapesJson["shapes"][i]["pnts"].size(); j++) {

            float x = shapesJson["shapes"][i]["pnts"][j]["x"];
            float y = shapesJson["shapes"][i]["pnts"][j]["y"];
            line.addVertex(x,y);
            lines.push_back(line);
        }
        
        ofPath p;
        p.moveTo(line.getVertices()[0]);
        for(int j=0; j<line.getVertices().size(); j++) {
            p.curveTo(line.getVertices()[j]);
        }
        p.close();
        
        int rdmIndex = floor(ofRandom(colors.size()));
        ofColor c = colors[rdmIndex];
        
        p.setColor(c);
        p.setStrokeColor(ofColor(0,0,0));
        p.setStrokeWidth(0);
        
        paths.push_back(p);
        
        ofImage shape;
        string url = shapesJson["shapes"][i]["img"];
        shape.load(url);
        
        VectorObject obj;
        obj.type = SHAPE_TYPE_IMG;
        obj.line = line;
        obj.path = p;
        obj.img = shape;
        
        vectorObjects.push_back(obj);
        
    }
    
}


void DrawScene::update() {
    
    ofApp * app = (ofApp*) ofGetAppPtr();

    float aperture = configJson["aperture"];
    dofPass->setAperture(aperture);
  // dofPass->setFocus(focus);

    if(hasConfigChanged()) {
        
        colors.clear();
        for(int i=0; i<configJson["colors"].size(); i++) {
            
            ofColor c;
            c.r = configJson["colors"][i]["r"];
            c.g = configJson["colors"][i]["g"];
            c.b = configJson["colors"][i]["b"];
            c.a = configJson["colors"][i]["a"];
            colors.push_back(c);
            
        }

        
    }
    
    float scale =  (float)app->videoOutputWidth / (float)app->videoInputWidth;

    // we allocate openCV images if needed
    if(!colorImg.bAllocated) {
        
        colorImg.allocate(app->videoInputWidth,app->videoInputHeight);
        grayImage.allocate(app->videoInputWidth,app->videoInputHeight);
        
    }
    
    // we do the countour finding
    colorImg.setFromPixels(app->resizedInputImg.getPixels());
    colorImg.mirror(true, false);

    grayImage = colorImg;
    
    float thresold = configJson["thresold"];
    grayImage.threshold(thresold);
    
    contourFinder.findContours(grayImage, 20, (app->videoInputWidth*.75 * app->videoInputHeight * .75), 10, true);
    
    for (int j = 0; j < contourFinder.nBlobs; j++){
        
        contourFinder.blobs[j].area     *= scale;
        contourFinder.blobs[j].centroid *= scale;
        
    }

    
}

void DrawScene::updateCamera(ofBaseHasPixels & input){
    
    
}

void DrawScene::captureImages() {
    
    // first we simply create a mask
    // we already have it ! grayImage ( inverted though )
    
    ofTexture mask;
    grayImage.invert();
    mask.loadData(grayImage.getPixels());
    mask.draw(0.0,0.0);
    
    // create a fbo for masking...
    ofFbo fbo;
    fbo.allocate(grayImage.width, grayImage.height, GL_RGBA);
    fbo.begin();
    ofEnableAlphaBlending();
    ofClear(255, 0);
    shader.begin();
    shader.setUniformTexture("imageMask", mask, 1);
    colorImg.draw(0.0,0.0);
    shader.end();
    fbo.end();
    
    ofEnableAlphaBlending();
    
    ofPixels pix;
    fbo.readToPixels(pix);
    
    // basically we have to loop into the shapes
    for (int i = 0; i < contourFinder.nBlobs; i++){
        
        // we create an image for each shape and then set a mask.
        ofImage shape;
        shape.setFromPixels(pix);
        shape.crop(contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
        shape.save("drawn_images/imagetest_"+ofToString(i)+".png");
        
    }
    
}

void DrawScene::captureShapes(int mode) {
    
    // first we simply create a mask
    // we already have it ! grayImage ( inverted though )
    
    ofTexture mask;
    grayImage.invert();
    mask.loadData(grayImage.getPixels());
    mask.draw(0.0,0.0);
    
    // create a fbo for masking...
    ofFbo fbo;
    fbo.allocate(grayImage.width, grayImage.height, GL_RGBA);
    fbo.begin();
    ofEnableAlphaBlending();
    ofClear(255, 0);
    shader.begin();
    shader.setUniformTexture("imageMask", mask, 1);
    colorImg.draw(0.0,0.0);
    shader.end();
    fbo.end();
    
    ofEnableAlphaBlending();
    
    ofPixels pix;
    fbo.readToPixels(pix);
    
   
    /*
    lines.clear();
    paths.clear();
    vectorObjects.clear();
     
     */
    
    float tolerance = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 10);
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        
        ofPolyline line;
        for(int j=0; j<contourFinder.blobs[i].nPts; j++) {
            line.addVertex(contourFinder.blobs[i].pts[j]);
        }
        line.close();
        line.simplify(tolerance);
        
        lines.push_back(line);
        
        ofPath p;
        p.moveTo(line.getVertices()[0]);
        for(int j=0; j<line.getVertices().size(); j++) {
            p.curveTo(line.getVertices()[j]);
        }
        p.close();
        
        int rdmIndex = floor(ofRandom(colors.size()));
        ofColor c = colors[rdmIndex];
        
        p.setColor(c);
        p.setStrokeColor(ofColor(0,0,0));
        p.setStrokeWidth(0);
        
        paths.push_back(p);
        
        // we create an image for each shape and then set a mask.
        ofImage shape;
        shape.setFromPixels(pix);
        
        shape.crop(contourFinder.blobs[i].boundingRect.x,
                   contourFinder.blobs[i].boundingRect.y,
                   contourFinder.blobs[i].boundingRect.width,
                   contourFinder.blobs[i].boundingRect.height);
        
        VectorObject obj;
        obj.type = mode;
        obj.line = line;
        obj.path = p;
        obj.img = shape;
        
        vectorObjects.push_back(obj);
        
    }
    
}


void DrawScene::draw() {
    
    ofEnableDepthTest();

    post.begin();

    ofPushMatrix();
    ofEnableAlphaBlending();
    ofScale(3,3);
    ofSetColor(255, 255);
    
    
    for(int i=0; i < vectorObjects.size(); i++ ) {
        
        // copy color attributes
        
        ofPushMatrix();
        ofTranslate(vectorObjects[i].line.getCentroid2D().x, vectorObjects[i].line.getCentroid2D().y);
       // ofRotateZ(vectorObjects[i].rot);
        ofTranslate(0.0,0.0,vectorObjects[i].scalez);

        ofTranslate(-vectorObjects[i].line.getCentroid2D().x, -vectorObjects[i].line.getCentroid2D().y);
        ofSetColor(255);

        vectorObjects[i].updateScale();
        vectorObjects[i].draw();
        
        ofPopMatrix();
        
    }
    
   

    ofPopMatrix();

    post.end();
    
    
    ofDisableDepthTest();
    
    bird.draw();

}

void DrawScene::clear() {
    
    lines.clear();
    paths.clear();
    vectorObjects.clear();
    
}

void DrawScene::onMarkerTracked() {
    
}

void DrawScene::save() {
    
    // first create a folder with current time
    string folder   = ofToString(std::time(nullptr));
    string path     = "shapes/" + folder;
    ofDirectory f;
    f.createDirectory(path);
    f.close();
    
    ofJson result;
    for(int i=0; i < vectorObjects.size(); i++ ) {
        
        ofJson shape;
        
        for(int j=0; j < vectorObjects[i].line.getVertices().size(); j++) {
            
            ofJson pnt;
            pnt["x"] = vectorObjects[i].line.getVertices()[j].x;
            pnt["y"] = vectorObjects[i].line.getVertices()[j].y;
            shape["pnts"].push_back(pnt);
            
        }
        
        // save all images
        string imgPath = path + "/image_"+ofToString(i) + ".png";
        vectorObjects[i].img.save(imgPath);
        
        shape["img"] = imgPath;
        result["shapes"].push_back(shape);
        
        ofSaveJson(path + "/shapes.json", result);
        
    }
    
    
}

void DrawScene::drawOffScreen() {
    
    ofApp * app = (ofApp*) ofGetAppPtr();
    ofSetColor(255);
    app->resizedInputImg.draw(0.0,0.0);
    
    ofPushMatrix();
    ofTranslate(0.0, app->resizedInputImg.getHeight());
    ofScale(1, -1, 1);
    contourFinder.draw();
    ofPopMatrix();
    
}
