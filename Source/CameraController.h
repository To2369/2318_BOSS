#pragma once

#include<DirectXMath.h>

//カメラコントローラー
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
    //更新処理
    void Update(float elapsedTime);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; };
    void SetMaxAngle(float MaxAngle) { maxAngleX = DirectX::XMConvertToRadians(MaxAngle); }
    void SetMinAngle(float MinAngle) { minAngleX = DirectX::XMConvertToRadians(MinAngle); }
private:
    float BlendRate(bool& old_flag, bool changeflag, bool& flag, float elapsedTime);
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };

    int Changecount = 0;//ゲーム開始時にブレンド計算をしないよう、縛るための変数
    float             rollSpeed = DirectX::XMConvertToRadians(90);
    float             range = 10.0f;
    bool flag = false; //切り替わったか確認フラグ
    //X軸回転制限
    float             maxAngleX = DirectX::XMConvertToRadians(60);
    float             minAngleX = DirectX::XMConvertToRadians(-60);
    DirectX::XMFLOAT2 cameraangle{};//マウス用
    DirectX::XMFLOAT3 old_Pos{};//位置保存用
    DirectX::XMFLOAT3 origin_Pos{};//新しい位置
    DirectX::XMFLOAT3 eye{};
    float Rate{};
    float blendtimer{};
    float blendsecond = 0.2f;
    bool old_changeflag = false;
};
