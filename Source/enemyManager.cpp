#include"EnemyManager.h"
#include"Graphics/Graphics.h"
#include"collision.h"
#include"enemySlaime.h"

void EnemyManager::Remove(Enemy* enemy)
{
    removes.insert(enemy);
}
//�X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
       
    }
    for (Enemy* enemy : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }
        delete enemy;
    }

   // �j�����X�g���N���A
    removes.clear();
    //�G���m�̏Փ˔���
    CollisionEnemyVsEnemies();
}

//�`�揈��
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
//�G�l�~�[�S�폜
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
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))

        {
            
            for (int i = 0; i < EnemyManager::GetEnemyCount(); i++)
            {

                EnemyManager& enemy = EnemyManager::Instance();
                //�ʒu
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