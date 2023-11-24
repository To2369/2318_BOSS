#pragma once
#include "Graphics/Shader.h"
#include"Graphics/Model.h"
//�O���錾
class ProjectileManager;


//�e��
class Projectile
{
public:
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {};

    //�X�V����
    virtual void Update(float elpsedTime) = 0;

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawdebugPrimitive();

    //�ʒu���擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; };

    //�����擾
    const DirectX::XMFLOAT3& GetDirection()const { return direction; };

    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale()const { return scale; };

    //���a�擾
    float GetRadius()const { return radius; }

    void SetAdjustPos(float posZ) { adjustPos = posZ; }
    float GetAdjustPos() { return adjustPos; }//�e�̓����蔻��̈ʒu

    //�j��
    void Destroy();
protected:
    //�s��X�V����
    void UpdateTransform();
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };//�ʒu
    DirectX::XMFLOAT3 direction = { 0,0,1 };//����
    DirectX::XMFLOAT3 scale = { 1,1,1 };//�g�k
    DirectX::XMFLOAT4X4 transform =
    { 1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1 };
    float adjustPos = 1.433;
    float radius = 0.1f;
    Model* model = nullptr;
    ProjectileManager* mgr = nullptr;
};