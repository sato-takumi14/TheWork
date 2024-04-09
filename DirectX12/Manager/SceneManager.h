#pragma once

/***********************************************************************************************************
*
*	SceneManager
*
*																				制作者：佐藤匠
*																				制作日：02/20
*
************************************************************************************************************
* Sceneクラスを管理するクラス
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