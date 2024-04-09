#pragma once

#include"Main.h"
#include"Transform.h"
#include"Renderer.h"

class GameObject;

class Geometry
{
private:

	uint mIndex = 0;

	typedef struct GeometryInfo {

	public:

		uint index;

		ID3D11Buffer* buffer;
		ID3D11ShaderResourceView* srv;

	public:

		GeometryInfo() : buffer(nullptr), srv(nullptr), index(0) {}
		GeometryInfo(cruint index) : buffer(nullptr), srv(nullptr), index(index) {}
		GeometryInfo(ID3D11Buffer* buffer, ID3D11ShaderResourceView* srv, cruint index)
			: buffer(buffer), srv(srv), index(index) {}

	}GeometryInfo;

	struct GeometryInstanceData {

		matrix world;
		//ver3 pos;
		MATERIAL material;
		bool active;

	};

	GeometryInfo* mGeometryInfo{};

	ID3D11Buffer* mBuffer{};
	ID3D11ShaderResourceView* mSrv{};

public:

	Geometry(cruint index = 3) : mIndex(index), mBuffer(nullptr) ,mSrv(nullptr) {}
	~Geometry();

	void Uninit();

	void SetGeometry();
	void SetGeometry(const list<Transform>& transforms);
	void SetGeometry(const GameObject* object);

	void Set(cruint index) { mIndex = index; }

	void Create(const list<ver3>& infos);
	void Create(const list<Transform>& transform);
	void Create(const GameObject* object);
	void Create(cruint num);

	cuint GetIndex()const { return mIndex; }

};