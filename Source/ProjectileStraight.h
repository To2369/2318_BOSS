#pragma once

#include"projectile.h"
class ProjectileStraight :public Projectile
{
public:
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight()override;
    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float speed_);
private:

    float speed = 0.1f;
    float lifeTime = 3.0f;
};