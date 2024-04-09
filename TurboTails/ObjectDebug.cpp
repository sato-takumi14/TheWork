
#include"Main.h"

#include "ObjectDebug.h"

#include"Scene.h"
#include"SceneManager.h"
#include"GameObject.h"
#include"File.h"

typedef struct ObjectData {

	Transform Transform;
	string ClassName;
	string Tag;

	template <class DATA>
	void serialize(DATA& data) { data(CEREAL_NVP(Transform), CEREAL_NVP(ClassName), CEREAL_NVP(Tag)); }

}ObjectData;

list<GameObject*> ObjectDebug::mObject{};
string ObjectDebug::mFileName{};
unordered_map<string, list<GameObject*>> ObjectDebug::mObjectDatas{};

void ObjectDebug::Update()
{

	mObject.remove_if([](GameObject* obj) { return obj->GetDestroy(); });

	for (auto& data : mObjectDatas) { data.second.remove_if([](GameObject* obj) { return obj->GetDestroy(); }); }
	
}

void ObjectDebug::Uninit()
{

	
	Scene* scene = SceneManager::GetScene();
	string file = "asset\\debug\\" + scene->GetName() + ".json";

	unordered_map<string, list<Transform>> debug;

	for (auto& data : mObjectDatas) {

		for (auto& obj : data.second) {
			ObjectData debugdata;
			debug[data.first].push_back(obj->GetReferenceTransform());
		}
		
	}

	File::JsonSaves<Transform>(debug, file);

	mFileName = "";

	mObject.clear();

	for (auto& data : mObjectDatas) {
		data.second.clear();
	}

	mObjectDatas.clear();

}

void ObjectDebug::Load(crstring file)
{

	mFileName = "asset\\debug\\" + file;
	list<ObjectData> datas = File::LoadDatasBinary<ObjectData>(mFileName);

}

void ObjectDebug::SetObject(GameObject* obj)
{

	mObject.push_back(obj);

	mObjectDatas[obj->GetClass()].push_back(obj);

}
