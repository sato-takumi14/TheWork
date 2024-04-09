#pragma once

#include"Main.h"
#include"System\AssimpLoader.h"

class Scene;

namespace ModelManager
{

	class Data
	{
	public:
		class Assimp::Model* model;
		string file;
		std::list<string> anim;
		string scene;
	};

	class Device
	{
	private:

		static std::unordered_map<uint, Data> mAssimpModelData;
		static uint mIndex;
		static Scene* mScene;

	public:

		static void MasterInit();
		static void MasterUninit();

		static cuint LoadModel(crstring file);
		static cbool LoadAnimation(cruint id, crstring file, crstring name);
		static cbool LoadAnimation(cruint id, crstring file, crstring name, crfloat blend);
		static Assimp::Model* GetModel(cruint id);

		inline static void SetScene(Scene* scene) { mScene = scene; }

	};
}