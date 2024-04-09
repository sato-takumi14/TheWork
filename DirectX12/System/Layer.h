#pragma once

/***********************************************************************************************************
*
*	Layer
*
*																				����ҁF������
*																				������F02/20
*
************************************************************************************************************
* Scene��ɑ��݂���I�u�W�F�N�g�̕`����Ǘ�����N���X
************************************************************************************************************/

#include"Interface\GameObject.h"

class Layer
{
private:

	class Path
	{
	public:

		int mIndex{};
		std::function<void()> mBegin{};
		std::list<GameObject::Interface*> mObjects{};

	};

private:

	std::unordered_map<string, Path> mLayer{};

public:

	void Update();
	void Draw();
	void Uninit();

	void AddLayer(crstring path, crint index, const std::function<void()>& func);
	void AddGameObject(GameObject::Interface* obj);

};