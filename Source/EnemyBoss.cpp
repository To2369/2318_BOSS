#include"EnemyBoss.h"
#include"Graphics/Graphics.h"
#include"mathf.h"
#include"player.h"
#include"collision.h"
#include"PlayerManager.h"
//コンストラクタ
EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Boss/Boss.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
    //円柱の幅高さ設定
    radius = 1.0f;
    height = 1.0f;
    health = 30000;
    TransitionIdleState();
    srand(time(NULL));
}

//デストラクタ
EnemyBoss::~EnemyBoss()
{
    delete model;
}


void EnemyBoss::Update(float elapsedTime, FierdBuff& FB)
{
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Attack0:
        UpdateAttack0State(elapsedTime, FB);
        break;
    case State::Attack1:
        UpdateAttack1State(elapsedTime,FB);
        break;
    case State::Attack2:
        UpdateAttack2State(elapsedTime, FB);
        break;
    case State::Attack3:
        UpdateAttack3State(elapsedTime, FB);
        break;
    case State::Idle_Battle:
        UpdateBattleIdleState(elapsedTime, FB);
        break;
    case State::Damege:
        UpdateDamegeState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }
    //速力更新処理
    //UpdateVelocity(elapsedTime);

    //無敵時間更新
    UpdateInvincibletime(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);
    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
void EnemyBoss::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}


void EnemyBoss::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
    DebugRenderer* debugRnderer = Graphics::Instance().GetDebugRenderer();
    //debugRnderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));
    debugRnderer->DrawSphere(targetPositoin, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
}

//ノードとプレイヤーの衝突判定
void EnemyBoss::CollisionNodeVsPlayer(const char* nodename, float boneRadius)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        //ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //当たり判定表示
        Graphics::Instance().GetDebugRenderer()->DrawSphere(nodePosition, boneRadius, DirectX::XMFLOAT4(0, 0, 1, 1));

        //プレイヤーと当たり判定
        player& player = *PlayerManager::Instance().GetPlayer(0);
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            boneRadius,
            player.GetPosition(),
            player.getRadius(),
            player.GetHeight(),
            outPosition
        ))
        {
            //ダメージを与える
            if (player.ApplyDamage(1, 0.5f))
            {
                //敵を吹っ飛ばすベクトルを算出
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;

                //ｘZ平面に吹っ飛ばす力をかける
                float power = 10.0f;
                vec.x *= power;
                vec.z *= power;
                //Y方向にも力をかける
                vec.y = 5;

                //吹っ飛ばす
                player.AddImpulse(vec);
            }
        }
    }
}

void EnemyBoss::DamageFieldVsPlayer()
{
    //プレイヤーと当たり判定
    player& player = *PlayerManager::Instance().GetPlayer(0);
    if (player.GetDamageZone() == 10)
    {
        player.ApplyDamage(Damage, InvicivleTimer);
    }
}

//ダメージを受けたとき
void EnemyBoss::OnDamege()
{
    TransitionDamegeState();
}

//死亡したとき
void EnemyBoss::OnDead()
{
    TransitionDeathState();
}

//目標地点へ移動
void EnemyBoss::MoveToTarget(float elapsedTime, float speedRate)
{
    float vx = targetPositoin.x - position.x;
    float vz = targetPositoin.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx = vx / dist;
    vz = vz / dist;

    Move(vx, vz, moveSpeed * speedRate);
    Trun(elapsedTime, vx, vz, turnSpeed * speedRate);
}

//待機ステートへ遷移
void EnemyBoss::TransitionIdleState()
{
    state = State::Idle;
    stateTime = Mathf::RandameRange(2.0f, 10.0f);
    model->playAnimetion(Anim_Idle1, true);
}

//待機ステート更新処理
void EnemyBoss::UpdateIdleState(float elapsedTime)
{
    stateTime -= elapsedTime;
    
    if (stateTime < 0.0f)
    {
        TransitionBattleIdleState();
    }
}

//攻撃ステート0へ遷移
void EnemyBoss::TransitionAttack0State()
{
    state = State::Attack0;
    model->playAnimetion(Anim_Scream, false);
}

//攻撃ステート0更新処理
void EnemyBoss::UpdateAttack0State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        DamageFieldVsPlayer();
    }
    if (!model->IsPlayerAnimetion())
    {
        FB.SetDamagePanelState(DamagePanelState::Idle);

        TransitionIdleState();
    }
}

//攻撃ステート1へ遷移
void EnemyBoss::TransitionAttack1State()
{
    state = State::Attack1;
    model->playAnimetion(Anim_ClawAttack, false);
}

//攻撃ステート1更新処理
void EnemyBoss::UpdateAttack1State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        DamageFieldVsPlayer();
    }
    if (!model->IsPlayerAnimetion())
    {
        FB.SetDamagePanelState(DamagePanelState::Idle);

        TransitionIdleState();
    }
}

//攻撃ステート2へ遷移
void EnemyBoss::TransitionAttack2State()
{
    state = State::Attack2;
    model->playAnimetion(Anim_Sleep, false);
}

//攻撃ステート2更新処理
void EnemyBoss::UpdateAttack2State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        DamageFieldVsPlayer();
    }
    if (!model->IsPlayerAnimetion())
    {
        FB.SetDamagePanelState(DamagePanelState::Idle);

        TransitionIdleState();
    }
}

//攻撃ステート3へ遷移
void EnemyBoss::TransitionAttack3State()
{
    state = State::Attack3;
    model->playAnimetion(Anim_TakeOff, false);
}

//攻撃ステート3更新処理
void EnemyBoss::UpdateAttack3State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        DamageFieldVsPlayer();
    }
    if (!model->IsPlayerAnimetion())
    {
        FB.SetDamagePanelState(DamagePanelState::Idle);

        TransitionIdleState();
    }
}

//戦闘待機ステートへ遷移
void EnemyBoss::TransitionBattleIdleState()
{
    state = State::Idle_Battle;
    stateTime = Mathf::RandameRange(2.0f, 3.0f);
    model->playAnimetion(Anim_Idle2, false);
}

//戦闘待機ステート更新処理
void EnemyBoss::UpdateBattleIdleState(float elapsedTime, FierdBuff& FB)
{
    if (!model->IsPlayerAnimetion())
    {
        RandomState = (int)Mathf::RandameRange(0.0f, 3.0f);
        switch (RandomState)
        {
        case 0:
            TransitionAttack0State();
            FB.SetDamagePanelState(DamagePanelState::Attack0); 
            break;
        case 1:
            TransitionAttack1State(); 
            FB.SetDamagePanelState(DamagePanelState::Attack1); 
            break;
        case 2:
            TransitionAttack2State(); 
            FB.SetDamagePanelState(DamagePanelState::Attack2); 
            break;
        case 3:
            TransitionAttack3State(); 
            FB.SetDamagePanelState(DamagePanelState::Attack3); 
            break;
        }
    }
}

//ダメージステートへ遷移
void EnemyBoss::TransitionDamegeState()
{
    state = State::Damege;
    model->playAnimetion(Anim_GetHit, false);
}

//ダメージステート更新処理
void EnemyBoss::UpdateDamegeState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        TransitionBattleIdleState();
    }
}

//死亡ステートへ遷移
void EnemyBoss::TransitionDeathState()
{
    state = State::Death;
    model->playAnimetion(Anim_Death, false);
}

//死亡ステート更新処理
void EnemyBoss::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        Destroy();
    }
}