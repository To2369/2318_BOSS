#include"EnemySlaime.h"
#include"Graphics/Graphics.h"
#include"mathf.h"
#include "player.h"
#include"collision.h"
#include"PlayerManager.h"
//コンストラクタ
EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
    //円柱の幅高さ設定
    radius = 0.5f;
    height = 1.0f;
    TransitionWanderState();
    srand(time(NULL));
}

//デストラクタ
EnemySlime::~EnemySlime()
{
    delete model;
}

void EnemySlime::OnDamege()
{
    TransitionDamegeState();
}
void EnemySlime::Update(float elapsedTime,FierdBuff&FB)
{
    switch(state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Idle_Battle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damege:
        UpdateDamegeState(elapsedTime);
    case State::Death:
        UpdateDeathState(elapsedTime);
    }
    //速力更新処理
    UpdateVelocity(elapsedTime);
    //無敵時間更新
    UpdateInvincibletime(elapsedTime);
  
    //オブジェクト行列を更新
    UpdateTransform();
    
    model->UpdateAnimation(elapsedTime);
    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
void EnemySlime::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySlime::OnDead()
{

   TransitionDethState();
}

void EnemySlime::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
    DebugRenderer* debugRnderer = Graphics::Instance().GetDebugRenderer();
    debugRnderer->DrawCylinder(territoryOrigin,territoryRange,1.0f,DirectX::XMFLOAT4(0,1,0,1));
    debugRnderer->DrawSphere(targetPositoin, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
    //索敵距離
    debugRnderer->DrawCylinder(position, serachRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRnderer->DrawCylinder(position,attackRange,1.0f,DirectX::XMFLOAT4(1,0,0,1));
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

void EnemySlime::CollisionNodeVsPlayer(const char* nodename, float boneRadius)
{
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );
        Graphics::Instance().GetDebugRenderer()->DrawSphere(nodePosition, boneRadius, DirectX::XMFLOAT4(0, 0, 1, 1));

        PlayerManager& pl_m = PlayerManager::Instance();
        player* player = pl_m.GetPlayer(0);
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            boneRadius,
            player->GetPosition(),
            player->getRadius(),
            player->GetHeight(),
            outPosition
        ))
        {
            if (player->ApplyDamage(1, 0.5f))
            {
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;
                float power = 10.0f;
                vec.x *= power;
                vec.z *= power;
                vec.y = 5;
                player->AddImpulse(vec);

            }

        }
    }
}

bool EnemySlime::serchPlayer()
{

    const DirectX::XMFLOAT3& playerPosition = PlayerManager::Instance().GetPlayer(0)->GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < serachRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        vx /= distXZ;
        vz /= distXZ;
        float frontX = sinf(angle.y);
        float frontY = cosf(angle.y);
        float dot = (frontX * vx) + (frontY * vy);
        if (dot>0.0f)
        {
            return true;
        }
    }


    return false;
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::Idle_Battle;
    stateTime = Mathf::RandameRange(2.0f, 3.0f);
    model->playAnimetion(Anim_IdleBattle, true);

}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    targetPositoin = PlayerManager::Instance().GetPlayer(0)->GetPosition();
    stateTime -= elapsedTime;
    if (stateTime < 0.0f)
    {
        float vx = targetPositoin.x - position.x;
        float vy = targetPositoin.y - position.y;
        float vz = targetPositoin.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist<attackRange)
        {
            TransitionAttackState();
        }
        else
        {
            TransitionWanderState();
        }

    }
    MOveToTarget(elapsedTime, 0.0f);
}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;
    model->playAnimetion(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedtime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 0.35f)
    {
        CollisionNodeVsPlayer("EyeBall",0.2f);

    }
    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleBattleState();
    }

}

void EnemySlime::TransitionIdleState()
{
    state = State::Idle;
    stateTime = Mathf::RandameRange(3.0f, 5.0f);
    model->playAnimetion(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
{
    stateTime -= elapsedTime;
    if (serchPlayer())
    {
        TransitionPursuitState();
        return;
    }
    if (stateTime < 0.0f)
    {
        TransitionWanderState();
    }
    
}

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;
    stateTime = Mathf::RandameRange(3.0f, 5.0f);
    model->playAnimetion(Anim_RunFWD,true);
}

void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    targetPositoin = PlayerManager::Instance().GetPlayer(0)->GetPosition();
    MOveToTarget(elapsedTime, 1.0f);
    stateTime -= elapsedTime;
    if (stateTime < 0.0f)
    {
        TransitionIdleState();
    }


    float vx = targetPositoin.x - position.x;
    float vy = targetPositoin.y - position.y;
    float vz = targetPositoin.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < attackRange)
    {
        TransitionAttackState();
    }

}

void EnemySlime::TransitionDamegeState()
{
    state = State::Damege;
    model->playAnimetion(Anim_GetHit, false);
}

void EnemySlime::UpdateDamegeState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleBattleState();
    }
}

void EnemySlime::TransitionDethState()
{
    state = State::Death;
    model->playAnimetion(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        Destroy();
    }
}

void EnemySlime::SetRandomTargetPosition()
{
   
    
    DirectX::XMFLOAT3 random = { Mathf::RandameRange(-3,6),0, Mathf::RandameRange(-3,6) };
    targetPositoin.x = random.x;
    targetPositoin.y = 0;
    targetPositoin.z = random.z;

}

void EnemySlime::MOveToTarget(float elapsedTime, float speedRate)
{
    float vx = targetPositoin.x - position.x;
    float vz = targetPositoin.z - position.z;
    float dist = sqrtf(vx*vx+vz*vz);
    vx = vx/dist;
    vz = vz/dist;

    Move(vx, vz, moveSpeed * speedRate);
    Trun(elapsedTime, vx, vz, turnSpeed * speedRate);
  
}

void EnemySlime::TransitionWanderState()
{

    state = State::Wander;
    SetRandomTargetPosition();
    model->playAnimetion(Animation::Anim_WalkFWD,true);

}

void EnemySlime::UpdateWanderState(float elapsedTime)
{
    float vx = targetPositoin.x - position.x;
    float vz = targetPositoin.z - position.z;
    distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
       //SetRandomTargetPosition();
        TransitionIdleState();
    }
    MOveToTarget(elapsedTime,0.5f);
    //プレイヤー索敵
    if (serchPlayer())
    {

        TransitionPursuitState();
    }
}


