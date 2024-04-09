
#include"main.h"
#include "Box.h"

#include"ModelManager.h"
#include"Shader.h"
#include"Geometry.h"
#include"Random.h"
#include"Time.h"

void Box::Init()
{

	mTransform.pos = Vector3::up();

	mModelID = ModelManager::LoadModel("asset\\model\\box.obj", "Box");

	Shader::AddShader("multi", "MultiVertexLightingVS.cso", "MultiVertexLightingPS.cso");

	mClass = "Box";

}

void Box::Update()
{
	GameObject::Update();
	mTransform.rot.y += 0.01f;
}
