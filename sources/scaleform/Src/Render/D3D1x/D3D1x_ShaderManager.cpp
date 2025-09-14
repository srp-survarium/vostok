/**************************************************************************

Filename    :   D3D1x_ShaderManager.cpp
Content     :   
Created     :   Mar 2011
Authors     :   Bart Muzzin

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
#include "pch.h"

#include "Kernel/SF_String.h"
#include "D3D1x_ShaderManager.h"
#include "D3D1x_MeshCache.h"
#include "D3D1x_HAL.h"

namespace Scaleform { namespace Render { namespace D3D1x {

#define SF_RENDERER_PSHADER_PROFILE "ps_4_0"


#define DEF_FILLSET(vshader, declFlags)     (declFlags | vshader)
#define DEF_VSHADER(group, color, texture)  (VSGroup_##group | VSColor_##color | VSTexture_##texture)

static StdVertexShaderDesc  ShaderManager_VertexShaderDescriptors[(PrimFill_Type_Count-1) * 3] =
{
    // PrimFill_Mask
    DEF_FILLSET(DEF_VSHADER(Single, None, None),      0),
    DEF_FILLSET(DEF_VSHADER(Batch, None, None),       VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, None, None),   0),
    // PrimFill_SolidColor
    DEF_FILLSET(DEF_VSHADER(Single, Solid, None),     0),
    DEF_FILLSET(DEF_VSHADER(Batch, Solid, None),      VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, Solid, None),  0),
    // PrimFill_VColor
    DEF_FILLSET(DEF_VSHADER(Single, Vertex, None),    VFE_VColor),
    DEF_FILLSET(DEF_VSHADER(Batch, Vertex, None),     VFE_VColor|VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, Vertex, None), VFE_VColor),
    // PrimFill_VColor_EAlpha
    DEF_FILLSET(DEF_VSHADER(Single, Vertex, None),    VFE_VColor|VFE_EAlpha),
    DEF_FILLSET(DEF_VSHADER(Batch, Vertex, None),     VFE_VColor|VFE_EAlpha|VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, Vertex, None), VFE_VColor|VFE_EAlpha),
    // PrimFill_Texture
    DEF_FILLSET(DEF_VSHADER(Single, None, Gen1),      0),
    DEF_FILLSET(DEF_VSHADER(Batch, None, Gen1),       VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, None, Gen1),   0),
    // PrimFill_Texture_EAlpha
    DEF_FILLSET(DEF_VSHADER(Single, None, Gen1),      VFE_EAlpha),
    DEF_FILLSET(DEF_VSHADER(Batch, None, Gen1),       VFE_EAlpha|VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, None, Gen1),   VFE_EAlpha),
    // PrimFill_Texture_VColor
    DEF_FILLSET(DEF_VSHADER(Single, Vertex, Gen1),    VFE_VColor|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Batch, Vertex, Gen1),     VFE_VColor|VFE_IIndex|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Instanced, Vertex, Gen1), VFE_VColor|VFE_Weight),
    // PrimFill_Texture_VColor_EAlpha
    DEF_FILLSET(DEF_VSHADER(Single, Vertex, Gen1),    VFE_VColor|VFE_EAlpha|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Batch, Vertex, Gen1),     VFE_VColor|VFE_EAlpha|VFE_IIndex|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Instanced, Vertex, Gen1), VFE_VColor|VFE_EAlpha|VFE_Weight),
    // PrimFill_2Texture
    DEF_FILLSET(DEF_VSHADER(Single, None, Gen2),      VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Batch, None, Gen2),       VFE_IIndex|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Instanced, None, Gen2),   VFE_Weight),
    // PrimFill_2Texture_EAlpha
    DEF_FILLSET(DEF_VSHADER(Single, None, Gen2),      VFE_EAlpha|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Batch, None, Gen2),       VFE_EAlpha|VFE_IIndex|VFE_Weight),
    DEF_FILLSET(DEF_VSHADER(Instanced, None, Gen2),   VFE_EAlpha|VFE_Weight),
    // PrimFill_UVTexture
    DEF_FILLSET(DEF_VSHADER(Single, None, UV1),       VFE_UV32f),
    DEF_FILLSET(DEF_VSHADER(Batch, None, UV1),        VFE_UV32f|VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, None, UV1),    VFE_UV32f),  // Not needed
    // PrimFill_UVTextureAlpha_VColor
    DEF_FILLSET(DEF_VSHADER(Single, Vertex, UV1),     VFE_VColor|VFE_UV32f),
    DEF_FILLSET(DEF_VSHADER(Batch, Vertex, UV1),      VFE_VColor|VFE_UV32f|VFE_IIndex),
    DEF_FILLSET(DEF_VSHADER(Instanced, Vertex, UV1),  VFE_VColor|VFE_UV32f),  // Not needed
};



// Texture sample/weight operation done by pixel shader
// Indexed based on PrimitiveFillType + Texture type(s).

enum PSFillOp
{
    PSFillOp_None,
    PSFillOp_NoColor,
    PSFillOp_Texture,
    PSFillOp_Texture_VColor,
    PSFillOp_2Texture,
    PSFillOp_TextureA_VColor,
    PSFillOp_TextureYUV,
    PSFillOp_TextureYUVA,
    PSFillOp_Count
};

// ShaderManager_PixelShaderDescriptors combines PSFillOp_Texture and extra PSCap bits
// to define a 'psBits' key for each supported PSType.
// PSBit_Multiply, PSBit_AlphaWtite and PSBit_Instanced bits are not included because
// all of their combinations are created for each type declared here.
// All of these PSTypes are created during ShaderManager::Initialize


#define DEF_PSHADER(fillOp, bits) ((fillOp<<PSBit_FillOp_Shift)|bits)

// PSType -> { PSFillOp, EAlpha flag }
unsigned ShaderManager_PixelShaderDescriptors[PS_Type_Count-1] = 
{   
    DEF_PSHADER(PSFillOp_NoColor, 0),                      // PS_NoColor,
    DEF_PSHADER(PSFillOp_None, 0),                         // PS_Color,
    DEF_PSHADER(PSFillOp_None, PSBit_EAlpha),              // PS_Color_EAlpha,
    DEF_PSHADER(PSFillOp_Texture, 0),                      // PS_Texture,
    DEF_PSHADER(PSFillOp_Texture, PSBit_EAlpha),           // PS_Texture_EAlpha,
    DEF_PSHADER(PSFillOp_Texture_VColor, 0),               // PS_Texture_VColor,
    DEF_PSHADER(PSFillOp_Texture_VColor, PSBit_EAlpha),    // PS_Texture_VColor_EAlpha,
    DEF_PSHADER(PSFillOp_2Texture, 0),                     // PS_2Texture,
    DEF_PSHADER(PSFillOp_2Texture, PSBit_EAlpha),          // PS_2Texture_EAlpha,
    DEF_PSHADER(PSFillOp_TextureA_VColor, 0),              // PS_TextureA_VColor,
    DEF_PSHADER(PSFillOp_TextureYUV, 0),                   // PS_TextureYUV,
    DEF_PSHADER(PSFillOp_TextureYUVA, 0),                  // PS_TextureYUVA,
};



bool ShaderManager::createVertexShaderString( StringBuffer& sb, const StdVertexShaderDesc& desc, PositionType posType )
{
    VSGroupMode     groupMode = desc.GetGroupMode();
    VSColorInput    colorInput= desc.GetColorInput();
    VSTextureInput  textureInput = desc.GetTextureInput();
    bool            hasFactors = desc.HasFactors();

    sb.Clear();

    // Declarations    
    if (groupMode != VSGroup_Single)
        sb += "cbuffer InstanceData : register (b0) { float4 mat[240]; }\n"; // 10 * 24 (max instanced).
    else
        sb += "cbuffer SingleData   : register (b0) { float4 mat[10]; }\n"; // 10 * 1.

    if (colorInput == VSColor_Solid)
        sb += "cbuffer SolidData    : register (b1) { float4 scolor; }\n";

    sb += "void main(";
    switch( posType )
    {
    default:
    case PositionType_SInt16: sb += "int4 pos : SV_Position\n"; break;
    case PositionType_Float32: sb += "float4 pos : SV_Position\n"; break;
    }

    // Arguments

    if (colorInput == VSColor_Vertex)
        sb += ",uint4 vcolor : COLOR0\n";

    if (hasFactors)
    {
        sb += ",uint4 factors : COLOR1\n";
        if (groupMode == VSGroup_Instanced)
            sb += ",uint inst : SV_InstanceID\n";
    }
    else
    {
        SF_ASSERT(groupMode != VSGroup_Batch);
         // Instance -> factor
        if (groupMode == VSGroup_Instanced)
            sb += ",uint inst : SV_InstanceID\n";
    }
    
    if (textureInput == VSTexture_UV1)
        sb += ",float2 tc0 : TEXCOORD0\n";

    if ((colorInput == VSColor_Solid) || (colorInput == VSColor_Vertex))
        sb += ",out float4 oc0 : COLOR0\n";
    if (textureInput > VSTexture_None)
        sb += ",out float2 otc0 : TEXCOORD0\n";
    if (textureInput == VSTexture_Gen2)
        sb += ",out float2 otc1 : TEXCOORD1\n";

    // Use TEXCOORD for 'ofactors' as COLOR gets clamped to 1.
    if (hasFactors || (groupMode != VSGroup_Single))
        sb += ",out float4 ofactors : TEXCOORD2\n";

    if ( colorInput != VSColor_None || textureInput != VSTexture_None)
    {
        sb += ",out float4 cm : TEXCOORD3\n";
        sb += ",out float4 ca : TEXCOORD4\n";
    }

    // Position must come last, otherwise the input/output registers between VS/PS will not align.
    sb += ",out float4 opos : SV_Position\n";

    // Body
    sb += ") {\n";
    sb += "opos = float4(pos); \n";

    if (groupMode != VSGroup_Single)
    {
        if (groupMode == VSGroup_Instanced)
            sb += "int index = inst;\n";
        else
            sb += "int index = factors.r;\n";        

        switch(textureInput)
        {
        case VSTexture_None: sb += "index *= 6;\n"; break;
        case VSTexture_Gen1: sb += "index *= 8;\n"; break;
        case VSTexture_Gen2: sb += "index *= 10;\n"; break;
        case VSTexture_UV1:  sb += "index *= 6;\n"; break;
        }
    }
    else
    {
        sb += "int index = 0;\n";
    }


    // Transform the output position (4d).
    sb += "opos.x = dot(pos, mat[index]);\n";
    sb += "opos.y = dot(pos, mat[index+1]);\n";
    sb += "opos.z = dot(pos, mat[index+2]);\n";
    sb += "opos.w = dot(pos, mat[index+3]);\n";

    if ( colorInput != VSColor_None || textureInput != VSTexture_None)
    {
        // Color modification.
        sb += "cm = mat[index + 4];\n"
              "ca = mat[index + 5];\n";
    }

    // Apply texturing (if required).
    switch(textureInput)
    {
        case VSTexture_Gen1:
            sb += "otc0.x = dot(pos, mat[index+6]);\n"
                  "otc0.y = dot(pos, mat[index+7]);\n";
            break;
        
        case VSTexture_Gen2:
            sb += "otc0.x = dot(pos, mat[index+6]);\n"
                "otc0.y = dot(pos, mat[index+7]);\n"
                "otc1.x = dot(pos, mat[index+8]);\n"
                "otc1.y = dot(pos, mat[index+9]);\n";
            break;

        case VSTexture_UV1:
            sb += "otc0 = tc0;\n";
            break;
    }

    if (colorInput == VSColor_Solid)
        sb += "oc0 = scolor;\n";
    else if (colorInput == VSColor_Vertex)
        sb += "oc0 = vcolor / 255.0f;\n";
    

    if (hasFactors)
        sb += "ofactors = factors / 255.0f;\n";
    else if (groupMode == VSGroup_Instanced)
        sb += "ofactors = inst.xxxx / 255.0f;\n";

    sb+= "}\n";

    return true;
}





// TexturePShader is used to associate an ImageFormat with a PixelShader type
// that supports it for a given fill style. Some pixel shaders may support multiple
// formats; however, some formats, such as YUV require a custom-tailored shader.

struct TexturePShader
{
    // Pixel shader texture type capability flags. Every bit identifies texture format 
    // type that a shader can accept. ImageFormat is mapped to one of these types.
    // If the texture cap bit is set, this shader can be used with texture.
    enum TextureType
    {
        Texture_None  = 0x0010,
        Texture_A     = 0x0020,
        Texture_RGBA  = 0x0040,
        Texture_YUV   = 0x0100,
        Texture_YUVA  = 0x0200,
    };
    enum {
        PSType_Mask = 0x0F, 
    };

    unsigned           Flags;
    TexturePShader*    pNext;

    static  TextureType ImageFormatToTextureType(ImageFormat format);

    PSType      GetShader() const                         { return (PSType)(Flags & PSType_Mask); }
    bool        SupportsTextureType(TextureType tt) const { return (Flags & tt) != 0; }

    inline TexturePShader* MatchImageFormat(ImageFormat format);
};

// PixelShader definition combined PSType with set of texture type bits that it can support.
// DEF_TEXTURE_PSHADER_MULTIPLE is used to create a linked-list of different shaders so that
// the appropriate one is picked based on format.
#define DEF_TEXTURE_PSHADER(shader, t0)                 { (shader|TexturePShader::Texture_##t0), 0}
#define DEF_TEXTURE_PSHADER2(shader, t0, t1)            { (shader|TexturePShader::Texture_##t0|TexturePShader::Texture_##t1), 0 }
#define DEF_TEXTURE_PSHADER_MULTIPLE(shader, t0, other) { (shader|TexturePShader::Texture_##t0), other }

// Additional TexturePShader nodes needed for YUV/YUVA formats.
static TexturePShader TPS_SelectYUVA = DEF_TEXTURE_PSHADER(PS_TextureYUVA, YUVA);
static TexturePShader TPS_SelectYUV  = DEF_TEXTURE_PSHADER_MULTIPLE(PS_TextureYUV, YUV, &TPS_SelectYUVA);

// FillToPixelShader maps a fill style to as list of pixel shaders that can be
// used with it. Within the list, TextureType, obtained from the textures ImageFormat
// is used to determine the exact shader applied. 
static TexturePShader ShaderManager_FillToPixelShader[PrimFill_Type_Count] = 
{
    DEF_TEXTURE_PSHADER(PS_None, None),                                 // PrimFill_None
    DEF_TEXTURE_PSHADER(PS_NoColor, None),                              // PrimFill_Mask
    DEF_TEXTURE_PSHADER(PS_Color, None),                                // PrimFill_SolidColor
    DEF_TEXTURE_PSHADER(PS_Color, None),                                // PrimFill_VColor
    DEF_TEXTURE_PSHADER(PS_Color_EAlpha, None),                         // PrimFill_VColor_EAlpha
    DEF_TEXTURE_PSHADER_MULTIPLE(PS_Texture, RGBA, &TPS_SelectYUV),     // PrimFill_Texture
    DEF_TEXTURE_PSHADER_MULTIPLE(PS_Texture_EAlpha, RGBA, &TPS_SelectYUVA),// PrimFill_Texture_EAlpha
    DEF_TEXTURE_PSHADER(PS_Texture_VColor, RGBA),                       // PrimFill_Texture_VColor
    DEF_TEXTURE_PSHADER(PS_Texture_VColor_EAlpha, RGBA),                // PrimFill_Texture_VColor_EAlpha
    DEF_TEXTURE_PSHADER(PS_2Texture, RGBA),                             // PrimFill_2Texture
    DEF_TEXTURE_PSHADER(PS_2Texture_EAlpha, RGBA),                      // PrimFill_2Texture_EAlpha
    DEF_TEXTURE_PSHADER_MULTIPLE(PS_Texture, RGBA, &TPS_SelectYUV),     // PrimFill_UVTexture
    DEF_TEXTURE_PSHADER2(PS_TextureA_VColor, A, RGBA)                   // PrimFill_UVTextureAlpha_VColor
};

TexturePShader::TextureType TexturePShader::ImageFormatToTextureType(ImageFormat format)
{
    switch(format)
    {
    case Image_R8G8B8A8:
    case Image_B8G8R8A8:
    case Image_R8G8B8:
    case Image_B8G8R8:
    case Image_DXT1:
    case Image_DXT3:
    case Image_DXT5:
        return Texture_RGBA;
    case Image_A8:
        return Texture_A;
    case Image_Y8_U2_V2:
        return Texture_YUV;
    case Image_Y8_U2_V2_A8:
        return Texture_YUVA;

    // Image_None is a custom texture. Assume it maps to RGBA data.
    case Image_None:
        return Texture_RGBA;

    default:
        break;
    }
    return Texture_None;
}

inline TexturePShader* TexturePShader::MatchImageFormat(ImageFormat format)
{
    TexturePShader* p  = this;
    TextureType     tt = ImageFormatToTextureType(format);

    if (!p->SupportsTextureType(tt))
    {
        do { p = p->pNext; }
        while (p && !p->SupportsTextureType(tt));        
        SF_DEBUG_WARNING(!p, "D3D1x::ShaderManager failed to set pixel shader - unsupported ImageFormat");        
    }
    return p;
}




enum PSFillOpTextureType
{
    PSFillOpTex_None,
    PSFillOpTex_1,
    PSFillOpTex_2,
    PSFillOpTex_YUV,
    PSFillOpTex_YUVA
};

static UByte FillOp_TextureTypes[PSFillOp_Count] =
{
    PSFillOpTex_None,   // PSFillOp_None,
    PSFillOpTex_None,   // PSFillOp_NoColor,
    PSFillOpTex_1,      // PSFillOp_Texture,
    PSFillOpTex_1,      // PSFillOp_Texture_VColor,
    PSFillOpTex_2,      // PSFillOp_2Texture,
    PSFillOpTex_1,      // PSFillOp_TextureA_VColor,
    PSFillOpTex_YUV,    // PSFillOp_TextureYUV,
    PSFillOpTex_YUVA    // PSFillOp_TextureYUVA,
};
static bool FillOp_NeedsFactorArg[PSFillOp_Count] = 
{
    false,
    false,
    false,
    true,   // PSFillOp_Texture_VColor
    true,   // PSFillOp_2Texture
    false,
    false,
    true    // PSFillOp_TextureYUVA
};
static bool FillOp_NeedsColorArg[PSFillOp_Count] = 
{
    true,   // PSFillOp_None,
    false,  // PSFillOp_NoColor
    false,  // PSFillOp_Texture,
    true,   // PSFillOp_Texture_VColor,
    false,  // PSFillOp_2Texture,
    true,   // PSFillOp_TextureA_VColor,
    false,  // PSFillOp_TextureYUV,
    false   // PSFillOp_TextureYUVA,
};

static const char* FillOp_Body[PSFillOp_Count] = 
{
    // PSFillOp_None - pass along argument value
    "",
    // PSFillOp_NoColor
    "color = float4(1.0, 1.0, 1.0, 1.0);",
    // PSFillOp_Texture
    "color = tex0.Sample(sam0, tc0);\n",
    // PSFillOp_Texture_VColor
    "color = lerp(color, tex0.Sample(sam0, tc0), factors.g);\n",
    // PSFillOp_2Texture
    "color = lerp(tex1.Sample(sam1, tc1), tex0.Sample(sam0, tc0), factors.g);\n",
    
    // PSFillOp_TextureA_VColor
    // TBD: In old shaders this multiply was done after cxform
    "color.a *= tex0.Sample(sam0, tc0).a;\n",
    
    // PSFillOp_TextureYUV
    "float  Y = (float)tex_y.Sample(sam, tc0).a;\n"
    "float  U = (float)tex_u.Sample(sam, tc0).a - 128.f/255.f;\n"
    "float  V = (float)tex_v.Sample(sam, tc0).a - 128.f/255.f;\n"
    "color = float4( float3((float3(Y, Y, Y) - float3(16.f/255.f, 16.f/255.f, 16.f/255.f)) * 1.164), 1);\n"
    "color.r +=  V * 1.596f;\n"
    "color.g += -U * 0.392f - V * 0.813f;\n"
    "color.b +=  U * 2.017f;\n",
    // PSFillOp_TextureYUVA
    "float  Y = (float)tex_y.Sample(sam, tc0).a;\n"
    "float  U = (float)tex_u.Sample(sam, tc0).a - 128.f/255.f;\n"
    "float  V = (float)tex_v.Sample(sam, tc0).a - 128.f/255.f;\n"
    "color = float4( float3((float3(Y, Y, Y) - float3(16.f/255.f, 16.f/255.f, 16.f/255.f)) * 1.164), tex_a.Sample(sam, tc0).a);\n"
    "color.r +=  V * 1.596f;\n"
    "color.g += -U * 0.392f - V * 0.813f;\n"
    "color.b +=  U * 2.017f;\n"
};


bool ShaderManager::createPixelShaderString(StringBuffer& sb, unsigned psBits)
{
    PSFillOp            fillOp      = (PSFillOp)((psBits & PSBit_FillOp_Mask) >> PSBit_FillOp_Shift);
    PSFillOpTextureType textureType = (PSFillOpTextureType)FillOp_TextureTypes[fillOp];
    bool                colorArg    = FillOp_NeedsColorArg[fillOp];
    bool                factorArg   = FillOp_NeedsFactorArg[fillOp];
   
    if (psBits & (PSBit_Instanced|PSBit_EAlpha))
        factorArg = true;    

    sb.Clear();

    switch(textureType)
    {
    case PSFillOpTex_2:
        sb += "Texture2D    tex1  : register(t1);\n";
        sb += "SamplerState sam1  : register(s1);\n";
    case PSFillOpTex_1:
        sb += "Texture2D    tex0  : register(t0);\n";
        sb += "SamplerState sam0  : register(s0);\n";
        break;
    case PSFillOpTex_YUVA:
        sb += "Texture2D    tex_a : register(t3);\n";
    case PSFillOpTex_YUV:
        sb += "Texture2D    tex_y : register(t0);\n"
              "Texture2D    tex_u : register(t1);\n"
              "Texture2D    tex_v : register(t2);\n";
        sb += "SamplerState sam   : register(s0);\n";
    default:
        break;
    }

    sb += "void main(\n";

    if (colorArg)
        sb += "float4 color : COLOR0,\n";
    
    if (fillOp >= PSFillOp_Texture)
    {
        sb += "float2 tc0 : TEXCOORD0,\n";
        if (fillOp == PSFillOp_2Texture)
            sb += "float2 tc1 : TEXCOORD1,\n";
    }

    if (factorArg)
        sb += "float4 factors : TEXCOORD2,\n";

    if (fillOp != PSFillOp_NoColor)
        sb += "float4 cm : TEXCOORD3,\n"
              "float4 ca : TEXCOORD4,\n";

    sb += "out float4 oc : SV_Target)\n{\n"; 

    // Begin main() {

    if (!colorArg)
        sb += "float4 color;\n";

    sb += FillOp_Body[fillOp];

    if (fillOp != PSFillOp_NoColor)
    {
        // Color transform 
        sb += "color = color * cm + ca;\n";

        if (psBits & PSBit_EAlpha)
            sb += "color.a *= factors.a;\n";

        // Multiply lerp / Alpha write
        switch(psBits & (PSBit_Multiply|PSBit_AlphaWrite))
        {
        case PSBit_Multiply|PSBit_AlphaWrite:
            sb += "color  = lerp(1, color, color.a);\n";
            sb += "oc.rgb = color * color.a;\n"
                  "oc.a   = color.a;\n";
            break;
        case PSBit_AlphaWrite:
            sb += "oc.rgb = color * color.a;\n"
                  "oc.a   = color.a;\n";
            break;
        case PSBit_Multiply:
            sb += "oc = lerp(1, color, color.a);\n";
            break;
        case 0:
            sb += "oc = color;\n";
            break;
        }
    }
    else
    {   // No color, assign solid color constant (used for Masks).
        sb += "oc = color;\n";
    }
    sb+= "}\n";

    return true;
}



bool ShaderManager::registerVertexHandler(const StdVertexShaderDesc& desc, VertexHandler& vh)
{
    SF_AMP_SCOPE_RENDER_TIMER(__FUNCTION__, Amp_Profile_Level_Medium);

    StringBuffer             sb;
    Ptr<ID3D10Blob>          pvsbyteCode;

    for ( unsigned posType = 0; posType < PositionType_Count; ++posType )
    {
        unsigned key = desc.GetVertexShaderKey(posType);

        // Find and/or create vertex shader.
        Ptr<ID3D1x(VertexShader)>* pvs = VShaders.Find(&key);
        if ( !pvs )
        {
            if ( !createVertexShaderString(sb, desc, (PositionType)posType) ||
                 !createVertexShader(&vh.pVShader[posType], &pvsbyteCode, sb.ToCStr()))
            {
                return false;
            }
            *(VShaders.Add(&key)) = vh.pVShader[posType];
            vh.pVShader[posType]->Release();
        }
        else
        {
            vh.pVShader[posType] = pvs->GetPtr();
        }

        // Find and/or create input layout.
        Ptr<ID3D1x(InputLayout)>* pil = IFormats.Find(&key);
        if ( !pil)
        {
            if ( !createInputLayout(&vh.pLayout[posType], pvsbyteCode, desc, (PositionType)posType) )
            {
                return false;
            }
            *(IFormats.Add(&key)) = vh.pLayout[posType];
            vh.pLayout[posType]->Release();
        }
        else
        {
            vh.pLayout[posType] = pil->GetPtr();
        }
    }

    return true;
}

ID3D1x(PixelShader)*  ShaderManager::registerPixelShader(unsigned psBits)
{
    SF_AMP_SCOPE_RENDER_TIMER(__FUNCTION__, Amp_Profile_Level_Medium);

    StringBuffer                sb;
    Ptr<ID3D1x(PixelShader)>  ps;    
    Ptr<ID3D1x(PixelShader)>* pps= PShaders.Find(&psBits);
    if (pps)
        return *pps;

    if (!createPixelShaderString(sb, psBits) ||
        !createPixelShader(&ps, sb.ToCStr()))
        return 0;
    pps = PShaders.Add(&psBits);
    if (!pps)
        return 0;
    *pps = ps;    
    return *pps;
}


struct VertexElementBuilder
{
    VertexElement Elements[8];
    unsigned      Size, Count;

    VertexElementBuilder() : Size(0), Count(0) { }

    void    Add(unsigned attr, unsigned offsetDelta, unsigned size)
    {
        Elements[Count].Offset    = Size + offsetDelta;
        Elements[Count].Attribute = attr;
        Size += size;        
        SF_ASSERT(Count < 8);
        Count++;
    }
    void    Add(unsigned attr, unsigned size)   { Add(attr, 0, size); }

    void    Finish()
    {
        Elements[Count].Offset      = 0;
        Elements[Count++].Attribute = VET_None;
    }
};

struct D3D1xVertexDeclBuilder
{
    D3D1x(INPUT_ELEMENT_DESC)    Elements[8];
    WORD                        Count;

    D3D1xVertexDeclBuilder() : Count(0) { }

    void    Add(LPCSTR name, UINT index, DXGI_FORMAT format)
    {
        D3D1x(INPUT_ELEMENT_DESC) &e = Elements[Count++];
        e.SemanticName = name;
        e.SemanticIndex = index;
        e.Format = format;
        e.InputSlot = 0;
        e.AlignedByteOffset = D3D1x(APPEND_ALIGNED_ELEMENT);
        e.InputSlotClass = D3D1x(INPUT_PER_VERTEX_DATA);
        e.InstanceDataStepRate = 0;
    }
};


const VertexFormat* ShaderManager::registerVertexFormat(StdVertexShaderDesc& desc, PositionType posType)
{
    unsigned               flags = desc.Flags;
    VertexElementBuilder   ve;

    switch(posType)
    {
        default:
        case PositionType_SInt16:  ve.Add(VET_XY16i, 4); break;
        case PositionType_Float32: ve.Add(VET_XY32f, 8); break;
    }
    
    if (flags & VFE_VColor)
        ve.Add(VET_ColorRGBA8, 4);
    
    if (desc.HasFactors())
    {
        if (flags & VFE_IIndex)
            ve.Add(VET_Instance8, 0, 0);
        if (flags & VFE_EAlpha)
            ve.Add(VET_FactorAlpha8, 3, 0);
        if (flags & VFE_Weight)
            ve.Add(VET_T0Weight8, 1, 0);
        ve.Size += 4;
    }

    if (flags & VFE_UV32f)
        ve.Add(VET_UV32f, 8);
    ve.Finish();

    // Register formats if not registered yet.
    VertexElement *pelements = 0;
    VertexFormat  *pformat   = VFormats.Find(ve.Elements, ve.Count);
    if (pformat)
        return pformat;

    pformat = VFormats.Add(&pelements, ve.Elements, ve.Count);
    if (!pformat)
        return 0;
    pformat->Size      = ve.Size;
    pformat->pElements = pelements;
    return pformat;
}

// ***** ShaderManager

ShaderManager::ShaderManager()
: pDevice(0), pDeviceContext(0),
  PrimitivePSType(PS_None)
{
    memset(PSTable, 0, sizeof(PSTable));
    memset(VHandlerTable, 0, sizeof(VHandlerTable));

    SolidColorBuffer = 0;
    pPrevLayout   = 0;
    pPrevVShader = 0;
    PrevPSIndex  = 0;
    PrevTextures[0].Set(0, ImageFillMode());
    PrevTextures[1].Set(0, ImageFillMode());
}

ShaderManager::~ShaderManager()
{   
    Reset();
}

bool    ShaderManager::createVertexShader(ID3D1x(VertexShader) **ppshaderPtr,
                                          Ptr<ID3D10Blob> * pbyteCode,
                                          const char* pshaderText)
{
    Ptr<ID3D10Blob> pmsg;
    HRESULT hr;

#if !defined(SF_BUILD_SHIPPING)
    DWORD flags = D3D10_SHADER_DEBUG;
#else
    DWORD flags = D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

    hr = D3DX1x(CompileFromMemory)(pshaderText, (unsigned)strlen(pshaderText), 0, 0, 0, "main", 
        SF_RENDERER_VSHADER_PROFILE, flags, 0, 0, &pbyteCode->GetRawRef(), &pmsg.GetRawRef(), 0  );

    if (FAILED(hr) || !pbyteCode->GetPtr())
    {
        SF_DEBUG_WARNING2(1, "D3D1x::ShaderManager - vertex shader errors:\n%s\nShader Text:\n%s",
                          pmsg->GetBufferPointer(), pshaderText );
        return false;
    }
    hr = D3D1xCreateVertexShader(pDevice, (*pbyteCode)->GetBufferPointer(), (*pbyteCode)->GetBufferSize(), ppshaderPtr);
    if (FAILED(hr))
    {
        SF_DEBUG_WARNING1(1, "D3D1x::ShaderManager - Can't create vertex shader; error code = %d", hr);
        return false;
    }

    //SF_DEBUG_MESSAGE1(1, "VertexShaderText:\n%s\n", pshaderText);
    return true;
}

bool    ShaderManager::createInputLayout(ID3D1x(InputLayout)** pil, ID3D10Blob* pvsbyteCode, 
                                         const StdVertexShaderDesc& desc, PositionType positionType )
{
    unsigned             flags = desc.Flags;
    D3D1xVertexDeclBuilder vd;

    static const DXGI_FORMAT positionFormats[] =
    {
        DXGI_FORMAT_R16G16_SINT,
        DXGI_FORMAT_R32G32_FLOAT
    };

    vd.Add("SV_Position", 0, positionFormats[positionType]);

    if (flags & VFE_VColor)
        vd.Add("COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT);

    if (desc.HasFactors())
        vd.Add("COLOR", 1, DXGI_FORMAT_R8G8B8A8_UINT);

    if (flags & VFE_UV32f)
        vd.Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

    return SUCCEEDED( pDevice->CreateInputLayout(vd.Elements, vd.Count, 
        pvsbyteCode->GetBufferPointer(), pvsbyteCode->GetBufferSize(), pil ) );
}

bool    ShaderManager::createPixelShader(Ptr<ID3D1x(PixelShader)> *pshaderPtr,
                                         const char* pshaderText)
{
    if (!*pshaderPtr)
    {
        Ptr<ID3D10Blob> pshader;
        Ptr<ID3D10Blob> pmsg;
        HRESULT hr;

#if !defined(SF_BUILD_SHIPPING)
        DWORD flags = D3D10_SHADER_DEBUG;
#else
        DWORD flags = D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

        hr = D3DX1x(CompileFromMemory)(pshaderText, (unsigned)strlen(pshaderText), 0, 0, 0, "main", 
            SF_RENDERER_PSHADER_PROFILE, flags, 0, 0, &pshader.GetRawRef(), &pmsg.GetRawRef(), 0  );

        if (FAILED(hr))
        {
            SF_DEBUG_WARNING2(1, "D3D1x::ShaderManager - pixel shader errors:\n %s\nShader Text:\n%s",
                pmsg->GetBufferPointer(), pshaderText);
            return false;
        }
        if (!pshader ||
            ((hr = D3D1xCreatePixelShader(pDevice, pshader->GetBufferPointer(), pshader->GetBufferSize(), 
            &pshaderPtr->GetRawRef())) != S_OK) )
        {
            SF_DEBUG_WARNING2(1, "D3D1x::ShaderManager - Can't create pixel shader; error code = %d\nShader Text:\n%s", 
                hr, pshaderText );
            return false;
        }
    }

    //SF_DEBUG_MESSAGE1(1, "PixelShaderText:\n%s\n", pshaderText);
    return true;
}

bool ShaderManager::Initialize( ID3D1x(Device)* pdevice, ID3D1x(DeviceContext)* pcontext, const ProfileViews* profiler, UInt32 vmcflags )
{
    SF_ASSERT(!pDevice && !pDeviceContext);
    pDevice = pdevice;
    pDeviceContext = pcontext;
    Profiler = profiler;

    unsigned i, j;

    // Register vertex shaders
    for (i = 3; i < PrimFill_Type_Count * 3; i++)
    {
        StdVertexShaderDesc& desc = ShaderManager_VertexShaderDescriptors[i-3];
        VertexHandler&    vh   = VHandlerTable[i];

        vh.VShaderInit= false;
        for ( unsigned posType = 0; posType < PositionType_Count; ++posType )
        {
            vh.pFormat[posType] = registerVertexFormat(desc, (PositionType)posType);
            if ( !vh.pFormat[posType])
            {
                Reset();
                return false;
            }
        }

        if ( (vmcflags & HALConfig_DynamicShaderCompile) == 0 )
        {
            vh.VShaderInit = registerVertexHandler(desc, vh);
            if ( !vh.VShaderInit )
            {
                Reset();
                return false;
            }
        }
    }
    
    // Register pixel shaders-> separate loop
    for(i = 1; i < PS_Type_Count; i++)
    {
        unsigned maxLowCount = PSTable_ElementsPerType;
        for (j = 0; j < maxLowCount; j++)
        {
            if ( vmcflags & HALConfig_DynamicShaderCompile )
            {
                PSInitTable[i* PSTable_ElementsPerType + j] = false;
            }
            else
            {
                ID3D1x(PixelShader)* pshader = 
                    registerPixelShader(ShaderManager_PixelShaderDescriptors[i-1]|j);
                if (!pshader)
                {
                    Reset();
                    return false;
                }
                PSTable[i* PSTable_ElementsPerType + j] = pshader;
                PSInitTable[i* PSTable_ElementsPerType + j] = true;
            }
        }
    }

    // Create the solid color constant buffer.
    D3D1x(BUFFER_DESC) desc;
    memset(&desc, 0, sizeof desc);
    desc.ByteWidth      = 4 * sizeof(float);
    desc.Usage          = D3D1x(USAGE_DYNAMIC);
    desc.BindFlags      = D3D1x(BIND_CONSTANT_BUFFER);
    desc.CPUAccessFlags = D3D1x(CPU_ACCESS_WRITE);
    if ( FAILED( pDevice->CreateBuffer( &desc, 0, &SolidColorBuffer ) ))
        return false;

    return true;
}


void   ShaderManager::Reset()
{
    // Clear collections as they AddRef to resources
    VFormats.Clear();
    PShaders.Clear();
    VShaders.Clear();
    IFormats.Clear();

    if ( SolidColorBuffer )
    {
        SolidColorBuffer->Release();
        SolidColorBuffer =0;
    }

    pPrevLayout   = 0;
    PrevPSIndex  = 0;    
    pPrevVShader = 0;
    PrevTextures[0].Clear();
    PrevTextures[1].Clear();

    pDevice = 0;
    pDeviceContext = 0;
    memset(PSTable, 0, sizeof(PSTable));
    memset(VHandlerTable, 0, sizeof(VHandlerTable));
}

void    ShaderManager::BeginScene()
{
    pPrevLayout   = 0;
    PrevPSIndex  = 0;
    pPrevVShader = 0;
    PrevTextures[0].Clear();
    PrevTextures[1].Clear();

    // Always render tri-lists.
    pDeviceContext->IASetPrimitiveTopology(D3D1x(PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void    ShaderManager::EndScene()
{
}


// Applies the part of fill state that is shared between primitive batches.
bool   ShaderManager::ApplyPrimitiveFill(PrimitiveFill* pfill)
{
    // SolidColor value and Texture State is shared across the whole primitive,
    // so we apply it here.
    
    PrimitiveFillType fillType = Profiler->GetFillType(pfill->GetType()); 
    TexturePShader* pshader = &ShaderManager_FillToPixelShader[fillType];    

    if (fillType == PrimFill_SolidColor)
    {
        // Set vertex shader constant 0 to color.
        applySolidColorConstant(pfill->GetSolidColor());
    }

    else if (fillType >= PrimFill_Texture)
    {        
        D3D1x::Texture*  pt0 = (D3D1x::Texture*)pfill->GetTexture(0);
        DWORD           textureIndex = 0;
        SF_ASSERT(pt0);
        if (!pt0) return false;

        pshader = pshader->MatchImageFormat(pt0->GetImageFormat());
        if (!pshader) // No shader existed for specified format!!
        {
            PrimitivePSType = PS_None;
            return false;
        }
        
        ImageFillMode fm0 = pfill->GetFillMode(0);
        if (!PrevTextures[0].Match(pt0, fm0))
        {
            PrevTextures[0].Set(pt0, fm0);
            pt0->ApplyTexture(textureIndex, fm0);
        }
        textureIndex += pt0->GetTextureStageCount();

        if ((fillType == PrimFill_2Texture) || (fillType == PrimFill_2Texture_EAlpha))
        {
            D3D1x::Texture* pt1 = (D3D1x::Texture*)pfill->GetTexture(1);
            ImageFillMode  fm1 = pfill->GetFillMode(1);
            if (!PrevTextures[1].Match(pt1, fm1, textureIndex))
            {
                PrevTextures[1].Set(pt1, fm1, textureIndex);
                pt1->ApplyTexture(textureIndex, pfill->GetFillMode(1));
            }
            textureIndex += pt1->GetTextureStageCount();
        }
    }

    // Store PSType so that it can be applied in ApplyPrimitiveBatchFill
    PrimitivePSType = pshader->GetShader();
    return true;
}


bool ShaderManager::ApplyPrimitiveBatchFill(PrimitiveFillType fillType, UInt16 fillFlags,
                                            const VertexFormat* pformat,
                                            PrimitiveBatch::BatchType batchType)
{
    fillType = Profiler->GetFillType(fillType);

    // Determine vertex shader/declaration

    // TBD: SLOW.. take format/flag/vh from Batch?
    const VertexElement *ve = pformat->GetElement(VET_Pos, VET_Usage_Mask);
    SF_ASSERT(ve);    

    if (batchType > PrimitiveBatch::DP_Instanced)
        return false;
    SF_ASSERT(PrimitiveBatch::DP_Instanced == 2);
    
    VertexHandler&    vh = VHandlerTable[fillType * 3 + batchType];
    unsigned          declIndex = ((ve->Attribute & VET_CompType_Mask) != VET_F32) ? 0 : 1;

    // Delayed vertex shader / input layout creation
    if (!vh.VShaderInit)
    {
        StdVertexShaderDesc& desc = 
            ShaderManager_VertexShaderDescriptors[(fillType-1) * 3 + batchType];
        if ( !registerVertexHandler(desc, vh) || vh.pVShader == 0)
            return false;
        vh.VShaderInit = true;
    }

    if (pPrevLayout != vh.pLayout[declIndex])
    {
        pDeviceContext->IASetInputLayout(vh.pLayout[declIndex]);
        pPrevLayout = vh.pLayout[declIndex];
    }

    if ( pPrevVShader != vh.pVShader[declIndex] )
    {
        D3D1xVSSetShader( pDeviceContext, vh.pVShader[declIndex] );
        pPrevVShader = vh.pVShader[declIndex];
    }
    
    // Apply textures and their sample modes
    SF_COMPILER_ASSERT(PSBit_Multiply == FF_Multiply);
    SF_COMPILER_ASSERT(PSBit_AlphaWrite == FF_AlphaWrite);
    
    unsigned psIndex = (fillFlags & (FF_Multiply|FF_AlphaWrite)) | 
                       ((batchType == PrimitiveBatch::DP_Single) ? 0 : PSTable_InstancedBit) |
                       (PrimitivePSType << PSTable_PSTypeShift);
    
    if (PrevPSIndex != psIndex)
    {
        // Delayed pixel shader creation
        if (!PSInitTable[psIndex])
        {
            PSTable[psIndex] = 
                registerPixelShader(ShaderManager_PixelShaderDescriptors[PrimitivePSType-1]|
                                    (psIndex & 0x7));
            PSInitTable[psIndex] = true;
        }
        if (!PSTable[psIndex])
            return false;

        D3D1xPSSetShader(pDeviceContext, PSTable[psIndex] );
        PrevPSIndex = psIndex;
    }
    
    return true;
}

bool ShaderManager::ApplyClearMaskFill()
{
    // We use batched solid-color fill.    
    
    PrimitiveFillType fillType = Profiler->GetFillType(PrimFill_Mask);
    TexturePShader* pshader = &ShaderManager_FillToPixelShader[fillType]; 
    PrimitivePSType = pshader->GetShader();

    // TBD: It may make sense to pre-register a vertex format for MaskFill instead.

    // TBD: We may need to use a vertex with Z value if we plan to clear depth
    //      with draw commands in the future.
    return ApplyPrimitiveBatchFill(fillType, 0,
                                   &VertexXY16i::Format, PrimitiveBatch::DP_Batch);
}

bool ShaderManager::ApplySolidColorFill(Color c)
{    
    PrimitiveFillType fillType = Profiler->GetFillType(PrimFill_SolidColor);
    TexturePShader* pshader = &ShaderManager_FillToPixelShader[fillType]; 
    PrimitivePSType = pshader->GetShader();

    applySolidColorConstant(c);
    return ApplyPrimitiveBatchFill(fillType, 0,
                                   &VertexXY16i::Format, PrimitiveBatch::DP_Batch);
}


// Obtains formats that renderer will use for single, batches and instanced rendering of
// the specified source format.
//   - Filled in pointer may be the same as sourceFormat.
//   - 'instanced' format may be reported as 0, in which instancing is not supported for format.
void    ShaderManager::MapVertexFormat(
            PrimitiveFillType fill, const VertexFormat* sourceFormat,
            const VertexFormat** single,
            const VertexFormat** batch, const VertexFormat** instanced)
{

    // 1. Verify that sourceFormat has channels needed for fill.
    //    - same as matching destination non-instance formats
    //    - redundant since CheckVertexFormat iscalled in FillData constructor
    SF_ASSERT(PrimitiveFillData::CheckVertexFormat(fill, sourceFormat));    

    // Map: Fill -> VertexFormat, VertexDeclaration:
    //                16bit, 32bit Pos

    const VertexElement *ve = sourceFormat->GetElement(VET_Pos, VET_Usage_Mask);
    SF_ASSERT(ve);        
    unsigned            declIndex = ((ve->Attribute & VET_CompType_Mask) != VET_F32) ? 0 : 1;

    // 2. Get float/NoFloat value from VertexFormat
    // Index above based on this.
    
    *single    = VHandlerTable[fill * 3].pFormat[declIndex];
    *batch     = VHandlerTable[fill * 3 + 1].pFormat[declIndex];
    *instanced = VHandlerTable[fill * 3 + 2].pFormat[declIndex];
//    *instanced = 0;	// disable instancing on LOWEND HW
}



}}} // Scaleform::Render::D3D1x
