//
//  KarplusVoicer.h
//  karplusInstrument
//
//  Created by djst on 2/25/15.
//
//

#ifndef __karplusInstrument__KarplusVoicer__
#define __karplusInstrument__KarplusVoicer__

#include <stdio.h>
#include <Voice.h>

class KarplusVoicer {
    
private:
    static const int voiceCount = 127;

public:
    Voice* voices[voiceCount];
    
    KarplusVoicer();
    KarplusVoicer(float sampleRate);
    ~KarplusVoicer();
    
    Voice* getVoice(int midi_note);
    
    void setAttack(float a);
    void setDecay(float d);
    void setSustain(float s);
    void setRelease(float r);
    
    void setADSR(float a, float d, float s, float r);
    
};

#endif /* defined(__karplusInstrument__KarplusVoicer__) */
