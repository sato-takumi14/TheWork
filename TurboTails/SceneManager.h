#pragma once

class Scene;

class SceneManager
{
private:

	static Scene* mScene;
	static Scene* mNextScene;
	static Scene* mNextSceneLoad;

public:

	static void Init();
	static void Update();
	static void Draw();
	static void Uninit();

	static Scene* GetScene() { return mScene; }
	static void SceneChange(Scene* next);

	static void SetSceneLoad(Scene* next);
	static void SetNextSceneLoad(Scene* next);
	static void SetLoadingNextScene(Scene* next);

};