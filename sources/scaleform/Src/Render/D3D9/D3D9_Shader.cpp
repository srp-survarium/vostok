/**************************************************************************

Filename    :   D3D9_Shader.cpp
Content     :   
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#include "Render/D3D9/D3D9_Shader.h"
#include "Render/D3D9/D3D9_HAL.h"
#include "Kernel/SF_Debug.h"

// Generated .cpp files.
#include "Render/D3D9/D3D9_Shaders.h"
#include "Render/D3D9/D3D9_BinaryShaders.cpp"
#include "Render/D3D9/D3D9_Shaders.cpp"
#include <d3dx9shader.h>

namespace Scaleform { namespace Render { namespace D3D9 {

extern const char* ShaderUniformNames[Uniform::SU_Count];

bool VertexShader::Init(IDirect3DDeviceX* pdevice, const VertexShaderDesc* pd)
{
    pDesc = pd;

    if ( FAILED( pdevice->CreateVertexShader((const DWORD*)pDesc->pBinary, &pProg.GetRawRef())))
        return false;

    // The following code will determine if the shader generator has put the constants in the
    // table locations that we expect. If they are, this shouldn't be necessary. If they aren't,
    // the shader generator needs to be fixed.
    Ptr<ID3DXConstantTable> constantTable = 0;
    if ( FAILED( D3DXGetShaderConstantTable((const DWORD*)pDesc->pBinary, &constantTable.GetRawRef())))
        return false;

    for (unsigned i = 0; i < Uniform::SU_Count; i++)
    {
        const UINT MaxDescs = 8;
        D3DXCONSTANT_DESC descs[MaxDescs];
        if (!(pDesc->Uniforms[i].Location >= 0))
        {
            Uniforms[i] = -1;
            continue;
        }

        D3DXHANDLE hconstant = constantTable->GetConstantByName(0, ShaderUniformNames[i]);        
        UINT descCount = MaxDescs;
        if ( !hconstant || FAILED( constantTable->GetConstantDesc(hconstant, descs, &descCount) ))
        {
            SF_DEBUG_MESSAGE1(1, "Shader uniform: %s expected, but not found\n", ShaderUniformNames[i]);
            return false;
        }

        SF_ASSERT((int)descs[0].RegisterIndex == pDesc->Uniforms[i].Location);
        Uniforms[i] = descs[0].RegisterIndex;
        SF_ASSERT(Uniforms[i] >= 0);
    }

    return true;
}

void VertexShader::Shutdown( )
{
    pProg = 0;
}

bool FragShader::Init(IDirect3DDevice9* pdevice, const FragShaderDesc* pd)
{
    pDesc = pd;

    if ( FAILED( pdevice->CreatePixelShader((const DWORD*)pDesc->pBinary, &pProg.GetRawRef())))
        return false;

    // The following code will determine if the shader generator has put the constants in the
    // table locations that we expect. If they are, this shouldn't be necessary. If they aren't,
    // the shader generator needs to be fixed.
    Ptr<ID3DXConstantTable> constantTable = 0;
    if ( FAILED( D3DXGetShaderConstantTable((const DWORD*)pDesc->pBinary, &constantTable.GetRawRef())))
        return false;

    for (unsigned i = 0; i < Uniform::SU_Count; i++)
    {
        const UINT MaxDescs = 8;
        D3DXCONSTANT_DESC descs[MaxDescs];
        if (!(pDesc->Uniforms[i].Location >= 0))
        {
            Uniforms[i] = -1;
            continue;
        }

        D3DXHANDLE hconstant = constantTable->GetConstantByName(0, ShaderUniformNames[i]);
        UINT descCount = MaxDescs;
        if ( !hconstant || FAILED( constantTable->GetConstantDesc(hconstant, descs, &descCount) ))
        {
            // Some frag shaders contain scolor2, but only reference it during Highlight, so it
            // is sometimes compiled out of the constant table. This should be fixed, but is not crucial.
            if ( i != Uniform::SU_scolor2 )
            {
                SF_DEBUG_MESSAGE1(1, "Shader uniform: %s expected, but not found\n", ShaderUniformNames[i]);
                return false;
            }
            else
            {
                Uniforms[i] = -1;
                continue;
            }
        }

        SF_ASSERT((int)descs[0].RegisterIndex == pDesc->Uniforms[i].Location);
        Uniforms[i] = descs[0].RegisterIndex;
        SF_ASSERT(Uniforms[i] >= 0);
    }

    return true;
}

void FragShader::Shutdown( )
{
    pProg = 0;
}

struct D3DVertexDeclBuilder
{
    static const unsigned MaxElements = 8;
    D3DVERTEXELEMENT9     Elements[MaxElements];
    WORD                  Size[2], Count;

    D3DVertexDeclBuilder() : Count(0)
    { Size[0] = Size[1] = 0; }

    void    Add(WORD stream, unsigned type, WORD size,
                unsigned usage, BYTE usageIndex = 0)
    {
        SF_ASSERT(stream< MaxElements);

        D3DVERTEXELEMENT9 &e = Elements[Count++];
        e.Stream    = stream;
        e.Offset    = Size[stream];
        e.Type      = (BYTE)type;
        e.Method    = D3DDECLMETHOD_DEFAULT;
        e.Usage     = (BYTE)usage;
        e.UsageIndex= usageIndex;
        Size[stream]= Size[stream] + size;
        SF_ASSERT(Count < MaxElements);
    }

    void    Finish()
    {
        D3DVERTEXELEMENT9 e = D3DDECL_END();
        Elements[Count++] = e;
    }
};

SysVertexFormat::SysVertexFormat(IDirect3DDevice9* pdevice, const VertexFormat* vf, const VertexShaderDesc* pvdesc) : pVDecl(0)
{
    // The vertex format is ignored, except if it specifies a different format for the position; it is assumed that it will 
    // match the VertexShaderDesc's attribute definitions. They may not coincide exactly, for instance, the VSD's attributes 
    // for factors and batch indices may have already been combined.

    const VertexElement* ve = vf->GetElement(VET_Pos, VET_Usage_Mask);
    SF_ASSERT(ve); // must have position.
    
    D3DVertexDeclBuilder builder;
    const VertexShaderDesc& vdesc = *pvdesc;
    for (int attr = 0; attr < vdesc.NumAttribs; attr++)
    {
        unsigned type = vdesc.Attributes[attr].Type;
        WORD byteSize = (WORD)vdesc.Attributes[attr].ByteSize;
        if ( vdesc.Attributes[attr].Usage == D3DDECLUSAGE_POSITION)
        {
            switch( ve->Attribute & VET_CompType_Mask )
            {
                default: SF_ASSERT(0); // unsupported position format.
                case VET_S16: 
                    type = D3DDECLTYPE_SHORT2; 
                    byteSize = 2*sizeof(short);
                    break;
                case VET_F32: 
                    type = D3DDECLTYPE_FLOAT2; 
                    byteSize = 2*sizeof(float);
                    break;
            }
        }

        builder.Add((WORD)vdesc.Attributes[attr].Stream, 
                    type,
                    byteSize, 
                    vdesc.Attributes[attr].Usage,
                    (BYTE)vdesc.Attributes[attr].UsageIndex );
    }
    builder.Finish();

    if ( FAILED( pdevice->CreateVertexDeclaration(builder.Elements, &pVDecl.GetRawRef())))
    {
        SF_ASSERT(0);
        pVDecl = 0;
    }
    memcpy(VertexElements, builder.Elements, sizeof VertexElements);
}

bool ShaderInterface::SetStaticShader(VertexShaderDesc::ShaderType vshader, FragShaderDesc::ShaderType shader, const VertexFormat* pformat)
{
    CurShaders.pVFormat = pformat;
    CurShaders.pVS      = &pHal->StaticVShaders[vshader];
    CurShaders.pVDesc   = CurShaders.pVS->pDesc;
    CurShaders.pFS      = &pHal->StaticFShaders[shader];
    CurShaders.pFDesc   = CurShaders.pFS->pDesc;

    if ( pformat && !pformat->pSysFormat )
        (const_cast<VertexFormat*>(pformat))->pSysFormat = *SF_NEW SysVertexFormat(pHal->GetDevice(), pformat, CurShaders.pVS->pDesc);

    return (bool)CurShaders;
}

void ShaderInterface::SetTexture(Shader, unsigned stage, Render::Texture* ptexture, ImageFillMode fm)
{
    D3D9::Texture *pd3dTexture = (D3D9::Texture*)ptexture;
    SF_ASSERT(CurShaders.pFDesc->Uniforms[CurShaders.pFDesc->TexParams[stage]].Location >= 0 ); // Expected texture uniform does not exist in this shader.
    pd3dTexture->ApplyTexture(CurShaders.pFDesc->Uniforms[CurShaders.pFDesc->TexParams[stage]].Location, fm);
}

// Record the min/max shader constant registers, and just set the entire buffer in one call,
// instead of doing them individually. This could result in some uninitialized data to be sent,
// but it should be unreferenced by the shaders, and should be more efficient.
struct ShaderConstantRange
{
    ShaderConstantRange(IDirect3DDevice9* pdevice, float * data) : Min(SF_MAX_SINT), Max(-1), ShadowLocation(0), UniformData(data), pDevice(pdevice) { };
    bool Update( int location, int size, int shadowLocation, bool fragment )
    {
        location *= 4; // 4 elements per register.
        if ( (shadowLocation - location) != ShadowLocation && Max >= 0 )
        {
            // This shouldn't be the case, but if the storage is disjoint for some reason
            // we will need to upload the constants in separate steps.
            SF_ASSERT(0);
            if ( fragment )
                pDevice->SetPixelShaderConstantF(Min, UniformData + ShadowLocation, Max - Min);
            else
                pDevice->SetVertexShaderConstantF(Min, UniformData + ShadowLocation, Max - Min);
            ShadowLocation = shadowLocation - location;
            Min = SF_MAX_SINT;
            Max = -1;
            return true;
        }
        ShadowLocation = shadowLocation - location;
        Min = Alg::Min(Min, location/4);
        Max = Alg::Max(Max, (location + size + 3)/4); // 4 elements per register.
        return true;
    }
    int Min, Max, ShadowLocation;
    float* UniformData;
    IDirect3DDevice9* pDevice;
};

void ShaderInterface::Finish(unsigned meshCount)
{
    // Mesh count is unneeded - the constant registers that have been updated are tracked,
    // which includes any registers set for batching.
    SF_UNUSED(meshCount);

    IDirect3DDevice9* pdevice = pHal->GetDevice();
    ShaderConstantRange shaderConstantRangeFS(pdevice, UniformData);
    ShaderConstantRange shaderConstantRangeVS(pdevice, UniformData);
    for (int i = 0; i < Uniform::SU_Count; i++)
    {
        if (UniformSet[i])
        {
            if (CurShaders.pFS->Uniforms[i] >= 0)
            {
                SF_ASSERT(CurShaders.pFDesc->Uniforms[i].Location >= 0);
                shaderConstantRangeFS.Update( CurShaders.pFDesc->Uniforms[i].Location,
                                               CurShaders.pFDesc->Uniforms[i].Size,
                                               CurShaders.pFDesc->Uniforms[i].ShadowOffset,
                                               true );
            }
            else if (CurShaders->pVS->Uniforms[i] >= 0 )
            {
                SF_ASSERT(CurShaders.pVDesc->Uniforms[i].Location >= 0);
                shaderConstantRangeVS.Update( CurShaders.pVDesc->Uniforms[i].Location,
                                               CurShaders.pVDesc->Uniforms[i].Size,
                                               CurShaders.pVDesc->Uniforms[i].ShadowOffset,
                                               false );
            }
        }
    }

    if ( shaderConstantRangeVS.Max >= 0 )
    {
        pdevice->SetVertexShaderConstantF(
            shaderConstantRangeVS.Min, 
            UniformData + shaderConstantRangeVS.ShadowLocation, 
            shaderConstantRangeVS.Max - shaderConstantRangeVS.Min);
    }
    if ( shaderConstantRangeFS.Max >= 0 )
    {
        pdevice->SetPixelShaderConstantF(
            shaderConstantRangeFS.Min, 
            UniformData  + shaderConstantRangeFS.ShadowLocation, 
            shaderConstantRangeFS.Max - shaderConstantRangeFS.Min);
    }

    if (pLastVS != CurShaders.pVS->pProg)
    {
        pdevice->SetVertexShader(CurShaders.pVS->pProg);
        pLastVS = CurShaders.pVS->pProg;
    }

    IDirect3DVertexDeclaration9* pdecl = ((SysVertexFormat*)(CurShaders.pVFormat->pSysFormat.GetPtr()))->pVDecl;
    if (pLastDecl != pdecl)
    {
        pdevice->SetVertexDeclaration(pdecl);
        pLastDecl = pdecl;
    }

    if (pLastFS != CurShaders.pFS->pProg)
    {
        pdevice->SetPixelShader(CurShaders.pFS->pProg);
        pLastFS = CurShaders.pFS->pProg;
    }
}

// *** FilterShaderManager
bool FilterShaderManager::HasInstancingSupport() const
{
    return false;
}

void FilterShaderManager::MapVertexFormat(PrimitiveFillType fill, const VertexFormat* sourceFormat,
                                    const VertexFormat** single, const VertexFormat** batch, 
                                    const VertexFormat** instanced)
{
    unsigned                fillflags = 0;
    FragShaderType          shader = this->StaticShaderForFill(fill, fillflags, 0);
    const VertexShaderDesc* pshader = VertexShaderDesc::Descs[FragShaderDesc::VShaderForFShader[shader]];
    const VertexShaderDesc::VertexAttrDesc*   psvf = pshader->Attributes;
    const unsigned          maxVertexElements = 8;
    VertexElement           outf[maxVertexElements];
    unsigned                size = 0;
    int                     j = 0;

    for (int i = 0; i < pshader->NumAttribs; i++)
    {
        if ((psvf[i].Attr & (VET_Usage_Mask | VET_Index_Mask | VET_Components_Mask)) == (VET_Color | (1 << VET_Index_Shift) | 4))
        {
            // XXX - change shaders to use .rg instead of .ra for these
            outf[j].Offset = size;
            outf[j].Attribute = VET_T0Weight8;
            j++;
            outf[j].Offset = size+3;
            outf[j].Attribute = VET_FactorAlpha8;
            j++;
            size += 4;
            continue;
        }

        const VertexElement* pv = sourceFormat->GetElement(psvf[i].Attr & (VET_Usage_Mask|VET_Index_Mask), VET_Usage_Mask|VET_Index_Mask);
        if (!pv)
        {
            *batch = *single = *instanced = NULL;
            return;
        }
        outf[j] = *pv;
        outf[j].Offset = size;
        SF_ASSERT((outf[j].Attribute & VET_Components_Mask) > 0 && (outf[j].Attribute & VET_Components_Mask) <= 4);
        size += outf[j].Size();
        j++;
    }
    outf[j].Attribute = VET_None;
    outf[j].Offset = 0;
    *single = GetVertexFormat(outf, j+1, size);

    // Add in the batch instance after the factor arguments, if they are present.
    // Otherwise, add it at the end.
    bool instanceWithFactors = false;
    for ( int k = 0; k < j; k++ )
    {
        if (outf[k].Attribute == VET_T0Weight8)
        {
            memmove(&outf[k+2], &outf[k+1], (sizeof VertexElement)*(maxVertexElements-k-2));
            outf[k+1].Attribute = VET_Instance8;
            outf[k+1].Offset    = outf[k].Offset+1;
            instanceWithFactors = true;
            break;
        }
    }
    if ( !instanceWithFactors )
    {
        outf[j].Attribute = VET_Instance8;
        outf[j].Offset = size;
        size +=4;
    }
    outf[j+1].Attribute = VET_None;
    outf[j+1].Offset = 0;
    *batch = GetVertexFormat(outf, j+2, size);

    *instanced = 0;
}

bool FilterShaderManager::Initialize(HAL* phal)
{
    pDevice = phal->GetDevice();
    D3DCAPSx caps;
    pDevice->GetDeviceCaps(&caps);
    if ( caps.PixelShaderVersion < D3DPS_VERSION(3,0) )
        SupportsFilters = false;
    return true;
}

void FilterShaderManager::BeginScene()
{
    
}

void FilterShaderManager::EndScene()
{

}

void FilterShaderManager::Reset()
{
    pDevice = 0;
}

}}}
