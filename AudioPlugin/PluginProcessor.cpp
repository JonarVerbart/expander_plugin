#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameters())
{
    expander = new GainComputer;
    usrGaindB = *apvts.getRawParameterValue("gain");
    usrGainLin = dBToLin(usrGaindB);
    expander->setReleaseTime(100.0, getSampleRate());
    expander->setAttackTime(100.0, getSampleRate());
/*
    addParameter (gain = new juce::AudioParameterFloat ("gain", // parameterID
                                                        "Gain", // parameter name
                                                        0.0f,   // minimum value
                                                        1.0f,   // maximum value
                                                        0.5f)); // default value
                                                        */
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need.
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    usrGaindB = *apvts.getRawParameterValue("gain");
    if (usrGaindB != lastUsrGaindB) {
        usrGainLin = dBToLin(usrGaindB);
        lastUsrGaindB = usrGaindB;
        std::cout<<"usrGainLin: ";
        std::cout<<usrGainLin;
        std::cout<<"\n";
    }
    expander->setThreshold(*apvts.getRawParameterValue("threshold"));
    expander->setRatio(*apvts.getRawParameterValue("ratio"));
    expander->setAttackTime(*apvts.getRawParameterValue("attack"), getSampleRate());
    expander->setReleaseTime(*apvts.getRawParameterValue("release"), getSampleRate());


    //float usrGain = *apvts.getRawParameterValue("gain");

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        // ...do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            /*
            double currGain = expander->getGain(buffer.getSample(channel, sample));
            std::cout<<currGain;
            std::cout<<"\n";

            std::cout<<*apvts.getRawParameterValue("gain");
            std::cout<<"\n";

             */
            channelData[sample] = buffer.getSample(channel, sample) *
                    expander->getGain(buffer.getSample(channel, sample)) * usrGainLin;
        }
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}


//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);

    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (AudioPluginAudioProcessor::getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));

    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID{"gain",5},"Gain", -96.0f, 0.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID{"threshold",1},"Threshold", -96.0f, 0.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID{"ratio",2},"Ratio", 1.0f, 10.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID{"attack",3},"Attack", 0.1f, 500.0f, 100.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID{"release",4},"Release", 0.1f, 500.0f, 100.0f));
    return {parameters.begin(), parameters.end()};
}

float AudioPluginAudioProcessor::dBToLin(float valueIndB) {
    return pow(10.0f, valueIndB / 20);
}

/*

void getStateInformation (juce::MemoryBlock& destData) override
{
auto state = parameters.copyState();
std::unique_ptr<juce::XmlElement> xml (state.createXml());
copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes) override
{
std::unique_ptr<juce::XmlElement> xmlState (AudioPluginAudioProcessor::getXmlFromBinary (data, sizeInBytes));

if (xmlState.get() != nullptr)
if (xmlState->hasTagName (parameters.state.getType()))
parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

 */