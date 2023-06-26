//
// Created by Jonar Verbart on 26/06/2023.
//


#include "EnvelopeDetector.h"

EnvelopeDetector::EnvelopeDetector() {
}

EnvelopeDetector::~EnvelopeDetector() {

}

double EnvelopeDetector::getEnvelope(float inputValue){
    inputValue *= abs(inputValue);
    double currEnvelope = 0.0;
    if(inputValue>lastEnvelope) {
        currEnvelope = rcAttackTime*(lastEnvelope-inputValue) + inputValue;
    } else {
        currEnvelope = rcReleaseTime*(lastEnvelope-inputValue) + inputValue;
    }
    currEnvelope = pow(currEnvelope, 0.5);
    if(currEnvelope<=0){
        return -96.0;
    }
    return 20.0* log10(currEnvelope);
}

void EnvelopeDetector::setRCAttackTime(double attack_ms, double sampleRate) {
    rcAttackTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attack_ms * sampleRate * 0.001));
}
void EnvelopeDetector::setRCReleaseTime(double release_ms, double sampleRate) {
    rcReleaseTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (release_ms * sampleRate * 0.001));
}