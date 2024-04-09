#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：9/14
*
********************************************************************************************
* 描画管理クラス
********************************************************************************************/

#include"Main.h"

class GameObject;

enum class RendererPath {

	beginbg,
	begindepth,
	luminace,
	bloom,
	beginpp,
	begin,

	max
};

typedef struct {
	RendererPath path;
	int index;
	string name;
}Order;

class Layer
{
private:

	class Path
	{
	private:

		unordered_map<RendererPath, bool> mPath{};
		class GameObject* mPlayer{};

		void SetPathBegin();
		void SetPathPP();
		void SetPathBG();
		void SetPathDepth();
		void SetPathLuminance();

	public:

		void Update();
		void SetPath(const RendererPath& path);
		void SetPlayer(GameObject* player) { mPlayer = player; }

	};

	unordered_map<string, list<GameObject*>> mLayer{};
	Path mPath{};
	list<Order> mOrder{};

public:

	void Update();
	void Draw();
	void Uninit();

	void AddLayer(crstring layer, RendererPath path);
	void AddLayer(const Order& layer);
	void AddGameObject(GameObject* obj);

	void AddLayerGameObject(GameObject* obj, crstring layer);

};