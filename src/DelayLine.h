//
//  DelayLine.h
//  karplusInstrument
//
//  Created by djst on 12/20/14.
//
//

#ifndef karplusInstrument_DelayLine_h
#define karplusInstrument_DelayLine_h

#include "ofMain.h"

#define kMaxDelay 8192
#define N 8192

struct DelayLine {
    double	dly[kMaxDelay];							// double-precision delay line
    long	wp,rp,theDelay; // read, write pointers and delay length
    
    DelayLine() {
        // clear delay line on instantiation
        for (int indx = 0; indx < kMaxDelay; indx++) {
            dly[indx] = 0.0;
        }
        
        // init write and read pointers to zero
        wp = 0; rp = 0;
    }
    
    void reset() {
        // clear delay
        for (int indx = 0; indx < kMaxDelay; indx++) {
            dly[indx] = 0.0;
        }
    }
    
    void setDelay(long aDelay) {
        theDelay = aDelay;
    }
    
    // write data into line
    void write(double data) {
        dly[wp] = data;
    }

    // read data from line
    double read() {
        return dly[rp];
    }
    
    // advance read, write pointers
    void updatePointers()
    {
        wp--;
        if(wp < 0)
            wp = kMaxDelay-1;
        
        rp = wp+theDelay;
        
        if(rp > kMaxDelay-1)
            rp -= kMaxDelay;
        
        if(rp > kMaxDelay-1)
            rp = kMaxDelay-1;
        
        if(rp < 0)
            rp = 0;
    }
    
};



#endif
