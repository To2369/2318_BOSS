#pragma once

#include<vector>
#include"projectile.h"
#include<set>
class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    //更新処理
    void Update(float elpsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //デバッグプリミティブ描画
    void DrawdebugPrimitive();
    //弾丸登録
    void Register(Projectile* projectile);
    //弾丸全削除
    void clear();
    //弾丸数取得
    int GetProjectileCount()const { return static_cast<int>(Projectiles.size()); };
    //弾丸取得
    Projectile* GetProgectile(int index)
    {
        return Projectiles.at(index);
    }

    void Remove(Projectile* projectile);
private:
    std::set<Projectile*>remove;
    std::vector<Projectile*> Projectiles;
};
