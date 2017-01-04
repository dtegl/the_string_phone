//
//  FDNReverb.cpp
//  karplusInstrument
//
//  Created by djst on 3/5/15.
//
//

#include "Reverb.h"
#include <math.h>
#include <stdlib.h>


//------------------------------------------------------------------------------
Reverb::Reverb ()
{
    
    fs = 44100;
    pcoefs = coefs;									// pointer for filter coef assignment
    
    
    T60LowValue = 3.5;      // time, in seconds, for low freqs to decay 60dB
    T60LowKnob = Knob::value2knob(T60LowValue, T60LowLimits, T60LowTaper);
    
    T60HighValue = 0.1;			// time, in seconds, for high freqs to decay 60dB
    T60HighKnob = Knob::value2knob(T60HighValue, T60HighLimits, T60HighTaper);
    
    TransitionValue = 6700.0;			// transition freq, in Hz, between low and high freqs
    TransitionKnob = Knob::value2knob(TransitionValue, TransitionLimits, TransitionTaper);
    
    WetDryKnob = 0.5;		// output (wet/dry) mix

    
    for(int i=0; i<kNumDelays; i++){
        // set reverb delay lengths
        dl[i].setDelay(dlens[i]);
        // design filters for feedback loop
        designShelf(pcoefs,dlens[i], TransitionValue, T60LowValue, T60HighValue);
        // assign filter coefs
        fbfilt[i].setCoefs(coefs);
    }
    
    ParametricFcValue = 5000.0;
    ParametricGammaValue = dB2mag(0.0);
    ParametricQValue = 1;
    
    designParametric(parametric_coefs, ParametricFcValue, ParametricGammaValue, ParametricQValue);
    parametric[0].setCoefs(parametric_coefs);
    parametric[1].setCoefs(parametric_coefs);
    
    
}

//------------------------------------------------------------------------------
Reverb::~Reverb ()
{
    // nothing to do here
}


//------------------------------------------------------------------------------
void Reverb::setParameter (int index, float value)
{
    switch (index)
    {
        int i;
        case kParamT60low:
            T60LowValue = Knob::knob2value(T60LowKnob, T60LowLimits, T60LowTaper);
            for (i=0; i<kNumDelays ; i++){
                designShelf(pcoefs,dlens[i], TransitionValue, T60LowValue, T60HighValue);
                fbfilt[i].setCoefs(coefs);
            }
            break;
        case kParamT60high:
            T60HighKnob = value;
            T60HighValue = Knob::knob2value(T60HighKnob, T60HighLimits, T60HighTaper);
            for (i=0; i<kNumDelays ; i++){
                designShelf(pcoefs,dlens[i], TransitionValue, T60LowValue, T60HighValue);
                fbfilt[i].setCoefs(coefs);
            }
            break;
        case kParamTransition:
            TransitionKnob = value;
            TransitionValue = Knob::knob2value(TransitionKnob, TransitionLimits, TransitionTaper);
            for (i=0; i<kNumDelays ; i++){
                designShelf(pcoefs,dlens[i], TransitionValue, T60LowValue, T60HighValue);
                fbfilt[i].setCoefs(coefs);
            }
            break;
        case kParamWetDry:
            WetDryKnob = value;
            break;
        case kParamQ:
            ParametricQKnob = value;
            ParametricQValue = Knob::knob2value(ParametricQKnob, ParametricQLimits, ParametricQTaper);
            designParametric(parametric_coefs, ParametricFcValue, ParametricGammaValue, ParametricQValue);
            parametric[0].setCoefs(parametric_coefs);
            parametric[1].setCoefs(parametric_coefs);
            break;
        case kParamGamma:
            ParametricGammaKnob = value;
            ParametricGammaValue = dB2mag(Knob::knob2value(ParametricGammaKnob, ParametricGammaLimits, ParametricGammaTaper));
            designParametric(parametric_coefs, ParametricFcValue, ParametricGammaValue, ParametricQValue);
            parametric[0].setCoefs(parametric_coefs);
            parametric[1].setCoefs(parametric_coefs);
            break;
        case kParamFc:
            ParametricFcKnob = value;
            ParametricFcValue = Knob::knob2value(ParametricFcKnob, ParametricFcLimits, ParametricFcTaper);
            designParametric(parametric_coefs, ParametricFcValue, ParametricGammaValue, ParametricQValue);
            parametric[0].setCoefs(parametric_coefs);
            parametric[1].setCoefs(parametric_coefs);
            break;
            
        default :
            break;
    };
}


//------------------------------------------------------------------------------
void Reverb::designShelf(float* pcofs, long theLength, float transition, float t60low, float t60high)
{
    double a1,b0,b1,norm,b0z,b1z,a1z;
    double roundTrip=((double)(theLength))/fs;		// get delay length in seconds
    double g0,g1;  //Temp shelf gains at DC and Nyquist
    //  Design Shelf filter here. Must produce 1-pole, 1-zero filter coefficients
    //
    //  b1s + b0
    //  ---------
    //  a1s + 1
    //
    //  where b0 = asymptotic shelf gain at low frequency,
    //  b1/a1 = asymptotic shelf gain at high frequency.
    //  These gains must be computed so that, for the
    //  given corresponding delay time T, the signal has
    //  been reduced by the factor exp(-T/tau), where tau
    //  is the natural time constant associated with the
    //  desired T60.
    //  It is ok either to put the pole of the filter at the
    //  transition frequency, or to make the transition
    //  frequency equal to the geometric mean of the pole
    //  and the zero. Explain what you are doing for full
    //  credit.
    
    
    transition *= 2*M_PI; // transition in radians
    
    // There are two options to match the continous and discrete transition
    // frequency: a) pre-warping; b) modify the bilinear transform
    
#ifdef PRE_WARP
    transition = 2*fs*tan(transition/(2*fs));
#endif
    
    
    // analog shelf design
    
    // low frequency asymptote (determined by t60low)
    g0 = exp(roundTrip*log(0.001)/t60low);
    // high frequency asymptote (determined by t60high)
    g1 = exp(roundTrip*log(0.001)/t60high);
    
    // There are different options to define rho, which basically controls the
    // transition frequency
    
    // option 1) the pole frequency defines the transition (-3 dB)
    double rho = (transition);
    
    //    // option 2) point of maximum phase
    //	rho = (transition*sqrt(lpi/l0));
    // filter coefficients
    b0 = g0;
    a1 = 1.0f/rho;
    b1 = a1*g1;
    
#ifdef PRE_WARP
    // if we pre-warped, we use the unmodified bilinear transform
    norm=1+a1*(2*fs);			// do bilinear transform
    b0z=(b0+b1*(2*fs))/norm;	// (bilinear)
    b1z=(b0-b1*(2*fs))/norm;	// (bilinear)
    a1z=(1-a1*(2*fs))/norm;	// (bilinear)
#else
    // Bilinear transform matching the transition frequency between analog and
    // digital domain
    double c = transition / tan( transition/(2*fs) );
    norm =  1 + a1 * c;
    b0z = (b0 + b1 * c) / norm;
    b1z = (b0 - b1 * c) / norm;
    a1z = ( 1 - a1 * c) / norm;
#endif
    
    *(pcoefs) = b0z;						// return coefs
    *(pcoefs+1) = b1z;
    *(pcoefs+2) = a1z;
    
}


//------------------------------------------------------------------------------
void Reverb::process(float input, float &output)
{
        float inp0 = input;
        
        float accum,Out;
        int dlind,sumind;
        
        Out=0.0;

        for (dlind=0; dlind<kNumDelays ; dlind++) {
            accum=0.0;
            for (sumind=0; sumind<kNumDelays ; sumind++) {
                // add up contributions from each delay line through orthonormal matrix
                accum+=FB[dlind][sumind]*dl[sumind].read();
            }
            Out += accum;
            // add in contributions for current line
            accum += inp0;
            // filter data with shelf
            fbfilt[dlind].process(accum,accum);
            // write back into current delay line
            dl[dlind].write(accum);
        }
        for (dlind=0; dlind<kNumDelays ; dlind++)
            dl[dlind].updatePointers();
    
        parametric[0].process(Out, Out);
    
        // compute wet/dry output
        output = Out*WetDryKnob + inp0 * (1.0-WetDryKnob);
}



//------------------------------------------------------------------------------
void Reverb::bilinearTransform(float acoefs[], float dcoefs[])
{
    float b0, b1, b2, a0, a1, a2;		    //storage for continuous-time filter coefs
    float bz0, bz1, bz2, az0, az1, az2;	// coefs for discrete-time filter.
    
    // For easier looking code...unpack
    b0 = acoefs[0]; b1 = acoefs[1]; b2 = acoefs[2];
    a0 = acoefs[3]; a1 = acoefs[4]; a2 = acoefs[5];
    
    
    // TODO: apply bilinear transform
    ///////////////START//////////////////
    float T = 1/fs;
    float K = 2/T;
    
    bz0 = b2*(K*K) + b1*K + b0;
    bz1 = 2*b0 - 2*b2*(K*K);
    bz2 = b2*(K*K) - b1*K + b0;
    
    az0 = a2*(K*K) + a1*K + a0;
    az1 = 2*a0 - 2*a2*(K*K);
    az2 = a2*(K*K) - a1*K + a0;
    
    // return coefficients to the output
    dcoefs[0] = bz0/az0; dcoefs[1] = bz1/az0; dcoefs[2] = bz2/az0;
    dcoefs[3] = az1/az0; dcoefs[4] = az2/az0;
    
}


//------------------------------------------------------------------------------
void Reverb::designParametric(float* dcoefs, float center, float gain, float qval)
// design parametric filter based on input center frequency, gain, Q and sampling rate
{
    float b0, b1, b2, a0, a1, a2;		//storage for continuous-time filter coefs
    float acoefs[6];
    
    // TODO: design analog filter based on input gain, center frequency and Q
    // Remeber to handle the two cases: boost and cut!
    b0 = 1.0; b1 = 0.0; b2 = 0.0;
    a0 = 1.0; a1 = 0.0; a2 = 0.0;
    ///////////////START//////////////////
    float T = 1/fs;
    
    // Prewarp center frequency before applying bilinear transform
    float fc = (2/T)*tanf(center*T/2);
    float wc = fc*2*pi;
    
    // Transfer function coefs for boost
    if(gain >= 1.0) {
        b0 = 1.0; b1 = gain/(qval*wc); b2 = 1/(wc*wc);
        a0 = 1.0; a1 = 1/(qval*wc); a2 = b2;
    }
    // Transfer function coefs for cut
    if(gain < 1.0) {
        b0 = 1.0; b1 = 1/(qval*wc); b2 = 1/(wc*wc);
        a0 = 1.0; a1 = 1/(qval*gain*wc); a2 = b2;
    }
    
    ////////////////END/////////////////////	
    // pack the analog coeffs into an array and apply the bilinear tranform
    acoefs[0] = b0; acoefs[1] = b1; acoefs[2] = b2; 
    acoefs[3] = a0; acoefs[4] = a1; acoefs[5] = a2;
    
    // inputs the 6 analog coeffs, output the 5 digital coeffs
    bilinearTransform(acoefs, dcoefs);
    
}
