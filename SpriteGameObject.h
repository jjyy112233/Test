#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.h"

using namespace sf;
using namespace std;

class SpriteGameObject
{
protected:
	Sprite sprite;

	SpriteGameObject(const SpriteGameObject& ref);
	SpriteGameObject& operator=(const SpriteGameObject& ref);
public:
	SpriteGameObject(Texture& tex, Vector2f pos = Vector2f());
	virtual ~SpriteGameObject();

	virtual void Init();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void SetPosition(Vector2f pos);

	Vector2f GetPosition();
	Vector2f GetSize() const;
	void Translate(Vector2f delta);
	void SetOrigin(Origins orgin);

	virtual void SetFlipX(bool flip);
};

