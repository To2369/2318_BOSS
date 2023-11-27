#pragma once
#include"Graphics/Shader.h"
#include"enemy.h"


class EnemyBoss :public Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss()override;
    //���͍X�V����

    //�X�V����
    void Update(float elpsedTime, FierdBuff& FB)override;
    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader)override;
    void OnDamege()override;
    //���S�����Ƃ��ɌĂ΂��
    void OnDead()override;

    //�ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedTate);

    //�v���C���[���G
    bool SearchPlayer();

    void DrawDebugPrimitive()override;
private:
    void CollisionNodeVsPlayer(const char* nodename, float boneRadius);
    void DamageFieldVsPlayer();

    void TransitionIdleState();                         //�ҋ@�X�e�[�g�֑J��
    void UpdateIdleState(float elapsedTime);            //�ҋ@�X�e�[�g�X�V����

    void TransitionAttack0State();                       //�U���X�e�[�g0�֑J��
    void UpdateAttack0State(float elapsedTime, FierdBuff& FB);          //�U���X�e�[�g0�X�V����

    void TransitionAttack1State();                       //�U���X�e�[�g1�֑J��
    void UpdateAttack1State(float elapsedTime, FierdBuff& FB);          //�U���X�e�[�g1�X�V����

    void TransitionAttack2State();                       //�U���X�e�[�g2�֑J��
    void UpdateAttack2State(float elapsedTime, FierdBuff& FB);          //�U���X�e�[�g2�X�V����

    void TransitionAttack3State();                       //�U���X�e�[�g3�֑J��
    void UpdateAttack3State(float elapsedTime, FierdBuff& FB);          //�U���X�e�[�g3�X�V����

    void TransitionBattleIdleState();                   //�퓬�ҋ@�X�e�[�g�֑J��
    void UpdateBattleIdleState(float elapsedTime, FierdBuff& FB);      //�퓬�ҋ@�X�e�[�g�X�V����

    void TransitionDamegeState();                       //�_���[�W�X�e�[�g�֑J��
    void UpdateDamegeState(float elapsedTime);          //�_���[�W�X�e�[�g�X�V����

    void TransitionDeathState();                             //���S�X�e�[�g�֑J��
    void UpdateDeathState(float elapsedTime);                //���S�X�e�[�g�֍X�V����
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
        Anim_Land,  //������s��Ԃ���n�ʂɒ���
        Anim_Scream,
        Anim_ClawAttack,
        Anim_FlameAttack,
        Anim_Defend,//���݂Ă��ȁH
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
    float Damage = 1.0f;//�U����
    float InvicivleTimer = 1.0f;//���G����
    bool DeathFlag;
};