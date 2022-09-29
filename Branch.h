#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteGameObject.h"

enum class Sides
{
    Left,
    Right,
    None,
};

class Branch : public SpriteGameObject
{
private:
    SpriteGameObject* tree;
    Sides side;

public:
    Branch(Texture& tex, SpriteGameObject* tree = nullptr);
    virtual void Init() override;
    void SetSide(Sides side);
    Sides GetSide();

};
