//
//  FDNReverb.h
//  karplusInstrument
//
//  Created by djst on 3/5/15.
//
//

#ifndef __karplusInstrument__FDNReverb__
#define __karplusInstrument__FDNReverb__

#include <stdio.h>
#include "HalfBiquad.h"
#include "BiquadFilter.h"
#include "DelayLine.h"
#include "Knob.h"

#ifndef max
#define max(a,b)			(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)			(((a) < (b)) ? (a) : (b))
#endif

#ifndef dB
// if below -100dB, set to -100dB to prevent taking log of zero
#define dB(x)               20.0 * ((x) > 0.00001 ? log10(x) : log10(0.00001))
#endif

#ifndef dB2mag
#define dB2mag(x)           pow( 10.0, (x) / 20.0 )
#endif

#define kMaxLen			32

#define pi 3.14159265358979

class Reverb {
public:
    Reverb ();
    ~Reverb ();
    
    // Processing
    void process (float input, float& output);
    
    void setParameter (int index, float value);
    
    void designShelf(float* pcofs, long theLength, float transition, float T60low, float T60high);
    void bilinearTransform(float acoefs[], float dcoefs[]);
    void designParametric(float* dcoefs, float center, float gain, float qval);
    
protected:
    
    // param IDs
    enum {
        kParamT60low  = 0,
        kParamT60high,
        kParamTransition,
        kParamWetDry,
        kParamQ,
        kParamGamma,
        kParamFc,
        kNumParams
    };

    // internal state var declaration and initialization
    float fs;
    #define kNumDelays	12
    HalfBiquad fbfilt[kNumDelays];
    DelayLine dl[kNumDelays];
    float coefs[3];
    float*	pcoefs;
    
    // parametric section
    float parametric_coefs[5];
    BiquadFilter parametric[2];
    
    // knob vars
    float T60LowKnob, T60LowValue;
    float T60HighKnob, T60HighValue;
    float TransitionKnob, TransitionValue;
    float WetDryKnob; // since the mix in the [0,1] range, we can use the knob value directly
    float ParametricQKnob, ParametricQValue;
    float ParametricGammaKnob, ParametricGammaValue;
    float ParametricFcKnob, ParametricFcValue;
    
};

const static float InVecL[kNumDelays]={1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0};
const static float InVecR[kNumDelays]={0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0};
const static float OutVecL[kNumDelays]={1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0};
const static float OutVecR[kNumDelays]={0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0};

const static float dlens[kNumDelays]={2023,2153,2291,2438,2595,2761,2939,3127,3328,3542,3769,4011};


// UI controls limits and tapers
const static float T60LowLimits[2] = {0.01, 10.0};
const static float T60LowTaper = -1;

const static float T60HighLimits[2] = {0.01, 10.0};
const static float T60HighTaper = -1;

const static float TransitionLimits[2] = {50.0, 16000.0};
const static float TransitionTaper = -1;

const static float ParametricGammaLimits[2] = {-24.0, 24.0};
const static float ParametricGammaTaper = 1.0;

const static float ParametricFcLimits[2] = {50.0, 16000.0};
const static float ParametricFcTaper = -1.0;

const static float ParametricQLimits[2] = {0.25, 32.0};
const static float ParametricQTaper = -1.0;


// TODO: replace the identity "mixing" matrix with a matrix that actaully mixes!
// You can generate the Hadamard matrix or you can create a different
// one using the orthonorm.m matlab script provided.

 //Hadamard
 const static float FB[kNumDelays][kNumDelays]={
 { 0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887,    0.2887, },
 { 0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887, },
 { 0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887, },
 { 0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887, },
 { 0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887, },
 { 0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887, },
 { 0.2887,   -0.2887,   -0.2887,  - 0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887, },
 { 0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887,    0.2887, },
 { 0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,    0.2887, },
 { 0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887, },
 { 0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887,    0.2887, },
 { 0.2887,    0.2887,   -0.2887,    0.2887,    0.2887,    0.2887,   -0.2887,   -0.2887,   -0.2887,    0.2887,   -0.2887,   -0.2887, },
 };

 //decay 0.5
/*
 const static float FB[kNumDelays][kNumDelays]={
 {+0.58076, -0.45836, -0.10727, -0.29573, +0.04142, +0.31271, +0.44465, +0.14117, -0.12638, +0.11848, +0.08037, -0.00697, },
 {+0.37958, +0.54185, +0.09829, -0.34830, -0.52002, +0.05621, -0.17772, -0.05137, -0.23033, +0.10031, -0.17542, +0.17219, },
 {+0.35620, -0.07721, +0.51430, +0.57953, -0.25287, +0.13573, -0.17828, +0.11168, +0.09951, +0.01942, +0.30878, -0.18588, },
 {+0.07688, -0.04257, -0.54935, +0.22476, -0.16280, +0.15283, -0.22763, -0.18139, +0.23782, +0.58837, -0.15497, -0.28045, },
 {+0.47052, +0.11068, -0.37108, +0.26256, +0.37420, -0.36771, -0.29113, +0.22453, -0.32092, -0.21000, -0.03621, +0.03025, },
 {-0.02859, +0.38547, -0.08148, +0.36779, +0.25732, +0.67818, +0.22137, +0.04144, -0.02582, -0.13371, -0.24254, +0.23335, },
 {+0.23252, +0.09445, +0.06936, +0.24796, -0.01892, -0.35099, +0.50371, -0.65468, +0.03570, -0.08890, -0.21853, -0.08891, },
 {+0.09521, +0.22895, -0.37125, +0.01962, -0.27472, -0.13821, +0.25724, +0.23473, +0.58437, -0.26843, +0.36906, +0.18628, },
 {+0.31461, +0.05793, +0.24483, -0.30857, +0.44076, +0.07550, -0.31486, -0.15596, +0.61450, -0.07141, -0.18789, -0.03260, },
 {-0.00735, +0.19708, +0.25588, +0.08035, +0.17471, -0.32958, +0.31527, +0.48828, +0.12602, +0.57338, -0.23231, +0.11667, },
 {+0.00771, +0.32910, +0.00903, -0.10218, +0.35585, +0.04032, +0.03197, -0.29844, -0.14620, +0.35679, +0.71405, +0.06505, },
 {+0.03772, -0.34037, +0.00809, +0.16331, -0.06261, -0.06243, -0.18409, -0.21075, +0.05679, +0.16202, -0.01308, +0.86036, },
 };
*/
// decay 1
/*
 const static float FB[kNumDelays][kNumDelays]={
{+0.68409, +0.39064, +0.40283, -0.31239, +0.06259, +0.25365, -0.11018, -0.04714, +0.04250, +0.10155, +0.15611, +0.02119, },
{-0.63660, +0.33615, +0.65623, +0.08356, +0.01508, +0.16235, -0.03606, -0.03708, +0.08082, +0.03964, +0.06055, +0.05553, },
{-0.15411, -0.56166, +0.14116, -0.66279, -0.08373, +0.35409, +0.13222, +0.11495, +0.17243, -0.05382, -0.02560, -0.07213, },
{+0.17250, -0.57610, +0.43306, +0.34121, +0.49622, -0.07274, -0.11746, -0.06338, -0.22576, +0.03236, +0.11177, -0.02330, },
{-0.17099, +0.08705, -0.42675, +0.04820, +0.41890, +0.65663, -0.33115, -0.08895, -0.09058, +0.11975, +0.17648, +0.02935, },
{+0.16124, -0.19913, +0.07478, +0.50314, -0.61495, +0.50844, +0.15160, -0.08795, +0.00358, +0.06512, +0.06107, +0.01500, },
{+0.07306, +0.14287, -0.03449, +0.17959, +0.42063, +0.19442, +0.76992, +0.16918, +0.25270, -0.06229, -0.12820, -0.14318, },
{+0.05952, -0.07325, -0.00126, +0.20917, +0.01588, -0.05942, -0.37676, +0.52274, +0.70553, +0.14155, +0.05413, -0.08723, },
{-0.00648, -0.09779, -0.04157, -0.02837, +0.06293, -0.11335, +0.05186, -0.69917, +0.45551, +0.49388, -0.14799, +0.07881, },
{-0.04570, -0.02998, -0.03265, -0.07197, -0.02432, -0.07093, +0.22119, +0.38629, -0.24933, +0.70361, +0.16220, +0.45106, },
{+0.01287, -0.05912, -0.05544, +0.01014, +0.04101, -0.05979, +0.11065, -0.12378, +0.26725, -0.42958, +0.49056, +0.68097, },
{+0.08266, -0.00911, +0.06969, +0.04642, +0.07449, +0.16180, -0.15509, +0.09088, -0.00617, -0.13470, -0.78467, +0.53678, },
};
*/
//Random Mixing Matrix generated with orthonorm.m
/*
const static double FB[kNumDelays][kNumDelays]={
    
    {-0.04847, -0.17288, -0.23635, -0.52946, -0.23560, +0.16995, +0.29591, +0.03690, +0.42629, +0.15084, +0.36397, +0.34829, },
    {-0.44432, -0.38392, +0.44898, +0.44657, -0.01030, -0.04918, +0.22425, -0.10791, +0.19304, -0.06870, -0.02037, +0.38383, },
    {-0.16270, -0.38060, -0.37445, +0.12547, -0.00376, +0.18013, +0.26726, +0.07370, +0.30189, -0.21492, -0.27043, -0.59409, },
    {-0.17087, -0.29272, +0.22427, -0.41500, +0.32503, -0.14665, +0.32731, +0.18663, -0.52384, -0.18442, +0.23249, -0.17612, },
    {-0.38613, -0.03649, -0.69953, +0.19442, -0.01122, -0.06413, -0.07095, +0.03154, -0.42670, -0.04673, +0.01413, +0.35737, },
    {+0.55569, -0.29000, -0.04017, +0.24640, -0.40354, +0.05674, +0.40270, -0.23051, -0.37491, +0.14574, +0.03057, +0.02614, },
    {-0.08730, +0.23644, -0.10955, +0.25807, +0.50241, +0.14388, +0.28903, -0.31895, +0.05450, +0.46202, +0.38453, -0.18759, },
    {+0.12821, -0.35224, -0.10105, +0.18830, -0.02110, -0.62189, -0.27787, +0.28836, +0.17454, +0.27222, +0.36841, -0.16113, },
    {+0.01103, -0.28736, -0.06791, -0.33320, +0.25180, -0.28840, -0.06267, -0.42858, +0.01936, +0.38264, -0.55599, +0.10761, },
    {+0.19522, +0.11562, -0.01982, +0.11691, +0.25636, +0.03176, +0.34268, +0.69524, +0.04381, +0.27715, -0.35905, +0.24351, },
    {+0.44237, -0.03685, -0.18095, +0.07652, +0.46317, -0.18330, +0.08495, -0.17141, +0.22201, -0.58825, +0.08792, +0.27671, },
    {+0.16938, -0.47903, +0.03365, +0.03423, +0.28150, +0.61820, -0.47650, +0.10633, -0.08841, +0.10988, +0.09718, +0.10209, },
};
 */
/*
const static double FB[kNumDelays][kNumDelays]={
    //Identity Mixing Matrix (no mixing, actually)
    {+1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, +0.00000, },
    {+0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +0.00000, +1.00000, },
    
    
};
*/
#endif
