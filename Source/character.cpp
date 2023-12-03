#include "character.h"
#include"player.h"
#include"StageManager.h"
#include "mathf.h"
#include "enemySlaime.h"

//行列変換処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列作成
   // DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //３つの行列を組み合わせて、ワールド座標を作成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);



}



void Character::Move(float vx, float vz, float speed)
{
    //移動方向ベクトル
    moveVecX = vx;
    moveVecZ = vz;
    //最大速度チェック
    maxMoveSpeed = speed;
}

void Character::Trun(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    float length = sqrtf(vx * vx + vz * vz);
    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (length < 0.001f)
    {
        return;
    }

    //進行ベクトルを単位化
    vx /= length;
    vz /= length;

    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);//PLAYER
    float frontZ = cosf(angle.y);//PLAYER
                             //vx camera
                             //vy camera

    //回転角を求めるために２つの単位ベクトルの外積を計算する

    float dot = (frontX * vx) + (frontZ * vz);
    float rot = 1.0f - dot;

    if (rot > speed)
    {
        rot = speed;
    }
    //左右判定を行うために２つの単位ベクトルの外積を計算する
    float croos = (frontZ * vx) - (frontX * vz);

    //２Dの外積判定が正の場合か負の場合によって左右判定が行える
    //左右判定を行う事によって左右判定を選択する
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
    //上方向の力を設定
    velocity.y = speed;
}

void Character::UpdateVelocity(float elpsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elpsedTime;
    //垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);
    UpdateHorizontalVelocity(elapsedFrame);
    //垂直移動処理更新処理
    UpdateVerticalMove(elpsedTime);
    UpdateHorizontalMove(elpsedTime);
}
void Character::UpdateVerticalVelocity(float elpsedFrame)
{
    //重力処理
    velocity.y += gravity * elpsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    //垂直方向の移動量
    float my = velocity.y * elapsedTime;
    slopeRate = 0.0f;
    //キャラクターのY軸方向と異なる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,1,0 };
    //落下中
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + step0ffeset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my - 0.2f,position.z };
        //レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            normal = hit.normal;
            //地面に接地している
            position = hit.position;
            angle.y += hit.rotation.y;
            //傾斜率の計算
          /*  float normalLengthZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthZ + hit.normal.y));*/
            //着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
    //地面の向きに沿うようにXZ軸回転
    {
        //Y軸法線ベクトル方向に向くオイラー角回転を算出する
        float x = atan2f(normal.z, normal.y);
        float z = -atan2f(normal.x, normal.y);

        angle.x = Mathf::Lerp(angle.x, x, 0.1f);
        angle.z = Mathf::Lerp(angle.z, z, 0.1f);

    }

}

void Character::UpdateHorizontalVelocity(float elpsedFram)
{
    //XZ平面の速力を減速する
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    DirectX::XMFLOAT2 vel = { velocity.x,velocity.z };
    DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&vel);
    if (length > 0.0f)
    {
        //摩擦力
        float friction = this->friction * elpsedFram;
        if (!isGround)
        {
            friction *= airControl;

        }
        player::furic = friction;
        //摩擦による横方向の減速
        if (length > friction)
        {
            //速度を単位ベクトル化
            auto v = DirectX::XMVector2Normalize(vec);
            //単位ベクトル化した速度をfriction方向へスケーリング
            /*velocityX = velocityX * friction;
            velocityZ = velocityZ * friction;*/
            v = DirectX::XMVectorScale(v, friction);


            //スケーリングした
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
    //XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        //移動ベクトルがゼロベクトルでないなら加速する
        float moveVeclength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        if (moveVeclength > 0.0f)
        {
            //加速力
            float acceleration = this->acceleration * elpsedFram;
            if (!isGround)
            {
                acceleration *= airControl;
            }
            player::accele = acceleration;
            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //最大速度制限
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
    ////移動処理
   /* position.x+=velocity.x*elpsedTime;
    position.z+=velocity.z*elpsedTime;*/

    //水平速力計算
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elpsedTime;
        float mz = velocity.z * elpsedTime;
        //レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = DirectX::XMFLOAT3{ position.x,position.y + step0ffeset,position.z };
        DirectX::XMFLOAT3 end = DirectX::XMFLOAT3{ position.x + mx, position.y + step0ffeset, position.z + mz };

        //レイキャストによる壁判定
        HitResult hit;
#if 1
        if (StageManager::Instance().RayCast(start, end, hit))
        {

            //壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            //DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);
            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            //補正位置の計算
            DirectX::XMVECTOR S = DirectX::XMVectorScale(Normal, dot * 1.1f);
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));

#if 0
            //反射ベクトル
            DirectX::XMVECTOR S = DirectX::XMVectorScale(Normal, dot * 2.0f);
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
            DirectX::XMVECTOR refurect = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&p), DirectX::XMLoadFloat3(&hit.position));
#endif
            //hit2.positionとｐでレイを作成してさらにレイキャスト
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
        else if (position.x < -17.9f)//ステージのプレイヤー移動制限
        {
            position.x = -17.89f;
            position.z += mz;
        }
        else if (position.x > 17.9f)
        {
            position.x = 17.89f;
            position.z += mz;
        }
        else if (position.z < -12.0f)//ステージのプレイヤー移動制限
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
            //移動
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



    if (health <= 0)//死亡通知
    {
        OnDead();
    }
    else//ダメージ通知
    {
        OnDamege();
    }
    return true;
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //速力に力を加える
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
