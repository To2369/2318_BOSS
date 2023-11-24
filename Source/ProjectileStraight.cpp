#include"ProjectileStraight.h"
#include"PlayerManager.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
    :Projectile(manager)
{
    //  model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Arrow/Arrow.mdl");
    //�\���T�C�Y
   // scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 0.001f;
}

ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
    player* pl = PlayerManager::Instance().GetPlayer(0);

    //��������
   // lifeTime -= elapsedTime;
    lifeTime = 3;
    if (lifeTime <= 0)
    {
        //�������폜
        Destroy();

    }

    //�ړ�
    float speed = this->speed * elapsedTime;
    //x�̈ړ�����(�ʒu�{������������)
    position.x += direction.x * speed;
    //y�̈ړ�����
    position.y += direction.y * speed;
    //z�̈ړ�����
    position.z += direction.z * speed;

    /* DirectX::XMVECTOR dvec = DirectX::XMLoadFloat3(&direction);
     DirectX::XMVECTOR pvec = DirectX::XMLoadFloat3(&position);
     DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(pvec, DirectX::XMVectorScale(dvec, speed)));*/
     //�I�u�W�F�N�g�̍X�V����
    UpdateTransform();
    //���f���s��X�V
    model->UpdateTransform(transform);

}

void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

//����
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
    const DirectX::XMFLOAT3& position,
    float speed_)
{
    this->speed = speed_;
    this->direction = direction;
    this->position = position;
}
