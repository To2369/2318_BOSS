#include"EnemyBoss.h"
#include"Graphics/Graphics.h"
#include"mathf.h"
#include"player.h"
#include"collision.h"
#include"PlayerManager.h"
#include"SceneManager.h"
#include"SceneClear.h"
//�R���X�g���N�^
EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/Boss/Boss.mdl");
    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.05f;
    //�~���̕������ݒ�
    radius = 1.0f;
    height = 1.0f;
    maxHealth = 30000;
    health = maxHealth;
    TransitionFirstAction();
    srand((unsigned int)time(NULL));
    DeathFlag = false;
}

//�f�X�g���N�^
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
    //���͍X�V����
    //UpdateVelocity(elapsedTime);

    CollisionNodeVsPlayer0("Chest");

    CollisionNodeVsPlayer1("Head");
    //���G���ԍX�V
    UpdateInvincibletime(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);
    //���f���s��X�V
    model->UpdateTransform(transform);

    GamePad& gamePad = Input::Instance().GetGamePad();
    const GamePadButton anyButton = GamePad::BTN_A | GamePad::BTN_B;
    if (DeathFlag == true)
    {
        SceneManager::Instance().ChangeScene(new SceneClear);
    }
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
}



//�m�[�h�ƃv���C���[�̏Փ˔���
void EnemyBoss::CollisionNodeVsPlayer0(const char* nodename)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        nodepos0 = {
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        };
    }
}

void EnemyBoss::CollisionNodeVsPlayer1(const char* nodename)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodename);
    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
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

    //�v���C���[�Ɠ����蔻��
    player& player = *PlayerManager::Instance().GetPlayer(0);
    if (player.GetDamageZone() == 10)
    {
        FB.FierdAttackEffect();
        player.ApplyDamage(Damage, InvicivleTimer);
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
    model->playAnimetion(Anim_Idle1, true);
}

//��ԍŏ��̍s���J��(��x�����g��Ȃ�)
void EnemyBoss::TransitionFirstAction()
{
    state = State::FirstAction;
    model->playAnimetion(Anim_Scream, false);
}

//��ԍŏ��̍s���X�V����(��x�����g��Ȃ�)
void EnemyBoss::UpdateFirstActionState(float elapsedTime)
{
    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleState();
    }
}

//�ҋ@�X�e�[�g�֑J��
void EnemyBoss::TransitionIdleState()
{
    state = State::Idle;
    stateTime = IdleStateTimer;
    model->playAnimetion(Anim_Idle1, true);
}

//�ҋ@�X�e�[�g�X�V����
void EnemyBoss::UpdateIdleState(float elapsedTime)
{
    stateTime -= elapsedTime;
    
    if (stateTime < 0.0f)
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

//�U���X�e�[�g1�֑J��
void EnemyBoss::TransitionAttack1State()
{
    state = State::Attack1;
    model->playAnimetion(Anim_Scream, false);
}

//�U���X�e�[�g1�X�V����
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

//�U���X�e�[�g2�֑J��
void EnemyBoss::TransitionAttack2State()
{
    state = State::Attack2;
    model->playAnimetion(Anim_Scream, false);
}

//�U���X�e�[�g2�X�V����
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

//�U���X�e�[�g3�֑J��
void EnemyBoss::TransitionAttack3State()
{
    state = State::Attack3;
    model->playAnimetion(Anim_Scream, false);
}

//�U���X�e�[�g3�X�V����
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

//�퓬�ҋ@�X�e�[�g�֑J��
void EnemyBoss::TransitionBattleIdleState()
{
    state = State::Idle_Battle;
    RandomState = (int)Mathf::RandameRange(0.0f, 3.0f);
    RandomPanelState = RandomState;
    stateTime = AttackIdleStateTimer;
    model->playAnimetion(Anim_Idle2, true);
}

//�퓬�ҋ@�X�e�[�g�X�V����
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
        //TransitionBattleIdleState();
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
        DeathFlag = true;
    }
}