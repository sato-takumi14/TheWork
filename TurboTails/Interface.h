#pragma once

/*******************************************************************************************
* 
*																制作者：佐藤匠
*																制作日：9/14
* 
********************************************************************************************
* 全てのインターフェース用の基底クラス
********************************************************************************************/

#include"Main.h"

class Interface
{
protected:

	bool mDestroy{};

public:

	virtual void Init() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void Draw() {}
	virtual void Uninit() {}

	cbool Destroy() {
		if (mDestroy) {
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

	cbool GetDestroy()const { return mDestroy; }
	void SetDestroy(crbool set = true) { mDestroy = set; }

	virtual void Set() {}
	virtual void Set(crint set) {}
	virtual void Set(cruint set) {}
	virtual void Set(crfloat set) {}
	virtual void Set(crdouble set) {}
	virtual void Set(crbool set) {}
	virtual void Set(crver2 set) {}
	virtual void Set(crver3 set) {}
	virtual void Set(crver4 set) {}
	virtual void Set(crcolor set) {}
	virtual void Set(crstring set) {}

	virtual cint GetInt()const { return 0; }
	virtual cuint GetUnsignedInt()const { return 0; }
	virtual cfloat GetFloat()const { return 0.0f; }
	virtual cdouble GetDouble()const { return 0.0; }
	virtual cbool GetBool()const { return false; }
	virtual cver2 GetVer2()const { return ver2(); }
	virtual cver3 GetVer3()const { return ver3(); }
	virtual cver4 GetVer4()const { return ver4(); }
	virtual ccolor GetColor()const { return color(); }
	virtual cstring GetString()const { return string(); }

};