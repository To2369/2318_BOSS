#pragma once
#include<vector>
#include"enemy.h"
#include "set"
//enemy���Ǘ����邽�߂̃N���X
class EnemyManager
{
private:
    EnemyManager() {};
    ~EnemyManager() {};
public:
    //�B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }
    //�G�l�~�[�폜
    void Remove(Enemy* enemy);
    //�X�V����
    void Update(float elpsedTime);
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //�G�l�~�[�̓o�^
    void Register(Enemy* enemy);
    void Clear();
    void DrawDebugprimitive();
    //�G�l�~�[���擾
    int GetEnemyCount()const { return static_cast<int>(enemies.size()); }
    //�G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }
    void DrawDebugGUI();
private:
    //�G�l�~�[���m�̏Փ˔���
    void CollisionEnemyVsEnemies();
private:
    //�����̃G�l�~�[���Ǘ����邽�߁A�G�l�~�[�̃|�C���^�[��std::vector�ŊǗ�����
    std::vector<Enemy*> enemies;
    std::set<Enemy*>removes;
};
