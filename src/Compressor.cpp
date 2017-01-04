//-------------------------------------------------------------------------------------------------------
// VST Effect Plug-in
//
// Filename     : Compressor.cpp
// Created by   : music424 staff
// Company      : CCRMA - Stanford
// Description  : 
//
// Date         : 4/5/12
//-------------------------------------------------------------------------------------------------------

#include "Compressor.h"
#include <math.h>

//-------------------------------------------------------------------------------------------------------
Compressor::Compressor(float sampleRate, float ratio, float threshold, float attack, float release) {
    
    
    // translate knob position into parameter values
	//this->threshold = dB2lin(threshold);
	logthresh = dB(threshold);
    ofLog() << "logthresh: " << logthresh << ", threshold: " << this->threshold;
    attack_time = attack;
    release_time = release;;

    rms_detector.setTau(release_time, sampleRate);
    
    peak_detector.setTauAttack(attack, sampleRate);
    peak_detector.setTauRelease(release, sampleRate);
    
    comp_ratio = 20;
    
	dbgainval = 0.0;
    
}

//-------------------------------------------------------------------------------------------------------
Compressor::~Compressor ()
{
	// nothing to do here
}

//-----------------------------------------------------------------------------------------
void Compressor::process(float input, float &output)
{
    float level_estimate, log_level;

    ///////////  LEVEL DETECTOR  ////////////////////
    peak_detector.process(input, level_estimate);
        
    /////////////  GAIN COMPUTER  ///////////////////
    log_level = dB(level_estimate);
    
    gainval = 1.0f;
    if(log_level > logthresh)
    {
        dbgainval = logthresh - log_level + (log_level - logthresh) / comp_ratio;
        gainval = dB2lin(dbgainval);
    }
    
    ////////////// GAIN STAGE //////////////////////
    output = input*gainval;
}
