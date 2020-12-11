//---------------------------------------------------------------------------
//!	@file	math_Matrix.h
//!	@brief	行列ユーティリティー
//---------------------------------------------------------------------------
#pragma once

namespace math {

constexpr f32 PI = 3.14159265359f;//!< 円周率

namespace {
const float4 ZERO   = float4(0.0f, 0.0f, 0.0f, 0.0f);   //!< ゼロベクトル
const float4 AXIS_X = float4(1.0f, 0.0f, 0.0f, 0.0f);   //!< X軸ベクトル
const float4 AXIS_Y = float4(0.0f, 1.0f, 0.0f, 0.0f);   //!< Y軸ベクトル
const float4 AXIS_Z = float4(0.0f, 0.0f, 1.0f, 0.0f);   //!< Z軸ベクトル
const float4 AXIS_W = float4(0.0f, 0.0f, 0.0f, 1.0f);   //!< W軸ベクトル
}   // namespace

//! 単位行列
matrix identity();

//! 平行移動
matrix translate(const float3& v);

//! スケール
matrix scale(const float3& scale);

//! X軸中心の回転
//!	@param	[in]	radian	回転角(単位:ラジアン)
matrix rotateX(f32 radian);

//! Y軸中心の回転
//!	@param	[in]	radian	回転角(単位:ラジアン)
matrix rotateY(f32 radian);

//! Z軸中心の回転
//!	@param	[in]	radian	回転角(単位:ラジアン)
matrix rotateZ(f32 radian);

//! 任意の軸中心の回転
//!	@param	[in]	v		回転軸ベクトル
//!	@param	[in]	radian	回転角(単位:ラジアン)
matrix rotateAxis(const float3& v, f32 radian);

//! ビュー行列　(右手座標系)
//!	@param	[in]	position	視点位置
//!	@param	[in]	lookAt		注視点
//!	@param	[in]	worldUp		世界の上方向のベクトル
matrix lookAtRH(const float3& position, const float3& lookAt, const float3& worldUp = math::AXIS_Y.xyz);

//! 投影行列　(右手座標系)
//!	@param	[in]	fovy		画角(単位:ラジアン)
//!	@param	[in]	aspectRatio	アスペクト比
//!	@param	[in]	nearZ		近クリップ面Z値
//!	@param	[in]	farZ		遠クリップ面Z値
matrix perspectiveFovRH(f32 fovy, f32 aspectRatio, f32 nearZ, f32 farZ);

}   // namespace math
