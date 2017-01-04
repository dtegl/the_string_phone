//
//  MoogVCF.h
//  karplusInstrument
//
//  Created by djst on 3/22/15.
//
//

#ifndef __karplusInstrument__MoogVCF__
#define __karplusInstrument__MoogVCF__

#include <stdio.h>
#include <math.h>

class MoogVCF {
public:
    MoogVCF();
    void process(float input, float &output);
    void setCutoff(float cutoff);
    void setRes(float res);
    
private:
    float cutoff, res;
    float f, fb;
    float in1,in2,in3,in4;
    float out1,out2,out3,out4;
    
};

#endif /* defined(__karplusInstrument__MoogVCF__) */
