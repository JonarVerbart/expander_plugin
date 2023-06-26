//
// Created by Jonar Verbart on 26/06/2023.
//

#include "GainComputer.h"

GainComputer::GainComputer() {
    threshold = 0.0;
    ratio = 1.0;
    envDetector = new EnvelopeDetector();
}
GainComputer::~GainComputer() {}

void GainComputer::setThreshold(double newThreshold){
    if (threshold == newThreshold){
        return;
    } else {
        threshold = newThreshold;
    }
}

void GainComputer::setRatio(double newRatio){
    if (ratio == newRatio){
        return;
    } else {
        ratio = newRatio;
    }
}

void GainComputer::setAttackTime(double attackTime, double sampleRate){
    envDetector->setRCAttackTime(attackTime, sampleRate);
}

void GainComputer::setReleaseTime(double releaseTime, double sampleRate){
    envDetector->setRCReleaseTime(releaseTime, sampleRate);
}

void GainComputer::calcEnvelope(float inputValue) {
    detectorOutput = envDetector->getEnvelope(inputValue);
}

double GainComputer::getOutputLevel(){
    if (detectorOutput<= threshold){
        return detectorOutput;
    } else {
        return threshold + ((detectorOutput - threshold)/ratio);
    }
}

double GainComputer::getGain(float inputValue){
    calcEnvelope(inputValue);
    gaindB = getOutputLevel() - detectorOutput;
    linearGain = pow(10, gaindB/20);
    return linearGain;
}