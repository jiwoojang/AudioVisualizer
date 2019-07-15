// Main file where all the magic happens

#include "AudioVis.h"
#include "AudioObject.h"
#include "Visualizer.h"

using namespace std;

int main(int argc, char *argv[])
{
	string wavPath;

	cout << "Enter the name of the .wav file in the Resources folder to visualize." << endl;
	cout << "Please include the.wav extension" << endl;
	cin  >> wavPath;

	AudioObject audio("Resources/" + wavPath, 16384);

	if (audio.Init())
	{
		audio.PlaySound();
	}
	else
	{
		cout << "Error: Could not read audio file" << endl;
		return 0;
	}
	
#ifdef DEBUG_DRAW
	RenderWindow window(VideoMode(900, 900, 32), "Window");

	Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event)) {}

		audio.Update();

		window.clear();
		window.draw(audio.drawingPoints);
		window.display();
	}
#else
	// Make the OpenGL renderer
	Visualizer visualizer(1024, 768);

	if (!visualizer.Init())
	{
		cout << "Error opening OpenGL renderer" << endl;
		return 0;
	}

	// Just play audio if not debugging
	while (audio.IsPlaying())
	{
		visualizer.Update();
	}
#endif // DEBUG_DRAW
	return 0;
}
