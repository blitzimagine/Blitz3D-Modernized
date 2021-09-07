
#include "std.h"
#include "gxaudio.h"

#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <soloud_openmpt.h>
#include <soloud_audiosource.h>

float gxAudio::roll = 1.0f;
float gxAudio::dopp = 1.0f;
float gxAudio::dist = 1.0f;

struct StaticChannel : public gxChannel {
    virtual void play() = 0;
};

struct SoundChannel : public gxChannel {
    SoundChannel() :channel(-1) {
    }
    void set(int n) {
        channel = n;
    }
    void stop() {
        gSoloud.stop(channel);
    }
    void setLooping(bool looping)
    {
        gSoloud.setLooping(channel, looping);
    }
    void setPaused(bool paused) {
        gSoloud.setPause(channel, paused);
    }
    void setPitch(int pitch) {
        gSoloud.setSamplerate(channel, pitch);
        //FSOUND_SetFrequency(channel, pitch);
    }
    void setVolume(float volume) {
        gSoloud.setVolume(channel, volume);// * 1.92f);
        //FSOUND_SetVolume(channel, (int)(volume * 255.0f));
    }
    void setPan(float pan) {
        gSoloud.setPan(channel, pan);
        //FSOUND_SetPan(channel, (int)((pan + 1) * 127.5f));
    }
    void set3d(const float pos[3], const float vel[3]) {
        gSoloud.set3dSourceParameters(channel, pos[0], pos[1], pos[2], vel[0], vel[1], vel[2]);
    }
    void set3dOptions(float roll, float dopp, float distFactor)
    {
        gSoloud.set3dSourceAttenuation(channel, SoLoud::AudioSource::ATTENUATION_MODELS::EXPONENTIAL_DISTANCE, roll);
        gSoloud.set3dSourceDopplerFactor(channel, dopp);

        float minDist = 1.0f;
        float maxDist = 1000000000.0f;
        gSoloud.set3dSourceMinMaxDistance(channel, minDist * distFactor, maxDist * distFactor);
    }
    bool isPlaying() {
        // TODO: This might be incorrect for checking if the sound is playing
        return gSoloud.isValidVoiceHandle(channel) && !gSoloud.getPause(channel);
    }
private:
    unsigned int channel;
};

/*struct CDChannel : public gxChannel {
    void play(int track, int mode) {
        stop();
        int cd_mode = FSOUND_CD_PLAYONCE;
        if (mode == gxAudio::CD_MODE_LOOP) cd_mode = FSOUND_CD_PLAYLOOPED;
        else if (mode == gxAudio::CD_MODE_ALL) cd_mode = FSOUND_CD_PLAYCONTINUOUS;
        FSOUND_CD_SetPlayMode(0, cd_mode);
        FSOUND_CD_Play(0, track);
    }
    void stop() {
        FSOUND_CD_Stop(0);
    }
    void setLooping(bool looping)
    {
        // Dummy Func
    }
    void setPaused(bool paused) {
        FSOUND_CD_SetPaused(0, paused);
    }
    void setPitch(int pitch) {
    }
    void setVolume(float volume) {
        FSOUND_CD_SetVolume(0, (int)(volume * 255.0f));
    }
    void setPan(float pan) {
    }
    void set3d(const float pos[3], const float vel[3]) {
    }
    bool isPlaying() {
        return true;
    }
};*/

struct StreamChannel : public StaticChannel {
    StreamChannel(AUDIO_SOURCE* s) :stream(s) {
        channel = gSoloud.play(*s);
        gSoloud.setProtectVoice(channel, true);
        //gSoloud.setInaudibleBehavior(channel, true, false);
    }
    ~StreamChannel() {
        delete stream;
        //FSOUND_Stream_Close(stream);
    }
    void play() {
        stop();
        channel = gSoloud.play(*stream);
        gSoloud.setProtectVoice(channel, true);
        //gSoloud.setInaudibleBehavior(channel, true, false);
    }
    void stop() {
        gSoloud.setProtectVoice(channel, false);
        // TODO: should this be gSoloud.stopAudioSource(*stream); instead?
        gSoloud.stop(channel);
        channel = -1;
    }
    void setLooping(bool looping)
    {
        gSoloud.setLooping(channel, looping);
    }
    void setPaused(bool paused) {
        gSoloud.setPause(channel, paused);
    }
    void setPitch(int pitch) {
        gSoloud.setSamplerate(channel, pitch);
        //FSOUND_SetFrequency(channel, pitch);
    }
    void setVolume(float volume) {
        gSoloud.setVolume(channel, volume);// * 1.92f);
        //FSOUND_SetVolume(channel, (int)(volume * 255.0f));
    }
    void setPan(float pan) {
        gSoloud.setPan(channel, pan);
        //FSOUND_SetPan(channel, (int)((pan + 1) * 127.5f));
    }
    void set3d(const float pos[3], const float vel[3]) {
    }
    void set3dOptions(float roll, float dopp, float dist) {}
    bool isPlaying() {
        // TODO: This might be incorrect for checking if the sound is playing
        return gSoloud.isValidVoiceHandle(channel) && !gSoloud.getPause(channel);
    }
private:
    AUDIO_SOURCE* stream;
    int channel;
};

struct MusicChannel : public StaticChannel {
    MusicChannel(AUDIO_SOURCE* m) :module(m) {
        play();
    }
    ~MusicChannel() {
        delete module;
        //FMUSIC_FreeSong(module);
    }
    void play() {
        channel = gSoloud.play(*module);

        gSoloud.setProtectVoice(channel, true);
        //gSoloud.setInaudibleBehavior(channel, true, false);
    }
    void stop() {
        gSoloud.setProtectVoice(channel, false);

        // TODO: should this be gSoloud.stopAudioSource(*module); instead?
        gSoloud.stop(channel);
    }
    void setLooping(bool looping)
    {
        // Soloud always loops OpenMPT modules
        //gSoloud.setLooping(channel, looping);
    }
    void setPaused(bool paused) {
        gSoloud.setPause(channel, paused);
    }
    void setPitch(int pitch) {
    }
    void setVolume(float volume) {
        gSoloud.setVolume(channel, volume * 2.5f);//1.72f);
        //FMUSIC_SetMasterVolume(module, (int)(volume * 255.0f));
    }
    void setPan(float pan) {
    }
    void set3d(const float pos[3], const float vel[3]) {
    }
    void set3dOptions(float roll, float dopp, float dist) {}
    bool isPlaying() {
        // TODO: This might be incorrect for checking if the sound is playing
        return gSoloud.isValidVoiceHandle(channel) && !gSoloud.getPause(channel);
    }
private:
    AUDIO_SOURCE* module;
    int channel;
};

static set<gxSound*> sound_set;
static vector<gxChannel*> channels;
static map<string, StaticChannel*> songs;
//static CDChannel* cdChannel;

static int next_chan;
static vector<SoundChannel*> soundChannels;

static gxChannel* allocSoundChannel(int n) {

    SoundChannel* chan = 0;
    for (int k = 0; k < (int)soundChannels.size(); ++k) {
        chan = soundChannels[next_chan];
        if (!chan) {
            chan = soundChannels[next_chan] = d_new SoundChannel();
            channels.push_back(chan);
        }
        else if (chan->isPlaying()) {
            chan = 0;
        }
        if (++next_chan == soundChannels.size()) next_chan = 0;
        if (chan) break;
    }

    if (!chan) {
        next_chan = soundChannels.size();
        soundChannels.resize(soundChannels.size() * 2);
        for (int k = next_chan; k < (int)soundChannels.size(); ++k) soundChannels[k] = 0;
        chan = soundChannels[next_chan++] = d_new SoundChannel();
        channels.push_back(chan);
    }

    chan->set(n);

    chan->setVolume(1.00f);

    // TODO: find a way to only call the following when the sound is 3d
    //chan->set3dOptions(gxAudio::roll, gxAudio::dopp, gxAudio::dist);
    //gSoloud.update3dAudio();
    return chan;
}

gxAudio::gxAudio(gxRuntime* r) :
    runtime(r) {
    next_chan = 0;
    soundChannels.resize(4096);
    for (int k = 0; k < 4096; ++k) soundChannels[k] = 0;

    //cdChannel = d_new CDChannel();
    //channels.push_back(cdChannel);
}

gxAudio::~gxAudio() {
    //free all channels
    for (; channels.size(); channels.pop_back()) delete channels.back();
    //free all sound_set
    while (sound_set.size()) freeSound(*sound_set.begin());
    soundChannels.clear();
    songs.clear();

    //FSOUND_Close();
}

gxChannel* gxAudio::play(AUDIO_SOURCE* sample) {

    SoLoud::handle n = gSoloud.play(*sample);
    return allocSoundChannel(n);
}

gxChannel* gxAudio::play3d(AUDIO_SOURCE* sample, const float pos[3], const float vel[3]) {

    SoLoud::handle n = gSoloud.play3d(*sample, pos[0], pos[1], pos[2], vel[0], vel[1], vel[2]);

    return allocSoundChannel(n);
}

void gxAudio::pause() {
}

void gxAudio::resume() {
}

gxSound* gxAudio::loadSound(const string& f, bool use3d) {

    SoLoud::Wav* sample = new SoLoud::Wav;
    if (sample->load(f.c_str()) != SoLoud::SO_NO_ERROR)
    {
        delete sample;
        return 0;
    }

    gxSound* sound = d_new gxSound(this, sample);
    sound_set.insert(sound);
    return sound;
}

gxSound* gxAudio::verifySound(gxSound* s) {
    return sound_set.count(s) ? s : 0;
}

void gxAudio::freeSound(gxSound* s) {
    if (sound_set.erase(s)) delete s;
}

void gxAudio::setPaused(bool paused) {
    gSoloud.setPauseAll(paused);
}

void gxAudio::setVolume(float volume) {
}

void gxAudio::set3dOptions(float roll, float dopp, float dist) {
    // save these for new channels
    gxAudio::roll = roll;
    gxAudio::dopp = dopp;
    gxAudio::dist = dist;

    // Update 3d options for all channels
    for (auto channel : channels)
    {
        channel->set3dOptions(roll, dopp, dist);
        //channel->setRolloffFactor(roll);
        //channel->setDopplerFactor(dopp);
        //channel->setDistanceFactor(dist);
    }
    gSoloud.update3dAudio();
}

void gxAudio::set3dListener(const float pos[3], const float vel[3], const float forward[3], const float up[3]) {
    gSoloud.set3dListenerParameters(pos[0], pos[1], pos[2], forward[0], forward[1], forward[2], up[0], up[1], up[2], vel[0], vel[1], vel[2]);
    gSoloud.update3dAudio();
}

gxChannel* gxAudio::playFile(const string& t, bool use_3d) {
    string f = tolower(t);
    StaticChannel* chan = 0;
    map<string, StaticChannel*>::iterator it = songs.find(f);
    if (it != songs.end()) {
        chan = it->second;
        chan->play();
        return chan;
    }
    else if (
        f.find(".raw") != string::npos ||
        f.find(".wav") != string::npos ||
        f.find(".mp2") != string::npos ||
        f.find(".mp3") != string::npos ||
        f.find(".ogg") != string::npos ||
        f.find(".wma") != string::npos ||
        f.find(".asf") != string::npos) {
        SoLoud::WavStream* stream = new SoLoud::WavStream;
        if (stream->load(f.c_str()) != SoLoud::SO_NO_ERROR)
        {
            delete stream;
            return 0;
        }
        chan = d_new StreamChannel(stream);
        chan->setLooping(true);
        chan->setVolume(1.00f);
    }
    else {
        SoLoud::Openmpt* module = new SoLoud::Openmpt;
        if (module->load(f.c_str()) != SoLoud::SO_NO_ERROR)
        {
            delete module;
            return 0;
        }
        chan = d_new MusicChannel(module);
        chan->setVolume(1.00f);

        // Soloud always loops OpenMPT modules
        //chan->setLooping(true);
    }
    channels.push_back(chan);
    songs[f] = chan;
    return chan;
}

//gxChannel* gxAudio::playCDTrack(int track, int mode) {
//	cdChannel->play(track, mode);
//	return cdChannel;
//}
