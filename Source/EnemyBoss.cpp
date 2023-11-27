#include"EnemyBoss.h"
#include"Graphics/Graphics.h"
#include"mathf.h"
#include"player.h"
#include"collision.h"
#include"PlayerManager.h"
//�R���X�g���N�^
EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Boss/Boss.mdl");
    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;
    //�~���̕������ݒ�
    radius = 1.0f;
    height = 1.0f;
    health = 30000;
    TransitionIdleState();
    srand(time(NULL));
}

//�f�X�g���N�^
EnemyBoss::~EnemyBoss()
{
    delete model;
}


void EnemyBoss::Update(float elapsedTime)
{
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack0:
        UpdateAttack0State(elapsedTime);
        break;
    case State::Attack1:
        UpdateAttack1State(elapsedTime);
        break;
    case State::Attack2:
        UpdateAttack2State(elapsedTime);
        break;
    case State::Attack3:
        UpdateAttack3State(elapsedTime);
        break;
    case State::Idle_Battle:
        UpdateBattleIdleState(elapsedTime);
        break;
    case State::Damege:
        UpdateDamegeState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }
    //���͍X�V����
    UpdateVelocity(elapsedTime);

    //���G���ԍX�V
    UpdateInvincibletime(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);
    //���f���s��X�V
    model->UpdateTransform(transform);
}

//�`�揈��
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
    //���G����
    debugRnderer->DrawCylinder(position, serachRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRnderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

//�m�[�h�ƃv���C���[�̏Փ˔���
void EnemyBoss::CollisionNodeVsPlayer(const char* nodename, float boneRadius)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //�����蔻��\��
        Graphics::Instance().GetDebugRenderer()->DrawSphere(nodePosition, boneRadius, DirectX::XMFLOAT4(0, 0, 1, 1));

        //�v���C���[�Ɠ����蔻��
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
            //�_���[�W��^����
            if (player.ApplyDamage(1, 0.5f))
            {
                //�G�𐁂���΂��x�N�g�����Z�o
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;

                //��Z���ʂɐ�����΂��͂�������
                float power = 10.0f;
                vec.x *= power;
                vec.z *= power;
                //Y�����ɂ��͂�������
                vec.y = 5;

                //������΂�
                player.AddImpulse(vec);
            }
        }
    }
}

//�_���[�W���󂯂��Ƃ�
void EnemyBoss::OnDamege()
{
    TransitionDamegeState();
}

//���S�����Ƃ�
void EnemyBoss::OnDead()
{
    TransitionDeathState();
}

//�ڕW�n�_�ֈړ�
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

//�v���C���[���G
bool EnemyBoss::SearchPlayer()
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
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

//�ҋ@�X�e�[�g�֑J��
void EnemyBoss::TransitionIdleState()
{
    state = State::Idle;
    model->playAnimetion(Anim_Idle1, true);
}

//�ҋ@�X�e�[�g�X�V����
void EnemyBoss::UpdateIdleState(float elapsedTime)
{
    stateTime -= elapsedTime;
    if (SearchPlayer())
    {
        //TransitionPursuitState();
    }
    targetPositoin = PlayerManager::Instance().GetPlayer(0)->GetPosition();
    float vx = targetPositoin.x - position.x;
    float vy = targetPositoin.y - position.y;
    float vz = targetPositoin.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < attackRange)
    {
        TransitionBattleIdleState();
    }
}

//�ǐՃX�e�[�g�֑J��
void EnemyBoss::TransitionPursuitState()
{
    state = State::Pursuit;
    stateTime = Mathf::RandameRange(3.0f, 5.0f);
    model->playAnimetion(Anim_Walk, true);
}

//�ǐՃX�e�[�g�X�V����
void EnemyBoss::UpdatePursuitState(float elapsedTime)
{
    targetPositoin = PlayerManager::Instance().GetPlayer(0)->GetPosition();
    MoveToTarget(elapsedTime, 1.0f);
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
        TransitionBattleIdleState();
    }
}

//�U���X�e�[�g0�֑J��
void EnemyBoss::TransitionAttack0State()
{
    state = State::Attack0;
    model->playAnimetion(Anim_Scream, false);
}

//�U���X�e�[�g0�X�V����
void EnemyBoss::UpdateAttack0State(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        CollisionNodeVsPlayer("Chest", 1.0f);

    }
    if (!model->IsPlayerAnimetion())
    {
        float vx = targetPositoin.x - position.x;
        float vy = targetPositoin.y - position.y;
        float vz = targetPositoin.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < attackRange)
        {
            TransitionIdleState();
            return;
        }
        TransitionBattleIdleState();
    }
}

//�U���X�e�[�g1�֑J��
void EnemyBoss::TransitionAttack1State()
{
    state = State::Attack1;
    model->playAnimetion(Anim_ClawAttack, false);
}

//�U���X�e�[�g1�X�V����
void EnemyBoss::UpdateAttack1State(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        CollisionNodeVsPlayer("Chest", 1.0f);

    }
    if (!model->IsPlayerAnimetion())
    {
        float vx = targetPositoin.x - position.x;
        float vy = targetPositoin.y - position.y;
        float vz = targetPositoin.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < attackRange)
        {
            TransitionIdleState();
            return;
        }
        TransitionBattleIdleState();
    }
}

//�U���X�e�[�g2�֑J��
void EnemyBoss::TransitionAttack2State()
{
    state = State::Attack2;
    model->playAnimetion(Anim_Sleep, false);
}

//�U���X�e�[�g2�X�V����
void EnemyBoss::UpdateAttack2State(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        CollisionNodeVsPlayer("Chest", 1.0f);

    }
    if (!model->IsPlayerAnimetion())
    {
        float vx = targetPositoin.x - position.x;
        float vy = targetPositoin.y - position.y;
        float vz = targetPositoin.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < attackRange)
        {
            TransitionIdleState();
            return;
        }
        TransitionBattleIdleState();
    }
}

//�U���X�e�[�g3�֑J��
void EnemyBoss::TransitionAttack3State()
{
    state = State::Attack3;
    model->playAnimetion(Anim_TakeOff, false);
}

//�U���X�e�[�g3�X�V����
void EnemyBoss::UpdateAttack3State(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 1.33f)
    {
        CollisionNodeVsPlayer("Chest", 1.0f);

    }
    if (!model->IsPlayerAnimetion())
    {
        float vx = targetPositoin.x - position.x;
        float vy = targetPositoin.y - position.y;
        float vz = targetPositoin.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < attackRange)
        {
            TransitionIdleState();
            return;
        }
        TransitionBattleIdleState();
    }
}

//�퓬�ҋ@�X�e�[�g�֑J��
void EnemyBoss::TransitionBattleIdleState()
{
    state = State::Idle_Battle;
    stateTime = Mathf::RandameRange(2.0f, 3.0f);
    model->playAnimetion(Anim_Idle2, false);
}

//�퓬�ҋ@�X�e�[�g�X�V����
void EnemyBoss::UpdateBattleIdleState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        RandomState = (int)Mathf::RandameRange(0.0f, 3.0f);
        switch (RandomState)
        {
        case 0:
            TransitionAttack0State();break;
        case 1:
            TransitionAttack1State(); break;
        case 2:
            TransitionAttack2State(); break;
        case 3:
            TransitionAttack3State(); break;
        }
    }
}

//�_���[�W�X�e�[�g�֑J��
void EnemyBoss::TransitionDamegeState()
{
    state = State::Damege;
    model->playAnimetion(Anim_GetHit, false);
}

//�_���[�W�X�e�[�g�X�V����
void EnemyBoss::UpdateDamegeState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        TransitionBattleIdleState();
    }
}

//���S�X�e�[�g�֑J��
void EnemyBoss::TransitionDeathState()
{
    state = State::Death;
    model->playAnimetion(Anim_Death, false);
}

//���S�X�e�[�g�X�V����
void EnemyBoss::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        Destroy();
    }
}