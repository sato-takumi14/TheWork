#pragma once

/***********************************************************************************************************
*
*	Interface
*
*																				制作者：佐藤匠
*																				制作日：02/19
*
************************************************************************************************************
* セットなど必要な時に多重継承で継承させるクラス
************************************************************************************************************/

#include"Main.h"
#include"System\Vector.h"

class Interface
{
private:



public:

	inline virtual void Set() {}
	inline virtual void Set(crint set) {}
	inline virtual void Set(cruint set) {}
	inline virtual void Set(crfloat set) {}
	inline virtual void Set(crdouble set) {}
	inline virtual void Set(crbool set) {}
	inline virtual void Set(crstring set) {}
	inline virtual void Set(crVector2 set) {}
	inline virtual void Set(crVector3 set) {}
	inline virtual void Set(crVector4 set) {}
	inline virtual void Set(crColor set) {}

	inline virtual cint		GetInt()const				{ return -1; }
	inline virtual cuint	GetUInt()const				{ return -1; }
	inline virtual cfloat	GetFloat()const				{ return 0.0f; }
	inline virtual cdouble	GetDouble()const			{ return 0.0; }
	inline virtual cbool	GetBool()const				{ return true; }
	inline virtual cstring	GetString()const			{ return ""; }
	inline virtual cVector2 GetVector2()const			{ return cVector2(); }
	inline virtual cVector3 GetVector3()const			{ return cVector3(); }
	inline virtual cVector4 GetVector4()const			{ return cVector4(); }
	inline virtual cColor	GetColor()const				{ return cColor(); }
	inline virtual cmatrix  GetMatrix()const			{ return cmatrix(); }

	inline virtual int*		GetPInt()					{ return nullptr; }
	inline virtual float*	GetPFloat()					{ return nullptr; }
	inline virtual double*	GetPDouble()				{ return nullptr; }
	inline virtual bool*	GetPBool()					{ return nullptr; }
	inline virtual string*	GetPString()				{ return nullptr; }
	inline virtual pVector2 GetPVector2()				{ return nullptr; }
	inline virtual pVector3 GetPVector3()				{ return nullptr; }
	inline virtual pVector4 GetPVector4()				{ return nullptr; }
	inline virtual pColor	GetPColor()					{ return nullptr; }

};