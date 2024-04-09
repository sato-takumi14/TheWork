
#include "Main.h"
#include "Layer.h"
#include"Renderer.h"

void Layer::Update()
{

	for (auto& data : mLayer) {
		data.second.mObjects.remove_if(
			[](GameObject::Interface* obj)
			{ return obj->GetDestory(); }
		);
	}

}

void Layer::Draw()
{

	Renderer::Begin();

	cint size = static_cast<cint>(mLayer.size());

	for (int i = 0; i < size; i++) {

		for (auto& data : mLayer) {

			if (data.second.mIndex == i) {

				data.second.mBegin();

				for (auto& obj : data.second.mObjects) {
					obj->Draw();
				}

				break;

			}

		}

	}

	Renderer::End();

}

void Layer::Uninit()
{

	mLayer.clear();

}

void Layer::AddLayer(crstring path, crint index, const std::function<void()>& func)
{

	mLayer[path] = Path{ index,func,std::list<GameObject::Interface*>() };

}

void Layer::AddGameObject(GameObject::Interface* obj)
{

	auto layer = obj->GetLayer();
	for (auto& data : layer) { mLayer[data].mObjects.push_back(obj); }

}
