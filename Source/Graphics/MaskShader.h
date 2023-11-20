#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class MaskShader : public SpriteShader
{
public:
	MaskShader(ID3D11Device* device);
	virtual ~MaskShader() {}

	// ï`âÊäJén
	void Begin(const RenderContext& rc);

	// ï`âÊ
	void Draw(const RenderContext& rc, const Sprite* model);

	// ï`âÊèIóπ
	void End(const RenderContext& rc);
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;

};