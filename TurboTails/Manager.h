#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/14
*
********************************************************************************************
* �I�u�W�F�N�g���Ǘ�������N���X
********************************************************************************************/

#include"Interface.h"

#include"Transform.h"

#include<list>

class GameObject;

class Manager : public Interface
{
protected:

	class Scene* mScene{};
	string mFilePath{};
	uint mManagerID = -1;
	string mTag{};

protected:

	void CreateMultiGameObject(crstring name, cruint state, const list<Transform> transforms)const;
	void CreateGameObject(crstring name, const list<Transform> transforms)const;
	void CreateGameObjects(crstring name, const list<Transform> transforms)const;

public:

	void SetScene(Scene* scene) { mScene = scene; }
	void SetFilePath(crstring path) { mFilePath = path; }

	void SetManagerID(cruint id) { mManagerID = id; }
	cuint GetManagerID()const { return mManagerID; }

	cstring GetTag()const { return mTag; }

};