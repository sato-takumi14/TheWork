#pragma once

/***********************************************************************************************************
*
*	Manager
*
*																				制作者：佐藤匠
*																				制作日：02/29
*
************************************************************************************************************
* オブジェクトを管理するマネージャーの基底クラス
************************************************************************************************************/

#include"Main.h"
#include"Component\Transform.h"

class Manager
{
protected:

	class Scene* mScene{};
	string mFilePath{};
	string mTag{};
	bool mStart{};
	bool mDestroy{};

protected:

	void CreateGameObject(crstring name, const std::list<Transform>& transforms)const;

public:

	inline virtual void Init() {}
	inline virtual void Start() {}
	inline virtual void Update() {}
	inline virtual void FixedUpdate() {}
	inline virtual void Draw() {}
	inline virtual void Uninit() {}

	inline void SetScene(Scene* scene) { mScene = scene; }
	inline void SetFilePath(crstring path) { mFilePath = path; }
	inline void SetTag(crstring tag) { mTag = tag; }
	inline void SetStart(crbool set) { mStart = set; }
	inline void SetDestroy(crbool set = true) { mDestroy = set; }

	inline Scene* GetScene() { return mScene; }
	inline cstring GetFilePath()const { return mFilePath; }
	inline cstring GetTag()const { return mTag; }
	inline cbool GetStart()const { return mStart; }
	inline cbool GetDestroy()const { return mDestroy; }

	inline cbool Destroy() {
		if (mDestroy) {
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

};