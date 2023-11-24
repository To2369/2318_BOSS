#pragma once

namespace Param
{
    static float  CameraMaxAngle;
    static float  CameraMinAngle;

    
    struct PlayerParam
    {
        float MoveSpeed = 0;//プレイヤーの移動速度
        float DamegeParam = 0;//弾のダメージ数値
        float Hp = 3;//プレイヤーHP
    };
    static   PlayerParam player_param{};


};
namespace Debugparam
{
  static  float frameRateCheck = 0;
  
};