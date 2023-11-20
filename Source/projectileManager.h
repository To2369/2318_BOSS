#pragma once

#include<vector>
#include"projectile.h"
#include<set>
class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    //XVˆ—
    void Update(float elpsedTime);
    //•`‰æˆ—
    void Render(RenderContext& rc, ModelShader* shader);
    //ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
    void DrawdebugPrimitive();
    //’eŠÛ“o˜^
    void Register(Projectile* projectile);
    //’eŠÛ‘Síœ
    void clear();
    //’eŠÛ”æ“¾
    int GetProjectileCount()const { return static_cast<int>(Projectiles.size()); };
    //’eŠÛæ“¾
    Projectile* GetProgectile(int index)
    {
        return Projectiles.at(index);
    }

    void Remove(Projectile* projectile);
private:
    std::set<Projectile*>remove;
    std::vector<Projectile*> Projectiles;
};
