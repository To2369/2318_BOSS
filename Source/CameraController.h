#pragma once

#include<DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
    CameraController() {};
    ~CameraController() {};
    void DrawDebugGUI();
    //更新処理
    void Update(float elapsedTime);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; };
private:
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    DirectX::XMFLOAT3 camerapos{0,0,0};
    DirectX::XMFLOAT3 cameraAngle{0,0,0};
    float             rollSpeed = DirectX::XMConvertToRadians(90);
    float             range = 10.0f;
    //X軸回転制限
    float             maxAngleX = DirectX::XMConvertToRadians(45);
    float             minAngleX = DirectX::XMConvertToRadians(-45);
    

};
