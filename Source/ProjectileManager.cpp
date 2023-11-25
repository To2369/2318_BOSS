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

    //破棄処理
    //projectilesの範囲for文中でerase()すると不具合が発生してしまうため、
    //更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
    for (Projectile* projectile : remove)
    {
        //std::vectorから要素を削除する場合はイテレータで削除しなければならない
        std::vector<Projectile*>::iterator it = std::find(Projectiles.begin(), Projectiles.end(),
                                                                               projectile);
        if (it != Projectiles.end())
        {
            Projectiles.erase(it);
        }
        delete projectile;
    }
    //破棄リストをクリア
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

//弾丸削除
void ProjectileManager::Remove(Projectile* projectile)
{
    //破棄リストに追加
    remove.insert(projectile);
}
