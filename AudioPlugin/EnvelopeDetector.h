//
// Created by Jonar Verbart on 26/06/2023.
//

#ifndef EXPANDER_PLUGIN_ENVELOPEDETECTOR_H
#define EXPANDER_PLUGIN_ENVELOPEDETECTOR_H
#include "math.h"
#include "iostream"


class EnvelopeDetector {
public:
    EnvelopeDetector();
    ~EnvelopeDetector();

    double getEnvelope(float inputValue);
    void setRCAttackTime(double attack_ms, double sampleRate);
    void setRCReleaseTime(double release_ms, double sampleRate);

private:
    //double currEnvelope;
    double rcAttackTime;
    double rcReleaseTime;
    double lastEnvelope;
    const double TLD_AUDIO_ENVELOPE_ANALOG_TC = -0.99967234081320612357829304641019;
};


#endif //EXPANDER_PLUGIN_ENVELOPEDETECTOR_H
