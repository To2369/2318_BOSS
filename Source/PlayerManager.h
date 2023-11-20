#pragma once
#include "player.h"
#include<set>
#include <vector>
using namespace std;
class PlayerManager
{
public:
    static PlayerManager& Instance()
    {
        static PlayerManager ins;
        return ins;

    }
    void Update(float elapsedTime);
    void Render(RenderContext&rc,ModelShader*shader);
    void clear();
    void Remove(player* pl);
    void Register(player *pl) { players.emplace_back(pl); }
    void DrawDebugGUI();
    player *GetPlayer(int num) { return players.at(num); }
private:
    vector<player*>players;
    set<player*>remove;

};
