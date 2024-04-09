#pragma once

#include"Main.h"
#include"Component.h"

#define FUNCTION __func__

typedef struct {

	char function[256];
	char name[256];
	float data;
	char newline;

}DebugData;

typedef struct {

	int id;


}DebugDataSub;

class Debug : public Component
{
private:

	class DebugValueData
	{
	protected:

		int mID = -1;

	public:

		DebugValueData() {}
		cint GetID()const { return mID; }
		virtual void Update() = 0;

	};

	class DebugValueDataFloat : public DebugValueData
	{
	private:

		float* mValue{};
		float mAdd{};

	public:

		DebugValueDataFloat() : mValue(nullptr), mAdd(0.0f) {}
		DebugValueDataFloat(crint id, float* value, crfloat add) : mValue(value), mAdd(add) { this->mID = id; }
		void Update()override;

	};

private:

	static list<DebugData> mData;
	static list<function<void()>> mResetFunction;

	int mIndex = 0;
	int mNowIndex = 0;
	list<DebugValueData*> mDebugData{};

public:

	Debug() {}

	void Update()override;
	void Uninit()override;

	cint SetDebugFloat(float* value, crfloat add);

	static void MasterUpdate();
	static void MasterUninit();
	static void Set(crstring function, crstring name, crfloat data);

	static void Log(crint data);
	static void Log(crfloat data);
	static void Log(crdouble data);
	static void Log(crstring data);
	static void Log(crver2 data);
	static void Log(crver3 data);
	static void Log(crver4 data);
	static void Log(crcolor data);

	static void AddResetFunction(const function<void()>& func);

};