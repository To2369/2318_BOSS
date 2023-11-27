#pragma once
#include"Graphics/Shader.h"
#include"enemy.h"


class EnemyBoss :public Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss()override;
    //速力更新処理

    //更新処理
    void Update(float elpsedTime, FierdBuff& FB)override;
    //描画処理
    void Render(RenderContext& rc, ModelShader* shader)override;
    void OnDamege()override;
    //死亡したときに呼ばれる
    void OnDead()override;

    //目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedTate);

    //プレイヤー索敵
    bool SearchPlayer();

    void DrawDebugPrimitive()override;
private:
    void CollisionNodeVsPlayer(const char* nodename, float boneRadius);
    void DamageFieldVsPlayer();

    void TransitionIdleState();                         //待機ステートへ遷移
    void UpdateIdleState(float elapsedTime);            //待機ステート更新処理

    void TransitionAttack0State();                       //攻撃ステート0へ遷移
    void UpdateAttack0State(float elapsedTime, FierdBuff& FB);          //攻撃ステート0更新処理

    void TransitionAttack1State();                       //攻撃ステート1へ遷移
    void UpdateAttack1State(float elapsedTime, FierdBuff& FB);          //攻撃ステート1更新処理

    void TransitionAttack2State();                       //攻撃ステート2へ遷移
    void UpdateAttack2State(float elapsedTime, FierdBuff& FB);          //攻撃ステート2更新処理

    void TransitionAttack3State();                       //攻撃ステート3へ遷移
    void UpdateAttack3State(float elapsedTime, FierdBuff& FB);          //攻撃ステート3更新処理

    void TransitionBattleIdleState();                   //戦闘待機ステートへ遷移
    void UpdateBattleIdleState(float elapsedTime, FierdBuff& FB);      //戦闘待機ステート更新処理

    void TransitionDamegeState();                       //ダメージステートへ遷移
    void UpdateDamegeState(float elapsedTime);          //ダメージステート更新処理

    void TransitionDeathState();                             //死亡ステートへ遷移
    void UpdateDeathState(float elapsedTime);                //死亡ステートへ更新処理
private:
    enum class State
    {
        Idle,
        Damege,
        Attack0,
        Attack1,
        Attack2,
        Attack3,
        Death,
        Idle_Battle,
    };
    enum  Animation
    {
        Anim_Idle1,
        Anim_FlyForward,
        Anim_BasicAttack,
        Anim_TakeOff,
        Anim_Land,  //多分飛行状態から地面に着陸
        Anim_Scream,
        Anim_ClawAttack,
        Anim_FlameAttack,
        Anim_Defend,//怯みてきな？
        Anim_GetHit,
        Anim_Sleep,
        Anim_Walk,
        AnimRun,
        Anim_FlyFrameAttack,
        Anim_FlyGlide,
        Anim_Idle2,
        Anim_Death,
        Anim_FlyFloat

    };
    Model* model = nullptr;
private:

    State state = State::Idle;
    DirectX::XMFLOAT3 targetPositoin{};
    DirectX::XMFLOAT3 territoryOrigin{};
    float territoryRange = 10.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360);
    int RandomState = 0;
    float Damage = 1.0f;//攻撃力
    float InvicivleTimer = 1.0f;//無敵時間
    bool DeathFlag;
};