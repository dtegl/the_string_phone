//
//  PeakDetector.h
//  karplusInstrument
//
//  Created by djst on 4/26/15.
//
//

#ifndef karplusInstrument_PeakDetector_h
#define karplusInstrument_PeakDetector_h

// Peak detector
struct PeakDetector {
    
    float	b0_r, a1_r, b0_a, a1_a, levelEstimate;
    
    PeakDetector() {
        
        // default to pass-through
        this->a1_r = 0; // release coeffs
        this->b0_r = 1;
        this->a1_a = 0; // attack coeffs
        this->b0_a = 1;
        reset();
    }
    
    void setTauRelease(float tauRelease, float fs) {
        a1_r = exp( -1.0 / ( tauRelease * fs ) );
        b0_r = 1 - a1_r;
    }
    
    void setTauAttack(float tauAttack, float fs) {
        a1_a = exp( -1.0 / ( tauAttack * fs ) );
        b0_a = 1 - a1_a;
    }
    
    void reset() {
        // reset filter state
        levelEstimate=0;
    }
    
    void process (float input, float& output) {
        if ( fabs( input ) > levelEstimate )
            levelEstimate += b0_a * ( fabs( input ) - levelEstimate );
        else
            levelEstimate += b0_r * ( fabs( input ) - levelEstimate );
        output = levelEstimate;
    }
};


#endif
