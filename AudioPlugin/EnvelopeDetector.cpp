//
// Created by Jonar Verbart on 26/06/2023.
//


#include "EnvelopeDetector.h"

EnvelopeDetector::EnvelopeDetector() {
    //rcAttackTime = 100.0;
    //rcReleaseTime = 100.0;
    lastEnvelope = 0.0;
}

EnvelopeDetector::~EnvelopeDetector() {

}

double EnvelopeDetector::getEnvelope(float inputValue){
    inputValue = fabs(inputValue);
    inputValue *= inputValue;
    // std::cout << inputValue;
    // std::cout << "\n";
    double currEnvelope = 0.0;
    if(inputValue>lastEnvelope) {
        currEnvelope = rcAttackTime * (lastEnvelope-inputValue) + inputValue;
    } else {
        currEnvelope = rcReleaseTime * (lastEnvelope-inputValue) + inputValue;
    }
    lastEnvelope = currEnvelope;

    currEnvelope = pow(currEnvelope, 0.5);
    // std::cout << currEnvelope;
    // std::cout << "\n";
    if(currEnvelope<=0){
        return -96.0;
    }
/*
    double currEnvelopedB = 20.0 * log10(currEnvelope);
    std::cout.precision(4);
    std::cout<<currEnvelopedB;
    std::cout << "\n";
*/
    return 20.0 * log10(currEnvelope);
}

void EnvelopeDetector::setRCAttackTime(double attack_ms, double sampleRate) {
    rcAttackTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attack_ms * sampleRate * 0.001));
}
void EnvelopeDetector::setRCReleaseTime(double release_ms, double sampleRate) {
    rcReleaseTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (release_ms * sampleRate * 0.001));
}