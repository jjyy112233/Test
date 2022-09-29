#include "Player.h"
#include "InputMgr.h"


Player::Player(Texture& playerTex, Texture& rip, Texture& axe)
	:SpriteGameObject(playerTex), addScore(100),
	texPlayer(playerTex), texRip(rip), originalPos(2), isAlive(true)
{
	this->axe.setTexture(axe);
	Utils::SetOrigin(this->axe, Origins::MR);

	Vector2f size = GetSize();
	axePos.x = -size.x * 0.5f + 85;
	axePos.y = -size.y * 0.5f + 55;

	pos = Sides::Right;		//처음시작 오른쪽
	SetPosition(originalPos[(int)pos]); //오른쪽
	SetOrigin(Origins::BC);	//오리진 변경

	texLog.loadFromFile("graphics/log.png");

	deathSoundBuffer.loadFromFile("sound/death.wav");
	deathSound.setBuffer(deathSoundBuffer);
	ChopSoundBuffer.loadFromFile("sound/chop.wav");
	ChopSound.setBuffer(ChopSoundBuffer);
	for (int i = 0; i < 10; ++i)
	{
		auto log = new EffectLog(texLog, 2.f);
		unuseLogs.push_back(log);
	}
}

void Player::Set(Vector2f center)
{
	centerPos = center;
}

void Player::Init()
{
	sprite.setTexture(texPlayer, true);		//true로 해야 로컬바운드가 갱신됨
	Utils::SetOrigin(sprite, Origins::BC); // 이미지를 바꾸면 오리진 바꿔주기
	SetFlipX(pos == Sides::Left);		 //왼쪽에 있으면 Flip

	Vector2f size = GetSize();			//이미지 변경하고 나서 사이즈 가져옥
	originalPos[(int)Sides::Left].x = centerPos.x - size.x*2.f;
	originalPos[(int)Sides::Left].y = centerPos.y;
	originalPos[(int)Sides::Right].x = centerPos.x + size.x*2.f;
	originalPos[(int)Sides::Right].y = centerPos.y;
	SetPosition(originalPos[(int)pos]);


	isAlive = true;		//살려주고
	isChopping = false;	 //안바꿔줘도 상관 없지만 Init에서는 다 해주기

	while (!useLogs.empty())
	{
		auto log = useLogs.back();
		useLogs.pop_back();
		unuseLogs.push_back(log);
	}
}

void Player::Release()
{ 
	for (auto log : unuseLogs)
	{
		delete log;
	}
	unuseLogs.clear();

	for (auto log : useLogs)
	{
		delete log;
	}
	useLogs.clear();
}

void Player::Update(float dt)
{
	SpriteGameObject::Update(dt);
	auto it = useLogs.begin();
	while (it != useLogs.end())
	{
		(*it)->Update(dt);
		if (!(*it)->GetActive())
		{
			unuseLogs.push_back(*it);
			it = useLogs.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (!isAlive || dt == 0)
		return;

	isChopping = InputMgr::GetKey(Keyboard::Left) || InputMgr::GetKey(Keyboard::Right);
}

void Player::Draw(RenderWindow& window)
{
	SpriteGameObject::Draw(window);

	if(isChopping && isAlive)
		window.draw(axe);

	for (auto log : useLogs)
	{
		log->Draw(window);
	}
}

void Player::SetPosition(Vector2f pos)
{
	SpriteGameObject::SetPosition(pos);
	Vector2f axePos2 = axePos;
	if (axe.getScale().x < 0)
	{
		axePos2.x *= -1;
	}

	axe.setPosition(pos + axePos2);
	
}

void Player::SetFlipX(bool flip)
{
	SpriteGameObject::SetFlipX(flip);
	
	Vector2f scale = axe.getScale();
	scale.x = flip ? -abs(scale.x) : abs(scale.x);
	axe.setScale(scale);
}

void Player::ShowLogEffect()
{
	if (unuseLogs.empty())
		return;

	auto log = unuseLogs.front();
	unuseLogs.pop_front();
	useLogs.push_back(log);
	Vector2f force;

	force.x = pos == Sides::Left ? 1500 : -1500;
	force.y = -1500;

	float aForce = pos == Sides::Left ? 360 * 5 : -360 * 5;

	Vector2f pos = centerPos;
	pos.y = axe.getPosition().y;

	log->SetPosition(pos);
	log->Fire(force, aForce);
}

int Player::GetScore()
{
	return addScore;
}

bool Player::GetAlive() const
{
	return isAlive;
}

Sides Player::GetPos() const
{
	return pos;
}

void Player::Die()
{
	deathSound.setVolume(50);
	deathSound.setPitch(5);
	deathSound.play();
	isAlive = false;		//상태를 죽은걸로 바꿔주고
	isChopping = false;		//도끼를 그려주지 않겠다

	sprite.setTexture(texRip, true);		//이미지를 비석으로 //true로 해야 로컬바운드가 갱신됨
	Utils:: SetOrigin(sprite,Origins::BC); // 이미지를 바꾸면 오리진 바꿔주기
	SetFlipX(false);

	//if (!useLogs.empty())
	//{
	//	auto last = useLogs.back();
	//	useLogs.pop_back();
	//	unuseLogs.push_back(last);
	//}
	
}

void Player::Chop(Sides side)
{
	ChopSound.play();
	pos = side;

	SetFlipX(side == Sides::Left ? true : false);
	SetPosition(originalPos[(int)pos]);
	ShowLogEffect();
}

bool Player::Chop(Sides side, Sides branch)
{
	pos = side;
	SetFlipX(side == Sides::Left ? true : false);
	SetPosition(originalPos[(int)pos]);
	ChopSound.play();
	if (side == branch)
	{
		Die();
		return false;
	}

	ShowLogEffect();
	return true;
}
