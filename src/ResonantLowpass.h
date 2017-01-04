//
//  ResonantLowpass.h
//  karplusInstrument
//
//  Created by djst on 3/19/15.
//
//

#ifndef __karplusInstrument__ResonantLowpass__
#define __karplusInstrument__ResonantLowpass__

#include <stdio.h>
#include <math.h>
#include "ofMain.h"
#include "BiquadFilter.h"
#include "Utils.h"

class ResonantLowpass {
    
public:
    //Resonant lowpass filter
    float gainValue;	// filter gain, dB
    float fcValue;	// filter center frequency, Hz
    float qValue;	// filter resonance, ratio
    float drive;
    float fs;
    
    ResonantLowpass();
    
    void process(float input, float &output);
    void setFcValue(float fcValue);
    void setQValue(float qValue);
    
private:
    void designResonantLowPass(float* dcoefs, float center, float qval);
    
    float lpCoefs[5];	// filter coefficients
    BiquadFilter lpFilter;	// filter
    Utils utils;
    
   
    
};

#endif /* defined(__karplusInstrument__ResonantLowpass__) */
