#include"ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
   :Projectile(manager)
{
  //  model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Sword/Sword.mdl");
    //�\���T�C�Y
   // scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 3.0f;
}

ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{

    //��������
    lifeTime -= elapsedTime;
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

void ProjectileStraight::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

//����
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
                                const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}
