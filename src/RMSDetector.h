//
//  RMSDetector.h
//  karplusInstrument
//
//  Created by djst on 4/25/15.
//
//

#ifndef karplusInstrument_RMSDetector_h
#define karplusInstrument_RMSDetector_h

#define ORDER 5

#include <math.h>
#include <ofMain.h>

// Peak detector
struct RMSDetector {
    
    float	b0, a0, levelEstimate;
    
    RMSDetector() {
        
        // default to pass-through
        this->a0 = 0; // release coeffs
        this->b0 = 1;
        reset();
    }
    
    void setTau(float tau, float fs) {
        a0 = exp( -1.0 / ( tau * fs ) );
        b0 = 1 - a0;
    }
    
    void reset() {
        levelEstimate = 0;
    }

    void process (float input, float& output) {
        levelEstimate = a0 * (levelEstimate*levelEstimate) + b0 * (input*input);
        output = sqrt(levelEstimate);
    }

};
#endif
