#pragma once
#include"Graphics/Shader.h"
#include"enemy.h"
class TargetPoint :public Enemy
{
public:
    TargetPoint();
    ~TargetPoint()override;
    //�X�V����
    void Update(float elpsedTime, FierdBuff& FB)override;
    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader)override;

    void DrawDebugPrimitive()override;

    //�}�e���A���Ƌ|��̏Փ˔���
    void MaterialVsArrow(const char* materialname, float boneRadius);
private:
    Model* model = nullptr;
   
};