#include"enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

//デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{

//    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
//    //衝突判定用のデバッグ球を描画
//   // debugRendere->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
//     debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4{ 0,0,0,1 });
}