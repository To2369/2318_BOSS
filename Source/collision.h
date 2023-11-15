#pragma once
#include<DirectXMath.h>
#include"Graphics/Model.h"

//ヒット結果
struct HitResult
{
    DirectX::XMFLOAT3 position = {0,0,0};//レイとポリゴンの交点
    DirectX::XMFLOAT3 normal = { 0,0,0 };//衝突したポリゴンの法線ベクトル
    float             distance = 0.0f;   //レイの始点から交点までの距離
    int               materialIndex = -1;//衝突したポリゴンのマテリアル番号
    DirectX::XMFLOAT3 rotation={ 0,0,0 }; //回転量
};

//コリジョン
class Collision
{
 public:
      //球と球の交差判定
/// <summary>
/// InstersectSphereVsSphere
/// </summary>
/// <param name="positionA">位置A</param>
/// <param name="radiusA">半径A</param>
/// <param name="positionB">位置B</param>
/// <param name="radiusB">半径B</param>
/// <param name="outposition">押し出し</param>
/// <returns></returns>
  static bool InstersectSphereVsSphere(
         const DirectX::XMFLOAT3& positionA,
         float radiusA,
         const DirectX::XMFLOAT3& positionB,
         float radiusB,
         DirectX::XMFLOAT3&outposition
         );

  //円柱と円柱の交差判定

  /// <summary>
  /// //円柱と円柱の交差判定
  /// </summary>
  /// <param name="positionA">位置A</param>
  /// <param name="radius">半径A</param>
  /// <param name="height">高さA</param>
  /// <param name="positionB">位置B</param>
  /// <param name="radiusB">半径B</param>
  /// <param name="heightB">高さB</param>
  /// <param name="outposition">押し出し</param>
  /// <returns></returns>
  static bool IntersectCylinderVsCylinder(
      const DirectX::XMFLOAT3& positionA,
      float radius,
      float height,
      const DirectX::XMFLOAT3& positionB,
      float radiusB,
      float heightB,
      DirectX::XMFLOAT3& outposition);


  //球と円柱の交差判定
  
  /// <summary>
  /// 球と円柱の交差判定
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
    //レイモデルの交差判定
    static bool InstersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result);

};
