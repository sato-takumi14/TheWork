
#include"Main.h"

#include "Shader.h"
#include"Debug.h"

unordered_map<string, Shader::ShaderInfo*> Shader::mInfos{};

void Shader::Uninit()
{
	for (auto& data : mInfos) {
		data.second->Uninit();
		delete data.second;
	}
	mInfos.clear();
}

cbool Shader::AddShader(crstring name, crstring vertex, crstring pixel)
{
	for (auto& data : mInfos) { if (data.first == name)return false; }
	mInfos[name] = new Shader::ShaderInfo("shader\\" + vertex, "shader\\" + pixel);
	Debug::Log("シェーダー：" + name + "登録");
	return true;
}

void Shader::SetShader(crstring name)
{
	for (auto& data : mInfos) {
		if (data.first == name) {
			data.second->SetShader();
			break;
		}
	}
}
