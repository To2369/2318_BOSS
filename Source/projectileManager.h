#pragma once

#include<vector>
#include"projectile.h"
#include<set>
class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    //�X�V����
    void Update(float elpsedTime);
    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);
    //�f�o�b�O�v���~�e�B�u�`��
    void DrawdebugPrimitive();
    //�e�ۓo�^
    void Register(Projectile* projectile);
    //�e�ۑS�폜
    void clear();
    //�e�ې��擾
    int GetProjectileCount()const { return static_cast<int>(Projectiles.size()); };
    //�e�ێ擾
    Projectile* GetProgectile(int index)
    {
        return Projectiles.at(index);
    }

    void Remove(Projectile* projectile);
private:
    std::set<Projectile*>remove;
    std::vector<Projectile*> Projectiles;
};
