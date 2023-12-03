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

    void DrawDebugPrimitive()override;
private:
    void CollisionNodeVsPlayer0(const char* nodename);
    void CollisionNodeVsPlayer1(const char* nodename);
    void DamageFieldVsPlayer(FierdBuff& FB);

    void TransitionFirstAction();                       //一番最初の行動遷移(一度しか使わない)
    void UpdateFirstActionState(float elapsedTime);     //一番最初の行動更新処理(一度しか使わない)

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
        FirstAction,
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

    State state = State::FirstAction;
    DirectX::XMFLOAT3 targetPositoin{};
    DirectX::XMFLOAT3 territoryOrigin{};
    float territoryRange = 10.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360);
    int RandomState = -1;
    float Damage = 1.0f;//攻撃力
    float InvicivleTimer = 5.0f;//無敵時間
    int RandomPanelState = -1;
    bool RandomCheck = false;//ランダム値を設定できたかどうか
    float IdleStateTimer = 4.0f;//待機ステートから次に遷移するまでの時間
    float AttackIdleStateTimer = 2.0f;//攻撃待機ステートから次に遷移するまでの時間
    //DirectX::XMFLOAT2 IdleStateTimer={ 5.0f,1.0f };       //待機ステートから次に遷移するまでの時間
    //DirectX::XMFLOAT2 AttackIdleStateTimer = { 5.0f,3.0f };   //攻撃待機ステートから次に遷移するまでの時間
    bool DeathFlag;
};