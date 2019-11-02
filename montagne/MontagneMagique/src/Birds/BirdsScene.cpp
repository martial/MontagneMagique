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
    
    targetPitchPct  = 0.0;
    
    startColor      = ofColor(157,88, 88);
    endColor        = ofColor(193, 121, 27);
    
    colors.push_back(ofColor(27,99,27));
    colors.push_back(ofColor(193,117,20));
    colors.push_back(ofColor(157,88,88));
    colors.push_back(ofColor(49,112,103));
    
    currentParticle = NULL;
    
}

void BirdsScene::update() {
    
    hasConfigChanged();
    
    bool bGenerateAuto = configJson["auto-bubbles"];
    if(bGenerateAuto) {
        float noise = cos((float)ofGetElapsedTimeMillis() / 1000.0f);
        setPitchPct(noise);
        

    }
}


void BirdsScene::draw() {
    
    float pct = getInOuPct();
    
    if(this->marker && (pct <= 0 || !this->marker->getIsSolidFound()))
        return;
    
    float scale = 20;

    // delete old particles
    for(int i = particles.size() - 1; i >= 0; i --) {
        
        if(particles[i]->pos.x < particles[i]->scale * scale * 100) {
            particles[i] = NULL;
            particles.erase(particles.begin() + i);
        }
        
    }

    
    beginFlip();

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
    if (bIsSinging && targetPitchPct <= 0.0) {
    
        float timeDiff = ofGetElapsedTimeMillis() - pitchTime;
        
        int pitchEndDelay = configJson["pitch-end-delay"];
        if(timeDiff > pitchEndDelay) {
            bIsSinging = false;
            onPitchEnd();
        }
    }
    
    // smooth pct values
    float blurRate      = 0.9;
    this->pitchPct      = blurRate * pitchPct   +  (1.0f - blurRate) * targetPitchPct;
    
    
    float maxRadius = configJson["bubble-max-radius"];

    ofFill();
    // draw the particles
    for (int i=0; i<particles.size(); i++) {
        
        particles[i]->resetForce();
        
        particles[i]->depthVel = -ofMap(particles[i]->scale, 0.0, maxRadius, 2, 20);
        //ofVec2f frc (ofRandom(-0.01, -0.02), ofRandom(-0.001, -0.005));
        //frc *= .3;
        //particles[i]->addForce(frc.x, frc.y);
        
        particles[i]->addStoredForce();
        
        particles[i]->update();
        
    }
    
    // draw shadows of moving particles
    for (int i=0; i<particles.size(); i++) {
        
        ofSetColor(0, 40);
        
        ofFill();
        // draw shadow
        ofVec2f pos = particles[i]->pos + particles[i]->depthVel;
        drawCircle(pos, particles[i]->scale * scale, ofColor(0,40));
        
    }
    
    // draw  moving particles
    for (int i=0; i<particles.size(); i++) {
        
        ofFill();
        drawCircle(particles[i]->pos, particles[i]->scale * scale, particles[i]->color);
        
    }
        
    // if we are singing, let's grow the particle
    if(currentParticle ) {
        
        if( targetPitchPct > 0.0) {
            
            radiusVelFactor -= 0.45;
            radiusVelFactor = ofClamp(radiusVelFactor, 1.0, 99);
            
            // used for bubble speed
            float velMax    = configJson["bubble-vel-radius"];
            float vel       = ofMap(this->targetPitchPct, 0.0, 1.0, -velMax, -0.01);
            currentRadius   += vel * radiusVelFactor;
            
            // set a max for radius
            currentParticle->scale = ofClamp(currentRadius, -maxRadius, 0 );
            currentParticle->depthVel = -ofMap(currentParticle->scale, 0.0, maxRadius, 2, 20);

            
            ofColor c = startColor;
            c =  c.lerp(endColor, this->targetPitchPct);
            c.a = 210;
            
            currentParticle->color = c;
            
        }
        
        currentParticle->resetForce();
        currentParticle->update();
        
        ofFill();
        
        // draw shadow
        ofVec2f pos = currentParticle->pos + currentParticle->depthVel;
        drawCircle(pos, currentParticle->scale * scale, ofColor(0,40));
        
        // draw bubble
        drawCircle(currentParticle->pos, currentParticle->scale * scale, currentParticle->color);

    }
    
    endFlip();
    
    
    
}

void BirdsScene::drawCircle(ofVec2f pos, float radius, ofColor col) {
    
    
    ofSetColor(col);
    drawCircleNoise(pos, radius, col);
    
}

void BirdsScene::drawCircleNoise(ofVec2f pos, float radius, ofColor col) {
    
    int len = 15;

    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    vector<glm::vec2> out_vertices;
    vector<glm::vec2> in_vertices;
    
    for (int deg = 0; deg < 360; deg += 8) {
        
        auto noise_point    = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
        float noise_len     = ofMap(ofNoise(noise_point.x * 0.01, noise_point.y * 0.01, ofGetFrameNum() * 0.05), 0, 1, 5, len);
        float r             = radius + noise_len;
        
        float anchor        = radius * 0.75;
        out_vertices.push_back(glm::vec2(anchor - (r * cos(deg * DEG_TO_RAD)), anchor - (r * sin(deg * DEG_TO_RAD))));
        
    }
    
    ofBeginShape();
    ofVertices(out_vertices);
    ofEndShape(true);
    
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
    ofVec2f initialPos (ofRandom(6, 8),ofRandom(44, 50));
    
    if(!this->marker) {
        
        float middlex       = 1920 * .5;
        float middley       = 1080 * .5;
        float randomRange   = 100;

        initialPos.set(
                       ofRandom(middlex - randomRange,middlex + randomRange),
                       ofRandom(middley - randomRange,middley + randomRange)
                       );
        
    }
    
    currentParticle = new SimpleParticle();
    currentParticle->setInitialCondition(initialPos.x, initialPos.y,0.0,0.0);
    currentParticle->scale                  = .2;
    currentParticle->damping                = 0.9;
    currentParticle->life                   = 0.0;
    currentParticle->depthVel               = ofRandom(10, 20);
    currentParticle->storedForceValue       = ofVec2f(ofRandom(configJson["bubble-force-x-min"], configJson["bubble-force-x-max"]),
                                                      ofRandom(configJson["bubble-force-y-min"], configJson["bubble-force-y-max"]));
    currentParticle->storedForceRandomness  = .1;
    currentParticle->storedForceValueAccel  = ofVec2f(ofRandom(configJson["bubble-accel-x-min"], configJson["bubble-accel-x-max"]),
                                                      ofRandom(configJson["bubble-accel-y-min"], configJson["bubble-accel-y-max"]));
    currentRadius                           = 0.0;    
    radiusVel                               = ofRandom(0.001, 0.003);
    radiusVelFactor                         = 10.0;
    
   // ofLogNotice("onPitch start");

}

void BirdsScene::onPitchEnd() {
    
    particles.push_back(currentParticle);
    currentParticle = NULL;
   // ofLogNotice("onPitch End");

}
