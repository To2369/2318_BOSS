#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    //Effeckseerレンダラ作成
    EffekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 248);
    //Effeckseerマネージャー作成
    EffekseerManager = Effekseer::Manager::Create(2048);

    //Effeckseerレンダラの各種設定（特別なカスタマイズをしない場合は定期的に以下を設定でおｋ）
    EffekseerManager->SetSpriteRenderer(EffekseerRenderer->CreateSpriteRenderer());
    EffekseerManager->SetRibbonRenderer(EffekseerRenderer->CreateRibbonRenderer());
    EffekseerManager->SetRingRenderer(EffekseerRenderer  ->CreateRingRenderer());
    EffekseerManager->SetTrackRenderer(EffekseerRenderer ->CreateTrackRenderer());
    EffekseerManager->SetModelRenderer(EffekseerRenderer ->CreateModelRenderer());
    //Effekseerを左手座標系で計算する
    EffekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

}
//終了化
void EffectManager::Finalize()
{
    //EffekseerMnagerなどはスマートポインターによって破棄されるので何もしない
}
//更新処理
void EffectManager::Update(float elapsedTime)
{
    //エフェクト更新処理(引数にはフレームの経過時間を渡す)
    EffekseerManager->Update(elapsedTime * 60.0f);
}

void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //ビュー＆プロジェクション行列をEffekseerレンダラに設定
    EffekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    EffekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    //Effekseer描画開始
    EffekseerRenderer->BeginRendering();

    //Effekseer描画実行
    //マネージャー単位で描画するので描画順を制御する場合はマネージャーを複数個作成し、
    //Draw()関数を実行する順番で制御できそう
    EffekseerManager->Draw();
    //Effekseerの描画終了
    EffekseerRenderer->EndRendering();

}




