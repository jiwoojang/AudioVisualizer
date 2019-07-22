#pragma once

#define _USE_MATH_DEFINES
#include "AudioVis.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <valarray>
#include <complex>

using namespace std;
using namespace sf;

typedef complex<double>			complexVal;
typedef valarray<complexVal>	complexArray;

//==============================================================
// A class to wrap all the DPS and FFT processes for a .wav file
//==============================================================
class AudioObject 
{
public:

	AudioObject(string const& path, int const& bufferSize);
	~AudioObject() {};

	bool Init();
	void Update();
	void PlaySound();
	bool IsPlaying();

	const vector<float>& GetOutputBuckets() const { return outputBuckets; }

#ifdef DEBUG_DRAW
	VertexArray			drawingPoints;
#endif // DEBUG_DRAW

private:

	void ConstructWindow();
	void CollectSamples();
	void fft(complexArray& data);

	//--------------------------------------------------------------
	// Media management courtesy of SFML
	//--------------------------------------------------------------
	Sound		sound;
	SoundBuffer buffer;
	string		filePath;

	//--------------------------------------------------------------
	// For FFT and windowing functions
	//--------------------------------------------------------------
	vector<float>		windowCache;
	vector<complexVal>	samples;
	complexArray		data;

	//--------------------------------------------------------------
	// For post FFT processing
	//--------------------------------------------------------------
	vector<float>	outputBuckets;
	float			maxSampleIndex { 20000.0f };
	double			rawBucketMultiplier { 1.05f };
	int				rawBucketsPerOutput { 0 };

	int sampleRate; 
	int sampleCount;
	int sampleBufferSize;
	int frameNumber { 0 };
};