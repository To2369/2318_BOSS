#pragma once
#include<DirectXMath.h>
#include"Graphics/Model.h"
//�L����
class Character
{
public:
    Character() {};
    virtual ~Character() {};
    void UpdateTransform();
    //���a�擾
    float getRadius()const { return radius; };
    //�n�ʂɒ��n���Ă��邩
    bool IsGrounf()const { return isGround; }
    //�����擾
    float GetHeight()const { return height; }
    //�_���[�W��^����
    bool ApplyDamage(int damage, float invincibleTime);
    //�Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);
    //���N��Ԏ擾
    int GetHealth()const { return health; };
    //�ő匒�N��Ԏ擾
    int GetMaxHealth()const { return maxHealth; };
    float GetTimer() { return stateTime; }
private:
    //�������͍X�V����
    void UpdateVerticalVelocity(float elpsedFrame);
    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);
    //�������͍X�V����
    void UpdateHorizontalVelocity(float elpsedFram);
    //�����ړ��X�V����
    void UpdateHorizontalMove(float elpsedTime);


protected:
    float stateTime = 0.0f;
    int healt = 5;
    float height = 2.0f;
    float radius = 0.5f;
    float gravity = -1.0f;
    float invincibleTime_ = 2.0f;
    float friction = 0.5f;
    float acceleration = 1.0f;
    float maxMoveSpeed = 5.0f;
    float moveVecX = 0.0f;
    float moveVecZ = 0.0f;
    float airControl = 0.3f;
    float step0ffeset = 1.0f;
    float slopeRate = 0.0f;
    int   health = 5;
    int   maxHealth = 5;
    Model* model = nullptr;
    DirectX::XMFLOAT3   velocity = { 0,0,0 };
    DirectX::XMFLOAT3   position = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    DirectX::XMFLOAT3   scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform =
    {
        1,0,0,0,//�E����
        0,1,0,0,//�����
        0,0,1,0,//�O����

        0,0,0,1//�ʒu
    };
    bool isGround = false;
    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamege() {};
    //���S�����Ƃ��ɌĂ΂��
    virtual void OnDead() {};
protected:
    //�ړ�����
    void Move(float vx, float vz, float speed);
    //void Move(float elapsedTime,float vx, float vz, float speed);
    //���񏈗�
    void Trun(float elapsedTime, float vx, float vz, float speed);
    //�W�����v����
    void Jump(float speed);
    //���͏����X�V
    void UpdateVelocity(float elpsedTime);
    //���G���ԍX�V
    void UpdateInvincibletime(float elapsedTime);
    //���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {};
public:
    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition()const { return position; };
    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; };
    //��]�擾
    const DirectX::XMFLOAT3& GetAngle()const { return angle; };
    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; };
    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale()const { return scale; };
    //�X�P�[���擾
    void  SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; };
    Model* GetCharacterModel() { return model; }
};


