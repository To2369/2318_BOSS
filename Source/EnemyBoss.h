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
    void Update(float elpsedTime)override;
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


    void TransitionIdleState();                         //�ҋ@�X�e�[�g�֑J��
    void UpdateIdleState(float elapsedTime);            //�ҋ@�X�e�[�g�X�V����

    void TransitionPursuitState();                      //�ǐՃX�e�[�g�֑J��
    void UpdatePursuitState(float elapsedTime);         //�ǐՃX�e�[�g�X�V����

    void TransitionAttackState();                       //�U���X�e�[�g�֑J��
    void UpdateAttackState(float elapsedTime);          //�U���X�e�[�g�X�V����

    void TransitionBattleIdleState();                   //�퓬�ҋ@�X�e�[�g�֑J��
    void UpdateBattleIdleState(float elapsedTime);      //�퓬�ҋ@�X�e�[�g�X�V����

    void TransitionDamegeState();                       //�_���[�W�X�e�[�g�֑J��
    void UpdateDamegeState(float elapsedTime);          //�_���[�W�X�e�[�g�X�V����

    void TransitionDeathState();                             //���S�X�e�[�g�֑J��
    void UpdateDeathState(float elapsedTime);                //���S�X�e�[�g�֍X�V����
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
    float serachRange = 10.0f;                           //���G�͈�
    float attackRange = 5.0f;                           //�U���͈�
};