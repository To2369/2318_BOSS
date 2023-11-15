#pragma once
#include"Graphics/Model.h"
#include"projectile.h"
class ProjectileStraight :public Projectile
{
public:
    ProjectileStraight(ProjectileManager*manager);
    ~ProjectileStraight()override;
    //XVˆ—
    void Update(float elapsedTime);
    //•`‰æˆ—
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //”­Ë
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTime = 3.0f;
};