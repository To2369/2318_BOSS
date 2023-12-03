#include"EnemyBoss.h"
#include"Graphics/Graphics.h"
#include"mathf.h"
#include"player.h"
#include"collision.h"
#include"PlayerManager.h"
#include"SceneManager.h"
#include"SceneClear.h"
//コンストラクタ
EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Boss/Boss.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.05f;
    //円柱の幅高さ設定
    radius = 1.0f;
    height = 1.0f;
    maxHealth = 30000;
    health = maxHealth;
    TransitionFirstAction();
    srand((unsigned int)time(NULL));
    DeathFlag = false;
}

//デストラクタ
EnemyBoss::~EnemyBoss()
{
    delete model;
}


void EnemyBoss::Update(float elapsedTime, FierdBuff& FB)
{
    if (stateTime < 0.0f)
    {
        stateTime = 0.0f;
    }
    switch (state)
    {
    case State::FirstAction:
        UpdateFirstActionState(elapsedTime);
        break;
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

    CollisionNodeVsPlayer0("Chest");

    CollisionNodeVsPlayer1("Head");
    //無敵時間更新
    UpdateInvincibletime(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);
    //モデル行列更新
    model->UpdateTransform(transform);

    GamePad& gamePad = Input::Instance().GetGamePad();
    const GamePadButton anyButton = GamePad::BTN_A | GamePad::BTN_B;
    if (DeathFlag == true)
    {
        SceneManager::Instance().ChangeScene(new SceneClear);
    }
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
void EnemyBoss::CollisionNodeVsPlayer0(const char* nodename)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        //ノードのワールド座標
        nodepos0 = {
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        };
    }
}

void EnemyBoss::CollisionNodeVsPlayer1(const char* nodename)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        //ノードのワールド座標
        nodepos1= {
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        };
        Graphics::Instance().GetDebugRenderer()->DrawSphere(nodepos1, 10, DirectX::XMFLOAT4(0, 0, 1, 1));
    }
}

void EnemyBoss::DamageFieldVsPlayer(FierdBuff& FB)
{

    //プレイヤーと当たり判定
    player& player = *PlayerManager::Instance().GetPlayer(0);
    if (player.GetDamageZone() == 10)
    {
        FB.FierdAttackEffect();
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
    model->playAnimetion(Anim_Idle1, true);
}

//一番最初の行動遷移(一度しか使わない)
void EnemyBoss::TransitionFirstAction()
{
    state = State::FirstAction;
    model->playAnimetion(Anim_Scream, false);
}

//一番最初の行動更新処理(一度しか使わない)
void EnemyBoss::UpdateFirstActionState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleState();
    }
}

//待機ステートへ遷移
void EnemyBoss::TransitionIdleState()
{
    state = State::Idle;
    stateTime = IdleStateTimer;
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
    if (animationTime >= 0.1f && animationTime <= 3.0f)
    {
        DamageFieldVsPlayer(FB);
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
    model->playAnimetion(Anim_Scream, false);
}

//攻撃ステート1更新処理
void EnemyBoss::UpdateAttack1State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 3.0f)
    {
        DamageFieldVsPlayer(FB);
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
    model->playAnimetion(Anim_Scream, false);
}

//攻撃ステート2更新処理
void EnemyBoss::UpdateAttack2State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 3.0f)
    {
        DamageFieldVsPlayer(FB);
    }
    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleState();

        FB.SetDamagePanelState(DamagePanelState::Idle);
    }
}

//攻撃ステート3へ遷移
void EnemyBoss::TransitionAttack3State()
{
    state = State::Attack3;
    model->playAnimetion(Anim_Scream, false);
}

//攻撃ステート3更新処理
void EnemyBoss::UpdateAttack3State(float elapsedTime, FierdBuff& FB)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 3.0f)
    {
        DamageFieldVsPlayer(FB);
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
    RandomState = (int)Mathf::RandameRange(0.0f, 3.0f);
    RandomPanelState = RandomState;
    stateTime = AttackIdleStateTimer;
    model->playAnimetion(Anim_Idle2, true);
}

//戦闘待機ステート更新処理
void EnemyBoss::UpdateBattleIdleState(float elapsedTime, FierdBuff& FB)
{
    stateTime -= elapsedTime;

    if (model->IsPlayerAnimetion())
    {
        switch (RandomPanelState)
        {
        case 0:
            FB.SetDamagePanelState(DamagePanelState::Attack0);
            break;
        case 1:
            FB.SetDamagePanelState(DamagePanelState::Attack1);
            break;
        case 2:
            FB.SetDamagePanelState(DamagePanelState::Attack2);
            break;
        case 3:
            FB.SetDamagePanelState(DamagePanelState::Attack3);
            break;
        }
    }

    if (stateTime < 0.0f)
    {
        switch (RandomState)
        {
        case 0:
            TransitionAttack0State();
            break;
        case 1:
            TransitionAttack1State();
            break;
        case 2:
            TransitionAttack2State();
            break;
        case 3:
            TransitionAttack3State();
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
        //TransitionBattleIdleState();
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
        DeathFlag = true;
    }
}