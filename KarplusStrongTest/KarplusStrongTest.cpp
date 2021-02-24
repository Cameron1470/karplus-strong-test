// KarplusStrongTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PluckedNote.h"
#include "src/MattsAudioTools.h"


int main()
{
    PluckedNote gNote;

    gNote.setFrequency(196.0f);
    gNote.setSampleRate(48000.0f);
    gNote.setNoteLength(2.0f);
    gNote.generateNote();

    float durationInSamples = 2.0f * 48000.0f;

    float* samples = new float[durationInSamples];

    for (int i = 0; i < durationInSamples - 1; i++)
    {
        samples[i] = gNote.process();
    }

    AudioPlayerOpenAL myAudioPlayer;
    myAudioPlayer.playAudioData(samples, durationInSamples, 1, 48000, 16);



    return 0;

}

