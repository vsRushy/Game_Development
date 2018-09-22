#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "j1App.h"   // Error if not included ?

#define DEFAULT_MUSIC_FADE_TIME App->node.child("jAudio").attribute("default_music_fade_time").as_int()
#define MUSIC_VOLUME App->node.child("jAudio").attribute("music_volume").as_int()
#define EFFECTS_VOLUME App->node.child("jAudio").attribute("effects_volume").as_int()

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	_Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;
};

#endif // __j1AUDIO_H__