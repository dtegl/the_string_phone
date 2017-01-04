//
//  Biquad.h
//  karplusInstrument
//
//  Created by djst on 12/20/14.
//
//

#ifndef karplusInstrument_FormantFilter_h
#define karplusInstrument_FormantFilter_h

//------------------------------------------------------------------------------
// signal processing functions
/*

*/
#include "PeakDetector.h"

struct FormantFilter {

    int order;
    float power, pitch;
    vector<float> coefs;
    vector<float> Zss;
    
    PeakDetector rms;
    
    FormantFilter(int order) {
        this->order = order;
        coefs.assign(order, 0.0);
        Zss.assign(order, 0.0);
        
        rms.setTauAttack(0.001f, 44100);
        rms.setTauRelease(0.01f, 44100);
        
        reset();
    }
    void setCoefs(float* coefs, float power, float pitch) {
       // rms.process(power, power);
        this->power = power;
        this->pitch = pitch;
        
        // set filter coefficients [b0 b1 b2 a1 a2]
        for(int i = 0; i < order; i++) {
            this->coefs[i] = coefs[i];
        }
    }
    void reset() {
        // reset filter state
    }
    
    void process(float &output) {
        //if(pitch == 0) {
           // output = power * 20.0f * ( 2.0f * rand() / RAND_MAX - 1.0f );
        //}
        //else {
        //    output = power * pitch * 1.0f;
        //}

        for(int j = 0; j < order; j++ )
            output += Zss[j] * coefs[j];
        
        for(int j = order - 1; j > 0; j-- )
            Zss[j] = Zss[j-1];
        
        Zss[0] = output;
    }
};

#endif
