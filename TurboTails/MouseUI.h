#pragma once

/*******************************************************************************************
*
*																êßçÏé“ÅFç≤ì°è†
*																êßçÏì˙ÅF02/05
*
********************************************************************************************
* MouseUI
********************************************************************************************/

#include"GameObject.h"

class MouseUI : public GameObject
{
public:

	enum class SelectMouseUI {
		none,
		left,
		right,
		center,
	};

private:

	class MouseObject : public GameObject
	{
	private:

		DrawInfo mInfo{};
		uint mTexID = -1;
		ver4 mColor{};
		bool mFlag{};

	public:

		MouseObject(crstring file, crstring tag);

		void Init()override;
		void Start()override;
		void Update()override;

		const DrawInfo GetDrawInfo()const override { return mInfo; }

		void Set(crbool set)override { mFlag = set; }

	};

private:

	unordered_map<SelectMouseUI, GameObject*> mMouse{};

public:

	void Init()override;
	void Start()override;
	void Update()override;
	void Uninit()override;

	GameObject* GetMouseUI(const SelectMouseUI& ui) { return mMouse[ui]; }

	void Set(const SelectMouseUI& ui);

};