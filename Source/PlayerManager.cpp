#include "PlayerManager.h"
#include"Graphics/Graphics.h"
#include"collision.h"
#include"player.h"

void PlayerManager::Update(float elapsedTime)
{
    for (player* Player : players)
    {
        Player->Update(elapsedTime);
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
                
                static bool flag = false;
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
                }
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
                /* ImGui::InputFloat("furiction", &furic);
                 ImGui::InputFloat("acceleration", &accele);

                 ImGui::InputInt("jumpcount", &jumpCount);*/
            }
        }
    }
    ImGui::End();
}