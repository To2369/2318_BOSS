#include "PlayerManager.h"
#include"Graphics/Graphics.h"
#include"collision.h"
#include"player.h"
#include"parameter.h"
#include"FierdBuff.h"
void PlayerManager::Update(float elapsedTime, CameraController cameraCotrol,FierdBuff&FB)
{
    for (player* Player : players)
    {
        Player->Update(elapsedTime, cameraCotrol,FB);
    }
    for (player* pl : remove)
    {
        //std::vectorから要素を削除する場合はイテレータで削除しなければならない
        std::vector<player*>::iterator it = std::find(players.begin(), players.end(),
            pl);
        if (it != players.end())
        {
            players.erase(it);
        }
        delete pl;
    }

    // 破棄リストをクリア
    remove.clear();
    //敵同士の衝突判定
    //CollisionEnemyVsEnemies();
}

void PlayerManager::Render(RenderContext& rc,ModelShader* shader)
{
    for (player* pl : players)
    {
        pl->Render(rc, shader);
    }
}

void PlayerManager::clear()
{
    for (player* pl : players)
    {
        delete pl;
    }
    players.clear();
}

void PlayerManager::Remove(player* pl)
{
    remove.insert(pl);
}

void PlayerManager::DarwDebugPrimitive()
{
    for (player* pl : players)
    {
        pl->DarwDebugPrimitive();
    }
}

void PlayerManager::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("player", nullptr, ImGuiWindowFlags_None))
    {

        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (player* pl : players)
            {
                DirectX::XMFLOAT3 plPos = pl->GetPosition();
                /* static bool flag = false;
                 if (ImGui::Checkbox(u8"ワンショットアニメーション処理&アニメーション処理", &flag))
                 {
                     pl->GetCharacterModel()->SetOneShotMove(flag);
                     pl->GetCharacterModel()->Setanime_clip(flag);
                 };
                 float animetime = pl->GetCharacterModel()->GetAnimationTime();
                 if (ImGui::CollapsingHeader("animation_kanren", ImGuiTreeNodeFlags_DefaultOpen))
                 {
                     ImGui::InputFloat("Animetion_Freame", &animetime);
                     float ab = pl->GetCharacterModel()->GetanimetionBlendtimer();
                     ImGui::InputFloat("AnimetionBlend_Freame", &ab);
                 }*/
                ImGui::InputFloat3("player_Position" ,&plPos.x);
                if (ImGui::CollapsingHeader("changeFPSorTPS", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    if (ImGui::Button("TPS"))
                    {
                        pl->Set_TPPorFPS_Flag(true);
                    }
                    if (ImGui::Button("FPS"))
                    {
                        pl->Set_TPPorFPS_Flag(false);
                    }
                    //ラムダ関数
                    {
                        auto p = [](bool TorF)
                        {
                            switch (TorF)
                            {
                            case true:
                                return "TPS";
                                break;
                            case false:
                                return "FPS";
                                break;
                            }
                        };
                        std::string flagcheck = p(pl->GetPerspectiveChangeFlag().TPS);
                        ImGui::Text("PersPective:%s", flagcheck.c_str());
                    }

                    ImGui::Text("%s", pl->GetStringState(pl->GetState()).c_str());
                    static float ajust = 0;

                    ImGui::SliderFloat("hanteiPos", &ajust, 0.0f, 5.0f);

                    static float pr_Speed = 0;
                    ImGui::SliderFloat("tama_Speed",&pr_Speed,0.0f,5.0f);
                    pl->SetProjectileSpeed(pr_Speed);
                     
                    

                }
                if (ImGui::CollapsingHeader("MaterialNum", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    player* pl = players.at(0);
                    bool clearflag = false;
                    for (int i = 0; i < pl->GetMaterialCount(); i++)
                    {
                        int material = pl->GetMaterialNum(i);
                        ImGui::InputInt("MaterialNum", &material);
                    }
                    if (ImGui::Button("materialNumClear"))
                    {
                        clearflag = true;
                        pl->DebugMaterialClear(clearflag);
                    }
                }
             
                if (ImGui::CollapsingHeader("CircleParam", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    float radius = pl->GetCircleRadius();
                    int randP = pl->GetRand();
                    static float sp_radius = 0.01f;
                    DirectX::XMFLOAT2 Rp = pl->GetRandomPos();
                    ImGui::InputFloat("CircleRadius", &radius);
                    ImGui::InputFloat("SpeedRadius", &sp_radius);
                    pl->SetRadiusSpeed(sp_radius);
                    ImGui::Text("GetNowRandam_name:%s", pl->GetNowRandam_name().c_str());
                    ImGui::InputInt("Random_atai", &randP);
                    ImGui::InputFloat2("Random_Pos", &Rp.x);
                }

                if (ImGui::CollapsingHeader("magnification", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    int bairitu = pl->Getmagnification();
                    ImGui::InputInt("bairitu", &bairitu);
                }
               
            }
        }

    }
    ImGui::End();
}