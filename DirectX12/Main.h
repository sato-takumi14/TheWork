#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>
#include <fstream>
#include <comutil.h>
#include <iomanip>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable : 4005)
#pragma warning(disable : 4995)

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <DirectXTex\d3dx12.h>
#include <DirectXTex\DirectXTex.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d12shader.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "D3DCompiler.lib")

#if _DEBUG
#pragma comment (lib, "asset\\Lib\\Debug\\DirectXTex.lib")
#else
#pragma comment (lib, "asset\\Lib\\Relese\\DirectXTex.lib")
#endif // _DEBUG

#pragma comment (lib, "asset\\Lib\\assimp-vc143-mt.lib")

#include <cereal\cereal.hpp>
#include <cereal\types\list.hpp>
#include <cereal\types\unordered_map.hpp>
#include <cereal\types\string.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\archives\binary.hpp>

//using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace
{

	/*長い名称を使いやすく省略*/
	using uint = unsigned int;

	using string = std::string;
	using wstring = std::wstring;

	using matrix = DirectX::XMMATRIX;

	/*const*/
	using cint = const int;
	using cuint = const uint;
	using cfloat = const float;
	using cdouble = const double;
	using cbool = const bool;
	using cstring = const string;
	using cwstring = const wstring;
	using cmatrix = const matrix;

	/*参照系*/
	using rstring = string&;
	using rwstring = wstring&;

	/*const参照系*/
	using crint = cint&;
	using cruint = cuint&;
	using crfloat = cfloat&;
	using crdouble = cdouble&;
	using crbool = cbool&;
	using crstring = cstring&;
	using crwstring = cwstring&;

	/*ポインタ*/
	using pint = int*;
	using puint = uint*;
	using pfloat = float*;
	using pdouble = double*;
	using pbool = bool*;
	using pstring = string*;
	using pwstring = wstring*;

	/*constポインタ*/
	using cpint = const pint;
	using cpuint = const puint;
	using cpfloat = const pfloat;
	using cpdouble = const pdouble;
	using cpbool = const pbool;
	using cpstring = const pstring;
	using cpwstring = const pwstring;
	using cpmatrix = const matrix*;

	/*ウィンドウサイズ*/
	constexpr cfloat WindowWidth = 1280.0f;
	constexpr cfloat WindowHeight = 720.0f;

}
