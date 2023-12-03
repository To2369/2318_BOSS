#include "character.h"
#include"player.h"
#include"StageManager.h"
#include "mathf.h"
#include "enemySlaime.h"

//�s��ϊ�����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s��쐬
   // DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //�R�̍s���g�ݍ��킹�āA���[���h���W���쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h���W�����o��
    DirectX::XMStoreFloat4x4(&transform, W);



}



void Character::Move(float vx, float vz, float speed)
{
    //�ړ������x�N�g��
    moveVecX = vx;
    moveVecZ = vz;
    //�ő呬�x�`�F�b�N
    maxMoveSpeed = speed;
}

void Character::Trun(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    float length = sqrtf(vx * vx + vz * vz);
    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (length < 0.001f)
    {
        return;
    }

    //�i�s�x�N�g����P�ʉ�
    vx /= length;
    vz /= length;

    //���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);//PLAYER
    float frontZ = cosf(angle.y);//PLAYER
                             //vx camera
                             //vy camera

    //��]�p�����߂邽�߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����

    float dot = (frontX * vx) + (frontZ * vz);
    float rot = 1.0f - dot;

    if (rot > speed)
    {
        rot = speed;
    }
    //���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float croos = (frontZ * vx) - (frontX * vz);

    //�QD�̊O�ϔ��肪���̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //���E������s�����ɂ���č��E�����I������
    if (croos < 0.0f)
    {
        angle.y += -rot;
    }
    else
    {
        angle.y += rot;
    }


}

void Character::Jump(float speed)
{
    //������̗͂�ݒ�
    velocity.y = speed;
}

void Character::UpdateVelocity(float elpsedTime)
{
    //�o�߃t���[��
    float elapsedFrame = 60.0f * elpsedTime;
    //�������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);
    UpdateHorizontalVelocity(elapsedFrame);
    //�����ړ������X�V����
    UpdateVerticalMove(elpsedTime);
    UpdateHorizontalMove(elpsedTime);
}
void Character::UpdateVerticalVelocity(float elpsedFrame)
{
    //�d�͏���
    velocity.y += gravity * elpsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    //���������̈ړ���
    float my = velocity.y * elapsedTime;
    slopeRate = 0.0f;
    //�L�����N�^�[��Y�������ƈقȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,1,0 };
    //������
    if (my < 0.0f)
    {
        //���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x,position.y + step0ffeset,position.z };
        //���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x,position.y + my - 0.2f,position.z };
        //���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            normal = hit.normal;
            //�n�ʂɐڒn���Ă���
            position = hit.position;
            angle.y += hit.rotation.y;
            //�X�Η��̌v�Z
          /*  float normalLengthZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthZ + hit.normal.y));*/
            //���n����
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //�󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
    //�n�ʂ̌����ɉ����悤��XZ����]
    {
        //Y���@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
        float x = atan2f(normal.z, normal.y);
        float z = -atan2f(normal.x, normal.y);

        angle.x = Mathf::Lerp(angle.x, x, 0.1f);
        angle.z = Mathf::Lerp(angle.z, z, 0.1f);

    }

}

void Character::UpdateHorizontalVelocity(float elpsedFram)
{
    //XZ���ʂ̑��͂���������
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    DirectX::XMFLOAT2 vel = { velocity.x,velocity.z };
    DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&vel);
    if (length > 0.0f)
    {
        //���C��
        float friction = this->friction * elpsedFram;
        if (!isGround)
        {
            friction *= airControl;

        }
        player::furic = friction;
        //���C�ɂ�鉡�����̌���
        if (length > friction)
        {
            //���x��P�ʃx�N�g����
            auto v = DirectX::XMVector2Normalize(vec);
            //�P�ʃx�N�g�����������x��friction�����փX�P�[�����O
            /*velocityX = velocityX * friction;
            velocityZ = velocityZ * friction;*/
            v = DirectX::XMVectorScale(v, friction);


            //�X�P�[�����O����
            DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(vec, v));
            velocity.x = vel.x;
            velocity.z = vel.y;
        }
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }
    //XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        //�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        float moveVeclength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        if (moveVeclength > 0.0f)
        {
            //������
            float acceleration = this->acceleration * elpsedFram;
            if (!isGround)
            {
                acceleration *= airControl;
            }
            player::accele = acceleration;
            //�ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //�ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;

            }
            /*  if (isGround && slopeRate > 0.0f)
              {
                  velocity.y -= length * slopeRate * elpsedFram;
              }*/
        }
    }
    moveVecX = 0.0f;
    moveVecZ = 0.0f;

}

void Character::UpdateHorizontalMove(float elpsedTime)
{
    ////�ړ�����
   /* position.x+=velocity.x*elpsedTime;
    position.z+=velocity.z*elpsedTime;*/

    //�������͌v�Z
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //�����ړ��l
        float mx = velocity.x * elpsedTime;
        float mz = velocity.z * elpsedTime;
        //���C�̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = DirectX::XMFLOAT3{ position.x,position.y + step0ffeset,position.z };
        DirectX::XMFLOAT3 end = DirectX::XMFLOAT3{ position.x + mx, position.y + step0ffeset, position.z + mz };

        //���C�L���X�g�ɂ��ǔ���
        HitResult hit;
#if 1
        if (StageManager::Instance().RayCast(start, end, hit))
        {

            //�ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            //DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //�ǂ̖@��
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //���˃x�N�g����@���Ɏˉe
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);
            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            //�␳�ʒu�̌v�Z
            DirectX::XMVECTOR S = DirectX::XMVectorScale(Normal, dot * 1.1f);
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));

#if 0
            //���˃x�N�g��
            DirectX::XMVECTOR S = DirectX::XMVectorScale(Normal, dot * 2.0f);
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
            DirectX::XMVECTOR refurect = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&p), DirectX::XMLoadFloat3(&hit.position));
#endif
            //hit2.position�Ƃ��Ń��C���쐬���Ă���Ƀ��C�L���X�g
            HitResult hit2;
            if (StageManager::Instance().RayCast(hit.position, p, hit2))
            {

                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
            else
            {

                position.x = p.x;
                position.z = p.z;
            }

        }
        else if (position.x < -17.9f)//�X�e�[�W�̃v���C���[�ړ�����
        {
            position.x = -17.89f;
            position.z += mz;
        }
        else if (position.x > 17.9f)
        {
            position.x = 17.89f;
            position.z += mz;
        }
        else if (position.z < -12.0f)//�X�e�[�W�̃v���C���[�ړ�����
        {
            position.z = -11.999f;
            position.x += mx;
        }
        else if (position.z > 12.0f)
        {
            position.z = 11.999f;
            position.x += mx;
        }
        else
        {
            //�ړ�
            position.x += mx;
            position.z += mz;
        }
#endif
    }



}

bool Character::ApplyDamage(int damage, float invincibleTime)
{

    if (damage == 0)return false;
    if (health <= 0)return false;

    if (invincibleTime_ > 0.0f)return false;
    invincibleTime_ = invincibleTime;
    health -= damage;



    if (health <= 0)//���S�ʒm
    {
        OnDead();
    }
    else//�_���[�W�ʒm
    {
        OnDamege();
    }
    return true;
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

void Character::UpdateInvincibletime(float elapsedTime)
{
    if (invincibleTime_ >= 0.0f)
    {
        invincibleTime_ -= elapsedTime;
    }
}
