#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF01/10
*
********************************************************************************************
* StageSelectObject
********************************************************************************************/

#include"GameObject.h"

class StageSelectObject : public GameObject
{
private:

	class SelectUI : public GameObject
	{
	private:

		DrawInfo mInfo{};
		uint mTexID = -1;

	public:

		void Init()override;
		void Start()override;
		void Update()override;

		const DrawInfo GetDrawInfo()const override { return mInfo; }

	};

	typedef struct SelectInfo {

	public:

		string scene{};
		color color = Color::normal();

		template <class DATA>
		void serialize(DATA& data) {
			Color color(this->color);
			data(CEREAL_NVP(scene), CEREAL_NVP(color));
			color.Set(&this->color);
		}

	}SelectInfo;

private:

	static unordered_map<int, StageSelectObject::SelectInfo> mLoadSceneData;
	static int mSelectIndex;
	
	uint mModelID = -1;
	string mLoadScene{};
	SelectInfo mInfo{};

public:

	StageSelectObject() {}

	static void SceneDataLoad();

	void Init()override;
	void Start()override;
	void Update()override;
	void Draw()override;
	void Uninit()override;

	void OnCollisionEnter(crint id)override;
	void OnCollisionExit(crint id)override;
	void OnCollisionStay(crint id)override;

};