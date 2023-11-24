#pragma once

#include<DirectXMath.h>

//�J�����R���g���[���[
class CameraController
{
public:
    CameraController() {};
    ~CameraController() {};

    static CameraController& incetance()
    {
        static CameraController ins;
        return ins;
    }
    void DrawDebugGUI();
    //�X�V����
    void Update(float elapsedTime);

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; };
    void SetMaxAngle(float MaxAngle) { maxAngleX = DirectX::XMConvertToRadians(MaxAngle); }
    void SetMinAngle(float MinAngle) { minAngleX = DirectX::XMConvertToRadians(MinAngle); }
private:
    float BlendRate(bool& old_flag, bool changeflag, bool& flag, float elapsedTime);
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };

    int Changecount = 0;//�Q�[���J�n���Ƀu�����h�v�Z�����Ȃ��悤�A���邽�߂̕ϐ�
    float             rollSpeed = DirectX::XMConvertToRadians(90);
    float             range = 10.0f;
    bool flag = false; //�؂�ւ�������m�F�t���O
    //X����]����
    float             maxAngleX = DirectX::XMConvertToRadians(60);
    float             minAngleX = DirectX::XMConvertToRadians(-60);
    DirectX::XMFLOAT2 cameraangle{};//�}�E�X�p
    DirectX::XMFLOAT3 old_Pos{};//�ʒu�ۑ��p
    DirectX::XMFLOAT3 origin_Pos{};//�V�����ʒu
    DirectX::XMFLOAT3 eye{};
    float Rate{};
    float blendtimer{};
    float blendsecond = 0.2f;
    bool old_changeflag = false;
};
