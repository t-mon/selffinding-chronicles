cbuffer ConstantBuffer : register(b0)
{
    float4x4 qt_Matrix;
    float qt_Opacity;
};

Texture2D src : register(t0);
SamplerState srcSampler : register(s0);

float4 GrayscalePixelShader(float4 position : SV_POSITION, float2 coord : TEXCOORD0) : SV_TARGET
{
    float4 tex = src.Sample(srcSampler, coord);
    float3 col = dot(tex.rgb, float3(0.344, 0.5, 0.156));
    return float4(col, tex.a) * qt_Opacity;
}
