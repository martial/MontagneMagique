//
//  BirdsScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 21/02/2019.
//

#include "BirdsScene.hpp"

void BirdsScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    pitchMillis     = 0.0;
    bIsSinging      = false;
    
    startColor      = ofColor(157,88, 88);
    endColor        = ofColor(193, 121, 27);
    
   // backgroundImage.load(this->dataPath + "/oiseau.jpg");
   // backgroundImage.resize(ofGetWidth(), ofGetHeight());
    
    colors.push_back(ofColor(27,99,27));
    colors.push_back(ofColor(193,117,20));
    colors.push_back(ofColor(157,88,88));
    colors.push_back(ofColor(49,112,103));

  
    
}

void BirdsScene::update() {
    
    
}


void BirdsScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct <= 0 )
        return;
    
    float scale = 20;

    
    // delete old particles
    for(int i = particles.size() - 1; i >= 0; i --) {
        
        if(particles[i]->pos.x < particles[i]->scale * scale) {
            particles[i] = NULL;
            particles.erase(particles.begin() + i);
        }
        
    }

    
    ofPushMatrix();
    ofScale(1, -1.0);
    ofTranslate(0.0, -marker->height);
  //  ofSetColor(255, 50);
   // ofDrawRectangle(0.0, 0.0, marker->width, marker->height);
    
    
    // marker->image->draw(0.0, 0.0, marker->width, marker->height);
    
    // if value is above zero we actually have sound
    if(!bIsSinging && targetPitchPct > 0.0) {
        
        // we reset timer
        if(!bIsSinging) {
            onPitchStart();
        }
        pitchTime = ofGetElapsedTimeMillis();
            
        bIsSinging = true;
        
        
    }
    
    if(bIsSinging && targetPitchPct > 0.0)
        pitchTime = ofGetElapsedTimeMillis();
    
    // if we are singing but we don't receive anything..
    if (bIsSinging && targetPitchPct == 0.0) {
    
        float timeDiff = ofGetElapsedTimeMillis() - pitchTime;

        if(timeDiff > 100) {
            bIsSinging = false;
            onPitchEnd();
        }
    }
    
    
    
    
    // smooth pct values
    float blurRate      = 0.9;
    this->pitchPct      = blurRate * pitchPct   +  (1.0f - blurRate) * targetPitchPct;
    

    ofFill();
    // draw the particles
    for (int i=0; i<particles.size(); i++) {
        
        particles[i]->resetForce();
        ofVec2f frc (ofRandom(-0.01, -0.02), ofRandom(-0.001, -0.001));
        frc *= .3;
        particles[i]->addForce(frc.x, frc.y);
        particles[i]->update();
        
    }
    
    // draw shadows of moving particles
    for (int i=0; i<particles.size(); i++) {
        
       
        ofSetColor(0, 40);
        
        ofFill();
        ofPushMatrix();
        ofTranslate(0.0,0.0,0.0);
        // draw shadow
        ofVec2f pos = particles[i]->pos + particles[i]->depthVel;
        drawCircle(pos, particles[i]->scale * scale, ofColor(0,40));
        
        ofPopMatrix();
        
    }
    
    // draw  moving particles
    for (int i=0; i<particles.size(); i++) {
        
        ofFill();
        ofPushMatrix();
        ofTranslate(0.0,0.0,0.0);
        drawCircle(particles[i]->pos, particles[i]->scale * scale, particles[i]->color);
        ofPopMatrix();
        //ofLogNotice(" particles[i]->depth: ") <<  particles[i]->depth;
    }
    
    // if we are singing, let's grow the particle
    if(currentParticle ) {
        
        if( targetPitchPct > 0.0) {
            
            radiusVelFactor -= 0.45;
            radiusVelFactor = ofClamp(radiusVelFactor, 1.0, 99);
            
            float vel = ofMap(this->targetPitchPct, 0.0, 1.0, -0.008, 0.002);
            currentRadius += vel * radiusVelFactor;
            currentParticle->scale = currentRadius ;
            
            ofColor c = startColor;
            c =  c.lerp(endColor, this->targetPitchPct);
            
            //c.a = 125;
            currentParticle->color = c;
            
        }
        
        currentParticle->resetForce();
        currentParticle->update();
        
        ofFill();
        ofPushMatrix();
        ofTranslate(0.0,0.0,0.0);
        
        // draw shadow
        ofVec2f pos = currentParticle->pos + currentParticle->depthVel;
        drawCircle(pos, currentParticle->scale * scale, ofColor(0,40));
        
        // draw bubble
        drawCircle(currentParticle->pos, currentParticle->scale * scale, currentParticle->color);

        ofPopMatrix();
        
        
    }

    
    
    
    
    ofPopMatrix();
    
}

void BirdsScene::drawCircle(ofVec2f pos, float radius, ofColor col) {
    
    ofSetColor(col);
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofDrawEllipse(radius * .5, radius * .5, radius, radius);
    ofPopMatrix();
    
}


void BirdsScene::setPitchPct(float pct) {
    
    this->targetPitchPct = pct;
    
}

void BirdsScene::onPitchStart() {
    
    
    // pick random color for start
    int rdmIndex = floor(ofRandom(0,colors.size()));
    startColor = colors[rdmIndex];
    
    int lastIndex = rdmIndex;
    while (rdmIndex == lastIndex) {
        rdmIndex = floor(ofRandom(0,colors.size()));
    }
    
    endColor =  colors[rdmIndex];
    
    // choose initial pos
    ofVec2f initialPos (ofRandom(60, 70),ofRandom(80, 90));
    initialPos *= .3;
    
    currentParticle = new SimpleParticle();
    currentParticle->setInitialCondition(initialPos.x, initialPos.y,0.0,0.0);
    currentParticle->scale        = .2;
    currentParticle->damping      = 0.9;
    currentParticle->life        = 0.0;
    currentParticle->depthVel      = ofRandom(1, 2);
    currentRadius                 = 0.0;
    
    radiusVel                   = ofRandom(0.001, 0.003);
    radiusVelFactor             = 10.0;
}

void BirdsScene::onPitchEnd() {
    
    particles.push_back(currentParticle);
    currentParticle = NULL;
    ofLogNotice("onPitch End");

    
}
