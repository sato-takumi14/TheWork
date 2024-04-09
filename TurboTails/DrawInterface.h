#pragma once

#include"Renderer.h"
#include"Interface.h"
#include"Transform.h"

#include<list>

class GameObject;

/*描画情報*/
typedef struct DrawInfo {

public:

	Transform transform;

	ver3 wh;
	ver3 u;
	ver3 v;

public:

	DrawInfo() { u = v = Vector3::up(); }
	DrawInfo(const Transform& transform, crver3 wh, crver3 u = Vector3::up(), crver3 v = Vector3::up())
		: transform(transform), wh(wh), u(u), v(v) {}
	void AddU() {
		u.x += u.z;
		u.y += u.z;
	}

	void AddV() {
		v.x += v.z;
		v.y += v.z;
	}

	void AddUV() {

		u.x += u.z;
		u.y += u.z;

		if (u.x >= 1.0f) {
			v.x += v.z;
			v.y += v.z;
		}

	}

	void ResetUV() {

		u.x = 0.0f;
		u.y = u.z;

		v.x = 0.0f;
		v.y = v.z;

	}

	static cver3 CreateUV(crfloat uv) { return cver3(0.0f, uv, uv); }
	static cver3 CreateWH(crfloat wh, crbool z = false) { 
		if (z) { return cver3(wh, 0.0f, wh); }
		return cver3(wh, wh, 0.0f);
	}

	template <class DATA>
	void serialize(DATA& data) { 
		Vector3 wh(this->wh);
		Vector3 u(this->u);
		Vector3 v(this->v);
		data(CEREAL_NVP(transform), CEREAL_NVP(wh), CEREAL_NVP(u), CEREAL_NVP(v));
		wh.Set(&this->wh);
		u.Set(&this->u);
		v.Set(&this->v);
	}

}DrawInfo;

typedef struct GeometryInstancing
{
private:

	uint index;

public:

	ID3D11Buffer* buffer;
	ID3D11ShaderResourceView* srv;

public:

	GeometryInstancing(cruint index = 5) : 
		index(index), buffer(nullptr), srv(nullptr) {}
	~GeometryInstancing() {
		if (buffer)buffer->Release();
		if (srv)srv->Release();
	}

	void Uninit() {
		if (buffer) {
			buffer->Release();
			buffer = nullptr;
		}
		if (srv) {
			srv->Release();
			srv = nullptr;
		}
	}

	void Set() { Renderer::GetDeviceContext()->VSSetShaderResources(index, 1, &srv); }
	void Set(cruint set) { index = set; }

	cuint GetIndex()const { return index; }

	void Create(const list<DrawInfo>& info) {

		const uint size = static_cast<int>(info.size());
		ver3* pos = new ver3[size];
		int id = 0;
		for (auto& data : info) {
			pos[id] = data.transform.pos;
			id++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ver3) * size;
		bd.StructureByteStride = sizeof(ver3);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pos;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &buffer);

		delete[] pos;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = size;
		Renderer::GetDevice()->CreateShaderResourceView(buffer, &srvd, &srv);

	}

}GeometryInstancing;

class DrawInterface : public Interface
{
protected:

	GameObject* mTarget{};

	list<int> mTexIDs{};

	ID3D11VertexShader* mVertexShader{};	//頂点シェーダーオブジェクト
	ID3D11PixelShader* mPixelShader{};		//ピクセルシェーダーオブジェクト
	ID3D11InputLayout* mVertexLayout{};		//頂点レイアウトオブジェクト

	bool mMultiple{};
	bool mDraw = true;

	void LoadShader(crstring vertex, crstring pixel) {
		Renderer::CreateVertexShader(&mVertexShader, &mVertexLayout, vertex.c_str());
		Renderer::CreatePixelShader(&mPixelShader, pixel.c_str());
	}

	void SetShader() {
		Renderer::GetDeviceContext()->IASetInputLayout(mVertexLayout);
		Renderer::GetDeviceContext()->VSSetShader(mVertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(mPixelShader, NULL, 0);
	}

public:

	void Uninit()override {

		if (mVertexShader) {
			mVertexShader->Release();
			mVertexShader = nullptr;
		}

		if (mPixelShader) {
			mPixelShader->Release();
			mPixelShader = nullptr;
		}

		if (mVertexLayout) {
			mVertexLayout->Release();
			mVertexLayout = nullptr;
		}

		mTexIDs.clear();

	}

	void Set(crbool set) { mDraw = set; }
	cbool GetBool()const override { return mDraw; }

	virtual void SetAnimName(crstring name) {}
	virtual void SetAnimName(crstring name, crfloat blend) {}
	virtual void SetRotation(crfloat set = 0.0f) {}

};