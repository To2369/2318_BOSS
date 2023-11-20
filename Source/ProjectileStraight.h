#pragma once
#include"Graphics/Model.h"
#include"projectile.h"
class ProjectileStraight :public Projectile
{
public:
    ProjectileStraight(ProjectileManager*manager);
    ~ProjectileStraight()override;
    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);
    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTime = 3.0f;
};