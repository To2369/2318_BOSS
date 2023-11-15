#include "stageMain.h"

StageMain::StageMain()
{
    //ステージモデル読み込み
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
    
}
StageMain::~StageMain()
{
    //ステージモデルを破壊
    delete model;
}

void StageMain::Update(float elapsedTime)
{

}
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc,model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)
{
    return Collision::InstersectRayVsModel(start,end,model,Hit);
}
