#include"EnemyManager.h"
#include"Graphics/Graphics.h"
#include"collision.h"
#include"enemySlaime.h"

void EnemyManager::Remove(Enemy* enemy)
{
    removes.insert(enemy);
}
//更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
       
    }
    for (Enemy* enemy : removes)
    {
        //std::vectorから要素を削除する場合はイテレータで削除しなければならない
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }
        delete enemy;
    }

   // 破棄リストをクリア
    removes.clear();
    //敵同士の衝突判定
    CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(RenderContext& rc, ModelShader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(rc, shader);
    }
}

void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}
//エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
       delete  enemy;
       
    }
    enemies.clear();
}

void EnemyManager::DrawDebugprimitive()
{
    
    for (Enemy* enemy : enemies)
    {
        // debugRenderere->DrawSphere(enemy->GetPosition(), enemy->getRadius(), { 0,0,0,1 });
        enemy->DrawDebugPrimitive();
    }
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    EnemyManager& enemyManager=Instance();
    EnemyManager& enemyManager2 = Instance();

    int enemycount = enemyManager.GetEnemyCount();
    int enemycount2 = enemyManager2.GetEnemyCount();
    for (int i = 0; i < enemycount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        for (int j = i+1; j < enemycount2; j++)
        {
            DirectX::XMFLOAT3 outPosition;
            Enemy* enemy2 = enemyManager2.GetEnemy(j);
            if (Collision::IntersectCylinderVsCylinder(
                enemy->GetPosition(),
                enemy->getRadius(),
                enemy->GetHeight(),
                enemy2->GetPosition(),
                enemy2->getRadius(), 
                enemy2->GetHeight(),
                outPosition))
            {
                enemy2->SetPosition(outPosition);
            }
        }
    }
         
   
}

void EnemyManager::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Enmey", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))

        {
            
            for (int i = 0; i < EnemyManager::GetEnemyCount(); i++)
            {

                EnemyManager& enemy = EnemyManager::Instance();
                //位置
                DirectX::XMFLOAT3 E = enemy.GetEnemy(i)->GetPosition();
                float H = enemy.GetEnemy(i)->GetHealth();
                ImGui::InputFloat3("Position",&E.x);
                ImGui::InputFloat("HP", &H);
                float timme = enemy.GetEnemy(i)->GetTimer();
                ImGui::InputFloat("timme", &timme);
              
            }
        }
    }
    ImGui::End();
}