
#ifndef GXSOUND_H
#define GXSOUND_H

#include <soloud.h>
#include "gxchannel.h"

class gxAudio;
typedef SoLoud::AudioSource AUDIO_SOURCE;

class gxSound {
public:
	gxAudio* audio;

	gxSound(gxAudio* audio, AUDIO_SOURCE* sample);
	~gxSound();

private:
	bool defs_valid;
	float def_vol, def_pan;
	int def_freq, def_pri;
	AUDIO_SOURCE* sample;
	float pos[3], vel[3];
	bool looping;

	void setDefaults();

	/***** GX INTERFACE *****/
public:
	//actions
	gxChannel* play();
	gxChannel* play3d(const float pos[3], const float vel[3]);

	//modifiers
	void setLoop(bool loop);
	void setPitch(int hertz);
	void setVolume(float volume);
	void setPan(float pan);
};

extern SoLoud::Soloud gSoloud; // SoLoud engine

#endif
