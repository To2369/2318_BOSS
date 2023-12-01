#include "stageMain.h"

StageMain::StageMain()
{
    //ステージモデル読み込み
    model = new Model("Data/Model/ExampleStage/Stage.mdl");
    scale.x = 0.072f;
    scale.y = 0.01f;
    scale.z = 0.05f;
}
StageMain::~StageMain()
{
    //ステージモデルを破壊
    delete model;
}

void StageMain::Update(float elapsedTime)
{

    UpdateTransform();
    model->UpdateTransform(transform);
}
void StageMain::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc,model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)
{
    return Collision::InstersectRayVsModel(start, end, model, Hit);
}
