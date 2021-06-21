//グローバル変数
float4x4 gWVPMatrix;//ローカルから射影空間への座標変換
float4x4 gWVPVMatrix;//ローカルから射影空間への座標変換
float4x4 gWVPPMatrix;//ローカルから射影空間への座標変換
float ghj;

//テクスチャ
texture Tex;
sampler Samp = sampler_state
{
 Texture =<Tex>;
 MinFilter =LINEAR;
 MagFilter =LINEAR;
 MipFilter =LINEAR;

 AddressU =Clamp;
 AddressV =Clamp;
};
struct VS_IN
{
 float4 pos : POSITION;
 float2 tex : TEXCOORD0;
 float4 world : TEXCOORD1;
 float2 tipUV : TEXCOORD2;
// float4 world0 : TEXCOORD1;
// float4 world1 : TEXCOORD2;
// float4 world2 : TEXCOORD3;

}; 

//頂点シェーダからピクセルシェーダに渡すデータ
struct VS_OUTPUT
{
 float4 Pos : POSITION;
 float2 Tex : TEXCOORD0;

};

//頂点シェーダプログラム
VS_OUTPUT VS( VS_IN vsi)
{
 VS_OUTPUT Out = (VS_OUTPUT)0;//出力データ
 float4 oiu =  float4( vsi.pos.x + vsi.world.x,vsi.pos.y + vsi.world.y,vsi.pos.z + vsi.world.z,1.0f);
// Out.Pos =  mul(oiu,gWVPMatrix);
 Out.Pos =  mul( oiu,gWVPVMatrix);
 Out.Pos =  mul( Out.Pos,gWVPPMatrix);
 Out.Tex = vsi.tex + vsi.tipUV;//テクスチャ座標
 return Out;
}

//ピクセルシェーダプログラム
float4 PS( VS_OUTPUT In ):COLOR
{
 return tex2D( Samp, In. Tex );
}

//テクニック
technique tech
{
 pass P0
 {
  //シェーダ
  VertexShader = compile vs_2_0 VS();
  PixelShader = compile ps_2_0 PS();
 }
}