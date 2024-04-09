#pragma once

/***********************************************************************************************************
*
*	Vector
*
*																				制作者：佐藤匠
*																				制作日：02/20
*
************************************************************************************************************
* D3DXの2,3,4,COLORの自作版
************************************************************************************************************/

#include"Main.h"

namespace Vector
{

	class Vector
	{
	public:
		float x, y;
	};

	class Vector2 : public Vector
	{
	public:

		/*コンストラクタ*/
		inline Vector2() {
			this->x = 0.0f;
			this->y = 0.0f;
		}
		inline Vector2(crfloat x, crfloat y) {
			this->x = x;
			this->y = y;
		}

		/*シリアライズ*/
		template <class DATA>
		void serialize(DATA& data) { data(CEREAL_NVP(x), CEREAL_NVP(y)); }

		/*単位ベクトル*/
		inline static const Vector2 One() { return Vector2(1.0f, 1.0f); }
		inline static const Vector2 Zero() { return Vector2(0.0f, 0.0f); }
		inline static const Vector2 Up() { return Vector2(0.0, 1.0f); }
		inline static const Vector2 Down() { return Vector2(0.0, -1.0f); }
		inline static const Vector2 Left() { return Vector2(-1.0f, 0.0f); }
		inline static const Vector2 Right() { return Vector2(1.0f, 0.0f); }
		inline static const Vector2 Window() { return Vector2(WindowWidth, WindowHeight); }

		/*DirectXMathのXMFLOAT2に変換*/
		inline const XMFLOAT2 Float2()const { return XMFLOAT2(this->x, this->y); }
		inline static const XMFLOAT2 Float2(const Vector2& data) { return XMFLOAT2(data.x, data.y); }

		/*XMVECTORに変換*/
		inline const XMVECTOR Vector(crfloat z = 0.0f, crfloat w = 0.0f)const { return XMVectorSet(x, y, z, w); }
		inline const XMVECTOR Vector(const Vector2& vector, crfloat z = 0.0f, crfloat w = 0.0f) { return XMVectorSet(vector.x, vector.y, z, w); }

		/*長さ*/
		inline static cfloat Length(const Vector2& data) { return sqrtf((data.x * data.x) + (data.y * data.y)); }
		inline static cfloat LengthSq(const Vector2& data) { return ((data.x * data.x) + (data.y * data.y)); }

		/*内積*/
		inline static cfloat Dot(const Vector2& data1, const Vector2& data2) { return ((data1.x * data2.x) + (data1.y * data2.y)); }

		/*正規化*/
		inline static const Vector2 Normalize(Vector2* data) {
			cfloat len = Vector2::Length(*data);
			(*data).x /= len;
			(*data).y /= len;
			return (*data);
		}

		/*絶対値*/
		inline const Vector2 Abs()const { return Vector2(abs(this->x), abs(this->y)); }
		inline static const Vector2 Abs(const Vector2& data) { return Vector2(abs(data.x), abs(data.y)); }
		inline static const Vector2 Abs(const XMFLOAT2& data) { return Vector2(abs(data.x), abs(data.y)); }

		/*オペレーター*/
		inline Vector2& operator = (const XMFLOAT2& data) {
			this->x = data.x;
			this->y = data.y;
			return *this;
		}
		inline Vector2& operator = (const Vector2& data) {
			this->x = data.x;
			this->y = data.y;
			return *this;
		}

		inline Vector2 operator + ()const { return *this; }
		inline Vector2 operator - ()const { return Vector2(-this->x, -this->y); }

		inline Vector2 operator + (const Vector2& data)const { return Vector2(this->x + data.x, this->y + data.y); }
		inline Vector2& operator += (const Vector2& data) {
			this->x += data.x;
			this->y += data.y;
			return *this;
		}

		inline Vector2 operator - (const Vector2& data)const { return Vector2(this->x - data.x, this->y - data.y); }
		inline Vector2& operator -= (const Vector2& data) {
			this->x -= data.x;
			this->y -= data.y;
			return *this;
		}

		inline Vector2 operator * (const Vector2& data)const { return Vector2(this->x * data.x, this->y * data.y); }
		inline Vector2& operator *= (const Vector2& data) {
			this->x *= data.x;
			this->y *= data.y;
			return *this;
		}
		inline Vector2 operator * (crfloat data)const { return Vector2(this->x * data, this->y * data); }
		inline Vector2& operator *= (crfloat data) {
			this->x *= data;
			this->y *= data;
			return *this;
		}

		inline Vector2 operator / (const Vector2& data)const {
			Vector2 f = Vector2(1.0f / data.x, 1.0f / data.y);
			return Vector2(this->x * f.x, this->y * f.y);
		}
		inline Vector2& operator /= (const Vector2& data) {
			Vector2 f = Vector2(1.0f / data.x, 1.0f / data.y);
			this->x *= f.x;
			this->y *= f.y;
			return *this;
		}
		inline Vector2 operator / (crfloat data)const {
			float f = 1.0f / data;
			return Vector2(this->x * f, this->y * f);
		}
		inline Vector2& operator /= (crfloat data) {
			float f = 1.0f / data;
			this->x *= f;
			this->y *= f;
			return *this;
		}

		inline cbool operator == (const Vector2& data)const { return this->x == data.x && this->y == data.y; }
		inline cbool operator != (const Vector2& data)const { return this->x != data.x || this->y != data.y; }
		inline cbool operator < (const Vector2& data)const { return this->x < data.x && this->y < data.y; }
		inline cbool operator <= (const Vector2& data)const { return this->x <= data.x && this->y <= data.y; }
		inline cbool operator > (const Vector2& data)const { return this->x > data.x && this->y > data.y; }
		inline cbool operator >= (const Vector2& data)const { return this->x >= data.x && this->y >= data.y; }

		/*XMFLOAT2用のオペレーター*/
		inline Vector2 operator + (const XMFLOAT2& data)const { return Vector2(this->x + data.x, this->y + data.y); }
		inline Vector2& operator += (const XMFLOAT2& data) {
			this->x += data.x;
			this->y += data.y;
			return *this;
		}

		inline Vector2 operator - (const XMFLOAT2& data)const { return Vector2(this->x - data.x, this->y - data.y); }
		inline Vector2& operator -= (const XMFLOAT2& data) {
			this->x -= data.x;
			this->y -= data.y;
			return *this;
		}

		inline Vector2 operator * (const XMFLOAT2& data)const { return Vector2(this->x * data.x, this->y * data.y); }
		inline Vector2& operator *= (const XMFLOAT2& data) {
			this->x *= data.x;
			this->y *= data.y;
			return *this;
		}

		inline Vector2 operator / (const XMFLOAT2& data)const {
			Vector2 f = Vector2(1.0f / data.x, 1.0f / data.y);
			return Vector2(this->x * data.x, this->y * data.y);
		}
		inline Vector2& operator /= (const XMFLOAT2& data) {
			Vector2 f = Vector2(1.0f / data.x, 1.0f / data.y);
			this->x *= f.x;
			this->y *= f.y;
			return *this;
		}

	};

	class Vector3 : public Vector
	{
	public:

		float z;
	
	public:

		/*コンストラクタ*/
		inline Vector3() {
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}
		inline Vector3(crfloat x, crfloat y, crfloat z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/*シリアライズ*/
		template <class DATA>
		void serialize(DATA& data) { data(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z)); }

		/*単位ベクトル*/
		inline static const Vector3 One()		{ return Vector3(1.0f, 1.0f, 1.0f); }
		inline static const Vector3 Zero()		{ return Vector3(0.0f, 0.0f, 0.0f); }
		inline static const Vector3 Up()		{ return Vector3(0.0, 1.0f, 0.0f); }
		inline static const Vector3 Down()		{ return Vector3(0.0, -1.0f, 0.0f); }
		inline static const Vector3 Left()		{ return Vector3(-1.0f, 0.0f, 0.0f); }
		inline static const Vector3 Right()		{ return Vector3(1.0f, 0.0f, 0.0f); }
		inline static const Vector3 Forward()	{ return Vector3(0.0f, 0.0f, 1.0f); }
		inline static const Vector3 Back()		{ return Vector3(0.0f, 0.0f, -1.0f); }
		inline static const Vector3 Window()	{ return Vector3(WindowWidth, WindowHeight, 0.0f); }

		/*DirectXMathのXMFLOAT3に変換*/
		inline const XMFLOAT3 Float3()const { return XMFLOAT3(this->x, this->y, this->z); }
		inline static const XMFLOAT3 Float3(const Vector3& data) { return XMFLOAT3(data.x, data.y, data.z); }

		/*XMVECTORに変換*/
		inline const XMVECTOR Vector(crfloat w = 0.0f)const { return XMVectorSet(x, y, z, w); }
		inline static const XMVECTOR Vector(const Vector3& vector, crfloat w = 0.0f) { return XMVectorSet(vector.x, vector.y, vector.z, w); }

		/*長さ*/
		inline static cfloat Length(const Vector3& data) { return sqrtf((data.x * data.x) + (data.y * data.y) + (data.z * data.z)); }
		inline static cfloat LengthSq(const Vector3& data) { return ((data.x * data.x) + (data.y * data.y) + (data.z * data.z)); }

		/*内積*/
		inline static cfloat Dot(const Vector3& data1, const Vector3& data2) { return ((data1.x * data2.x) + (data1.y * data2.y) + (data1.z * data2.z)); }

		/*外積*/
		inline static const Vector3 Cross(Vector3* out, const Vector3& data1, const Vector3& data2) {

			Vector3 v;
			v.x = data1.y * data2.z - data1.z * data2.y;
			v.y = data1.z * data2.x - data1.x * data2.z;
			v.z = data1.x * data2.y - data1.y * data2.x;

			(*out) = v;

			return v;

		}

		/*正規化*/
		inline static const Vector3 Normalize(Vector3* data) {

			cfloat len = Vector3::Length((*data));
			(*data).x /= len;
			(*data).y /= len;
			(*data).z /= len;

			return (*data);

		}

		/*絶対値*/
		inline const Vector3 Abs()const { return Vector3(abs(this->x), abs(this->y), abs(this->z)); }
		inline static const Vector3 Abs(const Vector3& data) { return Vector3(abs(data.x), abs(data.y), abs(data.z)); }
		inline static const Vector3 Abs(const XMFLOAT3& data) { return Vector3(abs(data.x), abs(data.y), abs(data.z)); }

		/*ベクトル取得関数群*/
		/*前ベクトル*/
		/*前ベクトル*/
		inline static const Vector3 GetForward(const Vector3& rot) {
			XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
			return Vector3(matrix.r[2].m128_f32[0], matrix.r[2].m128_f32[1], matrix.r[2].m128_f32[2]);
		}

		/*右ベクトル*/
		inline static const Vector3 GetRight(const Vector3& rot) {
			matrix matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
			return Vector3(matrix.r[0].m128_f32[0], matrix.r[0].m128_f32[1], matrix.r[0].m128_f32[2]);
		}

		/*上ベクトル*/
		inline static const Vector3 GetUp(const Vector3& rot) {
			matrix matrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
			return Vector3(matrix.r[1].m128_f32[0], matrix.r[1].m128_f32[1], matrix.r[1].m128_f32[2]);
		}

		/*オペレーター*/
		inline Vector3& operator = (const XMFLOAT3& data) {
			this->x = data.x;
			this->y = data.y;
			this->z = data.z;
			return *this;
		}
		inline Vector3& operator = (const Vector3& data) {
			this->x = data.x;
			this->y = data.y;
			this->z = data.z;
			return *this;
		}

		inline Vector3 operator + ()const { return *this; }
		inline Vector3 operator - ()const { return Vector3(-this->x, -this->y, -this->z); }

		inline Vector3 operator + (const Vector3& data)const { return Vector3(this->x + data.x, this->y + data.y, this->z + data.z); }
		inline Vector3& operator += (const Vector3& data) {
			this->x += data.x;
			this->y += data.y;
			this->z += data.z;
			return *this;
		}

		inline Vector3 operator - (const Vector3& data)const { return Vector3(this->x - data.x, this->y - data.y, this->z - data.z); }
		inline Vector3& operator -= (const Vector3& data) {
			this->x -= data.x;
			this->y -= data.y;
			this->z -= data.z;
			return *this;
		}

		inline Vector3 operator * (const Vector3& data)const { return Vector3(this->x * data.x, this->y * data.y, this->z * data.z); }
		inline Vector3& operator *= (const Vector3& data) {
			this->x *= data.x;
			this->y *= data.y;
			this->z *= data.z;
			return *this;
		}
		inline Vector3 operator * (crfloat data)const { return Vector3(this->x * data, this->y * data, this->z * data); }
		inline Vector3& operator *= (crfloat data) {
			this->x *= data;
			this->y *= data;
			this->z *= data;
			return *this;
		}

		inline Vector3 operator / (const Vector3& data)const { 
			Vector3 f = Vector3(1.0f / data.x, 1.0f / data.y, 1.0f / data.z);
			return Vector3(this->x * f.x, this->y * f.y, this->z * f.z);
		}
		inline Vector3& operator /= (const Vector3& data) {
			Vector3 f = Vector3(1.0f / data.x, 1.0f / data.y, 1.0f / data.z);
			this->x *= f.x;
			this->y *= f.y;
			this->z *= f.z;
			return *this;
		}
		inline Vector3 operator / (crfloat data)const {
			float f = 1.0f / data;
			return Vector3(this->x * f, this->y * f, this->z * f);
		}
		inline Vector3& operator /= (crfloat data) {
			float f = 1.0f / data;
			this->x *= f;
			this->y *= f;
			this->z *= f;
			return *this;
		}

		inline cbool operator == (const Vector3& data)const {
			return this->x == data.x &&
				this->y == data.y &&
				this->z == data.z;
		}
		inline cbool operator != (const Vector3& data)const { return this->x != data.x || this->y != data.y || this->z != data.z; }
		inline cbool operator < (const Vector3& data)const {
			return this->x < data.x &&
				this->y < data.y &&
				this->z < data.z;
		}
		inline cbool operator <= (const Vector3& data)const {
			return this->x <= data.x &&
				this->y <= data.y &&
				this->z <= data.z;
		}
		inline cbool operator > (const Vector3& data)const {
			return this->x > data.x &&
				this->y > data.y &&
				this->z > data.z;
		}
		inline cbool operator >= (const Vector3& data)const {
			return this->x >= data.x &&
				this->y >= data.y &&
				this->z >= data.z;
		}

		/*XMFLOAT3のオペレーター*/
		inline Vector3 operator + (const XMFLOAT3& data)const { return Vector3(this->x + data.x, this->y + data.y, this->z + data.z); }
		inline Vector3& operator += (const XMFLOAT3& data) {
			this->x += data.x;
			this->y += data.y;
			this->z += data.z;
			return *this;
		}

		inline Vector3 operator - (const XMFLOAT3& data)const { return Vector3(this->x - data.x, this->y - data.y, this->z - data.z); }
		inline Vector3& operator -= (const XMFLOAT3& data) {
			this->x += data.x;
			this->y += data.y;
			this->z += data.z;
			return *this;
		}

		inline Vector3 operator * (const XMFLOAT3& data)const { return Vector3(this->x * data.x, this->y * data.y, this->z * data.z); }
		inline Vector3& operator *= (const XMFLOAT3& data) {
			this->x *= data.x;
			this->y *= data.y;
			this->z *= data.z;
			return *this;
		}

		inline Vector3 operator / (const XMFLOAT3& data)const {
			Vector3 f = Vector3(1.0f / data.x, 1.0f / data.y, 1.0f / data.z);
			return Vector3(this->x * f.x, this->y * f.y, this->z * f.z);
		}
		inline Vector3& operator /= (const XMFLOAT3& data) {
			Vector3 f = Vector3(1.0f / data.x, 1.0f / data.y, 1.0f / data.z);
			this->x *= f.x;
			this->y *= f.y;
			this->z *= f.z;
			return *this;
		}

	};

	class Vector4 : public Vector
	{
	public:

		float z, w;

	public:

		/*コンストラクタ*/
		inline Vector4() {
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		}
		inline Vector4(crfloat x, crfloat y, crfloat z, crfloat w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		/*シリアライズ*/
		template <class DATA>
		void serialize(DATA& data) { data(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w)); }

		/*単位ベクトル*/
		inline static const Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
		inline static const Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		inline static const Vector4 Left() { return Vector4(-1.0f, 0.0f, 0.0f, 0.0f); }
		inline static const Vector4 Right() { return Vector4(1.0f, 0.0f, 0.0f, 0.0f); }
		inline static const Vector4 Up() { return Vector4(0.0f, 1.0f, 0.0f, 0.0f); }
		inline static const Vector4 Down() { return Vector4(0.0f, -1.0f, 0.0f, 0.0f); }
		inline static const Vector4 Forward() { return Vector4(0.0f, 0.0f, 1.0f, 0.0f); }
		inline static const Vector4 Back() { return Vector4(0.0f, 0.0f, -1.0f, 0.0f); }

		/*DirectXMathのXMFLOAT4に変換*/
		inline const XMFLOAT4 Float4()const { return XMFLOAT4(this->x, this->y, this->z, this->w); }
		inline static const XMFLOAT4 Float4(const Vector4& data) { return XMFLOAT4(data.x, data.y, data.z, data.w); }

		/*XMVECTORに変換*/
		inline const XMVECTOR Vector()const { return XMVectorSet(x, y, z, w); }
		inline static const XMVECTOR Vector(const Vector4& vector) { return XMVectorSet(vector.x, vector.y, vector.z, vector.w); }

		/*長さ*/
		inline static cfloat Length(const Vector4& data) { return sqrtf((data.x * data.x) + (data.y * data.y) + (data.z * data.z) + (data.w * data.w)); }
		inline static cfloat LengthSq(const Vector4& data) { return ((data.x * data.x) + (data.y * data.y) + (data.z * data.z) + (data.w * data.w)); }

		/*内積*/
		inline static cfloat Dot(const Vector4& data1, const Vector4& data2) { return ((data1.x * data2.x) + (data1.y * data2.y) + (data1.z * data2.z) + (data1.w * data2.w)); }

		/*正規化*/
		inline static const Vector4 Normalize(Vector4* data) {

			cfloat len = Vector4::Length((*data));

			(*data).x /= len;
			(*data).y /= len;
			(*data).z /= len;
			(*data).w /= len;

			return (*data);

		}

		/*オペレーター*/
		inline Vector4& operator = (const XMFLOAT4& data) {
			this->x = data.x;
			this->y = data.y;
			this->z = data.z;
			this->w = data.w;
			return *this;
		}
		inline Vector4& operator = (const Vector4& data) {
			this->x = data.x;
			this->y = data.y;
			this->z = data.z;
			this->w = data.w;
			return *this;
		}

		inline Vector4 operator + (const Vector4& data)const { return Vector4(this->x + data.x, this->y + data.y, this->z + data.z, this->w + data.w); }
		inline Vector4& operator += (const Vector4& data) {
			this->x += data.x;
			this->y += data.y;
			this->z += data.z;
			this->w += data.w;
			return *this;
		}

		inline Vector4 operator - (const Vector4& data)const { return Vector4(this->x - data.x, this->y - data.y, this->z - data.z, this->w - data.w); }
		inline Vector4& operator -= (const Vector4& data) {
			this->x -= data.x;
			this->y -= data.y;
			this->z -= data.z;
			this->w -= data.w;
			return *this;
		}

		inline Vector4 operator * (const Vector4& data)const { return Vector4(this->x * data.x, this->y * data.y, this->z * data.z, this->w * data.w); }
		inline Vector4& operator *= (const Vector4& data) {
			this->x *= data.x;
			this->y *= data.y;
			this->z *= data.z;
			this->w *= data.w;
			return *this;
		}
		inline Vector4 operator * (crfloat data)const { return Vector4(this->x * data, this->y * data, this->z * data, this->w * data); }
		inline Vector4& operator *= (crfloat data) {
			this->x *= data;
			this->y *= data;
			this->z *= data;
			this->w *= data;
			return *this;
		}

		inline Vector4 operator / (const Vector4& data)const {
			Vector4 f = Vector4(1.0f / data.x, 1.0f / data.y, 1.0f / data.z, 1.0f / data.w);
			return Vector4(this->x * f.x, this->y * f.y, this->z * f.z, this->w * f.w);
		}
		inline Vector4& operator /= (const Vector4& data) {
			Vector4 f = Vector4(1.0f / data.x, 1.0f / data.y, 1.0f / data.z, 1.0f / data.w);
			this->x *= f.x;
			this->y *= f.y;
			this->z *= f.z;
			this->w *= f.w;
			return *this;
		}
		inline Vector4 operator / (crfloat data)const {
			float f = 1.0f / data;
			return Vector4(this->x * f, this->y * f, this->z * f, this->w * f);
		}
		inline Vector4& operator /= (crfloat data) {
			float f = 1.0f / data;
			this->x *= f;
			this->y *= f;
			this->z *= f;
			this->w *= f;
			return *this;
		}

		inline cbool operator == (const Vector4& data)const { return this->x == data.x && this->y == data.y && this->z == data.z && this->w == data.w; }
		inline cbool operator != (const Vector4& data)const { return this->x != data.x || this->y != data.y || this->z != data.z || this->w != data.w; }
		inline cbool operator < (const Vector4& data)const { return this->x < data.x && this->y < data.y && this->z < data.z && this->w < data.w; }
		inline cbool operator <= (const Vector4& data)const { return this->x <= data.x && this->y <= data.y && this->z <= data.z && this->w <= data.w; }
		inline cbool operator > (const Vector4& data)const { return this->x > data.x && this->y > data.y && this->z > data.z && this->w > data.w; }
		inline cbool operator >= (const Vector4& data)const { return this->x >= data.x && this->y >= data.y && this->z >= data.z && this->w >= data.w; }

		/*XMFLOAT3のオペレーター*/
		inline Vector4 operator + (const XMFLOAT4& data)const { return Vector4(this->x + data.x, this->y + data.y, this->z + data.z, this->w + data.w); }
		inline Vector4& operator += (const XMFLOAT4& data) {
			this->x += data.x;
			this->y += data.y;
			this->z += data.z;
			this->w += data.w;
			return *this;
		}

		inline Vector4 operator - (const XMFLOAT4& data)const { return Vector4(this->x - data.x, this->y - data.y, this->z - data.z, this->w - data.w); }
		inline Vector4& operator -= (const XMFLOAT4& data) {
			this->x -= data.x;
			this->y -= data.y;
			this->z -= data.z;
			this->w -= data.w;
			return *this;
		}

		inline Vector4 operator * (const XMFLOAT4& data)const { return Vector4(this->x * data.x, this->y * data.y, this->z * data.z, this->w * data.w); }
		inline Vector4& operator *= (const XMFLOAT4& data) {
			this->x *= data.x;
			this->y *= data.y;
			this->z *= data.z;
			this->w *= data.w;
			return *this;
		}

		inline Vector4 operator / (const XMFLOAT4& data)const {
			Vector4 f = Vector4(1.0f / data.x, 1.0f / data.y, 1.0f / data.z, 1.0f / data.w);
			return Vector4(this->x * f.x, this->y * f.y, this->z * f.z, this->w * f.w);
		}
		inline Vector4& operator /= (const XMFLOAT4& data) {
			Vector4 f = Vector4(1.0f / data.x, 1.0f / data.y, 1.0f / data.z, 1.0f / data.w);
			this->x *= f.x;
			this->y *= f.y;
			this->z *= f.z;
			this->w *= f.w;
			return *this;
		}

	};

	class Color
	{
	public:

		float r, g, b, a;

	public:

		inline Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
		inline Color(crfloat r, crfloat g, crfloat b, crfloat a) :
			r(r), g(g), b(b), a(a) {}

		template <class DATA>
		void serialize(DATA& data) { data(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(a)); }

		inline void operator = (const XMFLOAT4& data) {
			this->r = data.x;
			this->g = data.y;
			this->b = data.z;
			this->a = data.w;
		}
		inline void operator = (const Vector4& data) {
			this->r = data.x;
			this->g = data.y;
			this->b = data.z;
			this->a = data.w;
		}

		inline Color operator + (const Color& data)const { return Color(this->r + data.r, this->g + data.g, this->b + data.b, this->a + data.a); }
		inline Color& operator += (const Color& data) {
			this->r += data.r;
			this->g += data.g;
			this->b += data.b;
			this->a += data.a;
			return *this;
		}
		inline Color operator - (const Color& data)const { return Color(this->r - data.r, this->g - data.g, this->b - data.b, this->a - data.a); }
		inline Color& operator -= (const Color& data) {
			this->r -= data.r;
			this->g -= data.g;
			this->b -= data.b;
			this->a -= data.a;
			return *this;
		}
		inline Color operator * (const Color& data)const { return Color(this->r * data.r, this->g * data.g, this->b * data.b, this->a * data.a); }
		inline Color& operator *= (const Color& data) {
			this->r *= data.r;
			this->g *= data.g;
			this->b *= data.b;
			this->a *= data.a;
			return *this;
		}

		inline static const Color Black(crfloat a = 1.0f) { return Color(0.0f, 0.0f, 0.0f, a); }
		inline static const Color White(crfloat a = 1.0f) { return Color(1.0f, 1.0f, 1.0f, a); }
		inline static const Color Rrd(crfloat r = 1.0f, crfloat a = 1.0f) { return Color(r, 0.0f, 0.0f, a); }
		inline static const Color Green(crfloat g = 1.0f, crfloat a = 1.0f) { return Color(0.0f, g, 0.0f, a); }
		inline static const Color Blue(crfloat b = 1.0f, crfloat a = 1.0f) { return Color(0.0f, 0.0f, b, a); }

	};

	/*コンスト*/
	using cVector2 = const Vector2;
	using cVector3 = const Vector3;
	using cVector4 = const Vector4;
	using cColor = const Color;
	/*参照系*/
	using rVector2 = Vector2&;
	using rVector3 = Vector3&;
	using rVector4 = Vector4&;
	using rColor = Color&;
	/*コンスト参照*/
	using crVector2 = cVector2&;
	using crVector3 = cVector3&;
	using crVector4 = cVector4&;
	using crColor = cColor&;
	/*ポインタ*/
	using pVector2 = Vector2*;
	using pVector3 = Vector3*;
	using pVector4 = Vector4*;
	using pColor = Color*;
	/*コンストポインタ*/
	using cpVector2 = const pVector2;
	using cpVector3 = const pVector3;
	using cpVector4 = const pVector4;
	using cpColor = const pColor;

}

using namespace Vector;
