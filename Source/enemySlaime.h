#pragma once

#include"Graphics/Shader.h"
#include"enemy.h"
static float distSq;


class EnemySlime :public Enemy
{
public:
    EnemySlime();
    ~EnemySlime()override;
    //速力更新処理
   
    //更新処理
    void Update(float elpsedTime)override;
    //描画処理
    void Render(RenderContext& rc, ModelShader* shader)override;
    void OnDamege()override;
    //死亡したときに呼ばれる
    void OnDead()override;

    void DrawDebugPrimitive()override;
    void SetTerritory(const DirectX::XMFLOAT3&origin,float range);
   
  
private:
    void CollisionNodeVsPlayer(const char* nodename, float boneRadius);
    //プレイヤー索敵
    bool serchPlayer();
    void TransitionIdleBattleState();
    void UpdateIdleBattleState(float elapsedTime);

    void TransitionAttackState();
    void UpdateAttackState(float elapsedtime);
    void TransitionIdleState();
    void UpdateIdleState(float elapsedTime);
    void TransitionPursuitState();
    void UpdatePursuitState(float elapsedTime);
    void TransitionDamegeState();
    void UpdateDamegeState(float elapsedTime);
    void TransitionDethState();
    void UpdateDeathState(float elapsedTime);
    //ターゲット位置をランダムに設定
    void SetRandomTargetPosition();
    //目的地点へ移動
    void MOveToTarget(float elapsedTime, float speedRate);
    //徘徊ステート
    void TransitionWanderState();
    void UpdateWanderState(float elapsedTime);

private:
    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Attack,
        Idle_Battle,
        Damege,
        Death
    };
    enum  Animation
    {
        Anim_IdleNormal,
        Anim_IdleBattle,
        Anim_Attack1,
        Anim_Attack2,
        Anim_WalkFWD,
        Anim_WalkBWD,
        Anim_WalkLeft,
        Anim_WalkRight,
        Anim_RunFWD,
        Anim_SenseSomthingST,
        Anim_SenseSomthingPRT,
        Anim_Taunt,
        Anim_Victory,
        Anim_GetHit,
        Anim_Dizzy,
        Anim_Die

    };
    //Model* model = nullptr;
private:
    
    State state = State::Wander;
    DirectX::XMFLOAT3 targetPositoin{};
    DirectX::XMFLOAT3 territoryOrigin{};
    float territoryRange = 10.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(360);
    float serachRange = 5.0f;
    float attackRange=1.5f;
};


