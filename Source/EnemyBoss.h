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
    void Update(float elpsedTime)override;
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


    void TransitionIdleState();                         //待機ステートへ遷移
    void UpdateIdleState(float elapsedTime);            //待機ステート更新処理

    void TransitionPursuitState();                      //追跡ステートへ遷移
    void UpdatePursuitState(float elapsedTime);         //追跡ステート更新処理

    void TransitionAttackState();                       //攻撃ステートへ遷移
    void UpdateAttackState(float elapsedTime);          //攻撃ステート更新処理

    void TransitionBattleIdleState();                   //戦闘待機ステートへ遷移
    void UpdateBattleIdleState(float elapsedTime);      //戦闘待機ステート更新処理

    void TransitionDamegeState();                       //ダメージステートへ遷移
    void UpdateDamegeState(float elapsedTime);          //ダメージステート更新処理

    void TransitionDeathState();                             //死亡ステートへ遷移
    void UpdateDeathState(float elapsedTime);                //死亡ステートへ更新処理
private:
    enum class State
    {
        Idle,
        Pursuit,
        Damege,
        Attack,
        Death,
        Idle_Battle,
       /* Wander,
        Idle,
        Pursuit,
        Attack,
        Idle_Battle,
        Damege,
        Death*/
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
    float serachRange = 10.0f;                           //索敵範囲
    float attackRange = 5.0f;                           //攻撃範囲
};