//
//  ResonantLowpass.cpp
//  karplusInstrument
//
//  Created by djst on 3/19/15.
//
//

#include "ResonantLowpass.h"

ResonantLowpass::ResonantLowpass() {
    
    fs = 44100;
    gainValue = (float) 0.0;	// input filter gain, dB
    fcValue = (float) 1000.0;	// input filter center frequency, Hz
    qValue = (float) 5.0;	// input filter resonance, ratio
    
    // design new input filter
    designResonantLowPass(lpCoefs, fcValue, qValue);
    lpFilter.setCoefs(lpCoefs);

}

void ResonantLowpass::setFcValue(float fcValue) {
    // design new input filter
    this->fcValue = fcValue;
    designResonantLowPass(lpCoefs, fcValue, qValue);
    lpFilter.setCoefs(lpCoefs);
}

void ResonantLowpass::setQValue(float qValue) {
    // design new input filter
    this->qValue = qValue;
    designResonantLowPass(lpCoefs, fcValue, qValue);
    lpFilter.setCoefs(lpCoefs);
}

void ResonantLowpass::process(float input, float &output) {
    lpFilter.process(input, output);
}

void ResonantLowpass::designResonantLowPass(float* dcoefs, float center, float qval)
// design parametric filter based on input center frequency, gain, Q and sampling rate
{
    float b0, b1, b2, a0, a1, a2;		//storage for continuous-time filter coefs
    float acoefs[6];
    
    //Design parametric filter here. Filter should be of the form
    //
    //    2
    // b2s  + b1s + b0
    // ---------------
    //    2
    // a2s  + a1s + a0
    //
    // Parameters are center frequency in Hz, gain in dB, and Q.
    
    //TODO: design analog filter based on input gain, center frequency and Q
    ///////////////START//////////////////
    
    ////////// Prewarp:
    float T = 1/fs;
    float fc = (2/T)*tanf(center*T/2);
    //////////
    float wc = fc*2*PI;
    
    b0 = 1.0; b1 = 0.0; b2 = 0.0;
    a0 = 1.0; a1 = 1/(qval*wc); a2 = 1/(wc*wc);
    ////////////////END/////////////////////
    
    
    // pack the analog coeffs into an array and apply the bilinear tranform
    acoefs[0] = b0; acoefs[1] = b1; acoefs[2] = b2;
    acoefs[3] = a0; acoefs[4] = a1; acoefs[5] = a2;
    
    // inputs the 6 analog coeffs, output the 5 digital coeffs
    utils.bilinearTransformOrder2(acoefs, dcoefs);
    
}

