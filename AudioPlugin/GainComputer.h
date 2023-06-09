//
// Created by Jonar Verbart on 26/06/2023.
//

#ifndef EXPANDER_PLUGIN_GAINCOMPUTER_H
#define EXPANDER_PLUGIN_GAINCOMPUTER_H

#include "EnvelopeDetector.h"

class GainComputer {
public:
    GainComputer();
    ~GainComputer();
    void setThreshold(double newThreshold);
    void setRatio(double newRatio);
    void setAttackTime(double attackTime, double sampleRate);
    void setReleaseTime(double releaseTime, double sampleRate);
    void calcEnvelope(float inputValue);
    double getOutputLevel();
    double getGain(float inputValue);
    double gaindB;
    bool debugSliders;
private:
    double threshold;
    double ratio;
    double detectorOutput;
    double linearGain;
    double lastAttackTime;
    double lastReleaseTime;
    EnvelopeDetector* envDetector;
};

#endif //EXPANDER_PLUGIN_GAINCOMPUTER_H
