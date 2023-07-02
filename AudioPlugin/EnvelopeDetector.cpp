//
// Created by Jonar Verbart on 26/06/2023.
//

#include "EnvelopeDetector.h"

EnvelopeDetector::EnvelopeDetector() {
    lastEnvelope = 0.0;
}

EnvelopeDetector::~EnvelopeDetector() {
}

// Calculates the envelope of the input from rc times, effectively creating a charging/discharging effect.
double EnvelopeDetector::getEnvelope(float inputValue){
    inputValue = fabs(inputValue);  // rectifying
    inputValue *= inputValue;             // Square for RMS detection
    double currEnvelope = 0.0;
    if(inputValue>lastEnvelope) {   // Use attack time if level is rising, release if declining
        currEnvelope = rcAttackTime * (lastEnvelope-inputValue) + inputValue;
    } else {
        currEnvelope = rcReleaseTime * (lastEnvelope-inputValue) + inputValue;
    }
    lastEnvelope = currEnvelope;
    currEnvelope = pow(currEnvelope, 0.5);  // squire root for RMS detection
    if(currEnvelope<=0){    // return in dB
        return -96.0;
    }
    return 20.0 * log10(currEnvelope);
}

// Calculates rcAttackTime based on a curve simulating an RC circuit.
void EnvelopeDetector::setRCAttackTime(double attack_ms, double sampleRate) {
    rcAttackTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attack_ms * sampleRate * 0.001));
}
void EnvelopeDetector::setRCReleaseTime(double release_ms, double sampleRate) {
    rcReleaseTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (release_ms * sampleRate * 0.001));
}