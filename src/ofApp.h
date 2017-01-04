#pragma once

#include "ofMain.h"
//#include "ofxMidi.h"
#include "DelayLine.h"
#include "HalfBiquad.h"
#include "Utils.h"
#include "KarplusVoicer.h"
#include "Reverb.h"
#include "ResonantLowpass.h"
#include "MoogVCF.h"
#include "lpc.h"
#include "FormantFilter.h"
#include "WhiteNoiseGen.h"
#include "RMSDetector.h"
#include "Compressor.h"
#include <sndfile.hh>
#include <stdio.h>

//class ofApp : public ofBaseApp, public ofxMidiListener {
class ofApp : public ofBaseApp {
    
	public:
		
		void setup();
		void update();
		void draw();
        void exit();
    
        //void newMidiMessage(ofxMidiMessage& msg);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
				
		void audioIn(float * input, int bufferSize, int nChannels);
        void audioOut(float * output, int bufferSize, int nChannels);
    
        KarplusVoicer* voicer;
        Reverb reverb;
        MoogVCF moogFilter;
        //FormantFilter* formantFilter;
        //Compressor* compressor;
    
        float* leftchan;
		vector <float> input_vector;
        vector<float> output_vector;
    
        int bufferCounter;
        int drawCounter;
    
        float smoothedVol;
        float scaledVol;
        float rmsRatio;
    
        // LPC ATTRIBUTES
        //const static int order = 30; // Lets find 4 formants
        //lpc_data_* lpc_data;
        //float coefs[order];
        //float power;
        //float pitch;
    
        Utils utils;

        int sampleRate;
		
		ofSoundStream soundStream;
    
        stringstream text;
    
        //ofxMidiIn midiIn;
        //ofxMidiMessage midiMessage;
};
