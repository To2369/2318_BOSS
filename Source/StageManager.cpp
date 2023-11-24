#include "StageManager.h"
#include <imgui.h>
#include"stageMain.h"
void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

void StageManager::Render(ID3D11DeviceContext* device, Shader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(device, shader);
    }
}

void StageManager::Register(Stage* stage)
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

void StageManager::GUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("StageTransform", nullptr, ImGuiWindowFlags_None))
    {
        Stage* stmain1 = stages.at(0);
       
        static DirectX::XMFLOAT3 m1 = { 0.072f,0.01f,0.05f };

        ImGui::SliderFloat3("StageScale", &m1.x, 0.01, 1);
        stmain1->SetScale(m1);
        
    }
    ImGui::End();
}
