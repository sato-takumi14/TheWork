#pragma once

/***********************************************************************************************************
*
*	Layer
*
*																				制作者：佐藤匠
*																				制作日：02/20
*
************************************************************************************************************
* Scene上に存在するオブジェクトの描画を管理するクラス
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