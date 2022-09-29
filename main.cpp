#include <vector>
#include <list>
#include <SFML/Audio.hpp>
#include "ResourceManager.h"
#include "ActiveObject.h"
#include "InputMgr.h"
#include "Branch.h"
#include "Player.h"

using namespace sf;
using namespace std;

void UpdateBranches(vector<Branch*>& branches, int& current, vector<Vector2f>& posArr)
{
    current = (current + 1) % branches.size();

    for (int i = 0; i < branches.size(); ++i)
    {
        int index = (current + i) % branches.size();
        branches[index]->SetPosition(posArr[i]);
        if (i == branches.size() - 1)
        {
            branches[index]->SetSide((Sides)Utils::Range(0, 2));
        }
    }
}

int main()
{
    //화면설정
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!", Style::None);
    Vector2u size = window.getSize();

    //타임아웃 사운드 로드
    Sound timeOutSound;
    timeOutSound.setBuffer(*ResourceManager::GetInstance()->
        GetSoundBuffer("sound/out_of_time.wav"));

    //배경, 나무설정
    vector<SpriteGameObject*> gameobjectlist;
    gameobjectlist.push_back(new SpriteGameObject(*ResourceManager::GetInstance()->GetTexture("graphics/background.png")));
    SpriteGameObject* tree = new SpriteGameObject(*ResourceManager::GetInstance()->GetTexture("graphics/tree.png"),
        Vector2f(size.x * 0.5f, 910));
    tree->SetOrigin(Origins::BC);
    gameobjectlist.push_back(tree);

    //구름, 벌 시작위치
    vector<float> startVector = { 2000, -300 };
    vector<float> endVector = { -300, 2000 };

    //구름 세개 먼저 로드. 나무, 벌 나뭇가지 뒤로 그려줌
    for (int i = 0; i < 3; ++i)
    {
        auto activeobj = new ActiveObject(*ResourceManager::GetInstance()->GetTexture("graphics/cloud.png"));
        auto index = Utils::Range(0, 2);
        activeobj->Set(
            { 200,400 }, { 100,400 },
            { startVector[index], 0 }, { endVector[index], 0 });
        gameobjectlist.push_back(activeobj);
    }

    //가지 여섯개
    vector<Branch*> branches(6);
    vector<Vector2f> branchPosArr(branches.size());
    //로드하고
    for (int i = 0; i < branches.size(); ++i)
    {
        branches[i] = new Branch(*ResourceManager::GetInstance()->GetTexture("graphics/branch.png"), tree);
        branches[i]->SetSide((Sides)Utils::Range(0, 2));
        gameobjectlist.push_back(branches[i]);
    }

    //스코어는 플레이어에서 증가 시키고, 플레이어 로드
    int score = 0;
    Player* player = new Player(
        *ResourceManager::GetInstance()->GetTexture("graphics/player.png")
        , *ResourceManager::GetInstance()->GetTexture("graphics/rip.png")
        , *ResourceManager::GetInstance()->GetTexture("graphics/axe.png"));
    gameobjectlist.push_back(player);
    player->Set(tree->GetPosition());

    //벌 로드하고(벌이 플레이어 앞으로)
    auto activeobj = new ActiveObject(*ResourceManager::GetInstance()->GetTexture("graphics/bee.png"));
    activeobj->Set({ 200,400 }, { 500,1000 }, { 2000,0 }, { -100,0 });
    gameobjectlist.push_back(activeobj);
    
    //가지를 제외하고 나머지 초기화
    for (auto go : gameobjectlist)
    {
        go->Init();
    }

    //가지의 위치(높이)를
    int currentBranch = 0;
    float x = branches[0]->GetPosition().x;
    float y = 800;
    float offset = branches[0]->GetSize().y;
    offset += 100;
    //균등하게 잡아주고
    for (int i = 0; i < branches.size(); ++i)
    {
        branchPosArr[i] = Vector2f(x, y);
        y -= offset;
        branches[i]->SetSide((Sides)Utils::Range(0, 2));
        branches[i]->SetPosition(branchPosArr[i]);
    }
    //가지가 밑으로 내려오게(UpdateBranches 에서 SetSide해줌)

    //플레이어 위치 알아내서 맨 밑에 가지는 플레이어 반대편으로
    Sides playerPos = player->GetPos();
    playerPos = playerPos == Sides::Left ? Sides::Right : Sides::Left;
    branches[currentBranch]->SetSide(playerPos);

    //UI OBJ CREATES
    Text messageText;
    messageText.setFont(*ResourceManager::GetInstance()->GetFont("fonts/KOMIKAP_.ttf"));
    messageText.setCharacterSize(75);
    messageText.setFillColor(Color::Magenta);
    messageText.setString("Press Enter To Start!");
    Utils::SetOrigin(messageText, Origins::MC);
    messageText.setPosition(size.x * 0.5f, size.y * 0.5f);

    Text scoreText;
    scoreText.setFont(*ResourceManager::GetInstance()->GetFont("fonts/KOMIKAP_.ttf"));
    scoreText.setCharacterSize(100);
    scoreText.setFillColor(Color::Yellow);
    scoreText.setPosition(20, 20);
    Utils::SetOrigin(scoreText, Origins::TL);

    Vector2f timerBarSize(400, 80);
    RectangleShape timerBar;
    timerBar.setSize(timerBarSize);
    timerBar.setFillColor(Color::Red);
    timerBar.setPosition(size.x * 0.5f - timerBarSize.x * 0.5f, size.y - 100);

    //게임시간
    float duration = 4.0f;
    float timer = duration;

    //게임 상태
    bool isPuase = true;
    bool isgameover = false;

    Clock clock;
    while (window.isOpen())
    {
        scoreText.setString("Score: " + to_string(score));
        Time dt = clock.restart();
        Event ev;
        InputMgr::ClearInput();
        while (window.pollEvent(ev))
        {
            InputMgr::UpdateInput(ev);
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
            continue;
        }
        //게임이 진행중이거나, 플레이어가 살아있을때 좌우 키 입력 받고
        if (!isPuase && player->GetAlive())
        {
            if (!InputMgr::GetKey(Keyboard::Right) && InputMgr::GetKeyDown(Keyboard::Left))
            {
                UpdateBranches(branches, currentBranch, branchPosArr);
                if (player->Chop(Sides::Left, branches[currentBranch]->GetSide()))
                {
                    timer += 0.3f;
                    timer = min(timer, 4.f);
                    score += player->GetScore();
                }
            }
            //왼쪽이 눌리지 않은 상태로 상태로 오른쪽으로 이동
            if (!InputMgr::GetKey(Keyboard::Left) && InputMgr::GetKeyDown(Keyboard::Right))
            {
                UpdateBranches(branches, currentBranch, branchPosArr);
                if (player->Chop(Sides::Right, branches[currentBranch]->GetSide()))
                {
                    timer += 0.3f;
                    timer = min(timer, 4.f);
                    score += player->GetScore();
                }
            }
        }

        //엔터키를 눌렀을때
        if (InputMgr::GetKeyDown(Keyboard::Return))
        {         
            if (isgameover) //타임아웃이면 UI출력 및 초기화
            {
                messageText.setString("Press Enter To Start!");
                Utils::SetOrigin(messageText, Origins::MC);
                isgameover = !isgameover;
                timer = duration;
                score = 0;
                for (auto go : gameobjectlist)
                {
                    go->Init();
                }
                //맨 밑에 가지 플레이어 반대편으로 이동
                Sides playerPos = player->GetPos();
                playerPos = playerPos == Sides::Left ? Sides::Right : Sides::Left;
                branches[currentBranch]->SetSide(playerPos);
            }
            else
            {
                isPuase = !isPuase;
            }
        }

        //isPause면 델타타임 0으로 (움직이는 오브젝트들 멈춤)
        float deltatime = isPuase ? 0.f : dt.asSeconds();

        // 타임바 업데이트
        timer -= deltatime;
        if (timer < 0.f || !player->GetAlive())
        {
            if (!isPuase)
            {
                timeOutSound.play(); 
            }
            timer = 0.f;
        }
        if (timer == 0.f) 
        {
            messageText.setString("Game Over!");
            Utils::SetOrigin(messageText, Origins::MC);
            isPuase = true;
            isgameover = true;
        }
        float normTime = timer / duration;
        float timerSizeX = timerBarSize.x * normTime;
        timerBar.setSize({ timerSizeX, timerBarSize.y});
        timerBar.setPosition(size.x * 0.5f - timerSizeX * 0.5f, size.y - 100);

        //Update
        for (auto go : gameobjectlist)
        {
            go->Update(deltatime);
        }
        //Draw
        window.clear();
        for (auto go : gameobjectlist)
        {
            go->Draw(window);
        }
        if (isPuase)
        {
            window.draw(messageText);
        }

        window.draw(scoreText);
        window.draw(timerBar);

        window.display();
    }

    for (auto go : gameobjectlist)
    {
        go->Release();
        delete go;
    }

    gameobjectlist.clear();
    return 0;
}