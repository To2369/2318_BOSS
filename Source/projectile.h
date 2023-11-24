#pragma once
#include "Graphics/Shader.h"
#include"Graphics/Model.h"
//前方宣言
class ProjectileManager;


//弾丸
class Projectile
{
public:
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {};

    //更新処理
    virtual void Update(float elpsedTime) = 0;

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //デバッグプリミティブ描画
    virtual void DrawdebugPrimitive();

    //位置情報取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; };

    //方向取得
    const DirectX::XMFLOAT3& GetDirection()const { return direction; };

    //スケール取得
    const DirectX::XMFLOAT3& GetScale()const { return scale; };

    //半径取得
    float GetRadius()const { return radius; }

    void SetAdjustPos(float posZ) { adjustPos = posZ; }
    float GetAdjustPos() { return adjustPos; }//弾の当たり判定の位置

    //破棄
    void Destroy();
protected:
    //行列更新処理
    void UpdateTransform();
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };//位置
    DirectX::XMFLOAT3 direction = { 0,0,1 };//向き
    DirectX::XMFLOAT3 scale = { 1,1,1 };//拡縮
    DirectX::XMFLOAT4X4 transform =
    { 1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1 };
    float adjustPos = 1.433;
    float radius = 0.1f;
    Model* model = nullptr;
    ProjectileManager* mgr = nullptr;
};