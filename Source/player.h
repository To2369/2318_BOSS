#pragma once
#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"character.h"
#include"ProjectileManager.h"
#include <Effeck.h>

//�v���C���[
class player :public Character
{
protected:
    //���n�����Ƃ��ɌĂ΂��
    void OnLanding()override;
private:
    struct PerspectiveChangeFlag
    {
        bool TPS = false;
        bool FPS = false;
    };
    PerspectiveChangeFlag perspectivechange;//TPS,FPS�̎��_�؂�ւ��悤�N���X
public:
    static player& instance();
    static float furic;
    static float accele;
    player();
    ~player()override;
    //�X�V����
    void Update(float elapsedTime);
    //�e�ۂƓG�̏Փ˔���
    void ColiisionProjectileVsEnemy();
    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //�f�o�b�O�v���~�e�B�u�\��
    void DarwDebugPrimitive();
    //�e�ۓ��͏���
    void InputProjectile();
    //�W�����v���͏���
    bool InputJump();
 
    void OnDamege()override;
    void OnDead()override;
    //TPP
    void Set_TPPorFPS_Flag(bool set) {
        perspectivechange.TPS = set;
        perspectivechange.TPS ? perspectivechange.FPS = false : perspectivechange.FPS = true;
    }
    PerspectiveChangeFlag GetPerspectiveChangeFlag() { return this->perspectivechange; }
  
private:
   
private:
    bool InputAttack();
 
    //�ړ����͏���
    bool InputMove(float elapsedTime);
    void TransitionIdleState();
    void UpdateIdleState(float elapsedTime);
    void CollisionplayerVsEnemies();
    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec()const;
  
    //�ړ��X�e�[�g
    void TransitionAttackState();
    void UpdateAttackState(float elapsedTime);
    void TransitionMOveState();
    void UpdateMoveState(float elapsedTime);
    void TransitionJumpState();
    void UpdateJumpState(float elapsedTime);
    void TransitionLandState();
    void UpdateLandState(float elapsedTime);
    void TransitionDamegeState();
    void UpdateDamegeState(float elapsedTime);
    void TransitionDeathState();
    void UpdateDeathState(float elapsedTime);


private:
   
    //�m�[�h�Փˏ���
    void CollisionNodeVsEnemies(const char* nodename, float nodeRadius);
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Faling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking
    };
    enum PlayerNodeName
    {
        head,//��
        armar,//����
        rightarm,//�E�r
        leftarm,//���r
    };
   
    std::string NodeName[4]
    {
        "mixamorig:Head",
        "mixamorig:Spine2",
        "mixamorig:RightArm",
        "mixamorig:LeftArm",
    };
    ProjectileManager projectileManager;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int    jumpCount = 0;
    int    jumpLimit = 2;
    bool isHit_m = false;
    Effect* HitEffect = nullptr;
    float leftHandRadius = 0.4f;
    bool attackCollisionFlag;
   
   
private:
  
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Attack,
        Damege,
        Death
    };
    State state = State::Idle;
};
