#pragma once
#include <vector>
#include "Stage.h"

class StageManager
{
private:
    StageManager() {};
    ~StageManager() {};
public:
    static StageManager& Instance()
    {
        static StageManager instance_;
        return instance_;
    }

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext*device,Shader*shader);
    void Register(Stage*stage);
    void Clear();
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit);
private:
    std::vector<Stage*>stages;


};
