#pragma once
#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Input/Input.h"
#include"character.h"
#include"ProjectileManager.h"
#include <Effeck.h>
#include"CameraController.h"
#include<vector>
#include <cstdlib>
#include"FierdBuff.h"
#define MaxRandom10 66
#define MaxRandom09 55
#define MaxRandom08 45
#define MaxRandom07 36
#define MaxRandom06 28
#define MaxRandom05 21
#define MaxRandom04 15
#define MaxRandom03 10
#define MaxRandom02 5
#define MaxRandom01 1
enum PlayerNodeName
{
    head,//��
    armar,//����
    rightarm,//�E�r
    leftarm,//���r
};
//�v���C���[
class player :public Character
{
private:

    struct PerspectiveChangeFlag
    {
        bool TPS = false;
        bool FPS = false;
    };
    PerspectiveChangeFlag perspectivechange;//TPS,FPS�̎��_�؂�ւ��悤�N���X
    enum class State
    {
        Idle,
        Move,
        Aime,
        Attack,
        Damege,
        Death
    };
public:
   
    static float furic;
    static float accele;
    player();
    ~player()override;
    //�X�V����
    void Update(float elapsedTime, CameraController cameraCotrol,FierdBuff&FB);
    //�e�ۂƓG�̏Փ˔���
    void ColiisionProjectileVsMato();
    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //�f�o�b�O�v���~�e�B�u�\��
    void DarwDebugPrimitive();
    //�e�ۓ��͏���
    void InputProjectile();

    int Rand(float radius_);//�T�[�N���̔��a�ňႤ�l���Ƃ��Ă���
    void OnDamege()override;
    void OnDead()override;
public://Set�֐�
    //TPP
    void Set_TPPorFPS_Flag(bool set) {
        perspectivechange.TPS = set;
        perspectivechange.TPS ? perspectivechange.FPS = false : perspectivechange.FPS = true;
    }
    void SetMaterialNum(int material_) { ma.push_back(material_); }
    void SetDirection_P(DirectX::XMFLOAT3 dir) { dirc = dir; }
    void SetRadiusSpeed(float sp) { radiusSpeed = sp; }
    void SetRandomPos(DirectX::XMFLOAT2 pos) { randomPos = pos; }
    void SetProjectileSpeed(float speed) { projectileSpeed = speed; }
    void Setmagnification(int num) { magnification_P = num; }

public: //Get�֐�
    DirectX::XMFLOAT2 GetRandomPos() { return randomPos; }
    //�����݂ǂ̃����_���z����g�p���Ă邩
    std::string GetNowRandam_name() { return RandamParam_name; }
    float GetCircleRadius() { return circleRadius; }
    int GetRand() { return randP; }
    PerspectiveChangeFlag GetPerspectiveChangeFlag() { return this->perspectivechange; }
    std::string GetStringState(State state_) {

        switch (state)
        {
        case player::State::Idle:
            return "State::Idle";
            break;
        case player::State::Move:
            return"State::Move";
            break;
        case player::State::Attack:
            return"State::Attack";
            break;
        case player::State::Aime:
            return"State::Aime";
            break;
        case player::State::Damege:
            return"State::Damege";
            break;
        case player::State::Death:
            return"State::Death";
            break;

        }
    }
    State GetState() { return state; }
    char* GetNodename(PlayerNodeName name) { return NodeName[name]; }
    ProjectileManager& GetProjectileM() { return projectileManager; }
    int GetMaterialNum(int index) { return ma.at(index); }
    int GetMaterialCount() { return static_cast<int>(ma.size()); }
    //�e�̌����ݒ�Debug�p
    DirectX::XMFLOAT3 GetDirection_P() { return dirc; }
    float GetProjectileSpeed() { return projectileSpeed; }
    int Getmagnification() {return  magnification_P; }
public:
    void DebugMaterialClear(bool& flag) {

        if (flag)
        {
            flag = false;
            ma.clear();
        }
    }//�}�e���A���̐��l�����Z�b�g
private:


    //�ړ����͏���
    bool InputMove(float elapsedTime);
    void TransitionIdleState();
    void UpdateIdleState(float elapsedTime);
    void CollisionplayerVsEnemies();
    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec()const;
    DirectX::XMFLOAT3 GetFPSCameraAngleVec()const;

    //�ړ��X�e�[�g
    void TransitionAttackState();
    void UpdateAttackState(float elapsedTime, CameraController camera,FierdBuff&FB);
    void TransitionAimState();
    void UpateAimState(float elapsedTime);
    void TransitionMOveState();
    void UpdateMoveState(float elapsedTime);
    void TransitionDamegeState();
    void UpdateDamegeState(float elapsedTime);
    void TransitionDeathState();
    void UpdateDeathState(float elapsedTime);


private:
    void RandomParam(int rand_, DirectX::XMFLOAT2& ArrowDirection, float radius, std::string& name);
    //�m�[�h�Փˏ���
    void CollisionNodeVsEnemies(const char* nodename, float nodeRadius);
    enum Animation
    {
        Anim_Idle = 1,
        Anim_Running = 3,
        Anim_Aime_Idle = 5,
        Anim_Attack = 7,
        Anim_Death = 9,
        Anim_GetHit1 = 11,
    };


    char* NodeName[4]
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
    float leftHandRadius = 0.2f;
    bool attackCollisionFlag;


private:
    float circleRadius = 1.0f;
    std::string RandamParam_name;
    DirectX::XMFLOAT2 ArrowDirection_ver_1[MaxRandom10]
    {
        //1
        {1.0f,  0.f,},
        {0.9f,	0.1f},
        {0.8f,	0.2f},
        {0.7f,	0.3f},
        {0.6f,	0.4f},
        {0.5f,	0.5f},
        {0.4f,	0.6f},
        {0.3f,	0.7f},
        {0.2f,	0.8f},
        {0.1f,	0.9f},
        {0.f,   1.0f},
        //0.9
        {0.9f,  0.f, },
        {0.8f,  0.1f},
        {0.7f,  0.2f},
        {0.6f,  0.3f},
        {0.5f,  0.4f},
        {0.4f,  0.5f},
        {0.3f,  0.6f},
        {0.2f,  0.7f},
        {0.1f,  0.8f},
        {0.f,   0.9f},
        //0.8
        {0.8f,	0.f },
        {0.7f,	0.1f},
        {0.6f,	0.2f},
        {0.5f,	0.3f},
        {0.4f,	0.4f},
        {0.3f,	0.5f},
        {0.2f,	0.6f},
        {0.1f,	0.7f},
        {0.f,   0.8f},
        //0.7
        {0.7f,	0.f },
        {0.6f,	0.1f},
        {0.5f,	0.2f},
        {0.4f,	0.3f},
        {0.3f,	0.4f},
        {0.2f,	0.5f},
        {0.1f,	0.6f},
        {0.f ,	0.7f},
        //0.6
        {0.6f,	0.f  },
        {0.5f,	0.1f} ,
        {0.4f,	0.2f} ,
        {0.3f,	0.3f} ,
        {0.2f,	0.4f} ,
        {0.1f,	0.5f} ,
        {0.f,	0.6f} ,
        //0.5
       { 0.5f,	0.f },
       { 0.4f,	0.1f},
       { 0.3f,	0.2f},
       { 0.2f,	0.3f},
       { 0.1f,	0.4f},
       { 0.0f,  0.5f},
       //0.4
       {0.4f,	0.f,},
       {0.3f,	0.1f},
       {0.2f,	0.2f},
       {0.1f,	0.3f},
       {0.f,    0.4f},
       //0.3
       {0.3f,	0.f,},
       {0.2f,	0.1f},
       {0.1f,	0.2f},
       {0.f,    0.3f},
       {0.2f,	0.f },
       //0.2
       {0.1f,	0.f },
       {0.1f,	0.1f},
       {0.f ,   0.2f},
       {0.f,    0.1f},
       //0.1
       {0.f,    0.f,}

    };
    DirectX::XMFLOAT2 ArrowDirection_ver_09[MaxRandom09]
    {
        //0.9
        {0.9f,  0.f, },
        {0.8f,  0.1f},
        {0.7f,  0.2f},
        {0.6f,  0.3f},
        {0.5f,  0.4f},
        {0.4f,  0.5f},
        {0.3f,  0.6f},
        {0.2f,  0.7f},
        {0.1f,  0.8f},
        {0.f,   0.9f},
        //0.8
        {0.8f,	0.f },
        {0.7f,	0.1f},
        {0.6f,	0.2f},
        {0.5f,	0.3f},
        {0.4f,	0.4f},
        {0.3f,	0.5f},
        {0.2f,	0.6f},
        {0.1f,	0.7f},
        {0.f,   0.8f},
        //0.7
        {0.7f,	0.f },
        {0.6f,	0.1f},
        {0.5f,	0.2f},
        {0.4f,	0.3f},
        {0.3f,	0.4f},
        {0.2f,	0.5f},
        {0.1f,	0.6f},
        {0.f ,	0.7f},
        //0.6
        {0.6f,	0.f  },
        {0.5f,	0.1f} ,
        {0.4f,	0.2f} ,
        {0.3f,	0.3f} ,
        {0.2f,	0.4f} ,
        {0.1f,	0.5f} ,
        {0.f,	0.6f} ,
        //0.5
       { 0.5f,	0.f },
       { 0.4f,	0.1f},
       { 0.3f,	0.2f},
       { 0.2f,	0.3f},
       { 0.1f,	0.4f},
       { 0.0f,  0.5f},
       //0.4
       {0.4f,	0.f,},
       {0.3f,	0.1f},
       {0.2f,	0.2f},
       {0.1f,	0.3f},
       {0.f,    0.4f},
       //0.3
       {0.3f,	0.f,},
       {0.2f,	0.1f},
       {0.1f,	0.2f},
       {0.f,    0.3f},
       {0.2f,	0.f },
       //0.2
       {0.1f,	0.f },
       {0.1f,	0.1f},
       {0.f ,   0.2f},
       {0.f,    0.1f},
       //0.1
       {0.f,    0.f,}
    };
    DirectX::XMFLOAT2 ArrowDirection_ver_08[MaxRandom08]
    {
        //0.8
       {0.8f,	0.f },
       {0.7f,	0.1f},
       {0.6f,	0.2f},
       {0.5f,	0.3f},
       {0.4f,	0.4f},
       {0.3f,	0.5f},
       {0.2f,	0.6f},
       {0.1f,	0.7f},
       {0.f,   0.8f },
       //0.7
       {0.7f,	0.f },
       {0.6f,	0.1f},
       {0.5f,	0.2f},
       {0.4f,	0.3f},
       {0.3f,	0.4f},
       {0.2f,	0.5f},
       {0.1f,	0.6f},
       {0.f ,	0.7f},
       //0.6
       {0.6f,	0.f  },
       {0.5f,	0.1f} ,
       {0.4f,	0.2f} ,
       {0.3f,	0.3f} ,
       {0.2f,	0.4f} ,
       {0.1f,	0.5f} ,
       {0.f,	0.6f} ,
       //0.5
      { 0.5f,	0.f },
      { 0.4f,	0.1f},
      { 0.3f,	0.2f},
      { 0.2f,	0.3f},
      { 0.1f,	0.4f},
      { 0.0f,  0.5f },
      //0.4
      {0.4f,	0.f },
      {0.3f,	0.1f},
      {0.2f,	0.2f},
      {0.1f,	0.3f},
      {0.f,    0.4f },
      //0.3
      {0.3f,	0.f },
      {0.2f,	0.1f},
      {0.1f,	0.2f},
      {0.f,    0.3f },
      {0.2f,	0.f },
      //0.2
      {0.1f,	0.f },
      {0.1f,	0.1f},
      {0.f ,   0.2f },
      {0.f,    0.1f },
      //0.1
      {0.f,    0.f  }
    };
    DirectX::XMFLOAT2 ArrowDirection_ver_07[MaxRandom07]
    {
        //0.7
        {0.7f,	0.f },
        {0.6f,	0.1f},
        {0.5f,	0.2f},
        {0.4f,	0.3f},
        {0.3f,	0.4f},
        {0.2f,	0.5f},
        {0.1f,	0.6f},
        {0.f ,	0.7f},
        //0.6
        {0.6f,	0.f  },
        {0.5f,	0.1f} ,
        {0.4f,	0.2f} ,
        {0.3f,	0.3f} ,
        {0.2f,	0.4f} ,
        {0.1f,	0.5f} ,
        {0.f,	0.6f} ,
        //0.5
       { 0.5f,	0.f },
       { 0.4f,	0.1f},
       { 0.3f,	0.2f},
       { 0.2f,	0.3f},
       { 0.1f,	0.4f},
       { 0.0f,  0.5f},
       //0.4
       {0.4f,	0.f,},
       {0.3f,	0.1f},
       {0.2f,	0.2f},
       {0.1f,	0.3f},
       {0.f,    0.4f},
       //0.3
       {0.3f,	0.f,},
       {0.2f,	0.1f},
       {0.1f,	0.2f},
       {0.f,    0.3f},
       {0.2f,	0.f },
       //0.2
       {0.1f,	0.f },
       {0.1f,	0.1f},
       {0.f ,   0.2f},
       {0.f,    0.1f},
       //0.1
       {0.f,    0.f,}

    };
    DirectX::XMFLOAT2 ArrowDirection_ver_06[MaxRandom06]
    {
        //0.6
     {0.6f,	0.f  },
     {0.5f,	0.1f } ,
     {0.4f,	0.2f } ,
     {0.3f,	0.3f } ,
     {0.2f,	0.4f } ,
     {0.1f,	0.5f } ,
     {0.f,	0.6f } ,
     //0.5
    { 0.5f,	0.f  },
    { 0.4f,	0.1f },
    { 0.3f,	0.2f },
    { 0.2f,	0.3f },
    { 0.1f,	0.4f },
    { 0.0f,  0.5f},
    //0.4
    {0.4f,	0.f  },
    {0.3f,	0.1f },
    {0.2f,	0.2f },
    {0.1f,	0.3f },
    {0.f,    0.4f},
    //0.3
    {0.3f,	0.f  },
    {0.2f,	0.1f },
    {0.1f,	0.2f },
    {0.f,    0.3f},
    {0.2f,	0.f  },
    //0.2
    {0.1f,	0.f  },
    {0.1f,	0.1f },
    {0.f ,   0.2f},
    {0.f,    0.1f},
    //0.1
    {0.f,    0.f }

    };
    DirectX::XMFLOAT2 ArrowDirection_ver_05[MaxRandom05]
    {

        //0.5
       { 0.5f,	0.f },
       { 0.4f,	0.1f},
       { 0.3f,	0.2f},
       { 0.2f,	0.3f},
       { 0.1f,	0.4f},
       { 0.0f,  0.5f},
       //0.4
       {0.4f,	0.f},
       {0.3f,	0.1f},
       {0.2f,	0.2f},
       {0.1f,	0.3f},
       {0.f,    0.4f},
       //0.3
       {0.3f,	0.f},
       {0.2f,	0.1f},
       {0.1f,	0.2f},
       {0.f,    0.3f},
       {0.2f,	0.f },
       //0.2
       {0.1f,	0.f },
       {0.1f,	0.1f},
       {0.f ,   0.2f},
       {0.f,    0.1f},
       //0.1
       {0.f,    0.f}

    };
    DirectX::XMFLOAT2 ArrowDirection_ver_04[MaxRandom04]
    {
        //0.4
      {0.4f,	0.f},
      {0.3f,	0.1f},
      {0.2f,	0.2f},
      {0.1f,	0.3f},
      {0.f,     0.4f},
      //0.3
      {0.3f,	0.f},
      {0.2f,	0.1f},
      {0.1f,	0.2f},
      {0.f,    0.3f},
      {0.2f,	0.f},
      //0.2
      {0.1f,	0.f},
      {0.1f,	0.1f},
      {0.f ,   0.2f},
      {0.f,    0.1f},
      //0.1
      {0.f,    0.f}
    };
    DirectX::XMFLOAT2 ArrowDirection_ver_03[MaxRandom03]
    {
        //0.3
    {0.3f,	0.f},
    {0.2f,	0.1f},
    {0.1f,	0.2f},
    {0.f,    0.3f},
    {0.2f,	0.f},
    //0.2
    {0.1f,	0.f  },
    {0.1f,	0.1f },
    {0.f ,   0.2f},
    {0.f,    0.1f},
    //0.1
    {0.f,    0.f }
    };
    DirectX::XMFLOAT2 ArrowDirection_ver_02[MaxRandom02]
    {
        //0.2
     {0.1f,	0.f},
     {0.1f,	0.1f},
     {0.f ,   0.2f},
     {0.f,    0.1f},
     //0.1
     {0.f,    0.f}
    };
    DirectX::XMFLOAT2 ArrowDirection_ver_01{ 0,0 };


    std::vector<int> ma;
    State state = State::Idle;
    int randP = 0;
    float radiusSpeed;
    DirectX::XMFLOAT3 dirc;
    float projectileSpeed = 0;///�e�̃X�s�[�h
    DirectX::XMFLOAT2 randomPos;
    int magnification_P=0;
   
   
};
