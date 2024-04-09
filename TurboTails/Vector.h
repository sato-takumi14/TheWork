#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/17
*
********************************************************************************************
* Vectorクラス群：0,1のよく使うもので構成されている
********************************************************************************************/

#include"Main.h"

#define PI D3DX_PI

class Color
{
private:

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;

public:

	Color(crcolor data) : r(data.r), g(data.g), b(data.b), a(data.a) {}

	Color& operator = (crcolor data) {
		this->r = data.r;
		this->g = data.g;
		this->b = data.b;
		this->a = data.a;
		return *this;
	}

	void Set(color* data) {
		data->r = this->r;
		data->g = this->g;
		data->b = this->b;
		data->a = this->a;
	}

	template <class DATA>
	void serialize(DATA& data) {
		data(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(a));
	}

	static ccolor normal() { return color(1.0f, 1.0f, 1.0f, 1.0f); }
	static ccolor normalalpha(float alpha) { return color(1.0f, 1.0f, 1.0f, alpha); }
	static ccolor black() { return color(0.0f, 0.0f, 0.0f, 1.0f); }
	static ccolor red(float red) { return color(red, 0.0f, 0.0f, 1.0f); }
	static ccolor blue(float blue) { return color(0.0f, 0.0f, blue, 1.0f); }
	static ccolor green(float green) { return color(0.0f, green, 0.0f, 1.0f); }
	static ccolor transparent() { return color(0.0f, 0.0f, 0.0f, 0.0f); }

};

class Vector4
{
public:

	float x;
	float y;
	float z;
	float w;

public:

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(crver4 data) : x(data.x), y(data.y), z(data.z), w(data.w) {}

	Vector4& operator = (crver4 data) {
		x = data.x;
		y = data.y;
		z = data.z;
		w = data.w;
		return *this;
	}

	void Set(ver4* data) {
		data->x = x;
		data->y = y;
		data->z = z;
		data->w = w;
	}

	template <class DATA>
	void serialize(DATA& data) {
		data(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
	}

	static cver4 one() { return ver4(1.0f, 1.0f, 1.0f, 1.0f); }
	static cver4 zero() { return ver4(0.0f, 0.0f, 0.0f, 0.0f); }
	static cver4 onew(float w) { return ver4(1.0f, 1.0f, 1.0f, w); }
	static cver4 create(crver2 set, crfloat z = 0.0f, crfloat w = 0.0f) { return ver4(set.x, set.y, z, w); }
	static cver4 create(crver3 set, crfloat w = 0.0f) { return ver4(set.x, set.y, set.z, w); }
	static cver4 create(crfloat all) { return ver4(all, all, all, all); }

	static cver4 Create(crver3 data, crfloat w = 0.0f) { return cver4(data.x, data.y, data.z, w); }

};

class Vector3
{
public:

	float x;
	float y;
	float z;

public:

	Vector3(crver3 data) : x(data.x), y(data.y), z(data.z) {}

	Vector3& operator = (crver3 data) {
		x = data.x;
		y = data.y;
		z = data.z;
		return *this;
	}

	void Set(ver3* data) {
		data->x = x;
		data->y = y;
		data->z = z;
	}

	static cver3 zero() { return ver3(0.0f, 0.0f, 0.0f); }
	static cver3 one() { return ver3(1.0f, 1.0f, 1.0f); }
	static cver3 left() { return ver3(-1.0f, 0.0f, 0.0f); }
	static cver3 right() { return ver3(1.0f, 0.0f, 0.0f); }
	static cver3 up() { return ver3(0.0f, 1.0f, 0.0f); }
	static cver3 down() { return ver3(0.0f, -1.0f, 0.0f); }
	static cver3 forward() { return ver3(0.0f, 0.0f, 1.0f); }
	static cver3 back() { return ver3(0.0f, 0.0f, -1.0f); }
	static cver3 windowcenter() { return ver3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f); }
	static cver3 windowsize() { return ver3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); }

	static cver3 round(crver3 data) { return cver3(roundf(data.x), roundf(data.y), roundf(data.z)); }
	static cver3 absf(crver3 data) { return cver3(static_cast<float>(abs(data.x)), static_cast<float>(abs(data.y)), static_cast<float>(abs(data.z))); }

	static cver3 GetVector(const XMFLOAT3& data) { return cver3(data.x, data.y, data.z); }
	static const XMFLOAT3 SetXMVector(crver3 data) { return XMFLOAT3(data.x, data.y, data.z); }
	static cver3 Create(crver2 data, crfloat z = 0.0f) { return cver3(data.x, data.y, z); }

	/*前方向ベクトル*/
	static cver3 GetForward(crver3 rot) {
		D3DXMATRIX matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		D3DXVECTOR3 forward;
		forward.x = matrix._31;
		forward.y = matrix._32;
		forward.z = matrix._33;

		return forward;
	}

	/*右方向ベクトル*/
	static cver3 GetRight(crver3 rot) {
		D3DXMATRIX matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 right;
		right.x = matrix._11;
		right.y = matrix._12;
		right.z = matrix._13;

		return right;
	}

	/*上方向ベクトル*/
	static cver3 GetTop(crver3 rot) {
		D3DXMATRIX matrix;
		D3DXMatrixRotationYawPitchRoll(&matrix, rot.y, rot.x, rot.z);

		ver3 top;
		top.x = matrix._21;
		top.y = matrix._22;
		top.z = matrix._23;

		return top;
	}

	template <class DATA>
	void serialize(DATA& data) { data(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z)); }

};

class Vector2
{
public:

	static cver2 zero() { return ver2(0.0f, 0.0f); }
	static cver2 one() { return ver2(1.0f, 1.0f); }
	static cver2 left() { return ver2(-1.0f, 0.0f); }
	static cver2 right() { return ver2(1.0f, 0.0f); }
	static cver2 up() { return ver2(0.0f, 1.0f); }
	static cver2 down() { return ver2(0.0f, -1.0f); }
	static cver2 windowsize() { return ver2(SCREEN_WIDTH, SCREEN_HEIGHT); }
	static cver2 windowsizehalf() { return ver2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f); }

};