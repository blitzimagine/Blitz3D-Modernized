
#include "std.h"
#include "gxsound.h"
#include "gxaudio.h"

SoLoud::Soloud gSoloud; // SoLoud engine

gxSound::gxSound(gxAudio* a, AUDIO_SOURCE* s) :
	audio(a), sample(s), defs_valid(true), looping(false), def_freq(-1), def_vol(-1), def_pan(-2) {
	//FSOUND_Sample_GetDefaults(sample, &def_freq, &def_vol, &def_pan, &def_pri);
}

gxSound::~gxSound() {
	delete sample;
	//FSOUND_Sample_Free(sample);
}

void gxSound::setDefaults() {
	if (!defs_valid) {
		//FSOUND_Sample_SetDefaults(sample, def_freq, def_vol, def_pan, def_pri);
		defs_valid = true;
	}
}

gxChannel* gxSound::play() {
	setDefaults();
	gxChannel* channel = audio->play(sample);
	channel->setLooping(looping);
	if (def_freq != -1)
		channel->setPitch(def_freq);
	if (def_vol != -1)
		channel->setVolume(def_vol);
	if (def_pan != -2)
		channel->setPan(def_pan);
	return channel;
}

gxChannel* gxSound::play3d(const float pos[3], const float vel[3]) {
	setDefaults();
	return audio->play3d(sample, pos, vel);
}

void gxSound::setLoop(bool loop) {
	looping = loop;
	//FSOUND_Sample_SetMode(sample, loop ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF);
}

void gxSound::setPitch(int hertz) {
	def_freq = hertz;
	defs_valid = false;
}

void gxSound::setVolume(float volume) {
	def_vol = volume;
	defs_valid = false;
}

void gxSound::setPan(float pan) {
	def_pan = pan;//((pan + 1.0f) * 127.5f);
	defs_valid = false;
}

