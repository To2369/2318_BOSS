#include"TargetPoint.h"
#include"player.h"
#include"PlayerManager.h"
#include"collision.h"
//�R���X�g���N�^
TargetPoint::TargetPoint()
{
    model = new Model("Data/Model/Mato/Mato.mdl");
    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.08f;
    //�~���̕������ݒ�
    radius = 0.0f;
    height = 0.0f;
}

//�f�X�g���N�^
TargetPoint::~TargetPoint()
{
    delete model;
}

void TargetPoint::Update(float elapsedTime, FierdBuff& FB)
{
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    TargetPoint::MaterialVsArrow("Red", 2.0f);
    //���f���s��X�V
    model->UpdateTransform(transform);
}

//�`�揈��
void TargetPoint::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void TargetPoint::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
    DebugRenderer* debugRnderer = Graphics::Instance().GetDebugRenderer();
}

//�}�e���A���Ƌ|��̏Փ˔���
void TargetPoint::MaterialVsArrow(const char* materialname, float boneRadius)
{
   
}