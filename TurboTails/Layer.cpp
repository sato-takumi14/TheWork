
#include"Main.h"

#include"Renderer.h"
#include "Layer.h"
#include"GameObject.h"
#include"Light.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Camera.h"

void Layer::Update()
{
	
	mPath.Update();

	for (auto& data : mLayer) {
		data.second.remove_if([](GameObject* obj) {return obj->GetDestroy(); });
	}

	auto camera = SceneManager::GetScene()->GetMainCamera();

	for (auto& data : mLayer["3D"]) {
		data->SetActivate(camera->CheckView(data->GetPosition()));
	}

}

void Layer::Draw()
{
	
	int size = static_cast<int>(mOrder.size());

	for (int i = 0; i < size; i++) {

		for (auto& order : mOrder) {

			if (order.index == i) {

				mPath.SetPath(order.path);

				for (auto& data : mLayer[order.name]) {

					data->Draw();

				}

				break;

			}

		}

	}

	Renderer::End();

}

void Layer::Uninit()
{
	mLayer.clear();
}

void Layer::AddLayer(crstring layer, RendererPath path)
{
	
}

void Layer::AddLayer(const Order& layer)
{

	mOrder.push_back(layer);

}

void Layer::AddGameObject(GameObject* obj)
{
	
	auto layer = obj->GetLayer();

	for (auto& data : layer) {
		mLayer[data].push_back(obj);
	}

	if (obj->GetTag() == "Player") mPath.SetPlayer(obj);

}

void Layer::AddLayerGameObject(GameObject* obj, crstring layer)
{
	mLayer[layer].push_back(obj);
}

void Layer::Path::SetPathBegin()
{
	Renderer::Begin();
	Renderer::SetViewPortNormal();
}

void Layer::Path::SetPathPP()
{
	Renderer::BeginPP();
	Renderer::SetViewPortNormal();
}

void Layer::Path::SetPathBG()
{
	Renderer::BeginBG();
	Renderer::SetViewPortNormal();
}

void Layer::Path::SetPathDepth()
{

	LIGHT light;

	//ライトカメラ構造体の初期化
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	ver3 pos = ver3(-10.0f, 10.0f, -10.0f);
	light.Position = Vector4::Create(pos, 1.0f);
	//D3DXVECTOR3 lightTarget = playerPos;
	D3DXVECTOR3 lightTarget = Vector3::zero();
	D3DXVECTOR3 lightUp = Vector3::up();
	D3DXMatrixLookAtLH(&light.ViewMatrix, &pos, &lightTarget, &lightUp);
	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(
		&light.ProjectionMatrix, 1.0f,
		static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT),
		5.0f, 100.0f);

	//ライト情報セット
	Renderer::SetLight(light);
	Renderer::BeginDepth();
	Renderer::SetViewPortDepth();

	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

}

void Layer::Path::SetPathLuminance()
{
	Renderer::BeginLuminace();
	Renderer::SetLuminaceViewport();
}

void Layer::Path::Update()
{

	for (auto& data : mPath) {
		data.second = false;
	}

}

void Layer::Path::SetPath(const RendererPath& path)
{

	if (!mPath[path]) {

		mPath[path] = true;

		switch (path) {

		case RendererPath::begin:
			SetPathBegin();
			break;

		case RendererPath::beginpp:
			SetPathPP();
			break;

		case RendererPath::beginbg:
			SetPathBG();
			break;

		case RendererPath::begindepth:						
			SetPathDepth();
			break;

		case RendererPath::bloom:
			
			break;

		case RendererPath::luminace:
			SetPathLuminance();
			break;

		}

	}

}
