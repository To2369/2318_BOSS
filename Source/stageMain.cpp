#include "stageMain.h"

StageMain::StageMain()
{
    //�X�e�[�W���f���ǂݍ���
    model = new Model("Data/Model/ExampleStage/Stage.mdl");
}
StageMain::~StageMain()
{
    //�X�e�[�W���f����j��
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
