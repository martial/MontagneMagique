//
//  BigBang.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 18/02/2019.
//

#include "BigBang.hpp"
#include "ofApp.h"

void BigBang::setup(string dataPath) {
    
    radius                  = 10.0;
    threshold               = 80;
    repulsionScale          = 0.0;
    maxParticleLife         = 900;
    mode                    = 0;
    lineDistance            = 200;
    blobDistance            = 200;
    forceRandomNessScale    = 1.0;
    fadeInOutPct            = 0.1;
    particlesCreated        = 0;
    
}


void BigBang::update() {
        
    ofApp * app = (ofApp*) ofGetAppPtr();
    
    //scale = 640 / (float)app->videoInputWidth;
    scale = 2;
    
    // we allocate openCV images if needed
    if(!colorImg.bAllocated) {
    
        colorImg.allocate(app->videoInputWidth,app->videoInputHeight);
        grayImage.allocate(app->videoInputWidth,app->videoInputHeight);
        
    }
    
    // we do the countour finding
    colorImg.setFromPixels(app->resizedInputImg.getPixels());
    
    grayImage = colorImg;
    grayImage.threshold(100);
    
    if(mode != 2)
        contourFinder.findContours(grayImage, 20, (app->videoInputWidth * app->videoInputHeight), 10, false);    // find holes
    else {
        contourFinder.findContours(grayImage, 10,(app->videoInputWidth * app->videoInputHeight), 10, false);    // find holes

    }
    
    for (int j = 0; j < contourFinder.nBlobs; j++){
        
        contourFinder.blobs[j].area *= scale;
        contourFinder.blobs[j].centroid *= scale;
                                            
    }
    
   
    // delete old particles
    for(int i = particles.size() - 1; i >= 0; i --) {
        
        particles[i]->life += 1;
        if(particles[i]->life > maxParticleLife) {
            particles[i] = NULL;
            particles.erase(particles.begin() + i);
        }
        
    }
    
    // we set behaviour for different modes
    
    if(mode == 0) {
        
        app->bigBangScaleMin    = 1.3;
        app->bigBangScaleMax    = 6.5;

        fadeInOutPct            = 0.001;
        maxParticleLife         = 900;
        forceRandomNessScale    = 1.0;
        repulsionScale          -= .1;
        repulsionScale          = ofClamp(repulsionScale, 0.0, 10.0);
        
    }
    
    if(mode == 1) {
        
        glLineWidth(1);

        lineDistance            = 200;
        app->bigBangScaleMin    = 1.3;
        app->bigBangScaleMax    = 4.5;
        
        fadeInOutPct            = 0.05;
        maxParticleLife         = 900;
        forceRandomNessScale    = 1.0;
        
        repulsionScale          -= .025;
        repulsionScale          = ofClamp(repulsionScale, 0.0, 10.0);
        
        if(ofGetFrameNum() % 8 == 0)
            addParticles(2, app->bigBangScaleMin, app->bigBangScaleMax, app->bigBangDampingMin, app->bigBangDampingMax);
        
    }
    
    if(mode == 2) {
        
        glLineWidth(2);
        lineDistance            = 200;
        fadeInOutPct            = 0.05;
        maxParticleLife         = 5000;
        forceRandomNessScale    = 0.01;
        repulsionScale          = 0.0;
        
        if(ofGetFrameNum() % 24 == 0 && particles.size() < 10)
            addParticles(1, app->bigBangScaleMin, app->bigBangScaleMax, app->bigBangDampingMin, app->bigBangDampingMax);
        
        if(mode != lastMode) {
            
            for(int i = 0; i < particles.size(); i ++) {
                float deadlyLifeTime = maxParticleLife - (maxParticleLife * ofRandom(0.05, 0.1));
                if(particles[i]->life < deadlyLifeTime) {
                    particles[i]->life = deadlyLifeTime;

                }
            }
            
        }
        
    }
    
    if(mode == 3) {
        
        if(mode != lastMode) {
            
            //ofLogNotice("gogogo");
            for(int i = 0; i < particles.size(); i ++) {
                float deadlyLifeTime = maxParticleLife - (maxParticleLife * ofRandom(0.05, 0.1));
                if(particles[i]->life < deadlyLifeTime) {
                    particles[i]->life = deadlyLifeTime;
                    
                }
            }
            
        }

        
    }
    
    lastMode = mode;
    
}

void BigBang::draw() {
    
     ofApp * app = (ofApp*) ofGetAppPtr();
    
    for(int i = 0; i < particles.size(); i ++) {
        
        // reset forces
        particles[i]->resetForce();
        
        forceRandomNessScale = 1;
        forceRandomNessScale = 1;
        
        // add some randomness force
        float xFrc = ofRandom(-1.5 * forceRandomNessScale, 1.5 * forceRandomNessScale);
        float yFrc = ofRandom(-1.5 * forceRandomNessScale, 1.5 * forceRandomNessScale);
        particles[i]->addForce(xFrc, yFrc);
        
        // if we are in mode 0 and 1, add attraction and repulsion forces to the blobs
        if( mode != 2 && mode != 3) {
            
            for (int j = 0; j < contourFinder.nBlobs; j++){
                
                if(repulsionScale > 0.0) {
                    particles[i]->addRepulsionForce(contourFinder.blobs[j].centroid, 1920 * 5, repulsionScale);
                }
                
                particles[i]->addRepulsionForce(contourFinder.blobs[j].centroid, 1920 * 5, 0.3);
                particles[i]->addAttractionForce(contourFinder.blobs[j].centroid,1920 * 5, 0.8);
               
               
            }
        }
        
        // add damping
        particles[i]->addDampingForce(app->bigBangScaleDampingScale);
        particles[i]->update();
        
        // add ease in / out transition
        float durationFadePct = maxParticleLife * fadeInOutPct;
        
        // add little bit of noise
        float alpha = 1.0;
        if(mode == 2 || mode == 3) {
            
            float pct = ofNoise((ofGetElapsedTimef()+i) *5);
            pct = ofMap(pct, 0.0, 1.0, 0.7, 1.0 );
            alpha *= pct;
            
        }
        
        if(particles[i]->life < durationFadePct) {
            alpha = ofMap(particles[i]->life, 0, durationFadePct, 0.0, 1.0);
        }
        
        if(particles[i]->life > maxParticleLife - durationFadePct) {
            alpha = ofMap(particles[i]->life, maxParticleLife - durationFadePct, maxParticleLife, 1.0, 0.0);
        }
        
        // draw
        ofSetColor(255, 255 * alpha);
        ofDrawEllipse(particles[i]->pos.x,
                      particles[i]->pos.y,
                      particles[i]->scale * particles[i]->scale,
                      particles[i]->scale * particles[i]->scale);

        // we draw lines in mode 1
        
        if( mode == 1 ) {
            
            // if there is no blob, we don't have any reason to draw lines
            if(contourFinder.nBlobs > 0 ) {
                
                float minDistFromBlobs = particles[i]->pos.distance(contourFinder.blobs[0].centroid);
                
                for (int j = 1; j < contourFinder.nBlobs; j++){
                    minDistFromBlobs = MIN( minDistFromBlobs,particles[i]->pos.distance(contourFinder.blobs[j].centroid) );
                }
            
                // we draw lines only if we have a minimum distance away from a blob
                
                if( minDistFromBlobs > blobDistance) {
                    
                    float diffDistance          = 80;
                    float diff                  = blobDistance - minDistFromBlobs;
                    float diffDistanceMapped    = ofClamp(ofNormalize(diff, 0, diffDistance), 0, 1);
                    
                    //nConnecteds = 0;
                    particles[i]->nConnecteds = 0;
                    for(int j = 0; j < particles.size(); j++) {
                         
                         // not same and not everyone
                         if ( i != j && particles[j]->id % 4 == 0 && particles[j]->nConnecteds < 3) {
                           
                             float dist = particles[i]->pos.distance(particles[j]->pos);
                             if ( dist < lineDistance) {
                                 
                                 float a = ofClamp(ofMap(dist, 0, lineDistance, 255, 0), 0, 255);
                                 ofSetColor(255, a );
                                 ofDrawLine(particles[i]->pos, particles[j]->pos);
                                 
                                 particles[j]->nConnecteds++;
                                 
                                 //nConnecteds++;
                             }
                             
                         }
                         
                     }
                    
                }
                
            }
            
        }
      
    }
    
    // draw lines with elie layer
    if( mode == 2 ) {
        
        // if there is no blob, we don't have any reason to draw lines
        if(contourFinder.nBlobs > 0 ) {
            
             for(int i = 0; i < contourFinder.nBlobs; i ++) {
                 for(int j = 0; j < contourFinder.nBlobs; j ++) {
                
                    if ( i != j  ) {
                        
                        ofVec2f pos = ofVec2f (contourFinder.blobs[i].centroid);
                        float dist = pos.distance(contourFinder.blobs[j].centroid);
                        if ( dist < lineDistance) {
                            
                            float alpha = ofMap(dist, 0, lineDistance, 255, 0);
                            ofSetColor(255,  alpha);
                            ofDrawLine(pos, contourFinder.blobs[j].centroid);
                        }
                        
                    }
                
                 }
                 
             }
            
        }
        
    }
  
    if( app->app.arSceneManager.bDebugMode ) {
        
        ofPushMatrix();
        ofScale(scale, scale);
        contourFinder.draw();
        ofPopMatrix();
        
    }
    
    

}

void BigBang::addParticles(int nParticles, float minSize, float maxSize, float minDamp, float maxDamp) {
        
    ofApp * app = (ofApp*) ofGetAppPtr();
    
    vector<ofVec2f> positions;
    
    // we get positions from the borders
    if(mode == 0 ) {
        
            for(int i=0; i<1920; i++) {
            
            ofVec2f pos(i, 0.0);
            positions.push_back(pos);
            
            ofVec2f posy(i, 1080);
            positions.push_back(posy);
            
        }
        
        for(int i=0; i<1080; i++) {
            
            ofVec2f pos(0.0, i);
            positions.push_back(pos);
            
            ofVec2f posy(1920, i);
            positions.push_back(posy);
            
        }
        
    }
    // we get positions from the blobs
    else if ( mode == 1 ) {
        
        
         for (int j = 0; j < contourFinder.nBlobs; j++){
             positions.push_back(contourFinder.blobs[j].centroid);
         }
        
        
    }
     // we get positions randomly
    else if ( mode == 2 ) {
        
        for (int j = 0; j < 100; j++){
            positions.push_back(ofVec2f(ofRandom(1920),ofRandom(1080)));
        }
        
    }
    
    // if there is no position available, skip !
    if(positions.size() > 0 ) {

        for(int i=0; i<nParticles; i++) {
            
            SimpleParticle  * p = new SimpleParticle();
            p->id = particlesCreated;
            particlesCreated ++;
            int rdm = floor(ofRandom(positions.size()));
            ofVec2f pos = positions[rdm];
            p->setInitialCondition(pos.x,pos.y,0.0,0.0);
            p->scale        = ofRandom(minSize, maxSize);
            p->damping      = ofRandom(minDamp, maxDamp);
            p->life         = 0.0;
            particles.push_back(p);
            
        }
        
    }
    
    positions.clear();
    
}



void BigBang::startRepulsion(float scale) {
    ofApp * app = (ofApp*) ofGetAppPtr();
   // app->bigBangRepulsionFactor = scale;
    repulsionScale = scale * app->bigBangRepulsionFactor;
    
}
