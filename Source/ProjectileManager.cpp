#include "projectileManager.h"
#include"Graphics/Graphics.h"
#include"projectile.h"
ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    clear();
}

void ProjectileManager::Update(float elpsedTime)
{
    for (Projectile* tama : Projectiles)
    {
        tama->Update(elpsedTime);
    }

    //�j������
    //projectiles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Projectile* projectile : remove)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Projectile*>::iterator it = std::find(Projectiles.begin(), Projectiles.end(),
                                                                               projectile);
        if (it != Projectiles.end())
        {
            Projectiles.erase(it);
        }
        delete projectile;
    }
    //�j�����X�g���N���A
    remove.clear();

}

void ProjectileManager::Render(RenderContext&rc, ModelShader* shader)
{
    for (Projectile* tama : Projectiles)
    {
        tama->Render(rc, shader);
    }
}

void ProjectileManager::DrawdebugPrimitive(){
  //DebugRenderer* debugRenderere=Graphics::Instance().GetDebugRenderer();
    for (Projectile* tama : Projectiles)
    {

        tama->DrawdebugPrimitive();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    Projectiles.emplace_back(projectile);
}

void ProjectileManager::clear()
{
    for (Projectile* tama : Projectiles)
    {
        delete tama;
    }
}

//�e�ۍ폜
void ProjectileManager::Remove(Projectile* projectile)
{
    //�j�����X�g�ɒǉ�
    remove.insert(projectile);
}
