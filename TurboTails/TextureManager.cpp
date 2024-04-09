
#include"Main.h"

#include"Renderer.h"
#include "TextureManager.h"
#include"Scene.h"

unordered_map<uint, TexData> TextureManager::mData{};
int TextureManager::mIndex{};
Scene* TextureManager::mScene{};

void TextureManager::Uninit()
{

	for (auto& data : mData) {
		if (data.second.tex) {
			data.second.tex->Release();
			data.second.tex = nullptr;
		}
	}
	mData.clear();

	mIndex = 0;

}

void TextureManager::Uninit(crstring scene)
{

	erase_if(mData, [&](auto& data)->bool {

		if (data.second.scene == scene) {
			data.second.tex->Release();
			data.second.tex = nullptr;
			return true;
		}

		return false;
		});

}

cuint TextureManager::Load(crstring file, crstring name)
{

	for (auto& data : mData) {
		if (data.second.name == name) {
			if (data.second.file == file) {
				if (data.second.scene == mScene->GetName()) {
					return data.second.id;
				}				
			}
		}
	}

	ID3D11ShaderResourceView* tex{};

	D3DX11CreateShaderResourceViewFromFile(
		Renderer::GetDevice(),
		file.c_str(),
		NULL,
		NULL,
		&tex,
		NULL);
	assert(tex);

	mData[mIndex].tex = tex;
	mData[mIndex].file = file;
	mData[mIndex].name = name;
	mData[mIndex].id = mIndex;
	mData[mIndex].scene = mScene->GetName();

	return mIndex++;

}

ID3D11ShaderResourceView** TextureManager::GetTexture(cruint id)
{
	if (id < 0) return nullptr;

	return &mData[id].tex;
}
