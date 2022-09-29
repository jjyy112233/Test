#pragma once
#include "SpriteGameObject.h"

class ActiveObject : public SpriteGameObject
{
protected:
	float speed;
	Vector2i speedRange;
	Vector2i yRange;

	Vector2f direction;

	Vector2f startPos;
	Vector2f endPos;

public:
	ActiveObject(Texture& tex, Vector2f pos = Vector2f());
	virtual ~ActiveObject();

	virtual void Init();
	virtual void Update(float dt);

	void Set(Vector2i sRange, Vector2i yRange, Vector2f start, Vector2f end);
};