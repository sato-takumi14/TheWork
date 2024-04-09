#pragma once

/***********************************************************************************************************
*
*	SceneManager
*
*																				����ҁF������
*																				������F02/20
*
************************************************************************************************************
* Scene�N���X���Ǘ�����N���X
************************************************************************************************************/

class Scene;

class SceneManager
{
private:

	static class Scene* mScene;
	static class Scene* mNextScene;

public:

	static void MasterInit();
	static void MasterUpdate();
	static void MasterDraw();
	static void MasterUninit();

	static Scene* GetScene() { return mScene; }
	static void SceneChange(Scene* scene);

};