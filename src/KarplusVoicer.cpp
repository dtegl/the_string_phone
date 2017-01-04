//
//  KarplusVoicer.cpp
//  karplusInstrument
//
//  Created by djst on 2/25/15.
//
//

#include "KarplusVoicer.h"

KarplusVoicer::KarplusVoicer() {
    
}

KarplusVoicer::KarplusVoicer(float sampleRate) {
    for(int i = 0; i < 127; i++) {
        voices[i] = new Voice(sampleRate);
        voices[i]->setFrequency(i);
    }
}

KarplusVoicer::~KarplusVoicer() {
    for(int i = 0; i < 127; i++) {
        delete voices[i];
    }
}

Voice* KarplusVoicer::getVoice(int midi_note) {
    return voices[midi_note];
}

void KarplusVoicer::setADSR(float a, float d, float s, float r) {
    for(Voice* v : voices) {
        v->adsr.setAllTimes(a, d, s, r);
    }
}

void KarplusVoicer::setAttack(float a) {
    for(Voice* v : voices) {
        v->adsr.setAttackTime(a);
    }
}

void KarplusVoicer::setDecay(float d) {
    for(Voice* v : voices) {
        v->adsr.setDecayTime(d);
    }
}

void KarplusVoicer::setSustain(float s) {
    for(Voice* v : voices) {
        v->adsr.setSustainLevel(s);
    }
}

void KarplusVoicer::setRelease(float r) {
    for(Voice* v : voices) {
        v->adsr.setReleaseTime(r);
    }
}


