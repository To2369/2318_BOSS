#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};
	//�m�[�h����
	Node*FindNode(const char* name);
	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource; }

	//�A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTime);
	//�A�j���Đ�
	void playAnimetion(int index,bool loop,float blendSeconds=0.2f);
	void SetOneShotMove(bool flag) {OneShotMove=flag; };
	void Setanime_clip(bool flag) { anime_clip =flag; };
	float GetAnimationTime() {return currentAnimetionSeconds; };
	float GetanimetionBlendtimer() {return animetionBlendtimer; };
	//�A�j���[�V�������Đ�����Ă邩
	bool IsPlayerAnimetion()const;
	float GetCurrentAnimationSeconds()const { return currentAnimetionSeconds; }
private:
	ModelResource*	resource;
	std::vector<Node>				nodes;
	int currentAnimeationIndex = -1;//���݂̃A�j���[�V�����C���f�b�N�X
	float currentAnimetionSeconds = 0.0f;//���݂̃A�j���[�V����
	bool animationLoopFlag = false;
	bool animationEndlFlag= false;
	float animetionBlendtimer = 0.0f;
	float animationBlendSeconds = 0.0f;
	bool OneShotMove;
	bool anime_clip;
};
