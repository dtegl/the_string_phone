//-------------------------------------------------------------------------------------------------------
// VST Effect Plug-in
//
// Filename     : Compressor.h
// Created by   : music424 staff
// Company      : CCRMA - Stanford
// Description  : 
//
// Date         : 4/5/12
//-------------------------------------------------------------------------------------------------------

#ifndef __compressor__
#define __compressor__

#include "RMSDetector.h"
#include "PeakDetector.h"

#include <math.h>
#include "ofMain.h"

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

#ifndef dB2lin
#define dB2lin(x)           pow( 10.0, (x) / 20.0 )
#endif

#define kMaxLen             32

//-------------------------------------------------------------------------------------------------------
// The VST plug-in
class Compressor
{
public:
    Compressor(float sampleRate, float ratio, float threshold, float attack, float release);
	~Compressor ();
    
	// Processing
	void process (float input, float &output);
    
protected:
	float attack_time;        // attack time in seconds
	float release_time;       // release time in seconds
	float threshold;          // compression threshold in linear scale
	float logthresh;          // compression threshold in logarithmic scale
	float comp_ratio;         // compression ratio
    float detector_exponent;  // peak detector exponent
	float gainval;            // compressor's gain computer gain in linear scale
	float dbgainval;          // compressor's gain computer gain in dB scale
    
    PeakDetector peak_detector;
    RMSDetector rms_detector;
};



#endif
