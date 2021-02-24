/*
   ==============================================================================

   PluckedNote.cpp
   Created: 23 Feb 2021 6:04:44pm
   Author:  csmit

   ==============================================================================
 */

#define _USE_MATH_DEFINES
#include "PluckedNote.h"
#include <iostream>
#include <math.h>
#include <ctime>

//=======================================================================
PluckedNote::PluckedNote()
{
  generateNote();
}
//=======================================================================
PluckedNote::~PluckedNote()
{}
//=======================================================================
void PluckedNote::generateNote()
{
  //=======================================================================
  // KARPLUS-STRONG ALGORITHM

  // parameter calculation
  float rho = exp(-1 / ((float)frequency * T60 / log(1000))) / (abs(cos(2 * M_PI * frequency / sampleRate)));

  wtSize = floor(sampleRate * T60);               // duration of simulation in samples

  float Nexact = (sampleRate / frequency) - 0.5f; // ideal number of samples in delay line
  float N = floor(Nexact);                        // truncated delay line length
  float P = Nexact - N;                           // fractional delay length
  float C = (1 - P) / (1 + P);                    // calculate allpass filter coefficient

  srand(time(NULL));                              //random seed
  float* v = new float[N+1];                      //initialize input vector

  // fill input vector with white noise
  for (int count = 0; count < N + 1; count++)
  {
    float randNum = ((rand() % 10001) / 5000.0f) - 1.0f;
    v[count] = randNum;
  }

  float x0;
  float x1 = 0;

  if (waveTable != nullptr)
  {
      delete[] waveTable;
  }
  waveTable = new float[wtSize];

  // dynamics filter loop
  for (int n = 0; n < (N + 1); n++)
  {
    x0 = (1 - dynParam) * v[(int) n] + (dynParam * x1);
    waveTable[n] = x0;
    x1 = x0;
  }

  float yp1 = 0;                                // initializing previous output of allpas filter
  float yp0;                                    // initializing current output of allpass filter

  // karplus-strong algorithm loop
  for (int n = (N + 1); n < wtSize; n++)
  {
    yp0 = C * (waveTable[int(n - N)] - yp1) + waveTable[int(n - N - 1)];
    waveTable[n] = (rho / 2) * (yp0 + yp1);
    yp1 = yp0;
  }

  std::cout << "rho:\t\t" << rho << '\n';
  std::cout << "wtSize:\t\t" << wtSize << '\n';
  std::cout << "Nexact:\t\t" << Nexact << '\n';
  std::cout << "N:\t\t" << N << '\n';
  std::cout << "P:\t\t" << P << '\n';
  std::cout << "C:\t\t" << C << '\n';
  std::cout << "yp1:\t\t" << yp1 << '\n';
  std::cout << "yp0:\t\t" << yp0 << '\n';
  std::cout << "x1:\t\t" << x1 << '\n';
  std::cout << "x0:\t\t" << x0 << '\n';
}
//=============================================================================
// PROCESS FUNCTION

float PluckedNote::process()
{
  float sample = waveTable[currentSampleIndex];

  currentSampleIndex++;
  currentSampleIndex %= wtSize;

  return sample;

}
//=============================================================================
// SETTER FUNCTIONS

void PluckedNote::setFrequency(float freq)
{
  frequency = freq;
}
void PluckedNote::setSampleRate(float SR)
{
  sampleRate = SR;
}
void PluckedNote::setNoteLength(float noteLength)
{
  T60 = noteLength;
}
