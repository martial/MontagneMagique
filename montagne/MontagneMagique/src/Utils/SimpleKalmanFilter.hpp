//
//  SimpleKalmanFilter.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 10/04/2019.
//

#ifndef SimpleKalmanFilter_hpp
#define SimpleKalmanFilter_hpp

#include <stdio.h>

class SimpleKalmanFilter {
    
public:
    
    float q;     // process variance
    float r;     // estimate of measurement variance, change to see effect
    
    float xhat;  // a posteriori estimate of x
    float xhatminus;   // a priori estimate of x
    float p;     // a posteriori error estimate
    float pminus;      // a priori error estimate
    float kG ;    // kalman gain
    
    SimpleKalmanFilter() {
        
        setQ(q);
        setR(r);
        
        xhat    = 0.0;  // a posteriori estimate of x
        p       = 1.0;     // a posteriori error estimate
        kG      = 0.0;    // kalman gain
        
    }
    
    void setup() {
        setQ(1.0);
        setR(100.0);
        
        xhat    = 0.0;  // a posteriori estimate of x
        p       = 1.0;     // a posteriori error estimate
        kG      = 0.0;    // kalman gain
    }
    
    void setQ(float q = 1.0) {
        this->q = q;
    }
    
    void setR(float r = 2.0) {
        this->r = r;
    }
    
    float getXhat() {
        return this->xhat;
    }
    
    void predict() {
        xhatminus   = xhat;
        pminus      = p + q;
    }
    
    float correct(float x) {
        kG = pminus / (pminus + r);
        xhat = xhatminus + kG * (x - xhatminus);
        p = (1 - kG) * pminus;
        
        return xhat;
    }
    
    float predict_and_correct(float x) {
        predict();
        return correct(x);
    }
};

#endif /* SimpleKalmanFilter_hpp */
