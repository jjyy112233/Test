#include "InputMgr.h"
#include <iostream>
#include <algorithm>

vector<bool> InputMgr::downList(Keyboard::KeyCount, false); //개수, 초기화(false)
vector<bool> InputMgr::ingList(Keyboard::KeyCount, false);
vector<bool> InputMgr::upList(Keyboard::KeyCount, false);
vector<int> InputMgr::ingIdx;

void InputMgr::ClearInput()
{
    //fill(downList.begin(), downList.end(), false);
    //fill(upList.begin(), upList.end(), false);
    for (auto idx : ingIdx)
    {
        downList[idx] = false;
        upList[idx] = false;
    }
    ingIdx.clear();

}

void InputMgr::UpdateInput(Event& ev)
{
    switch (ev.type)
    {
    case Event::EventType::KeyPressed:
        if (ev.key.code != Keyboard::Unknown && !ingList[ev.key.code]) //처음에 눌렀을때
        {
            downList[ev.key.code] = true;  //다운 true    ->  다음프레임(while다시시작) 에서 clear로 false로 변경
            ingList[ev.key.code] = true;   //ing true
            ingIdx.push_back(ev.key.code);
        }
        break;
    case Event::EventType::KeyReleased:
        if (ev.key.code != Keyboard::Unknown)
        {
            ingList[ev.key.code] = false;
            upList[ev.key.code] = true;
            ingIdx.push_back(ev.key.code);
        }
        break;
    }
}

bool InputMgr::GetKeyDown(Keyboard::Key key)
{
    return downList[key];
}

bool InputMgr::GetKey(Keyboard::Key key)
{
    return ingList[key];
}

bool InputMgr::GetKeyUp(Keyboard::Key key)
{
    return upList[key];
}


/*
키 A를 누르면 처음 누를때 case Event::EventType::KeyPressed: 에 걸려서
downList 와 ingList에 들어가며,
main 에서 GetKeyDown 을 호출하면 downList에 들어있으니 TRUE가 반환된다.
main 에 while은 매 프레임마다 down과 ing를 초기화 해주니까.
A를 누르고 있는 동안 다음 프레임에서는 downList에 A가 들어있지 않으니
GetKeyDown을 호출해도 false가 나오며, GetKey를 호출하면 ingList에 들어있으니까 True가 반환.

A키를 누르지 않을시에는 case Event::EventType::KeyReleased:에 걸려서
upList에 A를 넣어주고, ingList에 A를 remove해준다.
그 이후에 GetKey를 호출하면 A가 없으니까 False가 반환되며,
GetKeyUp 을 호출하면 들어있으니까 True가 반환된다.
이후에 main에서 clear해주니까 키를 땐 이후에
GetKey와 GetKeyUp을 호출해도 아무것도 들어있지 않아서 false가 반환된다.

main의 while흐름이
키 입력 -> down,ing에 삽입 -> GetKeyDwon, GetKey 는 true , GetKeyUp은 false -> 다음프레임
-> down, up 클리어 -> dwon, up 은 false , GetKey는 true -> 다음프레임
... 키를 땐다 -> up삽입, ing에서 삭제 -> GetKeyUp은 true, GetKeyDown GetKey는 false -> 다음프레임 -> down, up 클리어
-> GetkeyDown, GetKey, GeyKeyUp 은 false
*/
