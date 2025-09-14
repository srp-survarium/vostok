/**************************************************************************

Filename    :   D3D1x_Shader.cpp
Content     :   
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
#include "pch.h"

#include "Render/D3D1x/D3D1x_Shader.h"
#include "Render/D3D1x/D3D1x_HAL.h"
#include "Kernel/SF_Debug.h"

// Generated .cpp files.
#include "Render/D3D1x/D3D1x_Config.h"
#include "Render/D3D1x/D3D1x_Shaders.h"
#include "Render/D3D1x/D3D1x_BinaryShaders.inl"
#include "Render/D3D1x/D3D1x_Shaders.inl"
#include <D3Dcompiler.h>
#include <D3D11Shader.h>

namespace Scaleform { namespace Render { namespace D3D1x {

extern const char* ShaderUniformNames[Uniform::SU_Count];

bool VertexShader::Init(ID3D1x(Device)* pdevice, const VertexShaderDesc* pd)
{
    pDesc = pd;
    if ( FAILED( D3D1xCreateVertexShader(pdevice, pDesc->pBinary, pDesc->BinarySize, &pProg.GetRawRef())))
        return false;

    // Need to parse the constant table to find out where the constant offsets are. They might not be as
    // reported in the Desc, because the shader compiler can align things in a way it doesn't predict.
    ID3D1x(ShaderReflection)* preflector;
#if SF_D3D_VERSION == 11
    if ( FAILED( D3DReflect(pDesc->pBinary, pDesc->BinarySize, IID_ID3D1x(ShaderReflection), (void**)&preflector )))
        return false;
#elif SF_D3D_VERSION == 10
    // D3DReflect doesn't seem to work for DX10. Use the deprecated function.
    if ( FAILED( D3D10ReflectShader(pDesc->pBinary, pDesc->BinarySize, &preflector )))
        return false;
#endif
    ID3D1x(ShaderReflectionConstantBuffer)* pcb = preflector->GetConstantBufferByIndex(0);
    for ( unsigned uniform = 0; uniform < Uniform::SU_Count; ++uniform)
    {
        ID3D1x(ShaderReflectionVariable)* pvar = pcb->GetVariableByName(ShaderUniformNames[uniform]);
        D3D1x(SHADER_VARIABLE_DESC) varDesc;
        if ( !pvar || FAILED(pvar->GetDesc(&varDesc)))
        {
            UniformOffset[uniform] = -1;
            continue;
        }
        UniformOffset[uniform] = varDesc.StartOffset;
    }
    return true;
}

void VertexShader::Shutdown( )
{
    pProg = 0;
}

bool FragShader::Init(ID3D1x(Device)* pdevice, const FragShaderDesc* pd)
{
    pDesc = pd;
    if ( !SUCCEEDED( D3D1xCreatePixelShader(pdevice, pDesc->pBinary, pDesc->BinarySize, &pProg.GetRawRef())) )
        return false;

    // Need to parse the constant table to find out where the constant offsets are. They might not be as
    // reported in the Desc, because the shader compiler can align things in a way it doesn't predict.
    ID3D1x(ShaderReflection)* preflector;
#if SF_D3D_VERSION == 11
    if ( FAILED( D3DReflect(pDesc->pBinary, pDesc->BinarySize, IID_ID3D1x(ShaderReflection), (void**)&preflector )))
        return false;
#elif SF_D3D_VERSION == 10
    // D3DReflect doesn't seem to work for DX10. Use the deprecated function.
    if ( FAILED( D3D10ReflectShader(pDesc->pBinary, pDesc->BinarySize, &preflector )))
        return false;
#endif
    ID3D1x(ShaderReflectionConstantBuffer)* pcb = preflector->GetConstantBufferByIndex(0);
    for ( unsigned uniform = 0; uniform < Uniform::SU_Count; ++uniform)
    {
        ID3D1x(ShaderReflectionVariable)* pvar = pcb->GetVariableByName(ShaderUniformNames[uniform]);
        D3D1x(SHADER_VARIABLE_DESC) varDesc;
        if ( !pvar || FAILED(pvar->GetDesc(&varDesc)))
        {
            UniformOffset[uniform] = -1;
            continue;
        }
        UniformOffset[uniform] = varDesc.StartOffset;
    }

    return true;
}

void FragShader::Shutdown( )
{
    pProg = 0;
}

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

SysVertexFormat::SysVertexFormat(ID3D1x(Device)* pdevice, const VertexFormat* vf, const VertexShaderDesc* pvdesc) : pVDecl(0)
{
    // The vertex format is ignored, except if it specifies a different format for the position; it is assumed that it will 
    // match the VertexShaderDesc's attribute definitions. They may not coincide exactly, for instance, the VSD's attributes 
    // for factors and batch indices may have already been combined.

    const VertexElement* ve = vf->GetElement(VET_Pos, VET_Usage_Mask);
    SF_ASSERT(ve); SF_UNUSED(ve); // must have position.
    
    D3D1xVertexDeclBuilder builder;
    const VertexShaderDesc& vdesc = *pvdesc;
    for (int attr = 0; attr < vdesc.NumAttribs; attr++)
    {
        builder.Add(vdesc.Attributes[attr].SemanticName, vdesc.Attributes[attr].SemanticIndex, 
                    (DXGI_FORMAT)vdesc.Attributes[attr].Format );
    }

    memcpy(&VertexElements, &builder.Elements, builder.Count * sizeof(D3D1x(INPUT_ELEMENT_DESC)));
    VertexElementCount = builder.Count;

    if ( FAILED( pdevice->CreateInputLayout(builder.Elements, builder.Count, pvdesc->pBinary, 
                                            pvdesc->BinarySize, &pVDecl.GetRawRef())))
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
    D3D1x::Texture *pd3dTexture = (D3D1x::Texture*)ptexture;
    SF_ASSERT(CurShaders.pFDesc->Uniforms[CurShaders.pFDesc->TexParams[stage]].Location >= 0 ); // Expected texture uniform does not exist in this shader.
    pd3dTexture->ApplyTexture(CurShaders.pFDesc->Uniforms[CurShaders.pFDesc->TexParams[stage]].Location, fm);
}

// Record the min/max shader constant registers, and just set the entire buffer in one call,
// instead of doing them individually. This could result in some uninitialized data to be sent,
// but it should be unreferenced by the shaders, and should be more efficient.
struct ShaderConstantRange
{
    ShaderConstantRange(HAL* phal, float* data) : UniformData(data), pConstantBuffer(0), pHal(phal) { };
    void Update( int offset, int size, int shadowLocation)
    {
        if ( offset < 0 )
            return;

        if (pConstantBuffer == 0 )
        {
            pConstantBuffer = pHal->getNextConstantBuffer();
            D3D1xMapBuffer(pHal->pDeviceContext, pConstantBuffer, 0, D3D1x(MAP_WRITE_DISCARD), 0, &MappedBuffer);
        }
        memcpy((float*)MappedBuffer.pData + (offset/sizeof(float)), UniformData + shadowLocation, size*sizeof(float) );
    }
    void Finish(bool bfragment)
    {
        if (pConstantBuffer)
        {
            D3D1xUnmapBuffer(pHal->pDeviceContext, pConstantBuffer, 0);
            if ( bfragment )
                pHal->pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
            else
                pHal->pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
        }
    }

    float* UniformData;
    D3D1x(MAPPED_BUFFER) MappedBuffer;
    ID3D1x(Buffer)*      pConstantBuffer;
    HAL*                 pHal;
};

void ShaderInterface::Finish(unsigned meshCount)
{
    // Mesh count is unneeded - the constant registers that have been updated are tracked,
    // which includes any registers set for batching.
    SF_UNUSED(meshCount);

    ShaderConstantRange shaderConstantRangeFS(pHal, UniformData);
    ShaderConstantRange shaderConstantRangeVS(pHal, UniformData);
    for (int i = 0; i < Uniform::SU_Count; i++)
    {
        if (UniformSet[i])
        {
            if (CurShaders.pFS->UniformOffset[i] >= 0)
            {
                shaderConstantRangeFS.Update( CurShaders.pFS->UniformOffset[i],
                                               CurShaders.pFDesc->Uniforms[i].Size,
                                               CurShaders.pFDesc->Uniforms[i].ShadowOffset);
            }
            else if (CurShaders.pVS->UniformOffset[i] >= 0 )
            {
                shaderConstantRangeVS.Update( CurShaders.pVS->UniformOffset[i],
                                               CurShaders.pVDesc->Uniforms[i].Size,
                                               CurShaders.pVDesc->Uniforms[i].ShadowOffset);
            }
        }
    }


    shaderConstantRangeVS.Finish(false);
    shaderConstantRangeFS.Finish(true);

    ID3D1x(DeviceContext)* pdevice = pHal->pDeviceContext;
    if (pLastVS != CurShaders.pVS->pProg)
    {
        D3D1xVSSetShader( pdevice, CurShaders.pVS->pProg);
        pLastVS = CurShaders.pVS->pProg;
    }

    ID3D1x(InputLayout)* pdecl = ((SysVertexFormat*)(CurShaders.pVFormat->pSysFormat.GetPtr()))->pVDecl;
    if (pLastDecl != pdecl)
    {
        pdevice->IASetInputLayout(pdecl);
        pLastDecl = pdecl;
    }

    if (pLastFS != CurShaders.pFS->pProg)
    {
        D3D1xPSSetShader(pdevice, CurShaders.pFS->pProg);
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
    const VertexShaderDesc* pshader = VertexShaderDesc::Descs_[FragShaderDesc::VShaderForFShader[shader]];
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
    VFormats.Clear();
}

}}}
