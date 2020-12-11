//---------------------------------------------------------------------------
//!	@file	typedef.h
//!	@brief	型定義
//---------------------------------------------------------------------------
#pragma once

using s8  = std::int8_t;     //!<  8bit符号あり整数
using u8  = std::uint8_t;    //!<  8bit符号なし整数
using s16 = std::int16_t;    //!< 16bit符号あり整数
using u16 = std::uint16_t;   //!< 16bit符号なし整数
using s32 = std::int32_t;    //!< 32bit符号あり整数
using u32 = std::uint32_t;   //!< 32bit符号なし整数
using s64 = std::int64_t;    //!< 64bit符号あり整数
using u64 = std::uint64_t;   //!< 64bit符号なし整数
using f32 = float;           //!< 単精度浮動小数点数
using f64 = double;          //!< 倍精度浮動小数点数

using intptr  = std::intptr_t;    //!< ポインタサイズの符号あり整数型
using uintptr = std::uintptr_t;   //!< ポインタサイズの符号なし整数型

using matrix=float4x4;

//! COMポインタ
//! IUnknownオブジェクトを管理
template<typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;
