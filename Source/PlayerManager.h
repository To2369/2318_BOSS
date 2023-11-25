#pragma once
#include "player.h"
#include<set>
#include <vector>
#include <DirectXMath.h>
#include"CameraController.h"

using namespace std;

class PlayerManager
{
public:
    static PlayerManager& Instance()
    {
        static PlayerManager ins;
        return ins;

    }
    void Update(float elapsedTime, CameraController cameraCotrol,FierdBuff&FB);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void clear();
    void Remove(player* pl);
    void Register(player* pl) { players.emplace_back(pl); }
    void DarwDebugPrimitive();
    void DrawDebugGUI();
    player* GetPlayer(int num) { return players.at(num); }


private:

    vector<player*>players;

    set<player*>remove;

};
