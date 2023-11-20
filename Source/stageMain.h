#pragma once
#include "Graphics/Model.h"
#include "Stage.h"

class StageMain:public Stage
{
public:
    StageMain();
    ~StageMain();
    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);
    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)override;
   
private:
    Model* model = nullptr;

};