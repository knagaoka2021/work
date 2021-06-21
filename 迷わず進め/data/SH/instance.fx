//�O���[�o���ϐ�
float4x4 gWVPMatrix;//���[�J������ˉe��Ԃւ̍��W�ϊ�
float4x4 gWVPVMatrix;//���[�J������ˉe��Ԃւ̍��W�ϊ�
float4x4 gWVPPMatrix;//���[�J������ˉe��Ԃւ̍��W�ϊ�
float ghj;

//�e�N�X�`��
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

//���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
struct VS_OUTPUT
{
 float4 Pos : POSITION;
 float2 Tex : TEXCOORD0;

};

//���_�V�F�[�_�v���O����
VS_OUTPUT VS( VS_IN vsi)
{
 VS_OUTPUT Out = (VS_OUTPUT)0;//�o�̓f�[�^
 float4 oiu =  float4( vsi.pos.x + vsi.world.x,vsi.pos.y + vsi.world.y,vsi.pos.z + vsi.world.z,1.0f);
// Out.Pos =  mul(oiu,gWVPMatrix);
 Out.Pos =  mul( oiu,gWVPVMatrix);
 Out.Pos =  mul( Out.Pos,gWVPPMatrix);
 Out.Tex = vsi.tex + vsi.tipUV;//�e�N�X�`�����W
 return Out;
}

//�s�N�Z���V�F�[�_�v���O����
float4 PS( VS_OUTPUT In ):COLOR
{
 return tex2D( Samp, In. Tex );
}

//�e�N�j�b�N
technique tech
{
 pass P0
 {
  //�V�F�[�_
  VertexShader = compile vs_2_0 VS();
  PixelShader = compile ps_2_0 PS();
 }
}