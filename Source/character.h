#pragma once
#include<DirectXMath.h>
#include"Graphics/Model.h"
//キャラ
class Character
{
public:
    Character() {};
    virtual ~Character() {};
    void UpdateTransform();
    //半径取得
    float getRadius()const { return radius; };
    //地面に着地しているか
    bool IsGrounf()const { return isGround; }
    //高さ取得
    float GetHeight()const { return height; }
    //ダメージを与える
    bool ApplyDamage(int damage, float invincibleTime);
    //衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);
    //健康状態取得
    int GetHealth()const { return health; };
    //最大健康状態取得
    int GetMaxHealth()const { return maxHealth; };
    float GetTimer() { return stateTime; }
private:
    //垂直速力更新処理
    void UpdateVerticalVelocity(float elpsedFrame);
    //垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);
    //水平速力更新処理
    void UpdateHorizontalVelocity(float elpsedFram);
    //水平移動更新処理
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
        1,0,0,0,//右方向
        0,1,0,0,//上方向
        0,0,1,0,//前方向

        0,0,0,1//位置
    };
    bool isGround = false;
    //ダメージを受けたときに呼ばれる
    virtual void OnDamege() {};
    //死亡したときに呼ばれる
    virtual void OnDead() {};
protected:
    //移動処理
    void Move(float vx, float vz, float speed);
    //void Move(float elapsedTime,float vx, float vz, float speed);
    //旋回処理
    void Trun(float elapsedTime, float vx, float vz, float speed);
    //ジャンプ処理
    void Jump(float speed);
    //速力処理更新
    void UpdateVelocity(float elpsedTime);
    //無敵時間更新
    void UpdateInvincibletime(float elapsedTime);
    //着地したときに呼ばれる
    virtual void OnLanding() {};
public:
    //位置取得
    const DirectX::XMFLOAT3& GetPosition()const { return position; };
    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; };
    //回転取得
    const DirectX::XMFLOAT3& GetAngle()const { return angle; };
    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; };
    //スケール取得
    const DirectX::XMFLOAT3& GetScale()const { return scale; };
    //スケール取得
    void  SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; };
    Model* GetCharacterModel() { return model; }
};


