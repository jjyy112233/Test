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

	pos = Sides::Right;		//ó������ ������
	SetPosition(originalPos[(int)pos]); //������
	SetOrigin(Origins::BC);	//������ ����

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
	sprite.setTexture(texPlayer, true);		//true�� �ؾ� ���ùٿ�尡 ���ŵ�
	Utils::SetOrigin(sprite, Origins::BC); // �̹����� �ٲٸ� ������ �ٲ��ֱ�
	SetFlipX(pos == Sides::Left);		 //���ʿ� ������ Flip

	Vector2f size = GetSize();			//�̹��� �����ϰ� ���� ������ ������
	originalPos[(int)Sides::Left].x = centerPos.x - size.x*2.f;
	originalPos[(int)Sides::Left].y = centerPos.y;
	originalPos[(int)Sides::Right].x = centerPos.x + size.x*2.f;
	originalPos[(int)Sides::Right].y = centerPos.y;
	SetPosition(originalPos[(int)pos]);


	isAlive = true;		//����ְ�
	isChopping = false;	 //�ȹٲ��൵ ��� ������ Init������ �� ���ֱ�

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
	isAlive = false;		//���¸� �����ɷ� �ٲ��ְ�
	isChopping = false;		//������ �׷����� �ʰڴ�

	sprite.setTexture(texRip, true);		//�̹����� ������ //true�� �ؾ� ���ùٿ�尡 ���ŵ�
	Utils:: SetOrigin(sprite,Origins::BC); // �̹����� �ٲٸ� ������ �ٲ��ֱ�
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
