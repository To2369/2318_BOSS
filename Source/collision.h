#pragma once
#include<DirectXMath.h>
#include"Graphics/Model.h"

//�q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 position = {0,0,0};//���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal = { 0,0,0 };//�Փ˂����|���S���̖@���x�N�g��
    float             distance = 0.0f;   //���C�̎n�_�����_�܂ł̋���
    int               materialIndex = -1;//�Փ˂����|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3 rotation={ 0,0,0 }; //��]��
};

//�R���W����
class Collision
{
 public:
      //���Ƌ��̌�������
/// <summary>
/// InstersectSphereVsSphere
/// </summary>
/// <param name="positionA">�ʒuA</param>
/// <param name="radiusA">���aA</param>
/// <param name="positionB">�ʒuB</param>
/// <param name="radiusB">���aB</param>
/// <param name="outposition">�����o��</param>
/// <returns></returns>
  static bool InstersectSphereVsSphere(
         const DirectX::XMFLOAT3& positionA,
         float radiusA,
         const DirectX::XMFLOAT3& positionB,
         float radiusB,
         DirectX::XMFLOAT3&outposition
         );

  //�~���Ɖ~���̌�������

  /// <summary>
  /// //�~���Ɖ~���̌�������
  /// </summary>
  /// <param name="positionA">�ʒuA</param>
  /// <param name="radius">���aA</param>
  /// <param name="height">����A</param>
  /// <param name="positionB">�ʒuB</param>
  /// <param name="radiusB">���aB</param>
  /// <param name="heightB">����B</param>
  /// <param name="outposition">�����o��</param>
  /// <returns></returns>
  static bool IntersectCylinderVsCylinder(
      const DirectX::XMFLOAT3& positionA,
      float radius,
      float height,
      const DirectX::XMFLOAT3& positionB,
      float radiusB,
      float heightB,
      DirectX::XMFLOAT3& outposition);


  //���Ɖ~���̌�������
  
  /// <summary>
  /// ���Ɖ~���̌�������
  /// </summary>
  /// <param name="spherePosition"></param>
  /// <param name="sphereRadius"></param>
  /// <param name="cylinderPosition"></param>
  /// <param name="cylinderRadius"></param>
  /// <param name="cylinderHeight"></param>
  /// <param name="outCylinderPosition"></param>
  /// <returns></returns>
  static bool IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition,
      float sphereRadius,
      const DirectX::XMFLOAT3& cylinderPosition,
      float cylinderRadius,
      float cylinderHeight,
      DirectX::XMFLOAT3& outCylinderPosition);
public:
    //���C���f���̌�������
    static bool InstersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result);

};
