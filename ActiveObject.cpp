#include "ActiveObject.h"

ActiveObject::ActiveObject(Texture& tex, Vector2f pos)
	:SpriteGameObject(tex, pos)
{
	//Init();
}

ActiveObject::~ActiveObject()
{
	//Release();
}

void ActiveObject::Init()
{
	speed = (rand() % (speedRange.y-speedRange.x)) + speedRange.x;
	//direction.x = -1;
	//startPos.x = 1919;
	//startPos.y = (rand() % 780) + 1;
	//endPos.x = -150;
	//endPos.y = startPos.y;
	startPos.y = ((rand() % (yRange.y = yRange.x)) + yRange.x);
	endPos.y = startPos.y;
	SetPosition(startPos);
}

void ActiveObject::Update(float dt)
{
	Vector2f pos = GetPosition();
	float distanceX = endPos.x - pos.x;
	if (abs(distanceX) < 10)
		Init();

	Translate(direction * speed * dt);
}

void ActiveObject::Set(Vector2i sRange, Vector2i yRange, Vector2f start, Vector2f end)
{
	speedRange = sRange;
	this->yRange = yRange;
	startPos = start;
	endPos = end;

	direction = (endPos.x > startPos.x) ? Vector2f(1, 0) : Vector2f(-1, 0);
}
