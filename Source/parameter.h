#pragma once

namespace Param
{
    static float  CameraMaxAngle;
    static float  CameraMinAngle;

    
    struct PlayerParam
    {
        float MoveSpeed = 0;//�v���C���[�̈ړ����x
        float DamegeParam = 0;//�e�̃_���[�W���l
        float Hp = 3;//�v���C���[HP
    };
    static   PlayerParam player_param{};


};
namespace Debugparam
{
  static  float frameRateCheck = 0;
  
};