#pragma once
#include<vector>
#include"enemy.h"
#include "set"
//enemyを管理するためのクラス
class EnemyManager
{
private:
    EnemyManager() {};
    ~EnemyManager() {};
public:
    //唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }
    //エネミー削除
    void Remove(Enemy* enemy);
    //更新処理
    void Update(float elpsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //エネミーの登録
    void Register(Enemy* enemy);
    void Clear();
    void DrawDebugprimitive();
    //エネミー数取得
    int GetEnemyCount()const { return static_cast<int>(enemies.size()); }
    //エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }
    void DrawDebugGUI();
private:
    //エネミー同士の衝突判定
    void CollisionEnemyVsEnemies();
private:
    //複数のエネミーを管理するため、エネミーのポインターをstd::vectorで管理する
    std::vector<Enemy*> enemies;
    std::set<Enemy*>removes;
};
