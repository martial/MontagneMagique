//
//  SimpleParticle.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#include "SimpleParticle.hpp"
#include "ofMain.h"

//------------------------------------------------------------
SimpleParticle::SimpleParticle(){
    setInitialCondition(0,0,0,0);
    damping = 0.01f;
    scale   = 1.0f;
}

//------------------------------------------------------------
void SimpleParticle::resetForce(){
    // we reset the forces every frame
    frc.set(0,0);
}

//------------------------------------------------------------
void SimpleParticle::addForce(float x, float y){
    // add in a force in X and Y for this frame.
    frc.x = frc.x + x;
    frc.y = frc.y + y;
}

//------------------------------------------------------------
void SimpleParticle::addDampingForce(){
    
    // the usual way to write this is  vel *= 0.99
    // basically, subtract some part of the velocity
    // damping is a force operating in the oposite direction of the
    // velocity vector
    
    frc.x = frc.x - vel.x * damping;
    frc.y = frc.y - vel.y * damping;
}

//------------------------------------------------------------
void SimpleParticle::setInitialCondition(float px, float py, float vx, float vy){
    pos.set(px,py);
    vel.set(vx,vy);
}

//------------------------------------------------------------
void SimpleParticle::update(){
    
    
    vel = vel + frc;
    
    ofVec2f newPos  = pos + vel;
    ofVec2f diffPos = newPos - pos;
    rotation  = ofRadToDeg(atan2(diffPos.y, diffPos.x));
    pos = newPos;
    
}

//------------------------------------------------------------
void SimpleParticle::draw(){
    
    ofCircle(pos.x, pos.y, 3);
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofScale(scale,scale);
    ofRotateDeg(90 + rotation);
    img->draw(0,0);
    
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CORNER);
    
    
}
