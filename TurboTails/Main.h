#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>



#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

HWND GetWindow();

#include<string>
#include<list>
#include<unordered_map>
#include<vector>
#include<DirectXMath.h>

using namespace std;
using namespace DirectX;

/*CRI SDKヘッダー*/
#include<cri_le_xpt.h>

/*CRI ADX2ヘッダー*/
#include<cri_le_atom_ex.h>
#include<cri_le_atom_wasapi.h>

#pragma comment (lib,"cri_ware_pcx64_le_import.lib")

#include<cereal\cereal.hpp>
#include<cereal\types\list.hpp>
#include<cereal\types\unordered_map.hpp>
#include<cereal\types\string.hpp>
#include<cereal\archives\json.hpp>
#include<cereal\archives\binary.hpp>

/********************************************************************************/

/*長いから省略*/
typedef struct D3DXVECTOR2 ver2;
typedef struct D3DXVECTOR3 ver3;
typedef struct D3DXVECTOR4 ver4;
typedef struct D3DXCOLOR color;
typedef struct D3DXQUATERNION quat;
typedef struct D3DXMATRIX matrix;

/*よく使うもの*/
typedef unsigned int uint;

/*const系のよく使うもの*/
typedef const int		cint;
typedef const uint		cuint;
typedef const float		cfloat;
typedef const double	cdouble;
typedef const bool		cbool;
typedef const ver2		cver2;
typedef const ver3		cver3;
typedef const ver4		cver4;
typedef const color		ccolor;
typedef const quat		cquat;
typedef const string	cstring;
typedef const matrix	cmatrix;

/*const参照系のよく使うもの*/
typedef cint&		crint;
typedef cuint&		cruint;
typedef cfloat&		crfloat;
typedef cdouble&	crdouble;
typedef cbool&		crbool;
typedef cver2&		crver2;
typedef cver3&		crver3;
typedef cver4&		crver4;
typedef ccolor&		crcolor;
typedef cquat&		crquat;
typedef cstring&	crstring;
typedef cmatrix&	crmatrix;

/********************************************************************************/