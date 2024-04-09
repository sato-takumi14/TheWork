
#include"Main.h"

#include "Scaffold.h"

#include"ModelManager.h"
#include"CircleCollider.h"
#include"DrawModel.h"
#include"Shader.h"

void Scaffold::Init()
{

	AddComponent(new CircleCollider(this, Vector3::up() * 0.5f, 1.0f));

	mTag = "Scaffold";
	mClass = "Scaffold";

	mLayer.push_back("3D");

	mModelID = ModelManager::LoadModel("asset\\model\\box.obj", "Scaffold");

}

void Scaffold::Start()
{
	Shader::AddShader("multi", "MultiVertexLightingVS.cso", "MultiVertexLightingPS.cso");
}

void Scaffold::Update()
{
}

void Scaffold::OnCollisionEnter(crint id)
{
}

void Scaffold::OnCollisionStay(crint id)
{
}

void Scaffold::OnCollisionExit(crint id)
{
}
