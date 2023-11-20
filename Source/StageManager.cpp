#include "StageManager.h"

void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

void StageManager::Render(RenderContext& rc, ModelShader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(rc,shader);
    }
}

void StageManager::Register(Stage*stage)
{
    stages.emplace_back(stage);
}

void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
    }
    stages.clear();
}

bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)
{
    bool result = false;
    Hit.distance = FLT_MAX;
    for (Stage* stage : stages)
    {
        HitResult hit;
        if (stage->RayCast(start, end, hit))
        {
            if (Hit.distance > hit.distance)
            {
               
                Hit = hit;
                result = true;
                return result;
            }
        }

    }

    
    return result;
}
