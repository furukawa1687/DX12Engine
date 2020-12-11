//---------------------------------------------------------------------------
//!	@file	math_Matrix.cpp
//!	@brief	行列ユーティリティー
//---------------------------------------------------------------------------

namespace math {

//---------------------------------------------------------------------------
//! 単位行列
//---------------------------------------------------------------------------
matrix identity()
{
    return {
        float4{ 1.0f, 0.0f, 0.0f, 0.0f },
        float4{ 0.0f, 1.0f, 0.0f, 0.0f },
        float4{ 0.0f, 0.0f, 1.0f, 0.0f },
        float4{ 0.0f, 0.0f, 0.0f, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! 平行移動
//---------------------------------------------------------------------------
matrix translate(const float3& v)
{
    return {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { v.xyz, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! スケール
//---------------------------------------------------------------------------
matrix scale(const float3& scale)
{
    auto s = scale;
    return {
        float4{ s.x, 0.0f, 0.0f, 0.0f },
        float4{ 0.0f, s.y, 0.0f, 0.0f },
        float4{ 0.0f, 0.0f, s.z, 0.0f },
        float4{ 0.0f, 0.0f, 0.0f, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! X軸中心の回転
//---------------------------------------------------------------------------
matrix rotateX(f32 radian)
{
    float1 s;
    float1 c;
    sincos(float1(radian), s, c);
    return {
        float4{ 1.0f, 0.0f, 0.0f, 0.0f },
        float4{ 0.0f, c, s, 0.0f },
        float4{ 0.0f, -s, c, 0.0f },
        float4{ 0.0f, 0.0f, 0.0f, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! Y軸中心の回転
//---------------------------------------------------------------------------
matrix rotateY(f32 radian)
{
    float1 s;
    float1 c;
    sincos(float1(radian), s, c);
    return {
        float4{ c, 0.0f, -s, 0.0f },
        float4{ 0.0f, 1.0f, 0.0f, 0.0f },
        float4{ s, 0.0f, c, 0.0f },
        float4{ 0.0f, 0.0f, 0.0f, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! Z軸中心の回転
//---------------------------------------------------------------------------
matrix rotateZ(f32 radian)
{
    float1 s;
    float1 c;
    sincos(float1(radian), s, c);
    return {
        float4{ c, s, 0.0f, 0.0f },
        float4{ -s, c, 0.0f, 0.0f },
        float4{ 0.0f, 0.0f, 1.0f, 0.0f },
        float4{ 0.0f, 0.0f, 0.0f, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! 任意の軸中心の回転
//---------------------------------------------------------------------------
matrix rotateAxis(const float3& v, f32 radian)
{
    float1 s;
    float1 c;
    sincos(float1(radian), s, c);

    float3 axis = normalize(v);
    float1 x    = axis.x;
    float1 y    = axis.y;
    float1 z    = axis.z;

    matrix m;
    m._11 = x * x * (1.0f - c) + c;
    m._12 = x * y * (1.0f - c) + z * s;
    m._13 = x * z * (1.0f - c) - y * s;
    m._14 = 0.0f;

    m._21 = x * y * (1.0f - c) - z * s;
    m._22 = y * y * (1.0f - c) + c;
    m._23 = y * z * (1.0f - c) + x * s;
    m._24 = 0.0f;

    m._31 = x * z * (1.0f - c) + y * s;
    m._32 = y * z * (1.0f - c) - x * s;
    m._33 = z * z * (1.0f - c) + c;
    m._34 = 0.0f;

    m._41 = 0.0f;
    m._42 = 0.0f;
    m._43 = 0.0f;
    m._44 = 1.0f;

    return m;
}

//---------------------------------------------------------------------------
//! ビュー行列　(右手座標系)
//---------------------------------------------------------------------------
matrix lookAtRH(const float3& position, const float3& lookAt, const float3& worldUp)
{
    float3 axisZ = normalize(position - lookAt);
    float3 axisX = normalize(cross(worldUp, axisZ));
    float3 axisY = cross(axisZ, axisX);

    f32 x = -dot(axisX, position);
    f32 y = -dot(axisY, position);
    f32 z = -dot(axisZ, position);

    return {
        float4{ axisX.x, axisY.x, axisZ.x, 0.0f },
        float4{ axisX.y, axisY.y, axisZ.y, 0.0f },
        float4{ axisX.z, axisY.z, axisZ.z, 0.0f },
        float4{ x, y, z, 1.0f },
    };
}

//---------------------------------------------------------------------------
//! 投影行列　(右手座標系)
//---------------------------------------------------------------------------
matrix perspectiveFovRH(f32 fovy, f32 aspectRatio, f32 nearZ, f32 farZ)
{
    f32 h  = 1.0f / tanf(fovy * 0.5f);
    f32 w  = h / aspectRatio;
    f32 nz = nearZ;
    f32 fz = farZ;

    return {
        float4{ w, 0.0f, 0.0f, 0.0f },
        float4{ 0.0f, h, 0.0f, 0.0f },
        float4{ 0.0f, 0.0f, fz / (nz - fz), -1.0f },
        float4{ 0.0f, 0.0f, nz * fz / (nz - fz), 0.0f }
    };
}

}   // namespace math
