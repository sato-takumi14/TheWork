#pragma once

/*******************************************************************************************
*
*																����ҁF������
*																������F9/14
*
********************************************************************************************
* �V�[����ɕ`�悳���I�u�W�F�N�g�̊��N���X
********************************************************************************************/

#include"Interface.h"

#include"Component.h"
#include"DrawInterface.h"
#include"Parent.h"
#include"StateManager.h"
#include"Transform.h"
#include"UI.h"
#include"AudioManager.h"

class MultiGameObject;

class GameObject : public Interface
{
private:

/*******************************************************************************************************************************************************/

	/*int*/
	int mClassID = -1;							/*�N���X����ID*/
	int mObjID = -1;							/*�I�u�W�F�N�g����ID*/

	/*bool*/
	bool mSetDebug{};							/*�f�o�b�O�ł̃Z�b�g���������ǂ���*/

	/*ver3*/
	ver3 mAddForce{};

/*******************************************************************************************************************************************************/

protected:

/*******************************************************************************************************************************************************/

	/*�ϐ�*/

	/*�N���X*/
	DrawInterface* mDraw{};						/*�`�悷��C���X�^���X*/
	class Scene* mScene{};						/*���݂̃V�[��*/
	Transform mTransform;						/*�g�����X�t�H�[��*/
	//TransformInfo mTransform{}
	Parent mParent{};							/*�e�q�֌W���Ǘ�����C���X�^���X*/
	StateManager* mStateManager{};				/*�X�e�[�g�p�^�[�����Ǘ�����C���X�^���X*/
	AudioManager* mAudioManager{};				/*�I�[�f�B�I���Ǘ�����C���X�^���X*/

	class MultiGameObject* mMultiGameObject{};

	D3DXQUATERNION mQuaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	/*�R���|�[�l���g*/
	list<Component*> mComponent{};				/*�R���|�[�l���g*/
	list<Component*> mStartComponent{};			/*�R���|�[�l���g�̃X�^�[�g���������邽�߂̃��X�g*/

	/*�\����*/

	/*string*/
	list<string> mLayer{};						/*���C���[��*/
	string mTag{};								/*�^�O*/
	string mClass{};							/*�N���X��*/

	/*bool*/
	bool mActivate = true;						/*�A�N�e�B�u���ǂ���*/
	
/*******************************************************************************************************************************************************/

protected:

	/*�p���悾���Ɏg���֐��Q*/
	void AddUV(ver3* uv) {

		uv->x += uv->z;
		uv->y += uv->z;

		if (uv->x >= 1.0f) {
			uv->x = 0.0;
			uv->y = uv->z;
		}

	}

/*******************************************************************************************************************************************************/

public:

	void Init()override {}

	void Update()override {

		if (!mActivate)return;

		for (auto& data : mStartComponent) {
			data->Start();
		}
		mStartComponent.clear();

		for (auto& data : mComponent) {
			data->Update();
		}

		if (mStateManager)mStateManager->Update();
		if (mAudioManager)mAudioManager->Update();

		mTransform.Update();
		mParent.Update();

	}

	void FixedUpdate()override {

		if (!mActivate)return;

		for (auto& data : mComponent) {
			data->FixedUpdate();
		}

	}

	void Draw()override {

		if (!mActivate)return;

		for (auto& data : mComponent) {
			data->Draw();
		}

		if (mDraw) {
			mDraw->Draw();
		}

	}

	void Uninit()override {

		for (auto& data : mComponent) {
			data->Uninit();
			delete data;
		}
		mComponent.clear();

		if (mDraw) {
			mDraw->Uninit();
			delete mDraw;
		}

		if (mStateManager)delete mStateManager;
		if (mAudioManager)delete mAudioManager;

	}

/*******************************************************************************************************************************************************/

	/*�����蔻��*/
	virtual void OnCollisionEnter(crint id) {}		/*���������u��*/
	virtual void OnCollisionExit(crint id) {}		/*���ꂽ�u��*/
	virtual void OnCollisionStay(crint id) {}		/*�������Ă����*/

/*******************************************************************************************************************************************************/
	/*Get*/

	/*string*/
	cstring GetTag()const							{ return mTag; }
	cstring GetClass()const							{ return mClass; }
	list<string> GetLayer()const					{ return mLayer; }
	virtual cstring GetShaderName()const			{ return ""; }
	virtual cstring GetVertexShaderName()const		{ return ""; }
	virtual cstring GetPixelShaderName()const		{ return ""; }
	virtual cstring GetParticleName()const			{ return ""; }

	/*int*/
	cint GetObjectID()const							{ return mObjID; }
	cint GetClassID()const							{ return mClassID; }
	virtual cuint GetModelID()const					{ return -1; }

	/*bool*/
	cbool GetDebug()const							{ return mSetDebug; }
	cbool GetActivate()const						{ return mActivate; }

	/*ver3*/
	virtual cver3 GetPosition()const				{ return mTransform.pos + mTransform.local; }
	virtual cver3 GetRotation()const				{ return mTransform.rot; }
	virtual cver3 GetScale()const					{ return mTransform.scale; }
	virtual cver3 GetLocalPosition()const			{ return mTransform.local; }

	virtual cver3 GetOffset()const					{ return Vector3::zero(); }
	cver3 GetAddForce()const						{ return mAddForce; }

	virtual cquat GetQuaternion()const				{ return quat(); }

	/*�|�C���^*/
	virtual int* GetPInt()							{ return nullptr; }
	virtual float* GetPFloat()						{ return nullptr; }
	virtual double* GetPDouble()					{ return nullptr; }
	virtual ver2* GetPVer2()						{ return nullptr; }
	virtual ver3* GetPVer3()						{ return nullptr; }
	virtual ver4* GetPVer4()						{ return nullptr; }


	/*�\���́E�N���X�E���̑�*/
	const Transform GetReferenceTransform()const	{ return mTransform; }
	Transform* GetTransform()						{ return &mTransform; }
	virtual const DrawInfo GetDrawInfo()const		{ return DrawInfo{}; }
	Parent* GetParent()								{ return &mParent; }
	virtual const MATERIAL GetMaterial()const		{ return MATERIAL(); }
	virtual const PARAMETER GetParameter()const		{ return PARAMETER(); }
	virtual GameObject* GetGameObject()const		{ return nullptr; }
	Scene* GetScene()const					{ return mScene; }

	virtual const list<Transform> GetTransforms()const { return list<Transform>(); }
	virtual const list<DrawInfo> GetDrawInfos()const { return list<DrawInfo>(); }

	virtual const D3DXMATRIX GetMatrix()const		{ return D3DXMATRIX(); }

	StateManager* GetStateManager()const {
		if (mStateManager)return mStateManager;
		return nullptr;
	}

/*******************************************************************************************************************************************************/
	/*Set*/

	/*int*/
	void SetObjectID(crint id)							{ mObjID = id; }
	void SetClassID(crint id)							{ mClassID = id; }
	virtual void SetModelID(cruint id)					{}

	/*string*/
	void SetTag(crstring tag)							{ mTag = tag; }
	virtual void SetAnim(crstring set)					{}
	virtual void SetVertexShaderName(crstring vertex)	{}
	virtual void SetPixelShaderName(crstring pixel)		{}

	/*bool*/
	void SetDebug(crbool set = true)					{ mSetDebug = set; }
	void SetActivate(crbool set = true)					{ mActivate = set; }

	/*ver3*/
	void SetPosition(crver3 pos)						{ mTransform.pos = (pos); }
	void SetRotation(crver3 rot)						{ mTransform.rot = (rot); }
	void SetScale(crver3 scale)							{ mTransform.scale = (scale); }
	void SetLocalPosition(crver3 local)					{ mTransform.local = local; }
	void SetAddPosition(crver3 add)						{ mTransform.pos += (add); }
	void SetAddRotation(crver3 add)						{ mTransform.rot += (add); }
	void SetAddScale(crver3 add)						{ mTransform.scale += (add); }
	void SetAddLocalPosition(crver3 add)				{ mTransform.local += (add); }

	/*matrix*/
	virtual void SetMatrix(const D3DXMATRIX& matrix)	{}
	virtual void SetQuartnion(crquat set)				{}

	/*���̑��N���X*/
	void SetScene(Scene* scene)							{ mScene = scene; }
	void SetTransform(const Transform& transform)		{ mTransform = transform; }
	virtual void AddTransform(const Transform& transform) {}
	virtual void SetMaterial(const MATERIAL& material)	{}

	virtual void SetGameObject(GameObject* set)			{}
	virtual void SetDrawInfo(const DrawInfo& info)		{}
	virtual void SetMultiGameObject(MultiGameObject* multi) {}

/*******************************************************************************************************************************************************/

	/*�R���|�[�l���g�ǉ�*/
	Component* AddComponent(Component* com) {
		com->Init();
		mComponent.push_back(com);
		return com;
	}

/*******************************************************************************************************************************************************/

	/*���̑�*/
	virtual void ChangeState(State* state, crint id) {}
	virtual cfloat GetHeight(crver3 pos)const { return cfloat(); }
	virtual GameObject* Create()const { return nullptr; }
	virtual const UI GetUIInfo()const { return UI{}; }
	virtual const list<GameObject*> GetGameObjects()const { return list<GameObject*>(); }

	void AddForce(crver3 add) { mAddForce = add; }
	void AddLayer(crstring add) { mLayer.push_back(add); }

	void UndoPosition() { mTransform.UndoPosition(); }

	void PlayAudio(crstring name) { mAudioManager->Play(name); }
	void StopAudio(crstring name) { mAudioManager->Stop(name); }

/*******************************************************************************************************************************************************/

};