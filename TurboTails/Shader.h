#pragma once

#include"Main.h"
#include"Renderer.h"

class Shader
{
public:

	typedef struct ShaderInfo
	{
	private:

		ID3D11VertexShader* vertex{};	//頂点シェーダーオブジェクト
		ID3D11PixelShader* pixel{};		//ピクセルシェーダーオブジェクト
		ID3D11InputLayout* layout{};	//頂点レイアウトオブジェクト

		void Load(crstring vertex, crstring pixel) {
			Renderer::CreateVertexShader(&this->vertex, &this->layout, vertex.c_str());
			Renderer::CreatePixelShader(&this->pixel, pixel.c_str());
		}

	public:

		ShaderInfo() : vertex(nullptr), pixel(nullptr), layout(nullptr) {}
		ShaderInfo(crstring vertex, crstring pixel) { Load(vertex, pixel); }
		~ShaderInfo() { Uninit(); }

		void Uninit() {
			if (this->vertex) {
				this->vertex->Release();
				this->vertex = nullptr;
			}
			if (this->pixel) {
				this->pixel->Release();
				this->pixel = nullptr;
			}
			if (this->layout) {
				this->layout->Release();
				this->layout = nullptr;
			}
		}

		void SetShader() {
			Renderer::GetDeviceContext()->IASetInputLayout(this->layout);
			Renderer::GetDeviceContext()->VSSetShader(this->vertex, NULL, 0);
			Renderer::GetDeviceContext()->PSSetShader(this->pixel, NULL, 0);
		}

	}ShaderInfo;

private:

	static unordered_map<string, Shader::ShaderInfo*> mInfos;

public:

	static void Uninit();

	static cbool AddShader(crstring name, crstring vertex, crstring pixel);
	static void SetShader(crstring name);

};