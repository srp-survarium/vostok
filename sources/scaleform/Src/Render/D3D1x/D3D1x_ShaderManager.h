/**************************************************************************

Filename    :   D3D1x_ShaderManager.h
Content     :   
Created     :   Mar 2011
Authors     :   Bart Muzzin

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_D3D1X_SHADERMANAGER_H
#define INC_SF_D3D1X_SHADERMANAGER_H
#pragma once

#include "Render/D3D1x/D3D1x_Config.h"
#include "Render/D3D1x/D3D1x_Texture.h"
#include "Render/D3D1x/D3D1x_MeshCache.h"

#include "Render/Render_HAL.h"
#include "Render/Render_Primitive.h"
#include "Render/Render_Containers.h"

namespace Scaleform { namespace Render { 
	
namespace D3D1x {

#define SF_RENDERER_VSHADER_PROFILE "vs_4_0"

// *** Vertex Shader / Vertex Declaration constansts

enum VSGroupMode
{
    VSGroup_Single      = 0x0000,
    VSGroup_Batch       = 0x0100,
    VSGroup_Instanced   = 0x0200,
    VSGroup_Mask        = 0x0300
};
enum VSColorInput
{
    VSColor_None        = 0x0000,
    VSColor_Solid       = 0x0400,
    VSColor_Vertex      = 0x0800,
    VSColor_Mask        = 0x0C00,
};
enum VSTextureInput
{
    VSTexture_None      = 0x0000,
    VSTexture_Gen1      = 0x1000,
    VSTexture_Gen2      = 0x2000,
    VSTexture_UV1       = 0x3000,
    VSTexture_Mask      = 0xF000,
};

// Generate VertexElement
enum VertexElementBits
{    
    // Elements of VertexFormat
    VFE_IIndex          = 0x0001, // Instance index
    VFE_EAlpha          = 0x0002,
    VFE_Weight          = 0x0004,
    VFE_VColor          = 0x0008, 
    VFE_UV32f           = 0x0010,

    VFE_Factors_Mask    = VFE_EAlpha | VFE_Weight | VFE_IIndex,
    VFE_Mask            = 0x00FF,
    
    VSBit_Factor        = 0x10000
};


// StdVertexShaderDesc describes the features and type of vertex shader to use.
struct StdVertexShaderDesc
{
    UInt32  Flags;

    VSGroupMode     GetGroupMode() const    { return (VSGroupMode)(Flags & VSGroup_Mask); }
    VSColorInput    GetColorInput() const   { return (VSColorInput)(Flags & VSColor_Mask); }
    VSTextureInput  GetTextureInput() const { return (VSTextureInput)(Flags & VSTexture_Mask); }
    bool            HasFactors() const      { return (Flags & VFE_Factors_Mask) != 0; }

    unsigned        GetVertexShaderKey(unsigned posType) const
    {
        unsigned key  = Flags & (VSGroup_Mask | VSColor_Mask | VSTexture_Mask);
        if (HasFactors())
            key |= VSBit_Factor;
        key |= (unsigned)posType;
        return key;
    }
};


// Pixel shader texture key bit flags. These bits are used to both generate and uniquely
// identify a pixel shader; note that they don't map one-to-one to PSTypes, as some
// combinations are not needed.
enum PSKeyBits
{
    // Low 8 bits, always created.
    // In addition to being used as a part of PShader key, there 3 bits
    // also index ShaderManager::PSTable.
    PSBit_Multiply    = 0x0001,
    PSBit_AlphaWrite  = 0x0002,
    PSBit_Instanced   = 0x0004,
    PSBit_LowBitCount = 3,  
    // Shader has EAlpha stage after cxform (requires factor).
    PSBit_EAlpha      = 0x0008,
    // PSFillOP index occupies the higher bits, identifies operation used
    // to assign color from texture(s).
    PSBit_FillOp_Mask = 0x00F0,
    PSBit_FillOp_Shift= 4,
};


// PSType describes the significantly-different types of pixel shaders used in
// renderer and indexable through ShaderManager::PStable.
// For each type there are 8 versions matching low PSBit values
// (Multiply, AlphaWrite, Instanced).
enum PSType
{
    PS_None,
    PS_NoColor, // Color = 1.0, used for masks.
    PS_Color,
    PS_Color_EAlpha,
    PS_Texture,
    PS_Texture_EAlpha,
    PS_Texture_VColor,
    PS_Texture_VColor_EAlpha,
    PS_2Texture,
    PS_2Texture_EAlpha,
    PS_TextureA_VColor,
    PS_TextureYUV,
    PS_TextureYUVA,
    PS_Type_Count
};


// ShaderManager - maintains shaders and declarations needed for rendering with 
// different fill styles and includes methods for applying them to the device.

class ShaderManager
{
    friend class HAL;

    ID3D1x(Device)*           pDevice;
    ID3D1x(DeviceContext)*    pDeviceContext;
    const ProfileViews*     Profiler;

    enum PositionType
    {
        PositionType_SInt16,
        PositionType_Float32,
        PositionType_Count
    };
    
    struct VertexHandler
    {
        const VertexFormat *  pFormat[PositionType_Count];
        ID3D1x(InputLayout)*    pLayout[PositionType_Count];
        ID3D1x(VertexShader)*   pVShader[PositionType_Count];
        bool                  VShaderInit;

        VertexHandler()
        {
            memset(pFormat, 0, sizeof pFormat);
            memset(pLayout, 0, sizeof pLayout);
            memset(pVShader, 0, sizeof pVShader);
            VShaderInit = false;
        }
    };

    enum {
        PSTable_PSTypeShift     = 3,
        PSTable_ElementsPerType = 1 << PSTable_PSTypeShift,
        PSTable_Size            = PS_Type_Count * PSTable_ElementsPerType,
        PSTable_InstancedBit    = PSBit_Instanced
    };

    // VHandlerTable for each type { Single, Batch, Instanced }
    VertexHandler           VHandlerTable[PrimFill_Type_Count  * 3];
    // Pixels shaders
    ID3D1x(PixelShader)*      PSTable[PSTable_Size];
    // Used in delayed shader binding: 'true' if shader was created
    bool                    PSInitTable[PSTable_Size];

    // PSType computed by ApplyPrimitiveFill
    PSType                  PrimitivePSType;

    // Information about previously applied texture to avoid redundant state apply.
    struct PrevTextureDesc
    {
        Texture*        pTexture;
        ImageFillMode   Fill;
        DWORD           Stage;
        void    Clear() { pTexture = 0; }
        void    Set(Texture* tex, ImageFillMode fill, DWORD stage = 0)
        { pTexture = tex; Fill = fill; Stage = stage; }
        bool    Match(Texture* tex, ImageFillMode fill) const
        { return (pTexture == tex) && (Fill == fill); }
        bool    Match(Texture* tex, ImageFillMode fill, DWORD stage) const
        { return (pTexture == tex) && (Fill == fill) && (Stage == stage); }
    };

    ID3D1x(Buffer)*                SolidColorBuffer;
    ID3D1x(InputLayout)*           pPrevLayout;
    ID3D1x(VertexShader)*          pPrevVShader;
    unsigned                     PrevPSIndex;
    PrevTextureDesc              PrevTextures[2];

    MultiKeyCollection<VertexElement, VertexFormat, 32>       VFormats;
    MultiKeyCollection<unsigned, Ptr<ID3D1x(PixelShader)> >   PShaders;
    MultiKeyCollection<unsigned, Ptr<ID3D1x(VertexShader)> >  VShaders;
    MultiKeyCollection<unsigned, Ptr<ID3D1x(InputLayout)> >   IFormats;

    bool            createInputLayout(ID3D1x(InputLayout)** pil, ID3D10Blob* pvsbyteCode, 
                                      const StdVertexShaderDesc& desc, PositionType positionType );
    bool            createVertexShader(ID3D1x(VertexShader)** ppshaderPtr,
                                       Ptr<ID3D10Blob>* pbyteCode,
                                       const char* pshaderText);
    bool            createPixelShader(Ptr<ID3D1x(PixelShader)> *pshaderPtr,
                                      const char* pshaderText);
    
    static bool createVertexShaderString(StringBuffer& sb, const StdVertexShaderDesc& desc, PositionType posType);
    static bool     createPixelShaderString(StringBuffer& sb, unsigned psBits);


    bool                 registerVertexHandler(const StdVertexShaderDesc& desc, VertexHandler& vh );
    ID3D1x(PixelShader)*   registerPixelShader(unsigned psBits);
    const VertexFormat*  registerVertexFormat(StdVertexShaderDesc& desc, PositionType posType);

public:
    void  applySolidColorConstant(Color c)
    {
        c = Profiler->GetColor(c);
        const float mult  = 1.0f / 255.0f;
        const float alpha = c.GetAlpha() * mult;
        float       rgba[4] = { c.GetRed() * mult,  c.GetGreen() * mult, c.GetBlue() * mult, alpha };

        D3D1x(MAPPED_BUFFER) mappedBuffer;
        D3D1xMapBuffer( pDeviceContext, SolidColorBuffer, 0, D3D1x(MAP_WRITE_DISCARD), 0, &mappedBuffer);
        memcpy( mappedBuffer.pData, rgba, sizeof(rgba));
        D3D1xUnmapBuffer( pDeviceContext, SolidColorBuffer, 0 );
        pDeviceContext->VSSetConstantBuffers(1, 1, &SolidColorBuffer);
    }

public:
    ShaderManager();
    ~ShaderManager();

    bool Initialize(ID3D1x(Device)* pdevice, ID3D1x(DeviceContext)* pcontext, const ProfileViews* profiler, UInt32 vmcflags);
    void   Reset();

    void    BeginScene();
    void    EndScene();

    // Applies the part of fill state that is shared between primitive batches.
    //  - Texture state is generally shared
    //  - If Apply returns false, rendering shouldn't take place
    bool   ApplyPrimitiveFill(PrimitiveFill* pfill);
    // Applies Primitive-Batch specific fills states.
    //  - pformat is passed since if is different from the one in pfill, due to being "Mapped"
    //    TBD: Can/should we map it as a part of pfill?
    bool   ApplyPrimitiveBatchFill(PrimitiveFillType fillType, UInt16 fillFlags,
                                   const VertexFormat* pformat, PrimitiveBatch::BatchType type);

    bool   ApplyClearMaskFill();
    bool   ApplySolidColorFill(Color c);

    // Obtains formats that renderer will use for single, batches and instanced rendering of
    // the specified source format.
    //   - Filled in pointer may be the same as sourceFormat.
    //   - 'instanced' format may be reported as 0,
    //     in which instancing is not supported for format.
    void    MapVertexFormat(PrimitiveFillType fill, const VertexFormat* sourceFormat,
                            const VertexFormat** single,
                            const VertexFormat** batch, const VertexFormat** instanced);

};


}}} // Scaleform::Render::D3D1x

#endif // INC_SF_D3D1X_SHADERMANAGER_H
