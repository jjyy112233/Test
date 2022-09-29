#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SpriteGameObject.h"
#include "Branch.h" 
#include "EffectLog.h"
#include <list>
//엥?1
using namespace sf;

class Player : public SpriteGameObject
{
protected:
	list<EffectLog*> unuseLogs;
	list<EffectLog*> useLogs;
	Texture& texPlayer;
	Texture& texRip;
	Texture texLog;
	SoundBuffer deathSoundBuffer;
	Sound deathSound;
	SoundBuffer ChopSoundBuffer;
	Sound ChopSound;
	Sprite axe;
	int addScore;
	bool isAlive;
	Sides pos;
	Vector2f centerPos;
	Vector2f axePos;
	vector<Vector2f> originalPos;

	bool isChopping;
public:
	Player(Texture& playerTex, Texture& rip, Texture& axe);
	void Set(Vector2f center);
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetPosition(Vector2f pos) override;
	virtual void SetFlipX(bool flip) override;

	void ShowLogEffect();
	int GetScore();
	bool GetAlive()const;
	Sides GetPos() const;

	void Die();
	void Chop(Sides side); // 움직임과 동시에 때리기도함
	bool Chop(Sides side, Sides branch); // 움직임과 동시에 때리기도함
};

