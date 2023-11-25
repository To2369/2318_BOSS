#include <imgui.h>
#include"player.h"
#include"camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "collision.h"
#include"ProjectileManager.h"
#include"ProjectileStraight.h"
#include"ProjectileHoming.h"
#include"CameraController.h"
#include<string>
#include "StageManager.h"
#include"parameter.h"

float player::accele;
float player::furic;


player::player()
{
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    model = new Model("Data/Model/player/player.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
    health = Param::player_param.Hp;
    TransitionIdleState();
    state = State::Idle;
   
    invincibleTime_ = 0.0f;
    attackCollisionFlag = false;
    dirc = {};
}
player::~player()
{
    delete HitEffect;
    delete model;
}
void player::Update(float elapsedTime, CameraController cameraCotrol,FierdBuff&FB)
{

    /*  GamePad& gamepad = Input::Instance().GetGamePad();
      if (gamepad.GetButtonDown() & GamePad::BTN_B)
      {
          model->playAnimetion(0, false);
      }
      if (!model->IsPlayerAnimetion())
      {
          model->playAnimetion(1, true);
      }*/


    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Aime:
        UpateAimState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime, cameraCotrol,FB);
    case State::Damege:
        UpdateDamegeState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }
    //速力更新処理
    UpdateVelocity(elapsedTime);
    //弾丸更新処理
    projectileManager.Update(elapsedTime);
    UpdateInvincibletime(elapsedTime);
    //プレイヤーと敵との衝突判定
    CollisionplayerVsEnemies();
    ColiisionProjectileVsMato();
    //オブジェクト更新処理
    UpdateTransform();
   
    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);
    if (!model->IsPlayerAnimetion())
    {

    }
    //モデル行列更新
    model->UpdateTransform(transform);

}

void player::TransitionIdleState()
{
    state = State::Idle;
    model->playAnimetion(Anim_Idle, true);
}

void player::UpdateIdleState(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButton() & Mouse::BTN_RIGHT)
    {
        circleRadius = 1.0f;
        TransitionAimState();
        return;
    }
    if (InputMove(elapsedTime))
    {

        TransitionMOveState();
    }



}

void player::CollisionplayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //衝突処理
        DirectX::XMFLOAT3 outPosition;


        if (Collision::IntersectCylinderVsCylinder(
            GetPosition(), //playerの位置
            getRadius(),//playerの半径
            GetHeight(),
            enemy->GetPosition(),//敵の位置
            enemy->getRadius(),//敵の半径
            enemy->GetHeight(),
            outPosition
        ))
        {

            enemy->SetPosition(outPosition);

        }

    }

}
DirectX::XMFLOAT3 player::GetMoveVec()const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //カメラ方向を取得
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    //移動ベクトルはXZ平面なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }
    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    //スティックの水平入力値をカメラ右方向に反映し
    //スティックの垂直入力値をカメラ前方向に反映し
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;
    return vec;
}
DirectX::XMFLOAT3 player::GetFPSCameraAngleVec() const
{
    // カメラ方向を取得
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // 移動ベクトルはXZ平面なベクトルになるようにする

    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    // 進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = cameraFrontX;
    vec.z = cameraFrontZ;

    // Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}


//移動入力処理
bool player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //移動処理
    //Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
    Move(moveVec.x, moveVec.z, moveSpeed);
    //旋回処理
    Trun(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
    return moveVec.x > 0 || moveVec.y > 0 || moveVec.z > 0 || moveVec.x < 0 || moveVec.y < 0 || moveVec.z < 0;
}

void player::TransitionMOveState()
{
    state = State::Move;
    model->playAnimetion(Anim_Running, true);
}

void player::UpdateMoveState(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButton() & Mouse::BTN_RIGHT)
    {
        circleRadius = 1.0f;
        TransitionAimState();
        return;
    }
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }



}


void player::TransitionDamegeState()
{
    state = State::Damege;
    model->playAnimetion(Anim_GetHit1, false);
}

void player::UpdateDamegeState(float elapsedTime)
{

    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleState();
    }
}

void player::TransitionDeathState()
{
    state = State::Death;
    model->playAnimetion(Anim_Death, false);
}

void player::UpdateDeathState(float elapsedTime)
{

}

void player::CollisionNodeVsEnemies(const char* nodename, float nodeRadius)
{

    DirectX::XMFLOAT3 nodePosition = {};
    Model::Node* node = model->FindNode(nodename);
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;
    EnemyManager& enemy = EnemyManager::Instance();
    int count = EnemyManager::Instance().GetEnemyCount();

    for (int i = 0; i < count; i++)
    {
        Enemy* enemys = enemy.GetEnemy(i);
        DirectX::XMFLOAT3 outpos = {};
        if (Collision::IntersectSphereVsCylinder(nodePosition, nodeRadius,
            enemys->GetPosition(),
            enemys->getRadius(),
            enemys->GetHeight(),
            outpos))
        {
            //吹き飛ばす
            {   //吹き飛ばす移動方向ベクトル
                DirectX::XMFLOAT3 impulse;
                //吹き飛ばす力
                const float power = 2.0f;
                //敵の位置
                DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemys->GetPosition());
                //nodeの位置
                DirectX::XMVECTOR nodeVec = DirectX::XMLoadFloat3(&nodePosition);
                //nodeから敵への方向ベクトルを計算
                DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(enemyVec, nodeVec);
                //方向ベクトルを正規化
                Vec = DirectX::XMVector3Normalize(Vec);
                DirectX::XMStoreFloat3(&impulse, Vec);
                //吹き飛ばす移動方向の速度ベクトルにpowerを掛けて設定
                impulse.x = impulse.x * power;
                impulse.y = impulse.y * power;
                impulse.z = impulse.z * power;

                enemys->AddImpulse(impulse);

            }
            if (enemys->ApplyDamage(1, 0.5f))//ヒットエフェクト再生
            {
                DirectX::XMFLOAT3 e = enemys->GetPosition();
                e.y += enemys->GetHeight() * 0.5f;
                HitEffect->Play(e);
            }


        }
    }

}




void player::OnDamege()
{
    TransitionDamegeState();
}

void player::OnDead()
{
    TransitionDeathState();
}

int player::Rand(float radius_)
{
    if (radius_ <= 1.0f)
    {
        if (radius_ <= 0.9f)
        {
            if (radius_ <= 0.8f)
            {
                if (radius_ <= 0.7f)
                {
                    if (radius_ <= 0.6f)
                    {
                        if (radius_ <= 0.5f)
                        {
                            if (radius_ <= 0.4f)
                            {
                                if (radius_ <= 0.3f)
                                {
                                    if (radius_ <= 0.2f)
                                    {
                                        if (radius_ <= 0.1f)
                                        {

                                            return 0;
                                        }

                                        return rand() % MaxRandom02;
                                    }

                                    return rand() % MaxRandom03;
                                }

                                return rand() % MaxRandom04;
                            }

                            return rand() % MaxRandom05;
                        }

                        return rand() % MaxRandom06;
                    }

                    return rand() % MaxRandom07;
                }

                return rand() % MaxRandom08;
            }

            return rand() % MaxRandom09;
        }

        return rand() % MaxRandom10;// MaxRandom10の値は６６
    }
}

void player::RandomParam(int rand_, DirectX::XMFLOAT2& ArrowDirection, float radius, std::string& name)
{
    randP = rand_;
    if (radius <= 1.0f)
    {
        if (radius <= 0.9f)
        {
            if (radius <= 0.8f)
            {
                if (radius <= 0.7f)
                {
                    if (radius <= 0.6f)
                    {
                        if (radius <= 0.5f)
                        {
                            if (radius <= 0.4f)
                            {
                                if (radius <= 0.3f)
                                {
                                    if (radius <= 0.2f)
                                    {
                                        if (radius <= 0.1f)
                                        {
                                            DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_01)));

                                            name = "MaxRandom01";
                                            return;
                                        }
                                        DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_02[rand_])));
                                        name = "MaxRandom02";
                                        return;
                                    }
                                    DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_02[rand_])));
                                    name = "MaxRandom03";
                                    return;
                                }
                                DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_04[rand_])));
                                name = "MaxRandom04";
                                return;
                            }
                            DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_05[rand_])));
                            name = "MaxRandom05";
                            return;
                        }
                        DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_06[rand_])));
                        name = "MaxRandom06";
                        return;
                    }
                    DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_07[rand_])));
                    name = "MaxRandom07";
                    return;
                }
                DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_08[rand_])));
                name = "MaxRandom08";
                return;
            }
            DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_09[rand_])));
            name = "MaxRandom09";
            return;
        }
        DirectX::XMStoreFloat2(&ArrowDirection, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&ArrowDirection), DirectX::XMLoadFloat2(&ArrowDirection_ver_1[rand_])));
        name = "MaxRandom10";
        return;
    }
}


void player::TransitionAttackState()
{
    state = State::Attack;
    model->playAnimetion(Anim_Attack, false);

}

void player::UpdateAttackState(float elapsedTime, CameraController camera,FierdBuff&FB)
{
    float animationTime = model->GetAnimationTime();
    float nextAnimationTime = 0.690f;//待機状態にいくまでのタイム
    float startAttackTime = 0.162f;
    float EndAttackTime = 0.182f;
    //framerate 0.017
    if (animationTime <= nextAnimationTime)
    {

        if (animationTime >= startAttackTime && animationTime <= EndAttackTime)
        {
            if (FB.GetBuffPanelState() == magnificationPanelState::State1)
            {
                FB.SetBuffPanelState(magnificationPanelState::State2);
            }
            else if (FB.GetBuffPanelState() == magnificationPanelState::State2)
            {
               FB.SetBuffPanelState(magnificationPanelState::State1);
            }

            InputProjectile();
        }
    }
    else
    {
        Set_TPPorFPS_Flag(true);
        TransitionIdleState();
    }

}

void player::TransitionAimState()
{
    state = State::Aime;
    model->playAnimetion(Anim_Aime_Idle, true);

}

void player::UpateAimState(float elapsedTime)
{
    Set_TPPorFPS_Flag(false);
    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.GetButton() & Mouse::BTN_RIGHT)
    {
        circleRadius -= radiusSpeed;
        if (circleRadius <= 0)
        {
            circleRadius = 1.0f;
        }
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {

            TransitionAttackState();
        }

        DirectX::XMFLOAT3 vec = GetFPSCameraAngleVec();
        Trun(elapsedTime, vec.x, vec.z, 800);

    }
    else
    {

        Set_TPPorFPS_Flag(true);
        TransitionIdleState();
    }
}


void player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
    projectileManager.Render(dc, shader);
}
//デバッグプリミティブ描画
void player::DarwDebugPrimitive()
{
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    ////衝突判定用のデバッグ玉を描画
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4{ 0,0,0,1 });
    //弾丸デバッグプリミティブ描画
    projectileManager.DrawdebugPrimitive();
    


    // Model::Node* LeftHandBone = model->FindNode("mixamorig:LeftHand");
    Model::Node* LeftHandBone = model->FindNode(GetNodename(PlayerNodeName::rightarm));

    debugRender->DrawSphere(DirectX::XMFLOAT3(
        LeftHandBone->worldTransform._41,
        LeftHandBone->worldTransform._42,
        LeftHandBone->worldTransform._43),
        leftHandRadius,
        DirectX::XMFLOAT4(1, 0, 0, 1)
    );


}
void player::InputProjectile()
{
    //直進弾丸発射
    DirectX::XMFLOAT3 dir{};
    DirectX::XMFLOAT3 pos{};
    DirectX::XMFLOAT2 ArrowDirection{};

    if (GetPerspectiveChangeFlag().FPS)
    {
        RandomParam(Rand(circleRadius), ArrowDirection, circleRadius, RandamParam_name);
        SetRandomPos(ArrowDirection);
        //前方向
        dir.x = Camera::instance().GetFront().x + ArrowDirection.x;
        dir.y = Camera::instance().GetFront().y + ArrowDirection.y;
        dir.z = Camera::instance().GetFront().z;

        //発射位置
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

    }
    //発射
    ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
    projectile->Launch(dir, pos, projectileSpeed);
    //projectileManager.Register(projectile);


    ////追尾弾丸発射
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    //{

    //    //前方向
    //    DirectX::XMFLOAT3 dir;
    //    dir.x = sinf(angle.y);//transform._31 * 100;
    //    dir.y = 0.0f;
    //    dir.z = cosf(angle.y);//transform._33 * 100;
    //    //発射位置（プレイヤーの腰あたり）
    //    DirectX::XMFLOAT3 pos;
    //    pos.x = position.x;
    //    pos.y = position.y + height*0.5;
    //    pos.z = position.z;
    //    //ターゲット（デフォルトではプレイヤーの前方）
    //    DirectX::XMFLOAT3 target;
    //    target.x = pos.x + dir.x * 1000.0f;
    //    target.y = pos.y + dir.y * 1000.0f;
    //    target.z = pos.z + dir.z * 1000.0f;
    //    //一番近くの敵をターゲットにする
    //    float dist = FLT_MAX;
    //    EnemyManager& enemyMnager = EnemyManager::Instance();
    //    int enemyCount = enemyMnager.GetEnemyCount();
    //    for (int i = 0; i < enemyCount; i++)
    //    {
    //        //敵との距離判定
    //        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //        //PLAYERの位置
    //        DirectX::XMVECTOR Player_pos = DirectX::XMLoadFloat3(&position);
    //        //ENEMYの位置
    //        DirectX::XMVECTOR Enemy_pos  = DirectX::XMLoadFloat3(&enemy->GetPosition());
    //        //playerからenemyまでのベクトルを計算
    //        DirectX::XMVECTOR VEC        = DirectX::XMVectorSubtract(Enemy_pos, Player_pos);
    //        //そのベクトルの長さを計算
    //        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(VEC);
    //        float d;
    //        DirectX::XMStoreFloat(&d, D);
    //        //すべての敵の中で一番近い敵をターゲット設定
    //        if (d < dist)
    //        {
    //            dist = d;
    //            target = enemy->GetPosition();
    //            target.y += enemy->GetHeight() * 0.5f;
    //        }

    //    }
    //    //発射
    //    ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
    //    projectile->Launch(dir, pos, target);
    //}



}

//球丸
void player::ColiisionProjectileVsMato()
{
    //EnemyManager& enemyManager = EnemyManager::Instance();
    ////すべての弾丸と全ての敵を総当たりで衝突判定
    //int progectileCount = projectileManager.GetProjectileCount();
    //int enemyCount = enemyManager.GetEnemyCount();
    //HitResult hit;
    //for (int i = 0; i < progectileCount; ++i)
    //{
    //    Projectile* projectile = projectile = projectileManager.GetProgectile(i);
    //    DirectX::XMFLOAT3 Start{ projectile->GetPosition() };
    //    DirectX::XMFLOAT3 End{ projectile->GetPosition().x,projectile->GetPosition().y,projectile->GetPosition().z + projectile->GetAdjustPos() };
    //    for (int j = 0; j < enemyCount; j++)
    //    {
    //        Enemy* enemy = enemyManager.GetEnemy(j);
    //        //衝突判定
    //        DirectX::XMFLOAT3 outPosition;
    //        if (Collision::InstersectRayVsModel(Start, End, enemy->GetCharacterModel(), hit))
    //        {
    //            SetMaterialNum(hit.materialIndex);
    //            //ダメージを与える
    //            if (magnification_P == 0)
    //            {
    //                magnification_P = 1;
    //            }
    //            if (enemy->ApplyDamage(1*magnification_P, 0.5f))
    //            {
    //                projectile->Destroy();
    //            }
    //        }
    //       
    //    }
    //}

}