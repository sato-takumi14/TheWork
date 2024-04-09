#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：1/04
*
********************************************************************************************
* DrawInstancedで描画するために描画するオブジェクトをまとめるオブジェクト
********************************************************************************************/

#include"GameObject.h"

class MultiGameObject : public GameObject
{
public:

	/*どの描画をするかのステート*/
	enum class MultiDrawGameObjectState{
		none,
		model,
		material,
	};

private:

	list<GameObject*> mObjects{};
	list<Transform> mTransforms{};
	MultiDrawGameObjectState mState = MultiDrawGameObjectState::none;
	MATERIAL mMaterial{};

	uint mModelID = -1;
	string mShader{};

public:

	MultiGameObject(
		Scene* scene,
		crstring instance,
		const list<Transform>& transform, 
		const MultiDrawGameObjectState& state = MultiDrawGameObjectState::none);

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;
	
	void AddGameObject(GameObject* obj);

	const list<Transform> GetTransforms()const override;
	const MATERIAL GetMaterial()const override { return mMaterial; }
	const list<GameObject*> GetGameObjects()const override { return mObjects; }

	void Remove(crint id);

};