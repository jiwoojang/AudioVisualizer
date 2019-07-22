#include "AudioObject.h"

AudioObject::AudioObject(string const& path, int const& bufferSize)
{
	filePath = path;
	sampleBufferSize = bufferSize;
}

bool AudioObject::Init()
{
	if (!buffer.loadFromFile(filePath))
	{
		cout << "Unable to load buffer" << endl;
		return false;
	}

	sound.setBuffer(buffer);

	sampleRate = buffer.getSampleRate() * buffer.getChannelCount();
	sampleCount = buffer.getSampleCount();
	
	if (sampleBufferSize > sampleCount)
	{
		sampleBufferSize = sampleCount;
	}

	ConstructWindow();

	samples.resize(sampleBufferSize);

	maxSampleIndex = min(sampleBufferSize / 2.f, 20000.f);

	rawBucketMultiplier = pow(10, log10(BUFFER_SIZE / 2) / (double)RAW_BUCKET_COUNT);

	rawBucketsPerOutput = RAW_BUCKET_COUNT / OUTPUT_BUCKET_COUNT;

	return true;
}

void AudioObject::PlaySound()
{
	// Resets the frame number counter as well
	frameNumber = 0;
	sound.play();
}

bool AudioObject::IsPlaying()
{
	return sound.getStatus() != SoundSource::Status::Stopped;
}

void AudioObject::ConstructWindow()
{
	for (int i = 0; i < sampleBufferSize; ++i)
	{
		windowCache.push_back(0.54 - 0.46*cos(2 * M_PI * i / (float)sampleBufferSize));
	}
}

void AudioObject::CollectSamples()
{
	frameNumber = sound.getPlayingOffset().asSeconds() * sampleRate;

	if (frameNumber + sampleBufferSize < sampleCount)
	{
		for (int i = 0; i < sampleBufferSize; ++i)
		{
			// We use a complex value, but the imaginary component is always zero
			samples[i] = complexVal(buffer.getSamples()[frameNumber + i] * windowCache[i], 0);
		}
	}
}

// Cooley-Turkey FFT 
// (in-place, breadth-first, decimation-in-frequency)
// Taken from rosettacode.org
// https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
void AudioObject::fft(complexArray& data)
{
	// DFT
	unsigned int N = data.size(), k = N, n;
	double thetaT = M_PI / N;
	complexVal phiT = complexVal(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				complexVal t = data[a] - data[b];
				data[a] += data[b];
				data[b] = t * T;
			}
			T *= phiT;
		}
	}

	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			complexVal t = data[a];
			data[a] = data[b];
			data[b] = t;
		}
	}

	// Normalize
	complexVal f = 1.0 / sqrt(N);
	for (unsigned int i = 0; i < N; i++)
		data[i] *= f;
}

void AudioObject::Update()
{
	// Collect samples for this frame
	CollectSamples();

	// Perform FFT on samples
	data = complexArray(samples.data(), sampleBufferSize);
	fft(data);

	// Clear and reserve the number of output buckets we will need 
	// Probably wont impact perf, but good habits don't hurt
	outputBuckets.clear();
	outputBuckets.reserve(OUTPUT_BUCKET_COUNT);

	int bucketCount = 1;
	double outputBucketAverage = 0;

	for (double i(1); i <= maxSampleIndex; i *= rawBucketMultiplier)
	{
		// Add every raw bucket value into the total average for the current output bucket
		outputBucketAverage += log10(abs(data[(int)i]));
		++bucketCount;

		// If we have counted enough raw buckets for a single output bucket
		if (bucketCount % rawBucketsPerOutput == 0)
		{
			// Average and push back
			outputBuckets.push_back(outputBucketAverage / rawBucketsPerOutput);

			// Reset counters
			outputBucketAverage = 0;
		}
	}

#ifdef DEBUG_DRAW
	drawingPoints.clear();

	drawingPoints.setPrimitiveType(Lines);
	Vector2f position(150, 500); 

	float max = 1;

	for (float i(1); i < maxSampleIndex; i *= 1.05)
	{
		Vector2f samplePosition(log(i) / log(maxSampleIndex), abs(data[(int)i]));
		float y = (-20 * log(samplePosition.y / max)) < 0 ? -20 * log(samplePosition.y / max) : 0;

		drawingPoints.append(Vertex(position + Vector2f(samplePosition.x * 500, y), Color::White));
		drawingPoints.append(Vertex(position + Vector2f(samplePosition.x * 500, 0), Color::White));
	}
#endif // DEBUG_DRAW
}
