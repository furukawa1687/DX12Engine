//---------------------------------------------------------------------------
//!	@file	precompile.h
//!	@brief	プリコンパイルヘッダー
//---------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------
//!	@defgroup	Windowsヘッダー
//--------------------------------------------------------------
//@{

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN   // 使用頻度の低いWindowsAPIを省略してヘッダー軽量化
#endif

#define NOMINMAX   // std::min std::maxとWindowsSDKのmin/maxマクロが衝突するためWindowsSDK側を無効化
#define STRICT     // Windowsオブジェクトの型を厳密に扱う

#include <windows.h>
#include <wrl.h>   // ComPtr用

//@}
//--------------------------------------------------------------
//!	@defgroup	STL(Standard Template Library)
//--------------------------------------------------------------
//@{

#include <array>
#include <string>
#include <string_view>
#include <map>
#include <unordered_map>
#include <functional>

//@}
//--------------------------------------------------------------
//!	@defgroup	DirectX関連
//--------------------------------------------------------------
//@{

#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>

#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex/DirectXTex.h>

#include "d3dx12.h"

// DirectXツールキット
#include "DirectXTK12/Inc/GamePad.h"

#pragma warning(push)
#pragma warning(disable : 26495)

// ベクトル演算ライブラリ hlslpp
#include "hlslpp/include/hlsl++.h"
using namespace hlslpp;

#pragma warning(pop)
//@}
//--------------------------------------------------------------
//!	@defgroup	アプリケーション
//--------------------------------------------------------------
//@{

#include "typedef.h"        // 型定義
#include "debug.h"          // デバッグ関連
#include "StringEncode.h"   // 文字列UTF8 ←→ UNICODE 相互変換
#include "math_Matrix.h"    // 行列ユーティリティー

//#include "camera.h"          // カメラ
//#include "gpu_Shader.h"      // GPUシェーダー
//#include "gpu_Render.h"      // GPUレンダリング管理
//#include "gpu_Texture.h"     // GPUテクスチャー
//#include "gpu_SwapChain.h"   // GPUスワップチェイン
//#include "GameSystem.h"

#include "PMDActor.h"
#include "PMDRenderer.h"

#include"Dx12Wrapper.h"
#include "Application.h"	// アプリケーション

#include "main.h"   // アプリケーション開始

//@}