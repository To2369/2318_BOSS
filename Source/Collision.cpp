#include "collision.h"

//���Ƌ��̌�������


bool Collision::InstersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,//Player
    float radiusA,
    const DirectX::XMFLOAT3& positionB,//enemy
    float radiusB,
    DirectX::XMFLOAT3& outposition
    )
{
    //A->B�̒P�ʃx�N�g�����Z�o

    //positionA��XMVECTOR�ɕϊ�
   DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    //positionB��XMVECTOR�ɕϊ�
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    //positionA����positionB�ւ̃x�N�g�����v�Z(positionB-positionA)
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract( PositionB ,PositionA);
    //vec�̒������v�Z
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(vec);

    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    //radiusA��radiusB�̒���
    float range = radiusA+radiusB;

    if (lengthSq>range)
    {
        return false;
    }
    //�������Ă���

    //vec�����̒P�ʃx�N�g�����擾
    vec=DirectX::XMVector3Normalize(vec);
    //range ���X�P�[�������x�N�g�����v�Z
    vec=DirectX::XMVectorScale(vec, range);
    //���̃x�N�g�����ʒuA����̂������ʒu�Ɉړ�����
    vec =DirectX::XMVectorAdd(PositionA,vec);
    //�o�͗p�̈ʒu�ɑ������
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
    
    //A�̑�����B�̓�����Ȃ瓖�����ĂȂ�

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
    // XZ���ʏ�ł͈̔̓`�F�b�N
  
   //positionA��XMVECTOR�ɕϊ�
    DirectX::XMVECTOR PosA = DirectX::XMLoadFloat2(&posA);
   //positionB��XMVECTOR�ɕϊ�
    DirectX::XMVECTOR PosB = DirectX::XMLoadFloat2(&posB);
   //positionA����positionB�ւ̃x�N�g�����v�Z(positionB-positionA)
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(PosB, PosA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2Length(vec);
   //vec�̒������v�Z
   float lengthSq;
   DirectX::XMStoreFloat(&lengthSq, LengthSq);
   //positionB-positionA�̋�����
    //radiusA+radiusB���傫��������
   float range = radiusB + radiusA;
    if (lengthSq>range)
    {
        return false;
    }
    //vec�����̒P�ʃx�N�g�����擾
    vec = DirectX::XMVector2Normalize(vec);
    //range ���X�P�[�������x�N�g�����v�Z
    vec = DirectX::XMVectorScale(vec, range);
    //���̃x�N�g�����ʒuA����̂������ʒu�Ɉړ�����
    vec = DirectX::XMVectorAdd(PosA, vec);
    //�o�͗p�̈ʒu�ɑ������
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, vec);

    outpositionB.x = resultPos.x;
    outpositionB.y = positionB.y;
    outpositionB.z = resultPos.y;

    return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition,//��
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,//�G�l�~�[
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
    DirectX::XMFLOAT2 sphere_pos = { spherePosition.x,spherePosition.z };//��
    DirectX::XMFLOAT2 cylinder_pos = { cylinderPosition.x, cylinderPosition.z };//�G�l�~�[

     //sphere_pos��XMVECTOR�ɕϊ�
    DirectX::XMVECTOR Sphere_Pos = DirectX::XMLoadFloat2(&sphere_pos);
    //cylinder_pos��XMVECTOR�ɕϊ�
    DirectX::XMVECTOR Cylinder_Pos = DirectX::XMLoadFloat2(&cylinder_pos);
    //Sphere_Pos����Cylinder_Pos�ւ̃x�N�g�����v�Z(Sphere_Pos-Cylinder_Pos)
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Cylinder_Pos,Sphere_Pos);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    float range = cylinderRadius + sphereRadius;
    if (lengthSq > range)
    {
        return false;
    }
    //vec�����̒P�ʃx�N�g�����擾
    Vec = DirectX::XMVector2Normalize(Vec);
    //range���X�P�[�������x�N�g�����v�Z
    Vec = DirectX::XMVectorScale(Vec, range);
    //���̃x�N�g�����ʒuA����̂������ʒu�Ɉړ�����
    Vec = DirectX::XMVectorAdd(Sphere_Pos, Vec);
    //�o�͗p�̈ʒu�ɑ������
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
#endif//����

    outCylinderPosition.x = spherePosition.x + (resultPos.x * range);
    outCylinderPosition.y = outCylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + (resultPos.y * range);
    return true;
}


inline DirectX::XMFLOAT3 GetTriangleNormVector(DirectX::XMFLOAT3 p0,
    DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
    //�@�O�p�`tr�̖@��n�����߂�
    //���R�_�����v���Ɍ����鑤��\�Ƃ���
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
    DirectX::XMFLOAT3	p;	// �����̎n�_
    DirectX::XMFLOAT3	d;	// �����̕���
    float				l;	// �����̒���
};
struct Triangle
{
    DirectX::XMFLOAT3	p0;
    DirectX::XMFLOAT3	p1;
    DirectX::XMFLOAT3	p2;
};

bool CollideRayVsTriangle(DirectX::XMFLOAT3* p, const Ray& r, const Triangle& t,float &neart)
{
    //�B����r�ƎO�p�`t�Ƃ̌�_p�����߂�(���������0.0f,0.0f,0.0f�Ƃ��邱��)
    //�O�p�`�̖@���ƌ����̕����x�N�g���̓��ς��v�Z�i�x�N�g�������������������Ă�����v���X�ɂȂ藠�ʁj
   DirectX::XMFLOAT3 n = GetTriangleNormVector(t.p0, t.p1, t.p2);
    DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);//�O�p�`�̖@��
    DirectX::XMVECTOR Rd = DirectX::XMLoadFloat3(&r.d);//�����̕���
    float fdot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(Rd, N));

    //���ς��}�C�i�X���ǂ�������(�\�Ȃ�𑱂���)
    if (fdot < 0.0f)
    {

        // �������^�����瓖�����Ă��Ȃ�������ifabs(fDot) > 1e-6f�j
        if (fabs(fdot) > 1e-6f)
        {

            // �����ƎO�p�`�̌�_�܂ł̌�����̋������v�Z�i�X�P�[���l�̌v�Z�j
            FLOAT l;
            DirectX::XMVECTOR tmp =
                DirectX::XMVectorSubtract(XMLoadFloat3(&t.p0),DirectX:: XMLoadFloat3(&r.p));
            l = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tmp, DirectX::XMLoadFloat3(&n))) / fdot;
            
                if(l>neart)
                {
                    return false;
                }

            // �������O���傫�����ǂ�������
            if (l > 0)
            {

                // ��_�̍��W�ʒu���v�Z

                DirectX::XMStoreFloat3(p, DirectX::XMVectorAdd(XMLoadFloat3(&r.p), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&r.d), l)));
                DirectX::XMVECTOR P = DirectX::XMLoadFloat3(p);
                // ��_����O�p�`�̂��ꂼ��̓_�܂ł̃x�N�g�����v�Z
                DirectX::XMVECTOR tmp0 = DirectX::XMVectorSubtract(XMLoadFloat3(&t.p0), P); // t.p0 - p
                DirectX::XMVECTOR tmp1 = DirectX::XMVectorSubtract(XMLoadFloat3(&t.p1), P); // t.p1 - p
                DirectX::XMVECTOR tmp2 = DirectX::XMVectorSubtract(XMLoadFloat3(&t.p2), P); // t.p2 - p

                // ��L�Ōv�Z�����x�N�g���̂��ꂼ��̊O�ς��v�Z
                DirectX::XMVECTOR n0 = DirectX::XMVector3Cross(tmp0, tmp1); // t.p0 - p �� t.p1 - p �̊O��
                DirectX::XMVECTOR n1 = DirectX::XMVector3Cross(tmp1, tmp2); // t.p1 - p �� t.p2 - p �̊O��
                DirectX::XMVECTOR n2 = DirectX::XMVector3Cross(tmp2, tmp0); // t.p2 - p �� t.p0 - p �̊O��

                // �v�Z�����O�ς̒l�ƎO�p�`�̓��ς����ׂē��������������Ă�����i���ς̒l���v���X�Ȃ�j
                if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(n0, N)) > 0 &&
                    DirectX::XMVectorGetX(DirectX::XMVector3Dot(n1, N)) > 0 &&
                    DirectX::XMVectorGetX(DirectX::XMVector3Dot(n2, N)) > 0)
                {
                    neart = l;
                    // �������O�p�`�Ɍ������Ă���
                    return true;
                }
            }
        }
    }
    //�C����r���O�p�`t�̕\������������A���A��_p���O�p�`t�̓����ɂ���ꍇ�̂݁Atrue��Ԃ�


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

    //���[���h��Ԃ̃��C�̒���
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        //���b�V���m�[�h���擾
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
        //���C�����[���h��Ԃ��烍�[�J����Ԃւ̕ϊ�
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        //���C�̎n�_
        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E,S);
        //���C�̕����x�N�g��
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, Length);
        //�O�p�`�i�ʁj�Ƃ̌�������
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
                //�O�p�`�̒��_�𒊏o
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
                    //���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
                    DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                        WorldTransform);
                    DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
                    DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
                    float dicetance;
                    DirectX::XMStoreFloat(&dicetance, WorldCrossLength);
                    //�q�b�g���ۑ�
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

    //���[���h��Ԃ̃��C�̒���
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
      //���b�V���m�[�h�擾
      const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
      //���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
      DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
      DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

      DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
      DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
      DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
      DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
      DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

      //���C�̒���
      float neart;
      DirectX::XMStoreFloat(&neart, Length);

      //�O�p�`�́i�ʁj�Ƃ̌������� 
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
              //�O�p�`�̒��_�𒊏o
              const ModelResource::Vertex& a = vertices.at(indices.at(index));
              const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
              const ModelResource::Vertex& c = vertices.at(indices.at(index+2));

              
              //�O�p�`�̎O�ӂ̃x�N�g�����Z�o
              DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), DirectX::XMLoadFloat3(&a.position));
              DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), DirectX::XMLoadFloat3(&b.position));
              DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), DirectX::XMLoadFloat3(&c.position));
              //�O�p�`�̖@���x�N�g�����Z�o
              DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

              //���ς̌��ʂ��v���X�Ȃ�Η�����
              DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
              float d;
              DirectX::XMStoreFloat(&d, Dot);
              if (d >= 0) continue;

              //���C�ƕ��ʂ̌�_���Z�o
              DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), S);
              DirectX::XMVECTOR X =DirectX::XMVectorDivide(DirectX::XMVector3Dot(N,SA),Dot);
              float x;
              DirectX::XMStoreFloat(&x, X);
              if (x<.0f || x>neart)continue;//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
                                            //�傫���Ƃ��̓X�L�b�v
              DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
              //��_���O�p�`�̓����ɂ��邩����
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

              //�ŋߋ������X�V
              neart = x;
              //��_�Ɩ@�����X�V
              HitPosition = P;
              HitNormal = N;
              materialIndex = subset.materialIndex;
          }
      }

      if (materialIndex >= 0)
      {
          //���[�J����Ԃ��烏�[���h��Ԃւ̕ϊ�
          DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                                                                             WorldTransform);
          DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
          DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
          float distance;
          DirectX::XMStoreFloat(&distance, WorldCrossLength);
          //�q�b�g���ۑ�
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
