//----------------------------------------------------------------------------
//!	@file	shader.fx
//!	@brief	シェーダーサンプルコード
//----------------------------------------------------------------------------

Texture2D		Texture[] : register(t0);
SamplerState	Sampler[] : register(s0);

// 定数バッファ
struct SceneConstantBuffer
{
	matrix	matWorld_;  // ワールド行列
	matrix	matView_;   // ビュー行列
	matrix	matProj_;   // 投影行列
};

ConstantBuffer<SceneConstantBuffer> scene : register(b0);


// 頂点シェーダー入力
struct VS_INPUT
{
	float4	position_ : POSITION;
	float4	color_    : COLOR;
};

// ピクセルシェーダー入力
struct PS_INPUT
{
	float4	position_ : SV_POSITION;
	float4	color_    : COLOR;
	float2	uv_		  : TEXCOORD0;
};

//----------------------------------------------------------------------------
// 頂点シェーダー
//----------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	// 座標変換
	output.position_ = mul(scene.matWorld_, input.position_);
	output.position_ = mul(scene.matView_, output.position_);
	output.position_ = mul(scene.matProj_, output.position_);

	output.color_ = input.color_;

	// 位置からUVを計算
	{
		// 2D投影とパースペクティブ補正を無効化 (w=1.0)
		output.position_ /= output.position_.w;
		output.uv_ = output.position_.xy * float2(0.5, -0.5) + 0.5;
	}
	return output;
}

//----------------------------------------------------------------------------
// テクスチャなし
//----------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_TARGET
{
	return input.color_;
}

//----------------------------------------------------------------------------
// テクスチャあり
//----------------------------------------------------------------------------
float4 PS_Texture(PS_INPUT input) : SV_TARGET
{
	uint index = input.position_.x < 640 ? 0 : 1;
	return Texture[index].Sample(Sampler[0], input.uv_);// * input.color_;
}
