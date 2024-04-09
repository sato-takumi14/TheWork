#pragma once

/***********************************************************************************************************
*
*	GameObject
*
*																				制作者：佐藤匠
*																				制作日：02/20
*
************************************************************************************************************
* Scene上に存在するオブジェクトの基底クラス
************************************************************************************************************/

#include"Component\Transform.h"
#include"Component\Parent.h"
#include"Interface\DrawInterface.h"
#include"Manager\StateManager.h"

class Scene;

namespace GameObject
{

	class Interface
	{
	private:

		bool mStart{};

	protected:

		/*クラス*/
		Scene* mScene{};
		Transform mTransform;
		DrawInterface::Device* mDrawDevice{};
		Parent::Device* mParent{};

		/*Manager*/
		StateManager::Device* mStateDevice{};

		/*ID*/
		uint mObjectID = -1;
		uint mDrawNum = 1;

		/*bool*/
		bool mDestroy{};
		bool mActive = true;
		bool mDraw = true;

		/*タグ*/
		string mTag{};

		/*コンポーネント*/
		std::list<Component*> mComponent{};
		std::list<Component*> mStartComponent{};

		/*レイヤーの名前リスト*/
		std::list<string> mLayer{};

	public:

		/*通常処理*/
		inline virtual void Init() {
			mParent = new Parent::Device(this);
			AddComponent(mParent);
			AddComponent(&mTransform);
		}
		inline virtual void Start() {
			if (not mActive)return;
			for (auto& com : mStartComponent) {
				com->Start();
				com->SetStart(true);
			}
			std::erase_if(mStartComponent, [](Component* com) { return com->GetStart(); });
		}

		inline virtual void Update() {
			if (not mActive)return;
			for (auto& com : mComponent) { com->Update(); }
			std::erase_if(mComponent, [](Component* com) { return com->Destroy(); });
			if (mStateDevice)mStateDevice->Update();
		}

		inline virtual void FixedUpdate() {
			if (not mActive)return;
			for (auto& com : mComponent) { com->FixedUpdate(); }
			std::erase_if(mComponent, [](Component* com) { return com->Destroy(); });
			if (mStateDevice)mStateDevice->FixedUpdate();
		}

		inline virtual void Draw() {
			if (not mActive && not mDraw)return;
			for (auto& com : mComponent) { com->Draw(); }
			if (mDrawDevice)mDrawDevice->Draw();
			if (mStateDevice)mStateDevice->Draw();
		}

		inline virtual void Uninit() {
			for (auto& com : mComponent) {
				com->Uninit();
				delete com;
			}
			mComponent.clear();
			mStartComponent.clear();
			if (mDrawDevice) {
				mDrawDevice->Uninit();
				delete mDrawDevice;
			}
			if (mStateDevice) {
				mStateDevice->Uninit();
				delete mStateDevice;
				mStateDevice = nullptr;
			}
		}

		/*Get*/
		inline cuint GetObjectID()const								{ return mObjectID; }
		inline cuint GetDrawNum()const								{ return mDrawNum; }

		inline crstring GetTag()const								{ return mTag; }

		inline cbool GetDestory()const								{ return mDestroy; }
		inline cbool GetStart()const								{ return mStart; }
		inline cbool GetActive()const								{ return mActive; }
		inline cbool GetDraw()const									{ return mDraw; }

		inline cVector3 GetWorldPosition()const						{ return mTransform.pos; }
		inline cVector3 GetLocalPosition()const						{ return mTransform.local; }
		inline cVector3 GetRotation()const							{ return mTransform.rot; }
		inline cVector3 GetScale()const								{ return mTransform.scale; }
		inline const Transform GetReferenceTransform()const			{ return mTransform; }
		inline Transform* GetTransform()							{ return &mTransform; }

		inline Scene* GetScene()									{ return mScene; }
		inline Parent::Device* GetParent()							{ return mParent; }

		inline const std::list<string> GetLayer()const				{ return mLayer; }

		inline cmatrix GetMatrix()const								{ return matrix(); }

		/*Set*/
		inline void SetObjectID(cruint id)							{ mObjectID = id; }
		inline void SetDrawNum(cruint num)							{ mDrawNum = num; }

		inline void SetTag(crstring tag)							{ mTag = tag; }
		inline void SetStart(crbool start)							{ mStart = start; }
		inline void SetScene(Scene* scene)							{ mScene = scene; }
		inline void SetActive(crbool active)						{ mActive = active; }
		inline void SetDraw(crbool draw)							{ mDraw = draw; }

		inline void SetWorldPosition(const Vector3& pos)			{ mTransform.pos = pos; }
		inline void SetAddWorldPosition(const Vector3& add)			{ mTransform.pos += add; }
		inline void SetLocalPosition(const Vector3& local) {
			mTransform.local = local;
			mParent->SetWorldPosition();
		}
		inline void SetAddLocalPosition(const Vector3& add) {
			mTransform.local += add;
			mParent->SetWorldPosition();
		}
		inline void SetRotation(const Vector3& rot)					{ mTransform.rot = rot; }
		inline void SetAddRotation(const Vector3& add)				{ mTransform.rot += add; }
		inline void SetScale(const Vector3& scale)					{ mTransform.scale = scale; }
		inline void SetAddScale(const Vector3& add)					{ mTransform.scale += add; }
		inline void SetTransform(const Transform& transform)		{ mTransform = transform; }
		inline virtual void SetObject(Interface* obj)				{}

		/*その他*/
		inline cbool Destroy() {
			if (mDestroy) {
				Uninit();
				delete this;
				return true;
			}
			else return false;
		}

		inline void UndoWorldPosition() { mTransform.UndoPosition(); }
		inline void UndoLocalPosition() { mTransform.UndoLocalPosition(); }
		inline void UndoRotation()		{ mTransform.UndoRotation(); }
		inline void UndoScale()			{ mTransform.UndoScale(); }

		/*コンポーネント追加*/
		inline void AddComponent(Component* com) {
			com->Init();
			mComponent.push_back(com);
			mStartComponent.push_back(com);
		}

		/*当たり判定*/
		inline virtual void OnCollisionEnter(cruint id) {}
		inline virtual void OnCollisionExit(cruint id) {}
		inline virtual void OnCollisionStay(cruint id) {}

	};

	class ThreeD : public Interface
	{
	private:



	protected:

		uint mModelID = -1;

	public:

		inline cuint GetModelID()const { return mModelID; }

		inline void SetModelID(cruint id) { mModelID = id; }

	};

	class TwoD : public Interface
	{
	private:



	protected:

		uint mTexID = -1;

	public:

		inline cuint GetTexID()const { return mTexID; }

		inline void SetTexID(cruint id) { mTexID = id; }

	};

	using pGameObjectInterface = Interface*;
	using pGameObject3D = ThreeD*;
	using pGameObject2D = TwoD*;

}
