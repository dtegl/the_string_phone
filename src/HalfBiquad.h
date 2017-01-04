//
//  HalfBiquad.h
//  karplusInstrument
//
//  Created by djst on 12/20/14.
//
//

#ifndef karplusInstrument_HalfBiquad_h
#define karplusInstrument_HalfBiquad_h

//------------------------------------------------------------------------------
//  one-pole, one-zero shelf filter
struct HalfBiquad {
    float	b1, a0, a1, z1;
    
    HalfBiquad()		{
        this->b1 = 0.0;
        this->a0 = 1.0;
        this->a1 = 0.0;
        reset();
    }
    
    void setCoefs (float* coefs)		//pointer to array: [b0 b1 b2 a1 a2]
    {
        this->b1 =* (coefs+2);
        this->a0 =* (coefs);
        this->a1 =* (coefs+1);
    }
    void reset() {
        z1 = 0;
    }
    
    void process (float input, float& output)
    {
        //Transposed Direct II Form
        output = z1 + input * a0;
        z1 = input * a1 - output * b1;
    }
};

#endif
