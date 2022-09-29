#include "ResourceManager.h"

string ResourceManager::filePath("Resources.csv");
ResourceManager::ResourceManager()
{
	LadAll();
}

ResourceManager::~ResourceManager()
{
	Release();
}

bool ResourceManager::LadAll()
{
	Release();
	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));

	auto ids = doc.GetColumn<string>(0);
	auto types = doc.GetColumn<int>(1);

	for (int i = 0; i < doc.GetRowCount(); i++)
	{
		if (!Load((ResourcesTypes)types[i], ids[i]))
			return false;
	}
	return true;
}

bool ResourceManager::LoadTexture(string id)
{
	if (texMap.find(id) != texMap.end())
		return false;

	Texture* texture = new Texture();

	if (!(texture->loadFromFile(id)))
	{
		delete texture;
		return false;
	}
	texMap[id] = texture;
	return true;
}

bool ResourceManager::LoadFont(string id)
{
	if (fontMap.find(id) != fontMap.end())
		return false;

	Font* font = new Font();

	if (!(font->loadFromFile(id)))
	{
		delete font;
		return false;
	}
	fontMap[id] = font;
	return true;
}

bool ResourceManager::LoadSoundBuffer(string id)
{
	if (soundMap.find(id) != soundMap.end())
		return false;

	SoundBuffer* sound = new SoundBuffer();

	if (!(sound->loadFromFile(id)))
	{
		delete sound;
		return false;
	}
	soundMap[id] = sound;
	return true;
}

bool ResourceManager::Load(ResourcesTypes type, string id)
{
	switch (type)
	{
	case ResourcesTypes::Texture:
		return LoadTexture(id);
		break;
	case ResourcesTypes::Font:
		return LoadFont(id);
		break;
	case ResourcesTypes::SoundBuffer:
		return LoadSoundBuffer(id);
		break;
	default:
		break;
	}
	//Error 여기다가 찍기
	return false;
}

Texture* ResourceManager::GetTexture(string id)
{
	auto it = texMap.find(id);
	if (it == texMap.end())
		return nullptr;
	else
		return it->second;
}
void ResourceManager::Release()
{
	for (auto it : texMap)
	{
		delete it.second;
	}
	texMap.clear();
	for (auto it : fontMap)
	{
		delete it.second;
	}
	fontMap.clear();
	for (auto it : soundMap)
	{
		delete it.second;
	}
	soundMap.clear();
}

Font* ResourceManager::GetFont(string id)
{
	auto it = fontMap.find(id);
	if (it == fontMap.end())
		return nullptr;
	else
		return it->second;
}

SoundBuffer* ResourceManager::GetSoundBuffer(string id)
{
	auto it = soundMap.find(id);
	if (it == soundMap.end())
		return nullptr;
	else
		return it->second;
}
