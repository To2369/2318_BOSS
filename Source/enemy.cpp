#include"enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive()
{

//    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
//    //�Փ˔���p�̃f�o�b�O����`��
//   // debugRendere->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
//     debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4{ 0,0,0,1 });
}