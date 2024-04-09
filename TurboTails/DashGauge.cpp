
#include "main.h"
#include "DashGauge.h"

#include"Draw2D.h"
#include"Scene.h"
#include"Camera.h"
#include"Time.h"
#include"SceneManager.h"
#include"UIManager.h"

void DashGauge::Init()
{

	mInfo.transform = mTransform;
	mInfo.wh = DrawInfo::CreateWH(30.0f);
	mInfo.u = mInfo.v = Vector3::up();

	mGauge = Vector4::one() * 100.0f;

	GameObject::SetActivate(false);

	mLayer.push_back("2D");
	mTag = "DashGauge";
	mClass = "DashGauge";

	mMainCamera = mScene->GetMainCamera();

	mOffset = ((Vector3::right() * 50.0f));

}

void DashGauge::Start()
{
	mDraw = new Draw2D(
		this,
		"shader\\GaugeVS.cso",
		"shader\\CircleGaugePS.cso",
		Vector3::zero(),
		Vector4::one(),
		list<int>()
	);
}

void DashGauge::Update()
{

	GameObject::Update();

	if (mReset) {
		mGauge.x += Time::GetDeltaTime();
		if (mGauge.x > mGauge.y) {
			if (mTime++ > 50) {
				mGauge.x = mGauge.y;
				mReset = false;
				mTime = 0;
				GameObject::SetActivate(false);
			}
		}
	}

}

void DashGauge::Draw()
{

	PARAMETER param;
	param.hp = mGauge;
	param.Diffuse = color(0.0f, 0.5f, 0.8f, 1.0f);
	param.Ambient = Color::normal() * 0.1f;
	param.Color = Color::normal() * 0.45f;
	Renderer::SetParameter(param);

	GameObject::Draw();

}

void DashGauge::Uninit()
{
	GameObject::Uninit();
}

void DashGauge::SetScreenPosition(crver3 data)
{
	matrix world, view, projection;

	ver3 pos = SceneManager::GetScene()->GetMainCamera()->GetPosition();
	ver3 up = Vector3::up();

	D3DXMatrixLookAtLH(&view, &pos, &data, &up);

	D3DXMatrixPerspectiveFovLH(&projection, 1.0f,
		static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT), 1.0f, 1000.0f);

	D3DXMatrixTranslation(&world, data.x, data.y, data.z);

	matrix transform = world * view * projection;

	ver3 screenPos(transform._41, transform._42, transform._43);

	mTransform.pos.x = (SCREEN_WIDTH * 0.5f) + (static_cast<float>(screenPos.x) * (SCREEN_WIDTH * 0.5f));
	mTransform.pos.y = (SCREEN_HEIGHT * 0.5f) - (static_cast<float>(screenPos.y) * (SCREEN_HEIGHT * 0.5f));

	mTransform.pos.z = 0.0f;

	mTransform.pos += mOffset;

}
