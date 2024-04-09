
#include"Main.h"

#include "Debug.h"

#include"Input.h"
#include"File.h"
#include"dbgstream.h"
#include"ActionKey.h"
#include"Input.h"

list<DebugData> Debug::mData{};
list<function<void()>> Debug::mResetFunction{};

void Debug::Update()
{

	for (auto& data : mDebugData) {
		if (mNowIndex == data->GetID()) {
			data->Update();
		}
	}

}

void Debug::Uninit()
{

	for (auto& data : mDebugData) {
		delete data;
	}
	mDebugData.clear();

}

cint Debug::SetDebugFloat(float* value, crfloat add)
{
	mDebugData.push_back(new DebugValueDataFloat(mIndex, value, add));
	return mIndex++;
}

void Debug::MasterUpdate()
{

	if (ActionKey::Action("reset")) {
		for (auto& data : mResetFunction)data();
		Debug::Log("Reset");
	}

}

void Debug::MasterUninit()
{

	File::WriteDatas<DebugData>(mData, "asset\\debug\\debug");

	mData.clear();
	mResetFunction.clear();

}

void Debug::Set(crstring function, crstring name, crfloat data)
{

	DebugData debug{};

	char_traits<char>::copy(debug.function, function.c_str(), 256);
	char_traits<char>::copy(debug.name, name.c_str(), 256);
	debug.data = data;
	debug.newline = '\n';

	mData.emplace_back(debug);

}

void Debug::Log(crint data)
{
	cdbg << data << endl;
}

void Debug::Log(crfloat data)
{	
	cdbg << data << endl;
}

void Debug::Log(crdouble data)
{
	cdbg << data << endl;
}

void Debug::Log(crstring data)
{
	cdbg << data << endl;
}

void Debug::Log(crver2 data)
{
	cdbg << "x : " << data.x << " y : " << data.y << endl;
}

void Debug::Log(crver3 data)
{
	cdbg << "x : " << data.x << " y : " << data.y << " z : " << data.z << endl;
}

void Debug::Log(crver4 data)
{
	cdbg << "x : " << data.x << " y : " << data.y << " z : " << data.z << " w : " << data.w << endl;
}

void Debug::Log(crcolor data)
{
	cdbg << "r : " << data.r << " g : " << data.g << " b : " << data.b << " a : " << data.a << endl;
}

void Debug::AddResetFunction(const function<void()>& func)
{
	mResetFunction.push_back(func);
}

void Debug::DebugValueDataFloat::Update()
{

	if (Input::GetKeyPress(DIK_UP)) {
		(*mValue) += mAdd;		
		Debug::Log("ílè„è∏");
	}

	if (Input::GetKeyPress(DIK_DOWN)) {
		(*mValue) -= mAdd;
		Debug::Log("ílâ∫ç~");
	}

	Debug::Log((*mValue));

}
