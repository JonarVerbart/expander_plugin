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
        std::cout<<"Threshold: ";
        std::cout<<threshold;
        std::cout<<"\n";
    }
}

void GainComputer::setRatio(double newRatio){
    if (ratio == newRatio){
        return;
    } else {
        ratio = newRatio;
        std::cout<<"Ratio: ";
        std::cout<<ratio;
        std::cout<<"\n";
    }
}

void GainComputer::setAttackTime(double attackTime, double sampleRate) {
    if (attackTime == lastAttackTime) {
        return;
    } else {
        envDetector->setRCAttackTime(attackTime, sampleRate);
        lastAttackTime = attackTime;
        std::cout<<"AttackTime: ";
        std::cout << attackTime;
        std::cout << "\n";
    }
}

void GainComputer::setReleaseTime(double releaseTime, double sampleRate) {
    if (releaseTime == lastReleaseTime) {
        return;
    } else {
        envDetector->setRCReleaseTime(releaseTime, sampleRate);
        lastReleaseTime = releaseTime;
        std::cout<<"ReleaseTime: ";
        std::cout << releaseTime;
        std::cout << "\n";
    }
}

void GainComputer::calcEnvelope(float inputValue) {
    detectorOutput = envDetector->getEnvelope(inputValue);
}

double GainComputer::getOutputLevel(){
    if (detectorOutput<= threshold){
        // std::cout << detectorOutput;
        // std::cout << "\n";
        return detectorOutput;
    } else {
        // std::cout << threshold + ((detectorOutput - threshold)/ratio);
        // std::cout << "\n";
        return threshold + ((detectorOutput - threshold)/ratio);
    }
}

double GainComputer::getGain(float inputValue){
    calcEnvelope(inputValue);
    gaindB = getOutputLevel() - detectorOutput;
    linearGain = pow(10, gaindB/20);
    return linearGain;
}