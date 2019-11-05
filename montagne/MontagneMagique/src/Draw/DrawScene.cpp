#include "DrawScene.hpp"
#include "ofApp.h"

void DrawScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    shader.load("assets/shaders/mask");
    
    for(int i=0; i<configJson["colors"].size(); i++) {
        
        ofColor c;
        c.r = configJson["colors"][i]["r"];
        c.g = configJson["colors"][i]["g"];
        c.b = configJson["colors"][i]["b"];
        c.a = configJson["colors"][i]["a"];
        colors.push_back(c);
        
    }

    /*
    // check if config.json has a load folder parameter
    string shapesFolder = configJson.value("auto-load-folder", "");
    if (!shapesFolder.empty()) {
        loadFolder(shapesFolder);
    }
     
     */
    
    ofApp * app = (ofApp*) ofGetAppPtr();
    post.init(app->videoOutputWidth, app->videoOutputHeight);
    post.setFlip(true);
    dofPass = post.createPass<DofPass>();
    dofPass->setEnabled(true);
    
    cam.setForceAspectRatio(false);
    cam.setAspectRatio(post.getWidth() / post.getHeight());
    cam.setPosition(ofVec3f(post.getWidth() * .5,  post.getHeight() * .5,  0));
    
    float camFarClip          = configJson["cam-farclip"];
    cam.setFarClip(camFarClip);
    
    string shapesFolder = configJson.value("auto-load-folder", "");
    if (!shapesFolder.empty()) {
        loadFolderOfImageForTerrain(shapesFolder);
    }
    
    ribbonPath.set(cam.getPosition());
    
    if(configJson["bird"] == 1)
        bird.setup();
    
    bigRibbon   = new ofxTwistedRibbon(150, ofColor(), 400);

    poszCount = 0;

    particle.load("assets/images/particle.png");
    particle.setAnchorPoint(particle.getWidth() * .5, particle.getHeight() * .5);
    
    bgParralaxBack.load("assets/images/mountains/MONT_1.png");
    bgParralaxFront.load("assets/images/mountains/MONT_2.png");

    intro();
    
    // launch particles
    int nPartices = configJson.value("numParticles", 300);

    nPoints = nPartices;
    for(int i=0; i<nPoints; i++) {
        ofVec3f pnt;
        pnt.set(ofRandom(-post.getWidth(), post.getWidth() * 2), ofRandom(0, post.getHeight()*3), ofRandom(cam.getFarClip()) );
        points.push_back(pnt);
        
        float scale = 10;
        points[i].x = cam.getPosition().x + -post.getWidth() * scale + ofRandom(post.getWidth() * 2 * scale);
        points[i].y = cam.getPosition().y + -post.getHeight() * scale+ ofRandom(post.getHeight() * 2 * scale);
    }
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
        
        ofImage * shape = new ofImage();
        string url = shapesJson["shapes"][i]["img"];
        shape->load(url);
        images.push_back(shape);

        VectorObject obj;
        obj.type = SHAPE_TYPE_IMG;
        obj.line = line;
        obj.path = p;
        obj.img = images[images.size() - 1];
        
        vectorObjects.push_back(obj);
        
    }
    
}

void DrawScene::loadFolderOfImageForTerrain(string folder) {
    
    ofDirectory dir(folder);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.allowExt("jpeg");
    dir.listDir();
    dir.sort();
    
    int nFiles = dir.size();
    
    for(int i=0; i<nFiles; i++) {
        
        ofImage * img = new ofImage();
        string url = folder + "/" + dir.getName(i);
        img->load(url);
        images.push_back(img);
        
        int rdm = 5;
        
        for(int j=0; j<rdm; j++) {

            VectorObject obj;
            obj.type = SHAPE_TYPE_TERRAIN;
            obj.img = images[images.size() - 1];
            obj.position.z = ofRandom(cam.getFarClip());
            obj.rot = ofRandom(-10, 10);
            obj.anchorPoint.x = img->getWidth() * .5;
            vectorObjects.push_back(obj);
            
        }
        
    }
    
    // int rdmIndex = floor(ofRandom(nFiles));
    // string vid = dir.getName(rdmIndex);
    // oscManager.sendMessage("/video/"+vid, "This is a test");
    
}

void DrawScene::update() {
    
    // update animations
    float step = 1.0f / 60.0f;
    backgroundColorTop.update(step);
    backgroundColorBottom.update(step);
    mountainsColor.update(step);
    mountainsSecondColor.update(step);
    
    cameraPosY.update(step);
    lookAtPosY.update(step);
    terrainCurve.update(step);
    terrainCols.update(step);
    terrainColsScale.update(step);
    
    particlesScale.update(step);
    
    birdMainScale.update(step);
    birdMainPosY.update(step);
    
    pathXAmplitude.update(step);
    pathYAmplitude.update(step);

    birdPosYCosAmp.update(step);
    
    pathColor.update(step);
    
    ofApp * app = (ofApp*) ofGetAppPtr();
    float aperture = configJson["aperture"];
    dofPass->setAperture(aperture);
    
    float focus = configJson["focus"];
    dofPass->setFocus(focus);
    
    float maxBlur = configJson["max-blur"];
    dofPass->setMaxBlur(maxBlur);

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
        
        bool realoadOnSave = configJson["reload-on-save"];
        if ( realoadOnSave )
            intro();
        
    }
    
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
    if(thresold == -1)
        thresold = ofGetMouseX();
    
    grayImage.threshold(thresold);
    
    float minArea = configJson["min-area"];
    contourFinder.findContours(grayImage, minArea, (app->videoInputWidth*.75 * app->videoInputHeight * .75), 8, false);
    
    float scale =  (float)app->videoOutputWidth / (float)app->videoInputWidth;

    for (int j = 0; j < contourFinder.nBlobs; j++){
        
        contourFinder.blobs[j].area     *= scale;
        contourFinder.blobs[j].centroid *= scale;
        
    }
    
    ofVec3f position;
  
    float camSpeed            = configJson["camera-speed"];
    float pathX               = configJson["path-x"];
    float pathY               = configJson["path-y"];

    float pathNoiseXAmplitude = pathXAmplitude.getCurrentValue();
    float pathNoiseYAmplitude = pathYAmplitude.getCurrentValue();
    float pathNoiseXSpeed     = configJson["path-noise-x-speed"];
    float pathNoiseYSpeed     = configJson["path-noise-y-speed"];
    
    float xNoise              = ofNoise(ofGetElapsedTimef() * pathNoiseXSpeed);
    float yNoise              = ofNoise(ofGetElapsedTimef() * pathNoiseYSpeed, 2);
    
    float camX                = configJson["cam-x"];
    float camY                = cameraPosY.getCurrentValue(); // anim
    float camDistance         = configJson["cam-distance"];
    float camFarClip          = configJson["cam-farclip"];

    float lightIntensity      = configJson["light-intensity"];
    float lightY              = configJson["light-y"];
    
    cam.setFarClip(camFarClip);
    
    ofVec3f path    = ribbonPath;
    path.x          = pathX + ofMap(xNoise, 0, 1, -pathNoiseXAmplitude, pathNoiseXAmplitude);
    path.y          = pathY + ofMap(yNoise, 0, 1, -pathNoiseYAmplitude, pathNoiseYAmplitude);
    path.z          += camSpeed;
    
    ribbonPath.set(path);
    
    int size            = bigRibbon->points.size();
    float zDiff         = ofNormalize(size, 0, bigRibbon->length) * camDistance;
    
    // test
    
    if(bigRibbon->points.size() > 0 ) {
        
        ofVec3f targetCamPos = ribbonPath;
        
        int target = ( bigRibbon->points.size() > 1 ) ? 1 : bigRibbon->points.size()-1;
        targetCamPos = bigRibbon->points[target];
        
       // ofLogNotice("ofgetMousex") << ofGetMouseX();
        
        ofVec3f camPos(targetCamPos.x + camX, targetCamPos.y + camY, targetCamPos.z - zDiff);
        cam.setPosition(camPos);
        
        target = ( bigRibbon->points.size() > 50 ) ? 50 : bigRibbon->points.size()-1;
        
        ofVec3f lookAtPnt   = bigRibbon->points[target];;
        lookAtPnt.y         += lookAtPosY.getCurrentValue();
        cam.lookAt(lookAtPnt);
        
        ofVec3f lightPos   = path;
        lightPos.y += lightY;
        pointLight.setPosition(lightPos);
        
    }
    
    /*
    ofColor color, secondColor;
    int hue = int(ofGetElapsedTimef() * 20) % 255;
    color.setHsb(hue, 120, 220);
    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
    hue     = int(ofGetElapsedTimef() * 10) % 255;
    secondColor.set(20);
    */
    
    // lerp and noise between two colors
    float noise = ofNoise(ofGetElapsedTimef() * 0.3);
    ofColor color = mountainsColor.getCurrentColor().lerp(mountainsSecondColor.getCurrentColor(), noise);
    
    bigRibbon->terrainCurve = terrainCurve.getCurrentValue();
    bigRibbon->terrainScale = terrainColsScale.getCurrentValue();
    bigRibbon->thickness    = round(terrainCols.getCurrentValue());
    bigRibbon->update(ribbonPath, color, pathColor.getCurrentColor());
    
    // lights
    pointLight.setAmbientColor( ofFloatColor(lightIntensity,lightIntensity,lightIntensity) );
    pointLight.setDiffuseColor( ofFloatColor(1.0f, 1.f, 1.f) );
    
    float drawingScale = configJson["drawing-position-scale"];
    for(int i=0; i<nPoints; i++) {
        
        if(points[i].z < cam.getPosition().z ) {
            
            points[i].x = cam.getPosition().x + -post.getWidth() * drawingScale + ofRandom(post.getWidth() * 2 * drawingScale);
            points[i].y = cam.getPosition().y + -post.getHeight() * drawingScale+ ofRandom(post.getHeight() * 2 * drawingScale);
            points[i].z = cam.getPosition().z + cam.getFarClip();
        }
        
    }
    
    vector<ofVec4f> positions = bigRibbon->getLastRowOfVerticesMiddle();

    if(positions.size() > 0 ) {
        
        for (auto vecObject = vectorObjects.begin(); vecObject != vectorObjects.end();){
            
            bool bIsBehindCamera = ((*vecObject).position.z < cam.getPosition().z );
            // remove if necessary
            if( (*vecObject).bWaitToBeRemoved && bIsBehindCamera) {
                
                (*vecObject).clear();
                vecObject = vectorObjects.erase(vecObject);
                
            } else {
            
                 if(bIsBehindCamera) {
                     
                     if( (*vecObject).type == SHAPE_TYPE_TERRAIN ) {
                         
                         int rdm = floor(ofRandom(positions.size()));
                         ofVec4f pos = positions[rdm];
                         
                         (*vecObject).position.x = pos.x;
                         (*vecObject).position.y = pos.y;
                         (*vecObject).position.z = pos.z;
                         
                         (*vecObject).scale.reset(0.0);
                         (*vecObject).setScaleTransition(ofRandom(0.5, 1.2), ofRandom(.8, 1.2), EASE_IN_OUT_BACK);
                         
                     } else {
                         
  
                         float sc = 3.0;
                         (*vecObject).position.x = cam.getPosition().x - (post.getWidth()* 0.5 * sc) + ((*vecObject).originalPosition.x * sc);
                         (*vecObject).position.y = cam.getPosition().y - (post.getHeight()* 0.5 * sc) + ((*vecObject).originalPosition.y * sc);
                         (*vecObject).position.z = cam.getPosition().z + cam.getFarClip();
                         (*vecObject).scale.reset(0.0);
                         (*vecObject).setScaleTransition(2.5, ofRandom(0.5, 0.8), EASE_IN_OUT_BACK);
                         
                         //vectorObjects[i].scale.reset(0.0);
                         //vectorObjects[i].setScaleTransition(ofRandom(0.5, 1.2), ofRandom(.8, 1.2), EASE_IN_OUT_BACK);
                     }

                   
                 }
                ++vecObject;
            }
        
        }
        
    }

}

void DrawScene::updateCamera(ofBaseHasPixels & input){}

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
    
    ofApp * app = (ofApp*) ofGetAppPtr();
    
    float inputWidth    = app->configJson["input-width"];
    float inputHeight   = app->configJson["input-height"];
    float outputWidth   = app->configJson["output-width"];
    float outputHeight  = app->configJson["output-height"];
    
    float scale         = outputWidth / inputWidth;
    
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
        ofImage * shape = new ofImage();
        shape->setFromPixels(pix);
        shape->crop(contourFinder.blobs[i].boundingRect.x,
                   contourFinder.blobs[i].boundingRect.y,
                   contourFinder.blobs[i].boundingRect.width,
                   contourFinder.blobs[i].boundingRect.height);
        
        images.push_back(shape);
        
        VectorObject obj;
        obj.type = SHAPE_TYPE_IMG;
        obj.line = line;
        obj.path = p;
        obj.img = images[images.size() - 1];
        obj.img->mirror(true,true);
        
        // reset position
        ofLogNotice("Capturing") <<obj.line.getCentroid2D() << " at scale " << scale;
        
        // we map from input to output        
        ofVec3f pos(obj.line.getCentroid2D().x, obj.line.getCentroid2D().y, ofRandom(5000));
        pos.x = ofMap(pos.x, 0, inputWidth, outputWidth, 0);
        pos.y = ofMap(pos.y, 0, inputHeight, 0, outputHeight);
        
        obj.position = pos;
        obj.originalPosition = pos; // store original position for reference
        obj.setAnchorPoint(ofVec3f(obj.img->getWidth() * .5, obj.img->getHeight() * .5));
        obj.setScaleTransition(scale, 0.5, CUBIC_EASE_OUT);
        vectorObjects.push_back(obj);
        
    }
    
    ofLogNotice("done with") << contourFinder.nBlobs << " shapes..";
    
}


void DrawScene::draw() {
    

   // ofSetBackgroundColor(255);
    
    post.begin();

    ofEnableAlphaBlending();
   
    
    ofSetColor(255, 255);

    // draw background gradient
    ofPushMatrix();
    ofTranslate(0.0, post.getHeight());
    ofScale(1, -1, 1);
    bgGradientMesh.clear();
    bgGradientMesh.clearColors();
    bgGradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    bgGradientMesh.addVertex( ofPoint(0,0) );
    bgGradientMesh.addColor(backgroundColorTop.getCurrentColor());
    bgGradientMesh.addVertex( ofPoint(post.getWidth(),0) );
    bgGradientMesh.addColor(backgroundColorTop.getCurrentColor());
    bgGradientMesh.addVertex( ofPoint(0,post.getHeight()) );
    bgGradientMesh.addColor(backgroundColorBottom.getCurrentColor());
    bgGradientMesh.addVertex( ofPoint(post.getWidth(),post.getHeight()) );
    bgGradientMesh.addColor(backgroundColorBottom.getCurrentColor());
    bgGradientMesh.draw();
    
    // draw background parralax
    float pathNoiseXAmplitude = 1000;

    float pathX     = configJson["path-x"];
    float distance  = ribbonPath.x - pathX;
    float diff      = ofMap(distance, -pathNoiseXAmplitude, pathNoiseXAmplitude, -100, 100);
    
    // we need to position if refering to intro position
    float camLookAtIntro    = configJson["cam-intro-y"];
    float dist              = cameraPosY.getCurrentValue() - camLookAtIntro;
    float yPosDiff          = post.getHeight();
    
    if(dist <= post.getHeight()) {
        yPosDiff =  dist;
    }
    
    bgParralaxBack.setAnchorPoint(bgParralaxBack.getWidth() * .5, 0);
    bgParralaxBack.draw(post.getWidth() * .5 - diff, yPosDiff);
    
    diff      = ofMap(distance, -pathNoiseXAmplitude, pathNoiseXAmplitude, -150, 150);
    
    bgParralaxFront.setAnchorPoint(bgParralaxBack.getWidth() * .5, 0);
    bgParralaxFront.draw(post.getWidth() * .5 - diff, yPosDiff);
    
    ofPopMatrix();
    
    ofEnableDepthTest();
    cam.begin();
    
    // terrain mesh
    ofEnableLighting();
    ofSetSmoothLighting(true);
    pointLight.enable();
    bigRibbon->draw(true, 4);
    pointLight.disable();
    ofDisableLighting();
    
    // objects
    glAlphaFunc(GL_GREATER, 0.5);
    for(int i=0; i < vectorObjects.size(); i++ ) {
        vectorObjects[i].draw();
    }
    
    // particles
    float pScale = particlesScale.getCurrentValue();
    if(pScale > 0.1) {
        ofSetColor(255);
        for(int i=0; i<nPoints; i++) {
            ofPushMatrix();
            ofTranslate(points[i]);
            ofScale(pScale);
            particle.draw(0.0,0.0, 10, 10);
            ofPopMatrix();
        }
    }
    
    glDisable(GL_ALPHA_TEST);
   
    cam.end();
    ofDisableDepthTest();
    post.end();
    
    ofEnableAlphaBlending();
    ofSetColor(255,255);
    if(configJson["bird"] == 1) {
        
        ofPushMatrix();
        float birdX         = configJson["bird-x"];
        float birdY         = configJson["bird-y"];
        birdY += birdMainPosY.getCurrentValue();
        float birdScale     = configJson["bird-scale"];
        
        float distance      = ribbonPath.x - birdX;
        float rot           = ofMap(distance, -800, 800, -10, 10);
        // add little noise to scale
        float scaleDiff = ofNoise(ofGetElapsedTimef() * .5) * 0.6;
        
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5);
        ofRotateZDeg(rot);
        bird.draw(birdX, birdY, (birdScale + scaleDiff) * birdMainScale.getCurrentValue());
        ofPopMatrix();
        glDisable(GL_ALPHA_TEST);

        
    }
    ofDisableAlphaBlending();
    
}

void DrawScene::clear() {
    
    lines.clear();
    paths.clear();
    vectorObjects.clear();
    
}

void DrawScene::undo() {
    
    // find last vecObject added
    for(int i=vectorObjects.size()-1; i>=0; i--) {
        if(vectorObjects[i].type == SHAPE_TYPE_IMG && !vectorObjects[i].bWaitToBeRemoved ) {
            vectorObjects[i].bWaitToBeRemoved = true;
           // ofLogNotice("remove object at ") << i;
            break;
        }
    }
    
}

void DrawScene::removeLastCaptured() {
    
    // find last vecObject added
    for(int i=0; i<vectorObjects.size(); i++) {
        if(vectorObjects[i].type == SHAPE_TYPE_IMG && !vectorObjects[i].bWaitToBeRemoved ) {
            vectorObjects[i].bWaitToBeRemoved = true;
            break;
        }
    }
}

void DrawScene::removeAll() {
    
    // find last vecObject added
    for(int i=0; i<vectorObjects.size(); i++) {
        
        if(vectorObjects[i].type == SHAPE_TYPE_IMG ) {
        vectorObjects[i].bWaitToBeRemoved = true;
        }
            
    }
    
}

void DrawScene::quit() {
    
    for(int i=0; i < vectorObjects.size(); i++ ) {

        vectorObjects[i].opacityVel = - ofRandom(0.001, 0.002);
        
    }
    
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
        vectorObjects[i].img->save(imgPath);
        
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

void DrawScene::intro() {
    
    // set global
    float camY              = configJson["cam-y"];
    cameraPosY.reset(camY);
    
    float camLookAtY        = configJson["cam-lookat-y"];
    lookAtPosY.reset(camY);
    
    float terrainCurveY     = configJson["terrain-curve"];
    terrainCurve.reset(terrainCurveY);
    
    float terrainColsVal    = configJson["terrain-cols"];
    terrainCols.reset(terrainColsVal);
    
    float terrainColsScaleVal    = configJson["terrain-cols-scale"];
    terrainColsScale.reset(terrainColsScaleVal);
    
    string initTopHexa      = configJson["terrain-colors"]["init"]["top-color"];
    string initBottomHexa   = configJson["terrain-colors"]["init"]["bottom-color"];
    string initMFirst       = configJson["terrain-colors"]["init"]["montain-1-color"];
    string initMSecond      = configJson["terrain-colors"]["init"]["montain-2-color"];
    string initPathHexa     = configJson["terrain-colors"]["init"]["path-color"];
    
    string introTopHexa     = configJson["terrain-colors"]["intro"]["top-color"];
    string introBottomHexa  = configJson["terrain-colors"]["intro"]["bottom-color"];
    string introMFirst      = configJson["terrain-colors"]["intro"]["montain-1-color"];
    string introMSecond     = configJson["terrain-colors"]["intro"]["montain-2-color"];
    string introPathHexa    = configJson["terrain-colors"]["intro"]["path-color"];

    backgroundColorTop.setColor(ofColor::fromHex(ofHexToInt(initTopHexa)));
    backgroundColorBottom.setColor(ofColor::fromHex(ofHexToInt(initBottomHexa)));
    mountainsColor.setColor(ofColor::fromHex(ofHexToInt(initMFirst)));
    mountainsSecondColor.setColor(ofColor::fromHex(ofHexToInt(initMSecond)));
    pathColor.setColor(ofColor::fromHex(ofHexToInt(initPathHexa)));

    //---------------------------------------------------- animation
    
    backgroundColorTop.setDuration(30);
    backgroundColorTop.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorTop.animateTo(ofColor::fromHex(ofHexToInt(introTopHexa)));
    
    backgroundColorBottom.setDuration(15);
    backgroundColorBottom.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorBottom.animateTo(ofColor::fromHex(ofHexToInt(introBottomHexa)));
    
    mountainsColor.setDuration(15);
    mountainsColor.setCurve(QUADRATIC_EASE_OUT);
    mountainsColor.animateTo(ofColor::fromHex(ofHexToInt(introMFirst)));
    
    mountainsSecondColor.setDuration(15);
    mountainsSecondColor.setCurve(QUADRATIC_EASE_OUT);
    mountainsSecondColor.animateTo(ofColor::fromHex(ofHexToInt(introMSecond)));
    
    pathColor.setDuration(15);
    pathColor.setCurve(QUADRATIC_EASE_OUT);
    pathColor.animateTo(ofColor::fromHex(ofHexToInt(introPathHexa)));
    
    int durationCam = 15;
    cameraPosY.setDuration(durationCam);
    cameraPosY.setCurve(QUADRATIC_EASE_OUT);
    float camIntro = configJson["cam-intro-y"];
    cameraPosY.animateTo(camIntro);
    
    lookAtPosY.setDuration(durationCam+2);
    lookAtPosY.setCurve(QUADRATIC_EASE_OUT);
    float camLookAtIntro = configJson["cam-lookat-intro-y"];
    lookAtPosY.animateTo(camLookAtIntro);
    
    terrainCurve.setDuration(durationCam + 5);
    terrainCurve.setCurve(QUADRATIC_EASE_OUT);
    float terrainCurveVal = configJson["terrain-curve-intro"];
    terrainCurve.animateToAfterDelay(terrainCurveVal, 5);
    
    terrainCols.setDuration(durationCam + 5);
    terrainCols.setCurve(QUADRATIC_EASE_OUT);
    terrainColsVal = configJson["terrain-cols-intro"];
    terrainCols.animateToAfterDelay(terrainColsVal, 5);
    
    terrainColsScale.setDuration(durationCam + 5);
    terrainColsScale.setCurve(EASE_IN_EASE_OUT);
    terrainColsScaleVal = configJson["terrain-cols-scale-intro"];
    terrainColsScale.animateToAfterDelay(terrainColsScaleVal, 7.5);
    
    // path amplitude
    float pathNoiseXAmplitude = configJson["path-noise-x-amplitude"];
    float pathNoiseYAmplitude = configJson["path-noise-y-amplitude"];
    
    pathXAmplitude.reset(pathNoiseXAmplitude);
    pathYAmplitude.reset(pathNoiseYAmplitude);
    
    particlesScale.setDuration(5);
    particlesScale.animateToAfterDelay(0.6, 0);
    
    birdMainScale.reset(1.0);
    
    birdMainPosY.reset(800);
    birdMainPosY.setDuration(2);
    birdMainPosY.setCurve(QUADRATIC_EASE_OUT);
    birdMainPosY.animateToAfterDelay(0, 3);
    
    birdPosYCosAmp.reset(0);
    birdPosYCosAmp.setDuration(6);
    birdPosYCosAmp.setCurve(QUADRATIC_EASE_OUT);
    birdPosYCosAmp.animateToAfterDelay(100, 0);

}

void DrawScene::scene1() { // go to space
    
    // test go to stars
    backgroundColorTop.setDuration(30);
    backgroundColorTop.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorTop.animateTo(ofColor::black);
    
    backgroundColorBottom.setDuration(15);
    backgroundColorBottom.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorBottom.animateTo(ofColor::black);
    
    cameraPosY.setDuration(7);
    cameraPosY.setCurve(LATE_EASE_IN_EASE_OUT);
    float camIntro = configJson["cam-intro-y"];
    cameraPosY.animateTo(10000);
    
    lookAtPosY.setDuration(7);
    lookAtPosY.setCurve(LATE_EASE_IN_EASE_OUT);
    float camLookAtIntro = configJson["cam-lookat-intro-y"];
    lookAtPosY.animateTo(10000);
    
    pathYAmplitude.setDuration(20);
    pathYAmplitude.setCurve(QUADRATIC_EASE_OUT);
    pathYAmplitude.animateToAfterDelay(50, 0);
    
    pathXAmplitude.setDuration(20);
    pathXAmplitude.setCurve(QUADRATIC_EASE_OUT);
    pathXAmplitude.animateToAfterDelay(50, 0);
    
    //
    float particleScale = configJson["particle-scale"];
    particlesScale.setDuration(3.0);
    particlesScale.animateToAfterDelay(particleScale, 0);

}

void DrawScene::scene2() { // change colors
    
    string secondPhaseTopHexa     = configJson["terrain-colors"]["second-phase"]["top-color"];
    string secondPhaseBottomHexa  = configJson["terrain-colors"]["second-phase"]["bottom-color"];
    string secondPhaseMFirst      = configJson["terrain-colors"]["second-phase"]["montain-1-color"];
    string secondPhaseMSecond     = configJson["terrain-colors"]["second-phase"]["montain-2-color"];
    string secondPhasePath        = configJson["terrain-colors"]["second-phase"]["path-color"];

    backgroundColorTop.setDuration(15);
    backgroundColorTop.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorTop.animateTo(ofColor::fromHex(ofHexToInt(secondPhaseTopHexa)));
    
    backgroundColorBottom.setDuration(15);
    backgroundColorBottom.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorBottom.animateTo(ofColor::fromHex(ofHexToInt(secondPhaseBottomHexa)));
    
    mountainsColor.setDuration(15);
    mountainsColor.setCurve(QUADRATIC_EASE_OUT);
    mountainsColor.animateTo(ofColor::fromHex(ofHexToInt(secondPhaseMFirst)));
    
    mountainsSecondColor.setDuration(15);
    mountainsSecondColor.setCurve(QUADRATIC_EASE_OUT);
    mountainsSecondColor.animateTo(ofColor::fromHex(ofHexToInt(secondPhaseMSecond)));
    
    pathColor.setDuration(15);
    pathColor.setCurve(QUADRATIC_EASE_OUT);
    pathColor.animateTo(ofColor::fromHex(ofHexToInt(secondPhasePath)));
    
    terrainCurve.setDuration(5);
    terrainCurve.setCurve(EASE_IN_OUT_BACK);
    terrainCurve.animateToAfterDelay(900, 0);
    
    float pathNoiseXAmplitude = configJson["path-noise-x-amplitude-second"];
    float pathNoiseYAmplitude = configJson["path-noise-y-amplitude-second"];
    
    pathXAmplitude.setDuration(5);
    pathXAmplitude.setCurve(QUADRATIC_EASE_OUT);
    pathXAmplitude.animateToAfterDelay(pathNoiseXAmplitude, 0);
    
    pathYAmplitude.setDuration(5);
    pathYAmplitude.setCurve(QUADRATIC_EASE_OUT);
    pathYAmplitude.animateToAfterDelay(pathNoiseYAmplitude, 0);
    
    terrainColsScale.setDuration(15);
    terrainColsScale.setCurve(EASE_IN_EASE_OUT);
    terrainColsScale.animateToAfterDelay(800, 0);
    
    cameraPosY.setDuration(15);
    cameraPosY.setCurve(EASE_IN_EASE_OUT);
    cameraPosY.animateTo(1300);
    
    cameraPosY.setDuration(7);
    cameraPosY.setCurve(EASE_IN_EASE_OUT);
    cameraPosY.animateTo(1300);
    
}

void DrawScene::outro() { // change colors
    
    pathYAmplitude.setDuration(20);
    pathYAmplitude.setCurve(QUADRATIC_EASE_OUT);
    pathYAmplitude.animateToAfterDelay(0, 0);
    
    pathXAmplitude.setDuration(20);
    pathXAmplitude.setCurve(QUADRATIC_EASE_OUT);
    pathXAmplitude.animateToAfterDelay(0, 0);
    
    backgroundColorTop.setDuration(15);
    backgroundColorTop.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorTop.animateTo(ofColor::black);
    
    backgroundColorBottom.setDuration(15);
    backgroundColorBottom.setCurve(QUADRATIC_EASE_OUT);
    backgroundColorBottom.animateTo(ofColor::black);
    
    particlesScale.setDuration(30);
    particlesScale.animateToAfterDelay(0.05, 0);
    
    birdMainScale.setDuration(30);
    birdMainScale.animateToAfterDelay(0.01, 0);
    
    birdPosYCosAmp.setDuration(20);
    birdPosYCosAmp.setCurve(QUADRATIC_EASE_OUT);
    birdPosYCosAmp.animateToAfterDelay(0, 0);
    
    removeAll();
    
}
