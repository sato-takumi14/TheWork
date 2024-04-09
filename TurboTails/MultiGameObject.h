#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F1/04
*
********************************************************************************************
* DrawInstanced�ŕ`�悷�邽�߂ɕ`�悷��I�u�W�F�N�g���܂Ƃ߂�I�u�W�F�N�g
********************************************************************************************/

#include"GameObject.h"

class MultiGameObject : public GameObject
{
public:

	/*�ǂ̕`������邩�̃X�e�[�g*/
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