#include "EffectLog.h"

const Vector2f EffectLog::Gravity = Vector2f(0.f, 2000.f);

EffectLog::EffectLog(Texture& tex, float duration)
	:SpriteGameObject(tex), duration(duration)
{
	SetOrigin(Origins::MC);
}

EffectLog::~EffectLog()
{
}

void EffectLog::Fire(const Vector2f v, float av)
{
	active = true;
	timer = 0;
	velocity = v;
	angularVelocity = av;
}

void EffectLog::Init()
{
	SpriteGameObject::Init();
	SetOrigin(Origins::MC);
}

void EffectLog::Release()
{
	SpriteGameObject::Release();
}

void EffectLog::Update(float dt)
{
	SpriteGameObject::Update(dt);

	timer += dt;
	if (timer > duration)
	{
		active = false;
	}

	float angle = sprite.getRotation();
	angle += angularVelocity * dt;
	sprite.setRotation(angle);

	velocity += Gravity * dt;
	Vector2f delta = velocity * dt;
	Translate(delta);
}

void EffectLog::Draw(RenderWindow& window)
{
	SpriteGameObject::Draw(window);
}

bool EffectLog::GetActive()
{
	return active;
}
