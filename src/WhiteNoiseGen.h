//
//  whiteNoiseGen.h
//  karplusInstrument
//
//  Created by djst on 4/16/15.
//
//

#ifndef karplusInstrument_WhiteNoiseGen_h
#define karplusInstrument_WhiteNoiseGen_h


#include <cstdlib>
#include <ctime>

struct WhiteNoiseGen {

    int q;
    float c1, c2, c3;
    float random, noise;
    /* Generate a new random seed from system time - do this once in your constructor */
    WhiteNoiseGen() {
        srand(time(0));
        
        q = 15;
        c1 = (1 << q) - 1;
        c2 = ((int)(c1 / 3)) + 1;
        c3 = 1.f / c1;
    }

    /* random number in range 0 - 1 not including 1 */
    float tick() {
        random = 0.f;
        noise = 0.f;
        /* the white noise */
        
        random = ((float)rand() / (float) RAND_MAX + 1);
        return (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
    }
};

#endif
