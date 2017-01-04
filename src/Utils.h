//
//  Utils.h
//  karplusInstrument
//
//  Created by djst on 12/20/14.
//
//

#ifndef karplusInstrument_Utils_h
#define karplusInstrument_Utils_h

#include <math.h>
#include "ofMain.h"

class Utils {

public:
    
    void setSampleRate(float fs);
    
    void bilinearTransformOrder1(float acoefs[], float dcoefs[]);
    void bilinearTransformOrder2(float acoefs[], float dcoefs[]);
    
    // MIDI to Frequency
    int frequencyToMIDI(float f);
    float midiToFrequnecy(int midinote);
    
    float fs;
    

    
    
    
    
    
    
    
    
};
#endif
