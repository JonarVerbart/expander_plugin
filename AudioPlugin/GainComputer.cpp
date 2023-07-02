//
// Created by Jonar Verbart on 26/06/2023.
//

#include "GainComputer.h"

GainComputer::GainComputer() {
    threshold = 0.0;
    ratio = 1.0;
    envDetector = new EnvelopeDetector();
    debugSliders = false; // prints slider values for debugging
}

GainComputer::~GainComputer() {}

void GainComputer::setThreshold(double newThreshold){
    if (threshold == newThreshold){
        return;
    } else {
        threshold = newThreshold;
        if(debugSliders) {
            std::cout << "Threshold: ";
            std::cout << threshold;
            std::cout << "\n";
        }
    }
}

void GainComputer::setRatio(double newRatio){
    if (ratio == newRatio){
        return;
    } else {
        ratio = newRatio;
        if(debugSliders) {
            std::cout << "Ratio: ";
            std::cout << ratio;
            std::cout << "\n";
        }
    }
}

void GainComputer::setAttackTime(double attackTime, double sampleRate) {
    if (attackTime == lastAttackTime) {
        return;
    } else {
        envDetector->setRCAttackTime(attackTime, sampleRate);
        lastAttackTime = attackTime;
        if(debugSliders) {
            std::cout << "AttackTime: ";
            std::cout << attackTime;
            std::cout << "\n";
        }
    }
}

void GainComputer::setReleaseTime(double releaseTime, double sampleRate) {
    if (releaseTime == lastReleaseTime) {
        return;
    } else {
        envDetector->setRCReleaseTime(releaseTime, sampleRate);
        lastReleaseTime = releaseTime;
        if(debugSliders) {
            std::cout << "ReleaseTime: ";
            std::cout << releaseTime;
            std::cout << "\n";
        }
    }
}

void GainComputer::calcEnvelope(float inputValue) {
    detectorOutput = envDetector->getEnvelope(inputValue);
}

// If envelope detector output is above threshold, calculate output level from threshold and ratio
double GainComputer::getOutputLevel(){
    if (detectorOutput<= threshold){
        return detectorOutput;
    } else {
        // Output level calculation for hard-knee downward compressor/upward expander
        return threshold + ((detectorOutput - threshold)/ratio);
    }
}

// Calculate the gain to be applied to the input signal in linear values
double GainComputer::getGain(float inputValue){
    calcEnvelope(inputValue);
    gaindB = getOutputLevel() - detectorOutput;
    linearGain = pow(10, gaindB/20);
    return linearGain;
}