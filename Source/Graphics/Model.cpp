#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include"ResouceManager.h"
// コンストラクタ
Model::Model(const char* filename)
{
	//// リソース読み込み
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = Resoucemanager::Instance().LoadModelResouce(filename);
	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

Model::Node* Model::FindNode(const char* name)
{
	
	int nodeCount = static_cast<int>(nodes.size());
	for (int index=0;index<nodeCount;index++)
	{
		
		if (std::strcmp(nodes[index].name, name) == 0)
		{
			
			return  &nodes[index];
		}

	}

	
	
	return nullptr;
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

void Model::UpdateAnimation(float elapsedTime)
{
	//再生中でないなら再生しない
	if (!IsPlayerAnimetion())return;
	if (animationEndlFlag)
	{

		animationEndlFlag = false;
		currentAnimeationIndex = -1;

		return;

	}
	float blendRate = 1.0f;
    const std::vector < ModelResource::Animation>& animesdatas=resource->GetAnimations();
	const ModelResource::Animation& animedata0 = animesdatas.at(1);
	const ModelResource::Animation& animedata1 = animesdatas.at(currentAnimeationIndex);
	
	const std::vector<ModelResource::Keyframe>& keydata0= animedata0.keyframes;
	const std::vector<ModelResource::Keyframe>& keydata1= animedata1.keyframes;
	float anime0 = keydata0.at(10).seconds;
	float anime1 = keydata1.at(10).seconds;
	if (currentAnimeationIndex != 1)
	{
		animetionBlendtimer += elapsedTime;
		blendRate = animetionBlendtimer / anime0 + anime1;
	}
	//animetionBlendtimer += elapsedTime;
	/*float brendstate = animetionBlendtimer+animationBlendSeconds*(blendRate-animetionBlendtimer);
	blendRate = blendRate - brendstate;*/

	//指定のアニメーションデータを取得
	const std::vector < ModelResource::Animation>& animetions = resource->GetAnimations();
	const ModelResource::Animation& animetion = animetions.at(currentAnimeationIndex);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animetion.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; keyIndex++)
	{

		const ModelResource::Keyframe& keyfream0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyfream1 = keyframes.at(keyIndex+1);
		if (currentAnimetionSeconds >= keyfream0.seconds &&
			currentAnimetionSeconds < keyfream1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を計算
			float rate = (currentAnimetionSeconds - keyfream0.seconds) / (keyfream1.seconds - keyfream0.seconds);
			
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
			{
				//２つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyfream0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyfream1.nodeKeys.at(nodeIndex);
				Node& node = nodes[nodeIndex];
			
				if (blendRate<1.0f)
				{
				    
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&node.scale), DirectX::XMLoadFloat3(&key1.scale), animationBlendSeconds);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&node.rotate), DirectX::XMLoadFloat4(&key1.rotate), animationBlendSeconds);

					DirectX::XMVECTOR T = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&node.translate), DirectX::XMLoadFloat3(&key1.translate), animationBlendSeconds);
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
					
				}
				else
				{
					DirectX::XMVECTOR scale = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&key0.scale), DirectX::XMLoadFloat3(&key1.scale), rate);
					DirectX::XMVECTOR rotate = DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&key0.rotate), DirectX::XMLoadFloat4(&key1.rotate), rate);
					DirectX::XMVECTOR translate = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&key0.translate), DirectX::XMLoadFloat3(&key1.translate), rate);

					DirectX::XMStoreFloat3(&node.scale, scale);
					DirectX::XMStoreFloat4(&node.rotate, rotate);
					DirectX::XMStoreFloat3(&node.translate, translate);
					
					
				}
			}
			break;
		}
	}
	
	

	//時間経過
	
	if (anime_clip)
	{
		return;
	}
	    currentAnimetionSeconds += elapsedTime;
	
	//再生時間を超えたら
	if (currentAnimetionSeconds > animetion.secondsLength)
	{
		
		if (OneShotMove)
		{
			currentAnimetionSeconds -= elapsedTime;
			return;
		}
		if (!animationLoopFlag)
		{

			animationEndlFlag = true;
		}
		//currentAnimetionSeconds -= animetion.secondsLength;
		currentAnimetionSeconds = 0;
		
	}


}

void Model::playAnimetion(int index,bool loop,float blendseconds)
{
	
	currentAnimeationIndex = index;
	currentAnimetionSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndlFlag = false;
	animetionBlendtimer = 0.0f;
	animationBlendSeconds = blendseconds;
}

bool Model::IsPlayerAnimetion() const
{
	
	if (currentAnimeationIndex < 0)
	{
		return false;
	}
	int a = resource->GetAnimations().size();
	if (currentAnimeationIndex >= resource->GetAnimations().size())
	{
		return false;
	}

	return true;
}
