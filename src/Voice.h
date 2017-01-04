//
//  Voice.h
//  karplusInstrument
//
//  Created by djst on 12/25/14.
//
//

#ifndef karplusInstrument_Voice_h
#define karplusInstrument_Voice_h

#include "DelayLine.h"
#include "Utils.h"
#include "ofMain.h"
#include "HalfBiquad.h"
#include "ADSR.h"

//------------------------------------------------------------------------------
// signal processing functions
struct Voice {

    DelayLine delay;
    HalfBiquad oneZero;
    stk::ADSR adsr;
    bool playing;
    float z1;
    float attenuation;
    
    Utils utils;
    float sampleRate, feedback;
    
    Voice(float sr) {
        sampleRate = sr;
        feedback = 0.9;
        
        //Init filter
        float oneZeroCoefs[3] = {.5, .5, 0}; // Simple OneZero lowpass
        oneZero.setCoefs(oneZeroCoefs);
        
        //Init ADSR values
        adsr.setAllTimes(1.0, .2, .7, 1.0);
    }
    
    void keyOn() {
        playing = true;
    }
    
    void keyOff() {
        delay.reset();
        playing = false;
    }
    
    void setADSRValues(float a, float d, float s, float r){
        adsr.setAllTimes(a, d, s, r);
    }
    
    void setFrequency(int midinote) {
        float delayLength = sampleRate/utils.midiToFrequnecy(midinote);
        delay.setDelay(delayLength);
        ofLog() << delayLength;
    }

    void process (float input, float& output) {
        output = delay.read();
        oneZero.process(output, z1);
        delay.write(input+z1*feedback);
        delay.updatePointers();
    }
};

#endif

