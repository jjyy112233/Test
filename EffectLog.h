#pragma once
#include "SpriteGameObject.h"
class EffectLog : public SpriteGameObject
{
private:
	Vector2f velocity;
	float angularVelocity;
	float duration;
	float timer;

	bool active;

public:
	static const Vector2f Gravity; //public Static const는 대문자로 하는 

	EffectLog(Texture& tex, float duration);
	~EffectLog();

	void Fire(const Vector2f v, float av);
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	bool GetActive();
};

