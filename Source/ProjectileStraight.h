#pragma once
#include"Graphics/Model.h"
#include"projectile.h"
class ProjectileStraight :public Projectile
{
public:
    ProjectileStraight(ProjectileManager*manager);
    ~ProjectileStraight()override;
    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);
    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTime = 3.0f;
};