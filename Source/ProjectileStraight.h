#pragma once

#include"projectile.h"
class ProjectileStraight :public Projectile
{
public:
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight()override;
    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);
    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float speed_);
private:

    float speed = 0.1f;
    float lifeTime = 3.0f;
};