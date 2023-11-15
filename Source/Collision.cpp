#include "collision.h"

//球と球の交差判定


bool Collision::InstersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,//Player
    float radiusA,
    const DirectX::XMFLOAT3& positionB,//enemy
    float radiusB,
    DirectX::XMFLOAT3& outposition
    )
{
    //A->Bの単位ベクトルを算出

    //positionAをXMVECTORに変換
   DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    //positionBをXMVECTORに変換
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    //positionAからpositionBへのベクトルを計算(positionB-positionA)
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract( PositionB ,PositionA);
    //vecの長さを計算
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(vec);

    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    //radiusAとradiusBの長さ
    float range = radiusA+radiusB;

    if (lengthSq>range)
    {
        return false;
    }
    //当たっている

    //vec方向の単位ベクトルを取得
    vec=DirectX::XMVector3Normalize(vec);
    //range 分スケールしたベクトルを計算
    vec=DirectX::XMVectorScale(vec, range);
    //そのベクトルを位置Aからのたした位置に移動させ
    vec =DirectX::XMVectorAdd(PositionA,vec);
    //出力用の位置に代入する
    DirectX::XMStoreFloat3(&outposition, vec);

    return true;
}

bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA, 
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outpositionB)
{
    
    //Aの足元がBの頭より上なら当たってない

    if (positionB.y+heightB<positionA.y)
    {
        return false;
    }

    if (positionA.y + heightA < positionB.y)
    {

        return false;
    }

    DirectX::XMFLOAT2 posA = { positionA.x,positionA.z };
    DirectX::XMFLOAT2 posB = { positionB.x,positionB.z };
    // XZ平面上での範囲チェック
  
   //positionAをXMVECTORに変換
    DirectX::XMVECTOR PosA = DirectX::XMLoadFloat2(&posA);
   //positionBをXMVECTORに変換
    DirectX::XMVECTOR PosB = DirectX::XMLoadFloat2(&posB);
   //positionAからpositionBへのベクトルを計算(positionB-positionA)
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(PosB, PosA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2Length(vec);
   //vecの長さを計算
   float lengthSq;
   DirectX::XMStoreFloat(&lengthSq, LengthSq);
   //positionB-positionAの距離が
    //radiusA+radiusBより大きかったら
   float range = radiusB + radiusA;
    if (lengthSq>range)
    {
        return false;
    }
    //vec方向の単位ベクトルを取得
    vec = DirectX::XMVector2Normalize(vec);
    //range 分スケールしたベクトルを計算
    vec = DirectX::XMVectorScale(vec, range);
    //そのベクトルを位置Aからのたした位置に移動させ
    vec = DirectX::XMVectorAdd(PosA, vec);
    //出力用の位置に代入する
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, vec);

    outpositionB.x = resultPos.x;
    outpositionB.y = positionB.y;
    outpositionB.z = resultPos.y;

    return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition,//球
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,//エネミー
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition)
{


    if (spherePosition.y + sphereRadius < cylinderPosition.y)
    {

        return false;
    }
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
    {

        return false;
    }

#if 1
    DirectX::XMFLOAT2 sphere_pos = { spherePosition.x,spherePosition.z };//球
    DirectX::XMFLOAT2 cylinder_pos = { cylinderPosition.x, cylinderPosition.z };//エネミー

     //sphere_posをXMVECTORに変換
    DirectX::XMVECTOR Sphere_Pos = DirectX::XMLoadFloat2(&sphere_pos);
    //cylinder_posをXMVECTORに変換
    DirectX::XMVECTOR Cylinder_Pos = DirectX::XMLoadFloat2(&cylinder_pos);
    //Sphere_PosからCylinder_Posへのベクトルを計算(Sphere_Pos-Cylinder_Pos)
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Cylinder_Pos,Sphere_Pos);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    float range = cylinderRadius + sphereRadius;
    if (lengthSq > range)
    {
        return false;
    }
    //vec方向の単位ベクトルを取得
    Vec = DirectX::XMVector2Normalize(Vec);
    //range分スケールしたベクトルを計算
    Vec = DirectX::XMVectorScale(Vec, range);
    //そのベクトルを位置Aからのたした位置に移動させ
    Vec = DirectX::XMVectorAdd(Sphere_Pos, Vec);
    //出力用の位置に代入する
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);
#endif 
#if 0
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float range = sphereRadius + cylinderRadius;
    float distXZ = sqrtf(vx * vx + vz * vz);
    if (distXZ > range)
    {
        return false;
    }
    vx /= distXZ;
    vz /= distXZ;
   //
#endif//試し

    outCylinderPosition.x = spherePosition.x + (resultPos.x * range);
    outCylinderPosition.y = outCylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + (resultPos.y * range);
    return true;
}


inline DirectX::XMFLOAT3 GetTriangleNormVector(DirectX::XMFLOAT3 p0,
    DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
    //①三角形trの法線nを求める
    //※３点が時計回りに見える側を表とする
    DirectX::XMFLOAT3 n = {};
    DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
    DirectX::XMVECTOR P0 = DirectX::XMLoadFloat3(&p0);
    DirectX::XMVECTOR P1 = DirectX::XMLoadFloat3(&p1);
    DirectX::XMVECTOR P2 = DirectX::XMLoadFloat3(&p2);

    N = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(P1, P0), DirectX::XMVectorSubtract(P2, P1));
    N = DirectX::XMVector3Normalize(N);
    DirectX::XMStoreFloat3(&n, N);
    return n;
}
struct Ray
{
    DirectX::XMFLOAT3	p;	// 光線の始点
    DirectX::XMFLOAT3	d;	// 光線の方向
    float				l;	// 光線の長さ
};
struct Triangle
{
    DirectX::XMFLOAT3	p0;
    DirectX::XMFLOAT3	p1;
    DirectX::XMFLOAT3	p2;
};

bool CollideRayVsTriangle(DirectX::XMFLOAT3* p, const Ray& r, const Triangle& t,float &neart)
{
    //③光線rと三角形tとの交点pを求める(非交差時は0.0f,0.0f,0.0fとすること)
    //三角形の法線と光線の方向ベクトルの内積を計算（ベクトルが同じ方向を向いていたらプラスになり裏面）
   DirectX::XMFLOAT3 n = GetTriangleNormVector(t.p0, t.p1, t.p2);
    DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);//三角形の法線
    DirectX::XMVECTOR Rd = DirectX::XMLoadFloat3(&r.d);//光線の方向
    float fdot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(Rd, N));

    //内積がマイナスかどうか判定(表ならを続ける)
    if (fdot < 0.0f)
    {

        // 光線が真横から当たっていないか判定（fabs(fDot) > 1e-6f）
        if (fabs(fdot) > 1e-6f)
        {

            // 光線と三角形の交点までの光線上の距離を計算（スケール値の計算）
            FLOAT l;
            DirectX::XMVECTOR tmp =
                DirectX::XMVectorSubtract(XMLoadFloat3(&t.p0),DirectX:: XMLoadFloat3(&r.p));
            l = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tmp, DirectX::XMLoadFloat3(&n))) / fdot;
            
                if(l>neart)
                {
                    return false;
                }

            // 距離が０より大きいかどうか判定
            if (l > 0)
            {

                // 交点の座標位置を計算

                DirectX::XMStoreFloat3(p, DirectX::XMVectorAdd(XMLoadFloat3(&r.p), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&r.d), l)));
                DirectX::XMVECTOR P = DirectX::XMLoadFloat3(p);
                // 交点から三角形のそれぞれの点までのベクトルを計算
                DirectX::XMVECTOR tmp0 = DirectX::XMVectorSubtract(XMLoadFloat3(&t.p0), P); // t.p0 - p
                DirectX::XMVECTOR tmp1 = DirectX::XMVectorSubtract(XMLoadFloat3(&t.p1), P); // t.p1 - p
                DirectX::XMVECTOR tmp2 = DirectX::XMVectorSubtract(XMLoadFloat3(&t.p2), P); // t.p2 - p

                // 上記で計算したベクトルのそれぞれの外積を計算
                DirectX::XMVECTOR n0 = DirectX::XMVector3Cross(tmp0, tmp1); // t.p0 - p と t.p1 - p の外積
                DirectX::XMVECTOR n1 = DirectX::XMVector3Cross(tmp1, tmp2); // t.p1 - p と t.p2 - p の外積
                DirectX::XMVECTOR n2 = DirectX::XMVector3Cross(tmp2, tmp0); // t.p2 - p と t.p0 - p の外積

                // 計算した外積の値と三角形の内積がすべて同じ方向を向いていたら（内積の値がプラスなら）
                if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(n0, N)) > 0 &&
                    DirectX::XMVectorGetX(DirectX::XMVector3Dot(n1, N)) > 0 &&
                    DirectX::XMVectorGetX(DirectX::XMVector3Dot(n2, N)) > 0)
                {
                    neart = l;
                    // 光線が三角形に交差している
                    return true;
                }
            }
        }
    }
    //④光線rが三角形tの表側から交差し、かつ、交点pが三角形tの内部にある場合のみ、trueを返す


    *p = {};
    return false;
}


bool Collision::InstersectRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
{

#if 0
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        //メッシュノードを取得
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
        //レイをワールド空間からローカル空間への変換
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        //レイの始点
        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E,S);
        //レイの方向ベクトル
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);
        //三角形（面）との交差判定
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT>indices = mesh.indices;
        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {

                UINT index = subset.startIndex +i;
                //三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index+2));

                DirectX::XMFLOAT3 p;
                Ray r;
                DirectX::XMStoreFloat3(&r.p, S);
                DirectX::XMStoreFloat3(&r.d, V);
                DirectX::XMStoreFloat(&r.l,Length);
                
                Triangle t;
                t.p0 = a.position;
                t.p1 = b.position;
                t.p2 = c.position;
                if (CollideRayVsTriangle(&p,r,t,neart))
                {

                    HitPosition = DirectX::XMLoadFloat3(&p);
                    DirectX::XMFLOAT3 n = GetTriangleNormVector(t.p0, t.p1, t.p2);
                    HitNormal = DirectX::XMLoadFloat3(&n);
                    materialIndex = subset.materialIndex;
                }
                if (materialIndex >= 0)
                {
                    //ローカル空間からワールド空間へ変換
                    DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                        WorldTransform);
                    DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
                    DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
                    float dicetance;
                    DirectX::XMStoreFloat(&dicetance, WorldCrossLength);
                    //ヒット情報保存
                    if (result.distance > dicetance)
                    {

                        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                        result.distance = dicetance;
                        result.materialIndex = materialIndex;
                        DirectX::XMStoreFloat3(&result.position, WorldPosition);
                        DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                        hit = true;
                    }


                }
            

                
            }

        }
    }

#endif 

    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
      //メッシュノード取得
      const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
      //レイをワールド空間からローカル空間へ変換
      DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
      DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

      DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
      DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
      DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
      DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
      DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

      //レイの長さ
      float neart;
      DirectX::XMStoreFloat(&neart, Length);

      //三角形の（面）との交差判定 
      const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
      const std::vector<UINT>indices = mesh.indices;

      int materialIndex = -1;
      DirectX::XMVECTOR HitPosition;
      DirectX::XMVECTOR HitNormal;
      for (const ModelResource::Subset& subset : mesh.subsets)
      {
          for (UINT i = 0; i < subset.indexCount; i += 3)
          {
              UINT index = subset.startIndex + i;
              //三角形の頂点を抽出
              const ModelResource::Vertex& a = vertices.at(indices.at(index));
              const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
              const ModelResource::Vertex& c = vertices.at(indices.at(index+2));

              
              //三角形の三辺のベクトルを算出
              DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), DirectX::XMLoadFloat3(&a.position));
              DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), DirectX::XMLoadFloat3(&b.position));
              DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), DirectX::XMLoadFloat3(&c.position));
              //三角形の法線ベクトルを算出
              DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

              //内積の結果がプラスならば裏向き
              DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
              float d;
              DirectX::XMStoreFloat(&d, Dot);
              if (d >= 0) continue;

              //レイと平面の交点を算出
              DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), S);
              DirectX::XMVECTOR X =DirectX::XMVectorDivide(DirectX::XMVector3Dot(N,SA),Dot);
              float x;
              DirectX::XMStoreFloat(&x, X);
              if (x<.0f || x>neart)continue;//交点までの距離が今までに計算した最近距離より
                                            //大きいときはスキップ
              DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
              //交点が三角形の内側にあるか判定
              DirectX::XMVECTOR PA     = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), P);
              DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
              DirectX::XMVECTOR Dot1   = DirectX::XMVector3Dot(Cross1, N);
              float dot1;
              DirectX::XMStoreFloat(&dot1, Dot1);
              if (dot1 < 0.0f)continue;

              DirectX::XMVECTOR PB     = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), P);
              DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
              DirectX::XMVECTOR Dot2   = DirectX::XMVector3Dot(Cross2, N);
              float dot2;
              DirectX::XMStoreFloat(&dot2, Dot2);
              if (dot2 < 0.0f)continue;

              DirectX::XMVECTOR PC     = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), P);
              DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC,CA);
              DirectX::XMVECTOR Dot3   = DirectX::XMVector3Dot(Cross3, N);
              float dot3;
              DirectX::XMStoreFloat(&dot3, Dot3);
              if (dot3 < 0.0f)continue;

              //最近距離を更新
              neart = x;
              //交点と法線を更新
              HitPosition = P;
              HitNormal = N;
              materialIndex = subset.materialIndex;
          }
      }

      if (materialIndex >= 0)
      {
          //ローカル空間からワールド空間への変換
          DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                                                                             WorldTransform);
          DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
          DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
          float distance;
          DirectX::XMStoreFloat(&distance, WorldCrossLength);
          //ヒット情報保存
          if (result.distance > distance)
          {
              DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

              result.distance = distance;
              result.materialIndex = materialIndex;
              DirectX::XMStoreFloat3(&result.position, WorldPosition);
              DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
              hit = true;
          }
      }
    }




    return hit;
}
