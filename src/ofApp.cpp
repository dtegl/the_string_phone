#include "ofApp.h"
#include <SineWave.h>

using namespace stk;

int midi_values[] = {60,61,62,63,64,65,66,67,68,69,70,71,72,73,74};
int key_values[] = {97, 119, 115, 101, 100, 102, 116, 103, 121, 104, 117, 106, 107, 111, 108};
int keys = 15;
float octave = 0;

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
	
	// 0 output channels, 
	// 2 input channels.
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
	soundStream.listDevices();
	
	//if you want to set a different device id 
	//soundStream.setDeviceID(2); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	
	int bufferSize = 1024;
    sampleRate 	= 44100;
    utils.setSampleRate(sampleRate);
    
    //Initialize voicer
    voicer = new KarplusVoicer(sampleRate);
    
    //compressor = new Compressor(sampleRate, 5.0f, 0.3f, 0.001f, 0.10f);
    
    //Init LPC
    //lpc_data = lpc_create();
    
    //Init Formant filer
    //formantFilter = new FormantFilter(order);

    //Audio buffers
    input_vector.assign(bufferSize, 0.0);
    output_vector.assign(bufferSize, 0.0);
    

    // of sound setup
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
	soundStream.setup(this, 2, 1, sampleRate, bufferSize, 4);
    
    moogFilter.setCutoff(1);
    
    // ########### MIDI SETUP ###############
    /*
    // print input ports to console
    midiIn.listPorts(); // via instance
    //ofxMidiIn::listPorts(); // via static as well
    
    // open port by number (you may need to change this)
    midiIn.openPort(2);
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add testApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
    */
   
}

//--------------------------------------------------------------
void ofApp::update(){
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //## ENERGY INPUT TO CONTROL CUTOFF
    //scaledVol = ofMap(smoothedVol, 0.0, 0.05, 0.1, 1.0, true);
    //moogFilter.setCutoff(scaledVol);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(0);

    /*
    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 20);
    text.str(""); // clear
     
     
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 34);
    text.str(""); // clear
    
    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 48);
    text.str(""); // clear
    ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 96);
    text.str(""); // clear
    ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 144);
    text.str(""); // clear
    ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 192);
    text.str(""); // clear
    if(midiMessage.status == MIDI_PITCH_BEND) {
        ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    }
    else {
        ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    }
    
    text << "delta: " << midiMessage.deltatime;
    ofDrawBitmapString(text.str(), 20, 240);
    text.str(""); // clear
    
    */
    // DRAW RMS MEASUREMENT
    ofNoFill();
    
    // draw the average volume:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(565, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofRect(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofCircle(200, 200, scaledVol * 190.0f);
    
    ofPopMatrix();
    ofPopStyle();
    
    drawCounter++;
    
    ofSetColor(225);
    string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount())+"\noctave: " + ofToString(octave);
    ofDrawBitmapString(reportString, 32, 589);
    
    //DRAW formant filter
    // draw the right channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 370, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("FormantFtiler", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
 
    ofPopMatrix();
    ofPopStyle();
    
}

void ofApp::exit() {
    // clean up
    //midiIn.closePort();
    //midiIn.removeListener(this);
}

//--------------------------------------------------------------
/*
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // Handle midi note on and off
    if(msg.velocity > 0) {
        voicer->getVoice(msg.pitch)->setFrequency(msg.pitch);
        voicer->getVoice(msg.pitch)->adsr.keyOn();

    }
    else {
        voicer->getVoice(msg.pitch)->adsr.keyOff();
    }
    
    

    float e = 0.01; // Make sure the values are positive
    // Resonant Lowpass filter
    if(msg.control == 74){
        const static float FcLimits[2] = {0.01, 1.0};
        const static float FcTaper = -1.0;
        
        float fc = Knob::knob2value(msg.value/127.0, FcLimits, FcTaper);
        moogFilter.setCutoff(fc);
        //lpFilter.setFcValue(fc);
        ofLog() << "Cutoff: " << fc;
    }
    
    if(msg.control == 71){
        float q = (msg.value / 127.0) * 4;
        moogFilter.setRes(q);
        //lpFilter.setQValue(q);
        ofLog() << "Resonance: " << q;
    }

    if(msg.control == 76){
        float drywet = (msg.value / 127.0);
        reverb.setParameter(4, drywet);
        //lpFilter.setQValue(q);
        ofLog() << "Drywet: " << drywet;
    }
    
    // Handle ADSR mapping
    if(msg.control == 5){
        float a = (msg.value / 127.0 * 10)+e;
        voicer->setAttack(a);
         ofLog() << "Attack: " << a;
    }
    if(msg.control == 0){
        float d = (msg.value / 127.0 * 2)+e;
        voicer->setDecay(d);
        ofLog() << "Decay: " << d;
    }
    if(msg.control == 73){
        float s = (msg.value) / 127.0;
        voicer->setSustain(s);
         ofLog() << "Sustain: " << s;
    }
    if(msg.control == 75){
        float r = (msg.value / 127.0 * 10)+e;
        voicer->setRelease(r);
        ofLog() << "Release: " << r;
    }
    
    ofLog() << "Midincontrol: " << msg.control << " value: " << msg.value;
    ofLog() << "Midinote: " << msg.pitch << ", Frequency: " << utils.midiToFrequnecy(msg.pitch);
    
    // make a copy of the latest message
    midiMessage = msg;
}
*/

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    for (int i = 0; i < bufferSize; i++){
        
        input_vector[i] = input[i]*0.5;
        
        //RMS for the graphics
        curVol += input_vector[i] * input_vector[i];
        numCounted += 1;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
    //float* inputchan = &input[0];
    //lpc_analyze(lpc_data, inputchan, bufferSize, coefs, order, &power, &pitch);
    //formantFilter->setCoefs(coefs, power, pitch);
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    float voiceOut = 0.0f;
    float out, inp0;
    
    for (int i = 0; i < bufferSize; i++){
        inp0 = input_vector[i];
        // Process each playing voice, sum their output
        out = 0.0f;
        for(Voice* v : voicer->voices) {
            if(v->adsr.getState() != v->adsr.IDLE) {
                v->process(inp0, voiceOut);
//                formantFilter->process(voiceOut);
                out += voiceOut * v->adsr.tick();
            }
        }
        
        //Moog VCF 24db resonant lowpass
        //moogFilter.process(out, out);
        //Add reverb
        //compressor->process(out*0.05, out);
  
        //reverb.process(out, out);
        
        //Peak compress signal to avoid clipping
        output[i*2] = output[i*2+1] = out;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 120) octave++;
    if(key == 122) octave--;
    if(octave < -2) octave = -2;
    if(octave > 2) octave = 2;
    
    for(int i = 0; i < keys; i++) {
        if(key_values[i] == key) {
            voicer->getVoice(midi_values[i])->setFrequency(midi_values[i]+(octave*12));
            voicer->getVoice(midi_values[i])->adsr.keyOn();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
    for(int i = 0; i < keys; i++) {
        if(key_values[i] == key) {
            voicer->getVoice(midi_values[i])->adsr.keyOff();
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



