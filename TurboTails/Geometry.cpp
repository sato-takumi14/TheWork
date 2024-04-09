
#include"Main.h"
#include "Geometry.h"

#include"GameObject.h"

Geometry::~Geometry()
{
	this->Uninit();
}

void Geometry::Uninit()
{

	if (mGeometryInfo) {
		mGeometryInfo->buffer->Release();
		mGeometryInfo->srv->Release();
		delete mGeometryInfo;
		mGeometryInfo = nullptr;
	}

}

void Geometry::SetGeometry()
{
	Renderer::GetDeviceContext()->VSSetShaderResources(mGeometryInfo->index, 1, &mGeometryInfo->srv);
}

void Geometry::SetGeometry(const list<Transform>& transforms)
{

	{

		auto device = Renderer::GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE msr;
		device->Map(mGeometryInfo->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		GeometryInstanceData* info = (GeometryInstanceData*)(msr.pData);
		uint id = 0;

		for (auto& data : transforms) {
			matrix scale, rot, trans;
			D3DXMatrixScaling(&scale, data.scale.x, data.scale.y, data.scale.z);
			D3DXMatrixRotationYawPitchRoll(&rot, data.rot.y, data.rot.x, data.rot.z);
			D3DXMatrixTranslation(&trans, data.pos.x, data.pos.y, data.pos.z);
			info[id].world= scale * rot * trans;
			D3DXMatrixTranspose(&info[id].world, &info[id].world);
			id++;
		}

		device->Unmap(mGeometryInfo->buffer, 0);

		device->VSSetShaderResources(mGeometryInfo->index, 1, &mGeometryInfo->srv);

	}

}

void Geometry::SetGeometry(const GameObject* object)
{

	auto device = Renderer::GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE msr;
	device->Map(mGeometryInfo->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	GeometryInstanceData* info = (GeometryInstanceData*)(msr.pData);
	uint id = 0;

	auto objects = object->GetGameObjects();

	for (auto& data : objects) {
		Transform transform = data->GetReferenceTransform();
		transform.pos += transform.local;
		quat quat;
		matrix scale, rot, trans;
		D3DXMatrixScaling(&scale, transform.scale.x, transform.scale.y, transform.scale.z);
		D3DXQuaternionRotationYawPitchRoll(&quat, transform.rot.y, transform.rot.x, transform.rot.z);
		D3DXMatrixRotationQuaternion(&rot, &quat);
		D3DXMatrixTranslation(&trans, transform.pos.x, transform.pos.y, transform.pos.z);
		info[id].world = scale * rot * trans;
		D3DXMatrixTranspose(&info[id].world, &info[id].world);
		info[id].material = data->GetMaterial();
		id++;
	}

	device->Unmap(mGeometryInfo->buffer, 0);

	device->VSSetShaderResources(mGeometryInfo->index, 1, &mGeometryInfo->srv);

}

void Geometry::Create(const list<ver3>& infos)
{

	{

		mGeometryInfo = new GeometryInfo(3);

		cuint size = static_cast<uint>(infos.size());
		ver3* pos = new ver3[size];
		int id = 0;
		for (auto& data : infos) { pos[id++] = data; }

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(ver3) * size;
		bd.StructureByteStride = sizeof(ver3);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pos;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mGeometryInfo->buffer);

		delete[] pos;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = size;
		Renderer::GetDevice()->CreateShaderResourceView(mGeometryInfo->buffer, &srvd, &mGeometryInfo->srv);

	}

}

void Geometry::Create(const list<Transform>& transform)
{

	{

		mGeometryInfo = new GeometryInfo(3);

		cuint size = static_cast<uint>(transform.size());
		GeometryInstanceData* info = new GeometryInstanceData[size];
		uint id = 0;
		for (auto& data : transform) {
			matrix scale, rot, trans;
			D3DXMatrixScaling(&scale, data.scale.x, data.scale.y, data.scale.z);
			D3DXMatrixRotationYawPitchRoll(&rot, data.rot.y, data.rot.x, data.rot.z);
			D3DXMatrixTranslation(&trans, data.pos.x, data.pos.y, data.pos.z);
			info[id].world = scale * rot * trans;
			D3DXMatrixTranspose(&info[id].world, &info[id].world);
			id++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(GeometryInstanceData) * size;
		bd.StructureByteStride = sizeof(GeometryInstanceData);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = info;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mGeometryInfo->buffer);

		delete[] info;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = size;
		Renderer::GetDevice()->CreateShaderResourceView(mGeometryInfo->buffer, &srvd, &mGeometryInfo->srv);

	}

}

void Geometry::Create(const GameObject* object)
{

	mGeometryInfo = new GeometryInfo(3);

	auto objects = object->GetGameObjects();
	cuint size = static_cast<uint>(objects.size());
	GeometryInstanceData* info = new GeometryInstanceData[size];
	uint id = 0;
	for (auto& data : objects) {
		Transform transform = data->GetReferenceTransform();
		transform.pos += transform.local;
		quat quat;
		matrix scale, rot, trans;
		D3DXMatrixScaling(&scale, transform.scale.x, transform.scale.y, transform.scale.z);
		D3DXQuaternionRotationYawPitchRoll(&quat, transform.rot.y, transform.rot.x, transform.rot.z);
		D3DXMatrixRotationQuaternion(&rot, &quat);
		D3DXMatrixTranslation(&trans, transform.pos.x, transform.pos.y, transform.pos.z);
		info[id].world = scale * rot * trans;
		D3DXMatrixTranspose(&info[id].world, &info[id].world);
		info[id].material = data->GetMaterial();
		id++;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GeometryInstanceData) * size;
	bd.StructureByteStride = sizeof(GeometryInstanceData);
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = info;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &mGeometryInfo->buffer);

	delete[] info;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = size;
	Renderer::GetDevice()->CreateShaderResourceView(mGeometryInfo->buffer, &srvd, &mGeometryInfo->srv);

}

void Geometry::Create(cruint num)
{

	mGeometryInfo = new GeometryInfo(3);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GeometryInstanceData) * num;
	bd.StructureByteStride = sizeof(GeometryInstanceData);
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &mGeometryInfo->buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = num;
	Renderer::GetDevice()->CreateShaderResourceView(mGeometryInfo->buffer, &srvd, &mGeometryInfo->srv);

}
