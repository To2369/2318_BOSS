#pragma once
#include"Graphics/Sprite.h"
#include"scnen.h"
#include<thread>
class SceneLoading :public Scene
{
public:

    SceneLoading(Scene* nextscene) :nextscene(nextscene) {}
    SceneLoading(){}
    ~SceneLoading()override {};
    //������
    void Initialize()override;
    //�I����
    void Finalize()override;
    //�X�V����
    void Update(float elapsedTime)override;
    //�`�揈��
    void Render()override;
private:
    //���[�f�B���O�X���b�h
    static void LadingThread(SceneLoading* scene);

private:
    //���̃V�[��
    Scene* nextscene = nullptr;
    //���̃V�[�����쐬���Ă����X���b�h
    std::thread* thread = nullptr;
    Sprite* sprite = nullptr;
    float    angle = 0.0f;
};
