#pragma once

/***********************************************************************************************************
*
*	DrawInterface
*
*																				制作者：佐藤匠
*																				制作日：03/06
*
************************************************************************************************************
* 描画関係の基底クラスをまとめたもの
************************************************************************************************************/

#include"System\Renderer.h"
#include"Component\Transform.h"

class GameObjectInterface;

namespace DrawInterface
{

	/*描画情報*/
	class Info
	{
	public:

		pTransform target{};
		Vector3 wh;
		Vector3 u;
		Vector3 v;

	public:

		inline Info() : target(nullptr), wh(Vector3::Zero()), u(Vector3::Zero()), v(Vector3::Zero()) {}
		inline Info(pTransform target, crVector3 wh, crVector3 u = Vector3::Up(), crVector3 v = Vector3::Up()) :
			target(target), wh(wh), u(u), v(v) {}

		/*UV値を増加させる*/
		inline void AddUV() {
			u.x += u.z;
			u.y += u.z;
			if (u.x >= 1.0f) {
				v.x += v.z;
				v.y += v.z;
			}
		}

		/*UV値をリセット*/
		inline void ResetUV() {
			u.x = 0.0f;
			u.y = u.z;
			v.x = 0.0f;
			v.z = v.z;
		}

		/*whの値を生成する*/
		static cVector3 CreateWH(crfloat wh, crbool z = false) {
			if (z)return cVector3(wh, 0.0f, wh);
			return cVector3(wh, wh, 0.0f);
		}

		/*シリアライズ*/
		template <class DATA>
		void serialize(DATA& data) {
			Transform transform = *target;
			data(CEREAL_NVP(transform), CEREAL_NVP(wh), CEREAL_NVP(u), CEREAL_NVP(v));
			*target = transform;
		}

	};

	class Device
	{
	protected:

		GameObjectInterface* mTarget{};
		bool mDraw{};

	public:

		inline virtual void Draw() = 0;
		inline virtual void Uninit() {}

		inline void Set(crbool set) { mDraw = set; }
		inline cbool GetBool()const { return mDraw; }

	};

}