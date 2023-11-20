#include <imgui.h>
#include"player.h"
#include"Input/Input.h"
#include"camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "collision.h"
#include"ProjectileManager.h"
#include"ProjectileStraight.h"
#include"ProjectileHoming.h"
#include<string>
float player::accele;
float player::furic;
static player* instance_;

player&player::instance()
{
    return *instance_;
}
void player::OnLanding()
{
    jumpCount = 0;
    float vel = velocity.y;
   
    if (state!=State::Damege&&state!=State::Death)
    {
        TransitionLandState();
    }
  
}
player::player()
{
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    model = new Model("Data/Model/Jammo/Jammo.mdl");
    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;
    HitEffect = new Effect("Data/Effect/Hit.efk");
    TransitionIdleState();
    state = State::Idle;
    instance_ = this;
    invincibleTime_ = 0.0f;
    attackCollisionFlag = false;
  
}
 player::~player()
 {
     delete HitEffect;
     delete model;
 }
void player::Update(float elapsedTime)
{
   
  /*  GamePad& gamepad = Input::Instance().GetGamePad();
    if (gamepad.GetButtonDown() & GamePad::BTN_B)
    {
        model->playAnimetion(0, false);
    }
    if (!model->IsPlayerAnimetion())
    {
        model->playAnimetion(1, true);
    }*/
    
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Jump:

        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
    case State::Damege:
        UpdateDamegeState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }
    //���͍X�V����
    UpdateVelocity(elapsedTime);
    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);
    UpdateInvincibletime(elapsedTime);
    //�v���C���[�ƓG�Ƃ̏Փ˔���
    CollisionplayerVsEnemies();
    ColiisionProjectileVsEnemy();
    //�I�u�W�F�N�g�X�V����
    UpdateTransform();
    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);
    if (!model->IsPlayerAnimetion())
    {
     
    }
    //���f���s��X�V
    model->UpdateTransform(transform);
   
}

void player::TransitionIdleState()
{
    state = State::Idle;
    model->playAnimetion(Anim_Idle, true);
}

void player::UpdateIdleState(float elapsedTime)
{
    if (InputMove(elapsedTime))
    {

        TransitionMOveState();
    }
   
    if (InputJump())
    {
        TransitionJumpState();
    }
    if (InputAttack())
    {
        TransitionAttackState();
    }
    InputProjectile();
}

void player::CollisionplayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓ˔���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        

        if (Collision::IntersectCylinderVsCylinder(
            GetPosition(), //player�̈ʒu
            getRadius(),//player�̔��a
            GetHeight(),
            enemy->GetPosition(),//�G�̈ʒu
            enemy->getRadius(),//�G�̔��a
            enemy->GetHeight(),
            outPosition
        ))
        {

            enemy->SetPosition(outPosition);

        }
      
    }

}
DirectX::XMFLOAT3 player::GetMoveVec()const
{
    //���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    //�J�����������擾
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    //�ړ��x�N�g����XZ���ʂȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }
    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
        //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f��
        //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f��
        //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax)+(cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;
    return vec;
}


//�ړ����͏���
bool player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //�ړ�����
    //Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
     Move(moveVec.x, moveVec.z, moveSpeed);
    //���񏈗�
    Trun(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
    return moveVec.x > 0 || moveVec.y > 0 || moveVec.z > 0|| moveVec.x < 0 || moveVec.y < 0 || moveVec.z < 0;
}

void player::TransitionMOveState()
{
    state = State::Move;
    model->playAnimetion(Anim_Running, true);
}

void player::UpdateMoveState(float elapsedTime)
{
    if (InputJump())
    {
        TransitionJumpState();
        return;
    }
    if (!InputJump()&& !InputMove(elapsedTime))
    {
        TransitionIdleState();
    }
    if (InputAttack())
    {
        TransitionAttackState();
    }
    InputProjectile();
}

void player::TransitionJumpState()
{
    state = State::Jump;
    model->playAnimetion(Anim_Jump,false);
}

void player::UpdateJumpState(float elapsedTime)
{
    if (!InputMove(elapsedTime) && isGround)
    {
        TransitionIdleState();
    }
    if (!InputJump()&&isGround)
    {

        TransitionIdleState();
    }
   
    if (velocity.y < 0)
    {

        model->playAnimetion(Anim_Faling, true);
    }

    InputProjectile();
}

void player::TransitionLandState()
{
    state = State::Land;
    model->playAnimetion(Anim_landing, false);
}

void player::UpdateLandState(float elapsedTime)
{

    static float LandTimer=0;
    LandTimer += 0.02f;
    if (LandTimer > 0.433f)
    {
        if (!InputJump())
        {
            TransitionIdleState();
            LandTimer = 0;
        }
        if (!InputMove(elapsedTime))
        {
            TransitionIdleState();
        }
    }
    InputProjectile();
}

void player::TransitionDamegeState()
{
    state = State::Damege;
    model->playAnimetion(Anim_GetHit1,false);
}

void player::UpdateDamegeState(float elapsedTime)
{

    if (!model->IsPlayerAnimetion())
    {
        TransitionIdleState();
    }
}

void player::TransitionDeathState()
{
    state = State::Death;
    model->playAnimetion(Anim_Death, false);
}

void player::UpdateDeathState(float elapsedTime)
{

}

void player::CollisionNodeVsEnemies(const char* nodename, float nodeRadius)
{

    DirectX::XMFLOAT3 nodePosition = {};
    Model::Node* node = model->FindNode(nodename);
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;
    EnemyManager& enemy = EnemyManager::Instance();
    int count = EnemyManager::Instance().GetEnemyCount();
    
    for (int i = 0; i < count; i++)
    {
        Enemy* enemys = enemy.GetEnemy(i);
        DirectX::XMFLOAT3 outpos = {};
        if (Collision::IntersectSphereVsCylinder(nodePosition, nodeRadius,
            enemys->GetPosition(),
            enemys->getRadius(),
            enemys->GetHeight(),
            outpos))
        {
            //������΂�
            {   //������΂��ړ������x�N�g��
                DirectX::XMFLOAT3 impulse;
                //������΂���
                const float power = 2.0f;
                //�G�̈ʒu
                DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemys->GetPosition());
                //node�̈ʒu
                DirectX::XMVECTOR nodeVec = DirectX::XMLoadFloat3(&nodePosition);
                //node����G�ւ̕����x�N�g�����v�Z
                DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(enemyVec, nodeVec);
                //�����x�N�g���𐳋K��
                Vec = DirectX::XMVector3Normalize(Vec);
                DirectX::XMStoreFloat3(&impulse, Vec);
                //������΂��ړ������̑��x�x�N�g����power���|���Đݒ�
                impulse.x = impulse.x * power;
                impulse.y = impulse.y * power;
                impulse.z = impulse.z * power;

                enemys->AddImpulse(impulse);
                
            }
             if(enemys->ApplyDamage(1, 0.5f))//�q�b�g�G�t�F�N�g�Đ�
            {
                DirectX::XMFLOAT3 e = enemys->GetPosition();
                e.y += enemys->GetHeight() * 0.5f;
                HitEffect->Play(e);
            }


        }
    }
   
}



bool player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount == 1)
        {
            jumpCount += 1;
            Jump(jumpSpeed);
            model->playAnimetion(Anim_Jump_Flip, false);
            return true;

        }
        if (jumpCount < jumpLimit)
        {
            jumpCount += 1;
            Jump(jumpSpeed);
            return true;
        }
    }
    return false;
}

void player::OnDamege()
{
    TransitionDamegeState();
}

void player::OnDead()
{
    TransitionDeathState();
}

bool player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButton() & GamePad::BTN_B)
    {
        return true;
    }
    return false;
}

void player::TransitionAttackState()
{
    state = State::Attack;
    model->playAnimetion(Anim_Attack,false);

}

void player::UpdateAttackState(float elapsedTime)
{
    static float anime_atk_time = 0;
    anime_atk_time += 0.02f;
    if (anime_atk_time>0.800f)
    {
      
        anime_atk_time = 0;
        if (!InputAttack())
        {
            TransitionIdleState();
        }
    }
    float animationTime=model->GetAnimationTime();
    attackCollisionFlag = animationTime > 0.4f;
    if (attackCollisionFlag)
    {
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}



void player::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
    projectileManager.Render(rc, shader);
}
//�f�o�b�O�v���~�e�B�u�`��
void player::DarwDebugPrimitive()
{
    DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
    ////�Փ˔���p�̃f�o�b�O�ʂ�`��
    //debugRender->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRender->DrawCylinder(position, radius, height, DirectX::XMFLOAT4{ 0,0,0,1 });
    //�e�ۃf�o�b�O�v���~�e�B�u�`��
    projectileManager.DrawdebugPrimitive();

   
    if (attackCollisionFlag)
    {
        Model::Node* LeftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRender->DrawSphere(DirectX::XMFLOAT3(
            LeftHandBone->worldTransform._41,
            LeftHandBone->worldTransform._42,
            LeftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
                                               
}
void player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //���i�e�۔���
    DirectX::XMFLOAT3 dir{};
    DirectX::XMFLOAT3 pos{};
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        if(!GetPerspectiveChangeFlag().TPS)
        {
            //�O����
            dir.x = Camera::instance().GetFront().x;
            dir.y = Camera::instance().GetFront().y;
            dir.z = Camera::instance().GetFront().z;

           //���ˈʒu
            pos.x = position.x;
            pos.y = position.y + height * 0.5f;
            pos.z = position.z;

        }
        //����
        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
      //projectileManager.Register(projectile);
    }

    ////�ǔ��e�۔���
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    //{

    //    //�O����
    //    DirectX::XMFLOAT3 dir;
    //    dir.x = sinf(angle.y);//transform._31 * 100;
    //    dir.y = 0.0f;
    //    dir.z = cosf(angle.y);//transform._33 * 100;
    //    //���ˈʒu�i�v���C���[�̍�������j
    //    DirectX::XMFLOAT3 pos;
    //    pos.x = position.x;
    //    pos.y = position.y + height*0.5;
    //    pos.z = position.z;
    //    //�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O���j
    //    DirectX::XMFLOAT3 target;
    //    target.x = pos.x + dir.x * 1000.0f;
    //    target.y = pos.y + dir.y * 1000.0f;
    //    target.z = pos.z + dir.z * 1000.0f;
    //    //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
    //    float dist = FLT_MAX;
    //    EnemyManager& enemyMnager = EnemyManager::Instance();
    //    int enemyCount = enemyMnager.GetEnemyCount();
    //    for (int i = 0; i < enemyCount; i++)
    //    {
    //        //�G�Ƃ̋�������
    //        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //        //PLAYER�̈ʒu
    //        DirectX::XMVECTOR Player_pos = DirectX::XMLoadFloat3(&position);
    //        //ENEMY�̈ʒu
    //        DirectX::XMVECTOR Enemy_pos  = DirectX::XMLoadFloat3(&enemy->GetPosition());
    //        //player����enemy�܂ł̃x�N�g�����v�Z
    //        DirectX::XMVECTOR VEC        = DirectX::XMVectorSubtract(Enemy_pos, Player_pos);
    //        //���̃x�N�g���̒������v�Z
    //        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(VEC);
    //        float d;
    //        DirectX::XMStoreFloat(&d, D);
    //        //���ׂĂ̓G�̒��ň�ԋ߂��G���^�[�Q�b�g�ݒ�
    //        if (d < dist)
    //        {
    //            dist = d;
    //            target = enemy->GetPosition();
    //            target.y += enemy->GetHeight() * 0.5f;
    //        }

    //    }
    //    //����
    //    ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
    //    projectile->Launch(dir, pos, target);
    //}
    


}

//����
void player::ColiisionProjectileVsEnemy()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    //���ׂĂ̒e�ۂƑS�Ă̓G�𑍓�����ŏՓ˔���
    int progectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < progectileCount; ++i)
    {
        Projectile* projectile = projectile = projectileManager.GetProgectile(i);
        for (int j = 0; j < enemyCount; j++)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);
            //�Փ˔���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->getRadius(),
                enemy->GetHeight(),
                outPosition))
            {

                //�_���[�W��^����
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    //������΂�
                    {   //������΂��ړ������x�N�g��
                        DirectX::XMFLOAT3 impulse;
                        //������΂���
                        const float power = 10.0f;
                        //�G�̈ʒu
                        DirectX::XMVECTOR enemyVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                        //�e�̈ʒu
                        DirectX::XMVECTOR projectileVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
                        //�e����G�ւ̕����x�N�g�����v�Z
                        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(enemyVec, projectileVec);
                        //�����x�N�g���𐳋K��
                        Vec=DirectX::XMVector3Normalize(Vec);
                        DirectX::XMStoreFloat3(&impulse, Vec);
                        //������΂��ړ������̑��x�x�N�g����power���|���Đݒ�
                        impulse.x = impulse.x * power;
                        impulse.y = impulse.y * power;
                        impulse.z = impulse.z * power;
                   
                        enemy->AddImpulse(impulse);
                        if (enemy->ApplyDamage(1, 0.5f))
                        {
                            DirectX::XMFLOAT3 e = enemy->GetPosition();
                            e.y += enemy->GetHeight() * 0.5f;
                            HitEffect->Play(e);
                        }
                    }
                    //�q�b�g�G�t�F�N�g�Đ�
                   
                    projectile->Destroy();

                };
            }
        }
    }
}