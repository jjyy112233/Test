#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include "rapidcsv.h"
#include "SingleTon.h"

using namespace std;
using namespace sf;
enum class ResourcesTypes
{
	Texture, Font, SoundBuffer
};
//ΩÃ±€≈Ê¿∏∑Œ ∫Ø∞Ê«œ±‚
class ResourceManager : public Singleton<ResourceManager>
{
private:
	map<string, Texture*> texMap;
	map<string, Font*> fontMap;
	map<string, SoundBuffer*> soundMap;

public:
	static string filePath;
	ResourceManager();
	~ResourceManager();
	
	bool LadAll();
	bool LoadTexture(string id);
	bool LoadFont(string id);
	bool LoadSoundBuffer(string id);
	bool Load(ResourcesTypes type, string id);
	void Release();
	Texture* GetTexture(string id);
	Font* GetFont(string id);
	SoundBuffer* GetSoundBuffer(string id);
};