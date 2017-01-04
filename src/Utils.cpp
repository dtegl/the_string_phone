//
//  Utils.cpp
//  karplusInstrument
//
//  Created by djst on 12/20/14.
//
//

#include "Utils.h"

void Utils::setSampleRate(float fs) {
    this->fs = fs;
}

void Utils::bilinearTransformOrder1(float acoefs[], float dcoefs[]) {
    float a1,b0,b1,norm,b0z,b1z,a1z;
    
    // Pre-warp center frequency
    float T = 1.0/fs;
    
    // ####### Define coefs
    b0 = acoefs[0];
    b1 = acoefs[1];
    a1 = acoefs[2];
    
    // ####### Bilinear transform #########
    float K = 2.0/T;
    
    norm = a1*K+1;
    b0z = (b1*K+b0)/norm;
    b1z = (b0-b1*K)/norm;
    a1z = (1-a1*K)/norm;
    
    // ######## Return coefs #########
    *(dcoefs) = b0z;
    *(dcoefs+1) = b1z;
    *(dcoefs+2) = a1z;
    
    ofLog() << "[" << dcoefs[0] << ", " << dcoefs[1] << ", " << dcoefs[2] << "]";

}


void Utils::bilinearTransformOrder2(float acoefs[], float dcoefs[])
{
    double b0, b1, b2, a0, a1, a2;		    //storage for continuous-time filter coefs
    double bz0, bz1, bz2, az0, az1, az2;	// coefs for discrete-time filter.
    
    // For easier looking code...unpack
    b0 = acoefs[0]; b1 = acoefs[1]; b2 = acoefs[2];
    a0 = acoefs[3]; a1 = acoefs[4]; a2 = acoefs[5];
    
    
    // Apply bilinear transform
    ///////////////START//////////////////
    float T = 1/fs;
    float K = 2/T;
    
    bz0 = b2*(K*K) + b1*K + b0;
    bz1 = 2*b0 - 2*b2*(K*K);
    bz2 = b2*(K*K) - b1*K + b0;
    
    az0 = a2*(K*K) + a1*K + a0;
    az1 = 2*a0 - 2*a2*(K*K);
    az2 = a2*(K*K) - a1*K + a0;

    
    // return coefficients to the output
    dcoefs[0] = bz0/az0; dcoefs[1] = bz1/az0; dcoefs[2] = bz2/az0;
    dcoefs[3] = az1/az0; dcoefs[4] = az2/az0;
    
    ofLog() << "[" << dcoefs[0] << ", " << dcoefs[1] << ", " << dcoefs[2] << ", " <<
                      dcoefs[3] << ", " << dcoefs[4] << "]";
    
}


float Utils::midiToFrequnecy(int midinote) {
    return powf(2.0, (midinote-69)/12.0f) * 440;
}
