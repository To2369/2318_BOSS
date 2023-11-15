#pragma once
#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"character.h"
#include"ProjectileManager.h"
#include <Effeck.h>

//プレイヤー
class player :public Character
{
protected:
    //着地したときに呼ばれる
    void OnLanding()override;
private:
    struct PerspectiveChangeFlag
    {
        bool TPS = false;
        bool FPS = false;
    };
    PerspectiveChangeFlag perspectivechange;//TPS,FPSの視点切り替えようクラス
public:
    static player& instance();
    static float furic;
    static float accele;
    player();
    ~player()override;
    //更新処理
    void Update(float elapsedTime);
    //弾丸と敵の衝突判定
    void ColiisionProjectileVsEnemy();
    //デバッグ用GUI描画
    void DrawDebugGUI();
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    //デバッグプリミティブ表示
    void DarwDebugPrimitive();
    //弾丸入力処理
    void InputProjectile();
    //ジャンプ入力処理
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
 
    //移動入力処理
    bool InputMove(float elapsedTime);
    void TransitionIdleState();
    void UpdateIdleState(float elapsedTime);
    void CollisionplayerVsEnemies();
    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec()const;
  
    //移動ステート
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
   
    //ノード衝突処理
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
        head,//頭
        armar,//胴体
        rightarm,//右腕
        leftarm,//左腕
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
