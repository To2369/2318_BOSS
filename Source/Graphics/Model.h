#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// モデル
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
	//ノード検索
	Node*FindNode(const char* name);
	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// リソース取得
	const ModelResource* GetResource() const { return resource; }

	//アニメーション更新処理
	void UpdateAnimation(float elapsedTime);
	//アニメ再生
	void playAnimetion(int index,bool loop,float blendSeconds=0.2f);
	void SetOneShotMove(bool flag) {OneShotMove=flag; };
	void Setanime_clip(bool flag) { anime_clip =flag; };
	float GetAnimationTime() {return currentAnimetionSeconds; };
	float GetanimetionBlendtimer() {return animetionBlendtimer; };
	//アニメーションが再生されてるか
	bool IsPlayerAnimetion()const;
	float GetCurrentAnimationSeconds()const { return currentAnimetionSeconds; }
private:
	ModelResource*	resource;
	std::vector<Node>				nodes;
	int currentAnimeationIndex = -1;//現在のアニメーションインデックス
	float currentAnimetionSeconds = 0.0f;//現在のアニメーション
	bool animationLoopFlag = false;
	bool animationEndlFlag= false;
	float animetionBlendtimer = 0.0f;
	float animationBlendSeconds = 0.0f;
	bool OneShotMove;
	bool anime_clip;
};
