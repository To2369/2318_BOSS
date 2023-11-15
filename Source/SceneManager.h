#pragma once
#include "scnen.h"

class SceneManager
{
private:
    SceneManager() {};
    ~SceneManager() {};
public:
    static SceneManager& Instance()
    {
        static SceneManager M_Instance;
        return M_Instance;
    }
    //�X�V����
    void Update(float elpsedTime);
    //�`�揈��
    void Render();
    //�V�[���N���A
    void Clear();//�Ǘ����Ă�V�[���̏I���������s���֐�
    //�V�[���؂�ւ�
    void ChangeScene(Scene* scene);
private:
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;

};

