/**************************************************************************

Filename    :   D3D9_HAL.cpp
Content     :   D3D9 Renderer HAL Prototype implementation.
Created     :   May 2009
Authors     :   Michael Antonov

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#include "Kernel/SF_Debug.h"
#include "Kernel/SF_Random.h"
#include "D3D9_HAL.h"
#include "Kernel/SF_HeapNew.h"
#include <d3dx9.h>

#include <stdio.h>

#if SF_CC_MSVC >= 1600 // MSVC 2010
#include <crtdefs.h>
#else
#include <xutility>
#endif // SF_CC_MSVC >= 1600 // MSVC 2010

namespace Scaleform { namespace Render { namespace D3D9 {

static PixMarker MaskMarker(0, false);
static PixMarker RenderTargetMarker(0, false);
static PixMarker FilterMarker(0, false);

// Defines all D3Dx color channels for D3DRS_COLORWRITEENABLE.
#define D3DCOLORWRITEENABLE_ALL D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | \
    D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN


// ***** HAL_D3D9

HAL::HAL(ThreadCommandQueue* commandQueue)
:   Render::HAL(commandQueue),
    pDevice(0),
    Cache(Memory::GetGlobalHeap(), MeshCacheParams::PC_Defaults),
    SManager(Cache.GetParams()),
    FSManager(&Profiler),
    ShaderData(getThis()),
    PrevBatchType(PrimitiveBatch::DP_None),
    // Mask/Stencil vars
    StencilChecked(false), StencilAvailable(false), MultiBitStencil(false),
    DepthBufferAvailable(false),
    StencilOpInc(D3DSTENCILOP_REPLACE),
    FillFlags(0)

{
}

HAL::~HAL()
{
    ShutdownHAL();
}

PrimitiveFill*  HAL::CreatePrimitiveFill(const PrimitiveFillData &data)
{
    return SF_HEAP_NEW(pHeap) PrimitiveFill(data);
}

template< class _MatrixType >
class MatrixUpdateAdapter_Meshes
{
public:
    MatrixUpdateAdapter_Meshes( const Primitive::MeshEntry* meshes, unsigned count, unsigned matricesPerMesh ) : 
      Count(count), MatricesPerMesh(matricesPerMesh), Meshes(meshes) { }

    typedef _MatrixType MatrixType;

    unsigned        GetCount() const { return Count; }
    unsigned        GetElementsPerTransform() const { return 4; } // always 4, even for 2D case.
    unsigned        GetMatricesPerMesh() const { return MatricesPerMesh; }
    const Matrix2F& GetVertexMatrix(unsigned i ) const { return Meshes[i].pMesh->VertexMatrix; }
    const HMatrix&  GetHMatrix(unsigned i) const { return Meshes[i].M; }

private:
    unsigned Count;
    unsigned MatricesPerMesh;
    const Primitive::MeshEntry* Meshes;
};

template< class _MatrixType >
class MatrixUpdateAdapter_Matrices
{
public:
    MatrixUpdateAdapter_Matrices( const StrideArray<const HMatrix>& matrixArray, unsigned matricesPerMesh, const Matrix2F & vertexMatrix ) : 
      MatrixArray(matrixArray), MatricesPerMesh(matricesPerMesh), VertexMatrix(vertexMatrix) { }

    typedef _MatrixType MatrixType;

    unsigned        GetCount() const { return (unsigned)MatrixArray.GetSize(); }
    unsigned        GetElementsPerTransform() const { return 4; } // always 4, even for 2D case.
    unsigned        GetMatricesPerMesh() const { return MatricesPerMesh; }
    const Matrix2F& GetVertexMatrix(unsigned i ) const { SF_UNUSED(i); return VertexMatrix; }
    const HMatrix&  GetHMatrix(unsigned i) const { return MatrixArray[i]; }

private:
    // Hide warnings (this class is never assigned to).
    MatrixUpdateAdapter_Matrices & operator=( const MatrixUpdateAdapter_Matrices & k )
    { SF_ASSERT(0); return *this; }

    const StrideArray<const HMatrix>& MatrixArray;
    unsigned MatricesPerMesh;
    const Matrix2F& VertexMatrix;
};

// Draws a range of pre-cached and preprocessed primitives
void HAL::DrawProcessedPrimitive(Primitive* pprimitive,
                                       PrimitiveBatch* pstart, PrimitiveBatch *pend)
{
    SF_AMP_SCOPE_RENDER_TIMER("HAL::DrawProcessedPrimitive", Amp_Profile_Level_High);
    PixMarker event(__FUNCTIONW__);

    if (!checkState(HS_InDisplay, __FUNCTION__) ||
        !pprimitive->GetMeshCount() )
        return;

    // If in overdraw profile mode, and this primitive is part of a mask, draw it in color mode.
    static bool drawingMask = false;
    if ( !Profiler.ShouldDrawMask() && !drawingMask && (HALState & HS_DrawingMask) )
    {
        drawingMask = true;
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL );
        pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE );
        DrawProcessedPrimitive(pprimitive, pstart, pend);
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0 );
        pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE );
        drawingMask = false;
    }

    SF_ASSERT(pend != 0);

    Primitive::MeshEntry* meshes = &pprimitive->Meshes[0];
    PrimitiveBatch* pbatch = pstart ? pstart : pprimitive->Batches.GetFirst();
    
    bool            shaderManagerSuccess = SManager.ApplyPrimitiveFill(pprimitive->pFill);

    // Assuption: all meshes within a batch are 2D or 3D.
    bool            has3D = pbatch->GetMeshCount() > 0 ? meshes[0].M.Has3D() : false;

    unsigned bidx = 0;
    while (pbatch != pend)
    {
        // pBatchMesh can be null in case of error, such as VB/IB lock failure.
        MeshCacheItem* pmesh = (MeshCacheItem*)pbatch->GetCacheItem();
        unsigned       meshIndex = pbatch->GetMeshIndex();

        if (pmesh)
        {
            Profiler.SetBatch((UPInt)pprimitive, bidx);

            bool primSuccess = SManager.ApplyPrimitiveBatchFill(pprimitive->pFill->GetType(), FillFlags,
                                                                pbatch->pFormat, pbatch->Type);

            if (shaderManagerSuccess && (HALState & HS_ViewValid) && primSuccess)
            {
                SF_ASSERT((pbatch->Type != PrimitiveBatch::DP_Failed) &&
                          (pbatch->Type != PrimitiveBatch::DP_Virtual));

                // Draw the object with cached mesh.
                UPInt   indexOffset = pmesh->IBAllocOffset / sizeof (IndexType);

                if (pbatch->Type != PrimitiveBatch::DP_Instanced)
                {
                    if ( has3D )
                    {
                        applyMatrixConstants(MatrixUpdateAdapter_Meshes<Matrix4F>(
                            meshes + meshIndex, 
                            pbatch->GetMeshCount(), 
                            pprimitive->MatricesPerMesh));
                    }
                    else
                    {
                        applyMatrixConstants(MatrixUpdateAdapter_Meshes<Matrix2F>(
                            meshes + meshIndex, 
                            pbatch->GetMeshCount(), 
                            pprimitive->MatricesPerMesh));
                    }

                    // Setup for single/batch rendering.
                    setLinearStreamSource(pbatch->Type);

                    AccumulatedStats.Meshes += pmesh->MeshCount;
                    AccumulatedStats.Triangles += pmesh->IndexCount / 3;
                }
                else
                {
                    StrideArray<const HMatrix> matrixArray(&meshes[meshIndex].M, pbatch->GetMeshCount(), sizeof(Primitive::MeshEntry));
                    if ( has3D )
                    {
                        applyMatrixConstants(MatrixUpdateAdapter_Matrices<Matrix4F>(
                            matrixArray, 
                            pprimitive->MatricesPerMesh, 
                            meshes[meshIndex].pMesh->VertexMatrix));
                    }
                    else
                    {
                        applyMatrixConstants(MatrixUpdateAdapter_Matrices<Matrix2F>(
                            matrixArray, 
                            pprimitive->MatricesPerMesh, 
                            meshes[meshIndex].pMesh->VertexMatrix));
                    }

                    setInstancedStreamSource(pbatch->GetMeshCount());  
   
                    AccumulatedStats.Meshes += pbatch->GetMeshCount();
                    AccumulatedStats.Triangles += (pmesh->IndexCount / 3) * pbatch->GetMeshCount();
                }

                pDevice->SetIndices(pmesh->pIndexBuffer->GetHWBuffer());
                pDevice->SetStreamSource(0, pmesh->pVertexBuffer->GetHWBuffer(),
                                         (UINT)pmesh->VBAllocOffset, pbatch->pFormat->Size);
                pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                                              0, 0, (UINT)(pmesh->VBAllocSize / pbatch->pFormat->Size),
                                              (UINT)indexOffset, pmesh->IndexCount / 3 );
     
                AccumulatedStats.Primitives++;
            }

            pmesh->MoveToCacheListFront(MCL_ThisFrame);
        }
        
        pbatch = pbatch->GetNext();
        ++bidx;
    }
}


class VSConstantsUpdateBuffer
{
public:

    enum {
        MaxInstances    = SF_RENDER_D3D9_INSTANCE_MATRICES,
        MaxInstanceData = SF_RENDER_D3D9_ROWS_PER_INSTANCE
    };

    VSConstantsUpdateBuffer(HAL* hal, const ProfileViews* profiler)
        : pHAL(hal), Profiler(profiler), InstanceCount(0), TextureCount(0) { }


    static const int ept = 4;

    template< class MatrixType >
    void    SetInstanceData(const StrideArray<HMatrix>& matrices,
                            unsigned startIndex,
                            unsigned count, unsigned textureCount,
                            const Matrix2F& vertexMatrix)
    {        
        InstanceCount = count;
        TextureCount  = textureCount;
       
        for (unsigned i = 0; i < InstanceCount; i++)
        {            
            const HMatrix& hm = matrices[startIndex + i];
            unsigned       k = (ept + (textureCount+1) * 2) * i;
            HAL::calculateTransform<MatrixType>(vertexMatrix, hm, pHAL->GetMatrices(), &Rows[k]);
            Profiler->GetCxform(hm.GetCxform()).GetAsFloat2x4(&Rows[k+ept]);
        }
    }

    void    SetTextureData(const Matrix2F* matrices,
                           const unsigned *indices, unsigned count)
    {
        for (unsigned i = 0; i < InstanceCount; i++)
        {
            unsigned k = (ept + (TextureCount+1) * 2) * i;

            if (count)
            {
                const Matrix2F& m0 = matrices[indices[0]];
                m0.GetAsFloat2x4(&Rows[k+ept+2]);
                if (count > 1)
                {
                    const Matrix2F& m1 = matrices[indices[1]];
                    m1.GetAsFloat2x4(&Rows[k+ept+4]);
                }
            }
        }
    }

    void    ApplyAll()
    {
        const unsigned matrixConstBase = 1; 
        pHAL->pDevice->SetVertexShaderConstantF(
            matrixConstBase + 0, 
            &Rows[0][0], (ept + 2 + 2 * TextureCount) * InstanceCount);
    }

    void    ApplyTextureData()
    {        
        if (TextureCount)
        {
            // TBD: We just skip firs set of matrices for now;
            // not sure if it's worth it calling separate updates.
            const unsigned matrixConstBase = 1;
            enum { Delta = 6 };
            pHAL->pDevice->SetVertexShaderConstantF(
                matrixConstBase + Delta, 
                &Rows[Delta][0], (ept + 2 + 2 * TextureCount) * InstanceCount - Delta);
        }
    }

private:
    HAL*       pHAL;
    const ProfileViews* Profiler;
    unsigned   InstanceCount;
    unsigned   TextureCount;
    float      Rows[MaxInstances * MaxInstanceData][4];
};



void HAL::DrawProcessedComplexMeshes(ComplexMesh* complexMesh,
                                           const StrideArray<HMatrix>& matrices)
{    
    SF_AMP_SCOPE_RENDER_TIMER(__FUNCTION__, Amp_Profile_Level_High);
    PixMarker event(__FUNCTIONW__);

    typedef ComplexMesh::FillRecord   FillRecord;
    typedef PrimitiveBatch::BatchType BatchType;
 
    MeshCacheItem* pmesh = (MeshCacheItem*)complexMesh->GetCacheItem();
    if (!checkState(HS_InDisplay, __FUNCTION__) || !pmesh)
        return;

    // If in overdraw profile mode, and this primitive is part of a mask, draw it in color mode.
    static bool drawingMask = false;
    if ( !Profiler.ShouldDrawMask() && !drawingMask && (HALState & HS_DrawingMask) )
    {
        drawingMask = true;
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL );
        pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE );
        DrawProcessedComplexMeshes(complexMesh, matrices);
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0 );
        pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE );
        drawingMask = false;
    }

    const FillRecord* fillRecords = complexMesh->GetFillRecords();
    unsigned    fillCount     = complexMesh->GetFillRecordCount();
    unsigned    instanceCount = (unsigned)matrices.GetSize();
    unsigned    indexBufferOffset = (unsigned)(pmesh->IBAllocOffset / sizeof(IndexType));
    unsigned    vertexSize      = 0;     // 0 => VertexBuffer wasn't applied yet.
    unsigned    vertexBaseIndex = 0;
    UByte       prevTextureCount= 0xFF;  // 0xFF => vsConstants.SetInstanceData not yet called.
    unsigned    formatIndex;
    BatchType   batchType;
    bool        has3D = instanceCount >= 0 ? matrices[0].Has3D() : false;

    if (instanceCount > 1 && SManager.HasInstancingSupport())
    {
        batchType = PrimitiveBatch::DP_Instanced;
        formatIndex = 1;
        // If (instanceCount >= MaxInstanced), we might pass an argument out of HW range here;
        // however,  that case is fixed by SetStreamSourceFreq below.
        setInstancedStreamSource(instanceCount);
    }
    else
    {
        batchType = PrimitiveBatch::DP_Single;
        formatIndex = 0;
        setLinearStreamSource(batchType);
    }

    pDevice->SetIndices(pmesh->pIndexBuffer->GetHWBuffer());

    VSConstantsUpdateBuffer vsConstants(this,&Profiler);

    for (unsigned fillIndex = 0; fillIndex < fillCount; fillIndex++)
    {
        const FillRecord& fr = fillRecords[fillIndex];

        Profiler.SetBatch((unsigned)complexMesh, fillIndex);

        // Apply fill.
        if (!SManager.ApplyPrimitiveFill(fr.pFill) ||
            !SManager.ApplyPrimitiveBatchFill(fr.pFill->GetType(), FillFlags, fr.pFormats[formatIndex], batchType))
            continue;

        // Modify stream source only when VertexSize changes.
        if (fr.pFormats[formatIndex]->Size != vertexSize)
            {
            vertexSize      = fr.pFormats[formatIndex]->Size;
            vertexBaseIndex = 0;
            pDevice->SetStreamSource(0, pmesh->pVertexBuffer->GetHWBuffer(),
                                     (UINT)(fr.VertexByteOffset + pmesh->VBAllocOffset),
                                     vertexSize);
            }
        else
        {
            // Identical size vertex meshes should be allocated consecutively,
            // as this is needed for SetStreamSource optimization to work.
            SF_ASSERT(fr.VertexByteOffset == (fillRecords[fillIndex-1].VertexByteOffset +
                      vertexSize * fillRecords[fillIndex-1].VertexCount));
        }

        UByte textureCount = fr.pFill->GetTextureCount();

        // If number of instances doesn't fit one buffer, we must iterate in blocks,
        // updating instance count every time.
        unsigned instanceLeft = instanceCount;
        unsigned instanceIndex, drawCount;

        for (instanceIndex = 0; instanceLeft > 0;
             instanceIndex += drawCount, instanceLeft-= drawCount)
        {
            SF_ASSERT( Cache.GetParams().MaxBatchInstances <= VSConstantsUpdateBuffer::MaxInstances );
            drawCount = batchType == PrimitiveBatch::DP_Instanced ? instanceLeft : 1;
            if (drawCount > Cache.GetParams().MaxBatchInstances)
            {
                drawCount = Cache.GetParams().MaxBatchInstances;
            }

            // If we require multiple loops, we can't optimize by only setting the instance data once.
            if ( instanceIndex != 0 )
                prevTextureCount = 0xFF;

            // Optimization: Instance matrices/cxform in buffer don't need to
            // be updated more then once if textureCount hasn't changed.
            if ( textureCount != prevTextureCount )
            {
                if ( has3D )
                {
                    vsConstants.SetInstanceData<Matrix4F>(matrices, instanceIndex,
                        drawCount, textureCount,
                        complexMesh->GetVertexMatrix());
                }
                else
                {
                    vsConstants.SetInstanceData<Matrix2F>(matrices, instanceIndex,
                        drawCount, textureCount,
                        complexMesh->GetVertexMatrix());
                }

                if (textureCount)
                {
                    vsConstants.SetTextureData(complexMesh->GetFillMatrixCache(),
                                               fr.FillMatrixIndex, textureCount);
                }

                prevTextureCount = textureCount;
                vsConstants.ApplyAll();
            }
            else if ( textureCount )
            {
                vsConstants.SetTextureData(complexMesh->GetFillMatrixCache(),
                    fr.FillMatrixIndex, textureCount);
                vsConstants.ApplyTextureData();
            }

            if ( batchType == PrimitiveBatch::DP_Instanced )
                pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | drawCount);

            pDevice->DrawIndexedPrimitive(
                D3DPT_TRIANGLELIST, (INT)vertexBaseIndex, 0, fr.VertexCount,
                fr.IndexOffset + indexBufferOffset, fr.IndexCount / 3 );
            
            AccumulatedStats.Triangles += (fr.IndexCount / 3) * drawCount;
            AccumulatedStats.Meshes += drawCount;
            AccumulatedStats.Primitives++;
        }

        vertexBaseIndex += fr.VertexCount;
    } // for (fill record)

    pmesh->MoveToCacheListFront(MCL_ThisFrame);
}

template< class MatrixUpdateAdapter >
void HAL::applyMatrixConstants(const MatrixUpdateAdapter & input )
{
    const unsigned count           = input.GetCount();
    const unsigned ept             = input.GetElementsPerTransform();
    const unsigned matricesPerMesh = input.GetMatricesPerMesh();
    float          rows[SF_RENDER_D3D9_INSTANCE_DATAROWS][4];

    unsigned i,j,k;
    for (i = 0, j = 0; i < count; i++, j+=matricesPerMesh)
    {
        const HMatrix &hm           = input.GetHMatrix(i);
        const Matrix2F vertexMatrix = input.GetVertexMatrix(i);

        k = i * (2*matricesPerMesh + ept);
        calculateTransform<MatrixUpdateAdapter::MatrixType>(vertexMatrix, hm, Matrices, &rows[k]);
        Profiler.GetCxform(hm.GetCxform()).GetAsFloat2x4(&rows[k+ept]);

        if (matricesPerMesh > 1)
        {
            // TBD: Do texture coordinates need to be adjusted
            //      by the VertexMatrix? What else?
            // Old renderer just passed the values through...
            Matrix  tm0(vertexMatrix, hm.GetTextureMatrix(0));
            tm0.GetAsFloat2x4(&rows[k+ept+2]);

            if (matricesPerMesh > 2)
            {
                Matrix tm1(vertexMatrix, hm.GetTextureMatrix(1));
                tm1.GetAsFloat2x4(&rows[k+ept+4]);
            }
        }
    }

    const unsigned matrixConstBase = 1;
    pDevice->SetVertexShaderConstantF(matrixConstBase + 0,
        &rows[0][0], (ept + 2*matricesPerMesh) * count);
}

// Explicit instantiation for 2D matrices.
template<>
void HAL::calculateTransform<Matrix2F>(const Matrix & m, const HMatrix& hm, const MatrixState& mstate, float (* dest)[4])
{
    // Get as 2D, and just set the remainder to identity.
    Matrix4F mtx(Matrix2F(m, hm.GetMatrix2D(), mstate.UserView ));
    mtx.GetAsFloat4x4(dest);
}

// Explicit instantiation for 4D matrices.
template<>
void HAL::calculateTransform<Matrix4F>(const Matrix & m, const HMatrix& hm, const MatrixState & mstate, float (* dest)[4])
{
    Matrix4F m4d(Matrix4F(mstate.GetUVP(), hm.GetMatrix3D()), m);
    m4d.GetAsFloat4x4(dest);
}

void HAL::applyRawMatrixConstants(const Matrix& m, const Cxform& cx)
{
    float   rows[8][4];
    Matrix4F  mat(Matrix2F(m, Matrices.UserView));

    mat.GetAsFloat4x4(&rows[0]);
    cx.GetAsFloat2x4(&rows[4]);

    const unsigned matrixConstBase = 1;
    pDevice->SetVertexShaderConstantF(matrixConstBase + 0, &rows[0][0], 6);
}

void HAL::applyRawMatrixConstants(const Matrix& m, const Cxform& cx, const Matrix& tm)
{
    float   rows[8][4];
    Matrix4F  mat(Matrix2F(m, Matrices.UserView));

    mat.GetAsFloat4x4(&rows[0]);
    cx.GetAsFloat2x4(&rows[4]);
    tm.GetAsFloat2x4(&rows[6]);

    const unsigned matrixConstBase = 1;
    pDevice->SetVertexShaderConstantF(matrixConstBase + 0, &rows[0][0], 8);
}


//--------------------------------------------------------------------
// *** Mask / Stencil support
//--------------------------------------------------------------------

// Mask support is implemented as a stack, enabling for a number of optimizations:
//
// 1. Large "Clipped" marks are clipped to a custom viewport, allowing to save on
//    fill-rate when rendering both the mask and its content. The mask area threshold
//    that triggers this behavior is determined externally.
//      - Clipped masks can be nested, but not batched. When erased, clipped masks
//        clear the clipped intersection area.
// 2. Small masks can be Batched, having multiple mask areas with multiple mask
//    content items inside.
//      - Small masks can contain clipped masks either regular or clipped masks.
// 3. Mask area dimensions are provided as HMatrix, which maps a unit rectangle {0,0,1,1}
//    to a mask bounding rectangle. This rectangle can be rotated (non-axis aligned),
//    allowing for more efficient fill.
// 4. PopMask stack optimization is implemented that does not erase nested masks; 
//    Stencil Reference value is changed instead. Erase of a mask only becomes
//    necessary if another PushMask_BeginSubmit is called, in which case previous
//    mask bounding rectangles are erased. This setup avoids often unnecessary erase 
//    operations when drawing content following a nested mask.
//      - To implement this MaskStack keeps a previous "stale" MaskPrimitive
//        located above the MaskStackTop.

void HAL::PushMask_BeginSubmit(MaskPrimitive* prim)
{
    MaskMarker.Begin(__FUNCTIONW__);

    if (!checkState(HS_InDisplay, __FUNCTION__))
        return;

    Profiler.SetDrawMode(1);

    if (!StencilAvailable && !DepthBufferAvailable)
    {
        if (!checkMaskBufferCaps())
            return;
    }
    
    // These states are applicable to most stencil operations below.
    if (StencilAvailable)
    {
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
        pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_STENCILMASK, 0xFF);
        pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
        pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    }

    bool viewportValid = (HALState & HS_ViewValid) != 0;

    // Erase previous mask if it existed above our current stack top.
    if (MaskStackTop && (MaskStack.GetSize() > MaskStackTop) && viewportValid)
    {
        // Erase rectangles of these matrices; must be done even for clipped masks.
        if (StencilAvailable && (MultiBitStencil && (StencilOpInc != D3DSTENCILOP_REPLACE)))
        {
            // Any stencil of value greater then MaskStackTop should be set to it;
            // i.e. replace when (MaskStackTop < stencil value).
            DWORD maxStencilValue = (DWORD)MaskStackTop;
            pDevice->SetRenderState(D3DRS_STENCILREF, maxStencilValue);
            pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

            MaskPrimitive* erasePrim = MaskStack[MaskStackTop].pPrimitive;
            drawMaskClearRectangles(erasePrim->GetMaskAreaMatrices(), erasePrim->GetMaskCount());
        }
    }

    MaskStack.Resize(MaskStackTop+1);
    MaskStackEntry &e = MaskStack[MaskStackTop];
    e.pPrimitive       = prim;
    e.OldViewportValid = viewportValid;
    e.OldViewRect      = ViewRect; // TBD: Must assign
    MaskStackTop++;

    HALState |= HS_DrawingMask;

    if (prim->IsClipped() && viewportValid)
    {
        Rect<int> boundClip;

        // Apply new viewport clipping.
        if (!Matrices.OrientationSet)
        {
            const Matrix2F& m = prim->GetMaskAreaMatrix(0).GetMatrix2D();

            // Clipped matrices are always in View coordinate space, to allow
            // matrix to be use for erase operation above. This means that we don't
            // have to do an EncloseTransform.
            SF_ASSERT((m.Shx() == 0.0f) && (m.Shy() == 0.0f));
            boundClip = Rect<int>(VP.Left + (int)m.Tx(), VP.Top + (int)m.Ty(),
                                  VP.Left + (int)(m.Tx() + m.Sx()), VP.Top + (int)(m.Ty() + m.Sy()));
        }
        else
        {
            Matrix2F m = prim->GetMaskAreaMatrix(0).GetMatrix2D();
            m.Append(Matrices.Orient2D);

            RectF rect = m.EncloseTransform(RectF(0,0,1,1));
            boundClip = Rect<int>(VP.Left + (int)rect.x1, VP.Top + (int)rect.y1,
                                  VP.Left + (int)rect.x2, VP.Top + (int)rect.y2);
        }
        
        if (!ViewRect.IntersectRect(&ViewRect, boundClip))
        {
            ViewRect.Clear();
            HALState &= ~HS_ViewValid;
            viewportValid = false;
        }
        updateViewport();
        
        // Clear full viewport area, which has been resized to our smaller bounds.
        if ((MaskStackTop == 1) && viewportValid)
        {
            if (StencilAvailable)
                pDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 0.0f, 0);
            else if (DepthBufferAvailable)
                pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0); // Z -> 1.0f
        }
    }

    else if ((MaskStackTop == 1) && viewportValid)
    {
        // Clear view rectangles.
        if (StencilAvailable)
        {
            // Unconditionally overwrite stencil rectangles with REF value of 0.
            pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
            pDevice->SetRenderState(D3DRS_STENCILREF, 0);
            pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

            drawMaskClearRectangles(prim->GetMaskAreaMatrices(), prim->GetMaskCount());
            pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
        }
        else
        {
            // Depth clears bounds. Don't use drawMaskClearRectangles yet as it doesn't
            // set proper Z.
            UPInt maskCount = prim->GetMaskCount();
            for (UPInt i = 0; i< maskCount; i++)
            {
                const Matrix2F &m = prim->GetMaskAreaMatrix(i).GetMatrix2D();
                RectF           bounds(m.EncloseTransform(RectF(1.0f)));
                Rect<int>       boundClip((int)bounds.x1, (int)bounds.y1,
                                          (int)bounds.x2, (int)bounds.y2);
                boundClip.Offset(VP.Left, VP.Top);

                if (boundClip.IntersectRect(&boundClip, ViewRect))
                {
                    D3DRECT r = { boundClip.x1, boundClip.y1, boundClip.x2, boundClip.y2 };
                    pDevice->Clear(1, &r, D3DCLEAR_ZBUFFER, 0, 1.0f, 0); // Z -> 1.0f
                }
            }
        }
    }


    if (StencilAvailable)
    {
        bool canIncDec = (MultiBitStencil && (StencilOpInc != D3DSTENCILOP_REPLACE));

        // Increment only if we support it.
        if (canIncDec)
        {
            pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)(MaskStackTop-1));
            pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
            pDevice->SetRenderState(D3DRS_STENCILPASS, StencilOpInc);
        }
        else if (MaskStackTop == 1)
        {
            pDevice->SetRenderState(D3DRS_STENCILREF, 1);
            pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
        }
        else
        {   // If not supported, no change.
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
        }
    }
    else if (DepthBufferAvailable)
    {
        if (MaskStackTop == 1)
        {
            // Set the correct render states in order to not modify the color buffer
            // but write the default Z-value everywhere. According to the shader code: should be 0.
            pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
            pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
            pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
            pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
        }
        else
        {
            // No color write. Incr/Decr not supported.
            pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
            pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
            pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        }
    }
    ++AccumulatedStats.Masks;
}


void HAL::EndMaskSubmit()
{
    Profiler.SetDrawMode(0);

    if (!checkState(HS_InDisplay|HS_DrawingMask, __FUNCTION__))
        return;
    HALState &= ~HS_DrawingMask;    
    SF_ASSERT(MaskStackTop);

    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);
    
    if (StencilAvailable)
    {
        // We draw only where the (MaskStackTop <= stencil), i.e. where the latest mask was drawn.
        // However, we don't change the stencil buffer
        pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
        pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP); 
        // Stencil counter.
        pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)MaskStackTop);
        pDevice->SetRenderState(D3DRS_STENCILMASK, 0xFF);
    }
    else if (DepthBufferAvailable)
    {
        // Disable the Z-write and write only where the mask had written
        pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
    }
    MaskMarker.End();
}


void HAL::PopMask()
{
    PixMarker event(__FUNCTIONW__);

    if (!checkState(HS_InDisplay, __FUNCTION__))
        return;

    if (!StencilAvailable && !DepthBufferAvailable)
        return;
    SF_ASSERT(MaskStackTop);
    MaskStackTop--;

    if (MaskStack[MaskStackTop].pPrimitive->IsClipped())
    {
        // Restore viewport
        ViewRect = MaskStack[MaskStackTop].OldViewRect;

        if (MaskStack[MaskStackTop].OldViewportValid)
            HALState |= HS_ViewValid;
        else
            HALState &= ~HS_ViewValid;
        updateViewport();
    }

    // Disable mask or decrement stencil reference value.
    if (StencilAvailable)
    {
        if (MaskStackTop == 0)
            pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        else
        {
            // Change ref value down, so that we can draw using previous mask.
            pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)MaskStackTop);
            pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
        }
    }
    else if (DepthBufferAvailable)
    {
        // Disable the Z-write and write only where the mask had written
        pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    }
}


bool HAL::checkMaskBufferCaps()
{
    if (!StencilChecked)
    {
        // Test for depth-stencil presence.
        IDirect3DSurfaceX *pdepthStencilSurface = 0;
        pDevice->GetDepthStencilSurface(&pdepthStencilSurface);
        if (pdepthStencilSurface)
        {
            D3DSURFACE_DESC sd;
            pdepthStencilSurface->GetDesc(&sd);

            switch(sd.Format)
            {
            case D3DFMT_D24S8:
            case D3DFMT_D24X4S4:
//#if (SF_D3D_VERSION == 9)
            case D3DFMT_D24FS8:
//#endif
                MultiBitStencil = 1;
            case D3DFMT_D15S1:
                StencilAvailable = 1;
                break;
            }

            pdepthStencilSurface->Release();
            pdepthStencilSurface = 0;
            DepthBufferAvailable = 1;
        }
        else
            StencilAvailable = 0;

        StencilChecked = 1;
    }

    if (!StencilAvailable && !DepthBufferAvailable)
    {
#ifdef SF_BUILD_DEBUG
        static bool StencilWarned = 0;
        if (!StencilWarned)
        {
            SF_DEBUG_WARNING(1, "RendererHAL::PushMask_BeginSubmit used, but stencil is not available");
            StencilWarned = 1;
        }
#endif
        return false;
    }
    return true;
}


void HAL::drawMaskClearRectangles(const HMatrix* matrices, UPInt count)
{
    PixMarker event(__FUNCTIONW__);
    if (!SManager.ApplyClearMaskFill())
    {
        return;
    }

    // This operation is used to clear bounds for masks.
    // Potential issue: Since our bounds are exact, right/bottom pixels may not
    // be drawn due to HW fill rules.
    //  - This shouldn't matter if a mask is tessellated within bounds of those
    //    coordinates, since same rules are applied to those render shapes.
    //  - EdgeAA must be turned off for masks, as that would extrude the bounds.

    setLinearStreamSource(PrimitiveBatch::DP_Batch);
    pDevice->SetStreamSource(0, Cache.pMaskEraseBatchVertexBuffer.GetPtr(),
                             0, sizeof(VertexXY16iAlpha));

    // Draw the object with cached mesh.
    unsigned drawRangeCount = 0;
    for (UPInt i = 0; i < count; i+= (UPInt)drawRangeCount)
    {
        drawRangeCount = Alg::Min<unsigned>((unsigned)count, MeshCache::MaxEraseBatchCount);
        StrideArray<const HMatrix> argMatrices(matrices + i, drawRangeCount, sizeof(HMatrix));
        applyMatrixConstants(MatrixUpdateAdapter_Matrices<Matrix2F>(argMatrices, 1, Matrix2F::Identity));
        pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (UINT)drawRangeCount * 2);

        AccumulatedStats.Meshes += drawRangeCount;
        AccumulatedStats.Triangles += drawRangeCount * 2;
        AccumulatedStats.Primitives++;
    }
}

//--------------------------------------------------------------------
// Background clear helper, expects viewport coordinates.
void HAL::clearSolidRectangle(const Rect<int>& r, Color color)
{
    color = Profiler.GetClearColor(color);

    PixMarker event(__FUNCTIONW__);
    if (color.GetAlpha() == 0xFF)
    {
        // Do more efficient HW clear. Device::Clear expects render-target coordinates.
        D3DRECT  cr = { r.x1 + VP.Left, r.y1 + VP.Top,
                        r.x2 + VP.Left, r.y2 + VP.Top };
        D3DCOLOR d3dc = D3DCOLOR_XRGB(color.GetRed(), color.GetGreen(), color.GetBlue());
        pDevice->Clear(1, &cr, D3DCLEAR_TARGET, d3dc, 1.0f,  0);
    }
    else
    {
        if (!SManager.ApplySolidColorFill(color))
            return;

        Matrix2F m((float)r.Width(), 0.0f, (float)r.x1,
                   0.0f, (float)r.Height(), (float)r.y1);
        applyRawMatrixConstants(m, Cxform::Identity);
        setLinearStreamSource(PrimitiveBatch::DP_Batch);
        pDevice->SetStreamSource(0, Cache.pMaskEraseBatchVertexBuffer.GetPtr(),
                                 0, sizeof(VertexXY16iAlpha));

        pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    }
}



//--------------------------------------------------------------------
// *** BlendMode Stack support
//--------------------------------------------------------------------

// Structure describing color combines applied for a given blend mode.
struct BlendModeDesc
{
    D3DBLENDOP  BlendOp;
    D3DBLEND    SrcArg, DestArg;
};

struct BlendModeDescAlpha
{
    D3DBLENDOP  BlendOp;
    D3DBLEND    SrcArg, DestArg;
    D3DBLEND    SrcAlphaArg, DestAlphaArg;
};

void HAL::applyBlendMode(BlendMode mode, bool sourceAc, bool forceAc )
{    
    PixMarker event(__FUNCTIONW__);
    static BlendModeDesc modes[15] =
    {
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // None
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // Normal
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // Layer

        { D3DBLENDOP_ADD, D3DBLEND_DESTCOLOR, D3DBLEND_ZERO },          // Multiply
        // (For multiply, should src be pre-multiplied by its inverse alpha?)

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // Screen *??

        { D3DBLENDOP_MAX, D3DBLEND_SRCALPHA, D3DBLEND_ONE },            // Lighten
        { D3DBLENDOP_MIN, D3DBLEND_SRCALPHA, D3DBLEND_ONE },                    // Darken

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // Difference *??

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_ONE },            // Add
        { D3DBLENDOP_REVSUBTRACT, D3DBLEND_SRCALPHA, D3DBLEND_ONE },    // Subtract

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // Invert *??

        { D3DBLENDOP_ADD, D3DBLEND_ZERO, D3DBLEND_ONE },                // Alpha *??
        { D3DBLENDOP_ADD, D3DBLEND_ZERO, D3DBLEND_ONE },                // Erase *??  What color do we erase to?
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // Overlay *??
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },    // HardLight *??
    };

    static BlendModeDescAlpha acmodes[15] =
    {
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // None
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // Normal
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // Layer

        { D3DBLENDOP_ADD, D3DBLEND_DESTCOLOR, D3DBLEND_ZERO, D3DBLEND_DESTCOLOR, D3DBLEND_ZERO },      // Multiply
        // (For multiply, should src be pre-multiplied by its inverse alpha?)

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // Screen *??

        { D3DBLENDOP_MAX, D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLEND_SRCALPHA, D3DBLEND_ONE },          // Lighten
        { D3DBLENDOP_MIN, D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLEND_SRCALPHA, D3DBLEND_ONE },          // Darken

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // Difference *??

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLEND_ZERO, D3DBLEND_ONE },                   // Add
        { D3DBLENDOP_REVSUBTRACT, D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLEND_ZERO, D3DBLEND_ONE },           // Subtract

        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // Invert *??

        { D3DBLENDOP_ADD, D3DBLEND_ZERO, D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_ONE               },    // Alpha *??
        { D3DBLENDOP_ADD, D3DBLEND_ZERO, D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_ONE               },    // Erase *??
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },         // Overlay *??
        { D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },    // Hardlight *??
    };

    if (!pDevice)
        return;

    // For debug build
    SF_ASSERT(((unsigned) mode) < 15);
    // For release
    if (((unsigned) mode) >= 15)
        mode = Blend_None;

    mode = Profiler.GetBlendMode(mode);

    // Multiply requires different fill mode, save it in the HAL's fill flags.
    if ( mode == Blend_Multiply || mode == Blend_Darken )
        FillFlags |= FF_Multiply;
    else
        FillFlags &= ~FF_Multiply;

    if (VP.Flags & Viewport::View_AlphaComposite || forceAc)
    {
        pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
        pDevice->SetRenderState(D3DRS_BLENDOP, acmodes[mode].BlendOp);
        pDevice->SetRenderState(D3DRS_BLENDOPALPHA, acmodes[mode].BlendOp);
        if ( sourceAc && acmodes[mode].SrcArg == D3DBLEND_SRCALPHA )
            pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        else
            pDevice->SetRenderState(D3DRS_SRCBLEND, acmodes[mode].SrcArg);        
        pDevice->SetRenderState(D3DRS_DESTBLEND, acmodes[mode].DestArg);
        pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, acmodes[mode].SrcAlphaArg);
        pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, acmodes[mode].DestAlphaArg);
    }
    else
    {
        pDevice->SetRenderState(D3DRS_BLENDOP, modes[mode].BlendOp);
        pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
        if ( sourceAc && acmodes[mode].SrcArg == D3DBLEND_SRCALPHA )
            pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        else
            pDevice->SetRenderState(D3DRS_SRCBLEND, acmodes[mode].SrcArg);        
        pDevice->SetRenderState(D3DRS_DESTBLEND, modes[mode].DestArg);
    }
}

RenderTarget* HAL::CreateRenderTarget(IDirect3DSurface9* pcolor, IDirect3DSurface9* pdepth)
{
    D3DSURFACE_DESC sdesc;
    ImageSize dsSize(0), rtSize(0);

    if (SUCCEEDED(pcolor->GetDesc(&sdesc)))
        rtSize.SetSize(sdesc.Width, sdesc.Height);

    Ptr<RenderTarget> ptarget = pRenderBufferManager->CreateRenderTarget(rtSize, RBuffer_User, Image_R8G8B8A8, 0);

    Ptr<DepthStencilBuffer> pdsb = 0;
    if (SUCCEEDED(pdepth->GetDesc(&sdesc)))
    {
        dsSize.SetSize(sdesc.Width, sdesc.Height);
        pdsb = *SF_HEAP_AUTO_NEW(this) DepthStencilBuffer(0, dsSize);
    }

    HALData::UpdateData(ptarget, pcolor, pdsb, pdepth);
    return ptarget;
}

RenderTarget* HAL::CreateRenderTarget(Render::Texture* texture, bool needsStencil)
{
    D3D9::Texture* pt = (D3D9::Texture*)texture;

    // Cannot render to textures which have multiple HW representations.
    if ( !pt || pt->TextureCount != 1 )
        return 0;

    IDirect3DSurface9* prenderSurface = 0, *pdsSurface = 0;
    RenderTarget* prt = pRenderBufferManager->CreateRenderTarget(
        texture->GetSize(), RBuffer_Texture, texture->GetFormat(), texture);
    if ( !prt )
        return 0;
    pt->pTextures[0].pTexture->GetSurfaceLevel(0,&prenderSurface);

    Ptr<DepthStencilBuffer> pdsb;
    if ( needsStencil )
    {
        pdsb = *pRenderBufferManager->CreateDepthStencilBuffer(texture->GetSize());
        if ( pdsb )
        {
            DepthStencilSurface* surf = (D3D9::DepthStencilSurface*)pdsb->GetSurface();
            if ( surf )
                pdsSurface = surf->pDepthStencilSurface;
        }
    }
    HALData::UpdateData(prt, prenderSurface, pdsb, pdsSurface );
    prenderSurface->Release();

    return prt;
}

RenderTarget* HAL::CreateTempRenderTarget(const ImageSize& size, bool needsStencil)
{
    RenderTarget* prt = pRenderBufferManager->CreateTempRenderTarget(size);
    if ( !prt )
        return 0;

    HALData* phd = (HALData*)prt->GetHALData();
    if ( phd && (!needsStencil || phd->pDSSurface))
        return prt;

    Ptr<IDirect3DSurface9> prenderSurface = 0;
    IDirect3DSurface9* pdsSurface = 0;
    D3D9::Texture* pt = (D3D9::Texture*)prt->GetTexture();

    // Cannot render to textures which have multiple HW representations.
    SF_ASSERT(pt->TextureCount == 1); 
    pt->pTextures[0].pTexture->GetSurfaceLevel(0,&prenderSurface.GetRawRef());

    Ptr<DepthStencilBuffer> pdsb;
    if ( needsStencil )
    {
        pdsb = *pRenderBufferManager->CreateDepthStencilBuffer(size);
        if ( pdsb )
        {
            DepthStencilSurface* surf = (D3D9::DepthStencilSurface*)pdsb->GetSurface();
            if ( surf )
                pdsSurface = surf->pDepthStencilSurface;
        }
    }

    HALData::UpdateData(prt, prenderSurface, pdsb, pdsSurface);
    return prt;
}

bool HAL::SetRenderTarget(RenderTarget* ptarget, bool setState)
{
    // Cannot set the bottom level render target if already in display.
    if ( HALState & HS_InDisplay )
        return false;

    // When changing the render target while in a scene, we must flush all drawing.
    if ( HALState & HS_InScene)
        Flush();

    D3DSURFACE_DESC rtDesc;
    RenderTargetEntry entry;
    HALData* prtdata = (D3D9::HALData*)ptarget->GetHALData();
    IDirect3DSurface9* pd3dsurface= prtdata->pRenderSurface;
    if ( FAILED(pd3dsurface->GetDesc(&rtDesc)) )
        return false;

    if ( setState )
    {
        pDevice->SetRenderTarget(0, pd3dsurface);
        if ( prtdata->pDSSurface )
            pDevice->SetDepthStencilSurface(prtdata->pDSSurface);
        ++AccumulatedStats.RTChanges;
    }
    entry.pRenderTarget = ptarget;

    // Replace the stack entry at the bottom, or if the stack is empty, add one.
    if ( RenderTargetStack.GetSize() > 0 )
        RenderTargetStack[0] = entry;
    else
        RenderTargetStack.PushBack(entry);
    return true;
}

void HAL::PushRenderTarget(const RectF& frameRect, RenderTarget* prt)
{
    RenderTargetMarker.Begin(__FUNCTIONW__);

    HALState |= HS_InRenderTarget;
    RenderTargetEntry entry = {prt, Matrices, ViewRect, VP};
    Matrices.SetUserMatrix(Matrix2F::Identity);

    // Setup the render target/depth stencil on the device.
    if ( !prt )
    {
        SF_DEBUG_WARNING(1, __FUNCTION__ " - invalid render target.");
        RenderTargetStack.PushBack(entry);
        return;
    }

    HALData* phd = (D3D9::HALData*)prt->GetHALData();
    pDevice->SetRenderTarget(0, phd->pRenderSurface);
    if ( phd->pDSSurface )
        pDevice->SetDepthStencilSurface(phd->pDSSurface);

    ++AccumulatedStats.RTChanges;

    pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f,  0);

    // Setup viewport.
    Rect<int> viewRect = prt->GetRect(); // On the render texture, might not be the entire surface.
    const ImageSize& bs = prt->GetBufferSize();
    VP = Viewport(bs.Width, bs.Height, viewRect.x1, viewRect.y1, viewRect.Width(), viewRect.Height());    
    ViewRect.x1 = (int)frameRect.x1;
    ViewRect.y1 = (int)frameRect.y1;
    ViewRect.x2 = (int)frameRect.x2;
    ViewRect.y2 = (int)frameRect.y2;

    // Must offset the 'original' viewrect, otherwise the 3D compensation matrix will be offset.
    Matrices.ViewRectOriginal.Offset(-entry.OldViewport.Left, -entry.OldViewport.Top);
    Matrices.UVPOChanged = true;
    HALState |= HS_ViewValid;
    updateViewport();

    RenderTargetStack.PushBack(entry);
}

void HAL::PopRenderTarget()
{
    RenderTargetEntry& entry = RenderTargetStack.Back();
    RenderTarget* prt = entry.pRenderTarget;
    prt->SetInUse(false);
    if ( prt->GetType() == RBuffer_Temporary )
    {
        // Strip off the depth stencil surface/buffer from temporary targets.
        D3D9::HALData* plasthd = (D3D9::HALData*)prt->GetHALData();
        if ( plasthd->pDSSurface )
        {
            plasthd->pDSSurface->Release();
            plasthd->pDSSurface = 0;
        }
        plasthd->pDepthStencilBuffer = 0;
    }

    Matrices = entry.OldMatrixState;
    ViewRect = entry.OldViewRect;
    VP = entry.OldViewport;

    // Must reverse the offset of the 'original' viewrect.
    Matrices.ViewRectOriginal.Offset(entry.OldViewport.Left, entry.OldViewport.Top);
    Matrices.UVPOChanged = true;

    RenderTargetStack.PopBack();
    HALData* phd = 0;
    if ( RenderTargetStack.GetSize() > 0 )
    {
        RenderTargetEntry& back = RenderTargetStack.Back();
        phd = (D3D9::HALData*)back.pRenderTarget->GetHALData();
    }

    if ( RenderTargetStack.GetSize() == 1 )
        HALState &= ~HS_InRenderTarget;

    // Restore the old render target on the device.
    pDevice->SetRenderTarget(0, phd->pRenderSurface);
    if ( phd->pDSSurface )
        pDevice->SetDepthStencilSurface(phd->pDSSurface);
    else
        pDevice->SetDepthStencilSurface(0);

    ++AccumulatedStats.RTChanges;

    // Reset the viewport to the last render target on the stack.
    HALState |= HS_ViewValid;
    updateViewport();

    RenderTargetMarker.End();
}

bool HAL::createDefaultRenderBuffer()
{
    ImageSize rtSize;

    if ( GetDefaultRenderTarget() )
    {
        RenderTarget* prt = GetDefaultRenderTarget();
        rtSize = prt->GetSize();
    }
    else
    {
        IDirect3DSurface9* prenderTarget;
        IDirect3DSurface9* pdsTarget;
        D3DSURFACE_DESC rtDesc;
        if (FAILED(pDevice->GetRenderTarget(0, &prenderTarget)) ||             
            FAILED(prenderTarget->GetDesc(&rtDesc)) )
        {
            return false;
        }
        rtSize = ImageSize(rtDesc.Width, rtDesc.Height);

        Ptr<DepthStencilBuffer> pdsb = 0;
        D3DSURFACE_DESC dsDesc;
        ImageSize dsSize(0);
        if ( SUCCEEDED(pDevice->GetDepthStencilSurface(&pdsTarget)) &&
            SUCCEEDED(pdsTarget->GetDesc(&dsDesc)))
        {
            dsSize.SetSize(dsDesc.Width, dsDesc.Height);
            pdsb = *SF_HEAP_AUTO_NEW(this) DepthStencilBuffer(0, dsSize);
        }
        Ptr<RenderTarget> ptarget = *SF_HEAP_AUTO_NEW(this) RenderTarget(0, RBuffer_Default, rtSize );
        HALData::UpdateData(ptarget, prenderTarget, pdsb, pdsTarget );

        if ( pdsTarget )
            pdsTarget->Release();
        prenderTarget->Release();

        if ( !SetRenderTarget(ptarget))
            return false;
    }

    return pRenderBufferManager->Initialize(pTextureManager, Image_R8G8B8A8, rtSize);
}

void HAL::PushFilters(FilterPrimitive* prim)
{
    if (!checkState(HS_InDisplay, __FUNCTION__))
        return;
    if ( !FSManager.HasFilterSupport() )
        return;

    FilterStackEntry e = {prim, 0};

    // Queue the profiler off of whether masks should be draw or not.
    if ( !Profiler.ShouldDrawMask() )
    {
        Profiler.SetDrawMode(2);

        setLinearStreamSource(PrimitiveBatch::DP_Batch);
        pDevice->SetStreamSource(0, Cache.pMaskEraseBatchVertexBuffer.GetPtr(),
            0, sizeof(VertexXY16iAlpha));
        applyRawMatrixConstants(prim->GetFilterAreaMatrix().GetMatrix2D(), Cxform::Identity);
        SManager.ApplySolidColorFill(0xFFFFFFFF);
        pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);      
        FilterStack.PushBack(e);
        return;
    }

    if ( (HALState & HS_CachedFilter) )
    {
        FilterStack.PushBack(e);
        return;
    }

    // Disable masking from previous target, if this filter primitive doesn't have any masking.
    if ( MaskStackTop != 0 && !prim->GetMaskPresent() && prim->GetCacheState() != FilterPrimitive::Cache_Target )
    {
        if ( StencilAvailable )
            pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        else if ( DepthBufferAvailable )
            pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);
    }

    FilterMarker.Begin(__FUNCTIONW__);
    HALState |= HS_DrawingFilter;

    if ( prim->GetCacheState() ==  FilterPrimitive::Cache_Uncached )
    {
        // Draw the filter from scratch.
        const Matrix2F& m = e.pPrimitive->GetFilterAreaMatrix().GetMatrix2D();
        e.pRenderTarget = *CreateTempRenderTarget(ImageSize((UInt32)m.Sx(), (UInt32)m.Sy()), prim->GetMaskPresent());
        RectF frameRect(m.Tx(), m.Ty(), m.Tx() + m.Sx(), m.Ty() + m.Sy());
        PushRenderTarget(frameRect, e.pRenderTarget);
        applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, false, true);

        // If this primitive has masking, then clear the entire area to the current mask level, because 
        // the depth stencil target may be different, and thus does not contain the previously written values.
        if ( prim->GetMaskPresent())
        {
            if (StencilAvailable)
                pDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 0.0f, (DWORD)MaskStackTop);
            else if (DepthBufferAvailable)
                pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0); // Z -> 1.0f
        }
    }
    else
    {
        // Drawing a cached filter, ignore all draw calls until the corresponding PopFilters.
        // Keep track of the level at which we need to draw the cached filter, by adding entries to the stack.
        HALState |= HS_CachedFilter;
        CachedFilterIndex = (int)FilterStack.GetSize();
		GetRQProcessor().SetQueueEmitFilter(RenderQueueProcessor::QPF_Filters);
    }
    FilterStack.PushBack(e);
}

void HAL::PopFilters()
{
    if ( !FSManager.HasFilterSupport() )
        return;

    ShaderData.BeginScene(); // Uncache FS/VS/VD.
    SManager.BeginScene(); 

    Render::HAL::PopFilters();

    ShaderData.BeginScene(); // Uncache FS/VS/VD.
    SManager.BeginScene(); 
    FilterMarker.End();
}

void HAL::drawUncachedFilter(const FilterStackEntry& e)
{
    const FilterSet* filters = e.pPrimitive->GetFilters();
    unsigned filterCount = filters->GetFilterCount();
    const Filter* filter = 0;
    unsigned pass = 0, passes = 0;

    // Invalid primitive or rendertarget.
    if ( !e.pPrimitive || !e.pRenderTarget )
        return;

    // Bind the render target.
    SF_ASSERT(RenderTargetStack.Back().pRenderTarget == e.pRenderTarget);
    const int MaxTemporaryTextures = 3;
    Ptr<RenderTarget> temporaryTextures[MaxTemporaryTextures];
    memset(temporaryTextures, 0, sizeof temporaryTextures);

    // Fill our the source target.
    ImageSize size = e.pRenderTarget->GetSize();
    temporaryTextures[0] = e.pRenderTarget;

    setLinearStreamSource(PrimitiveBatch::DP_Batch);
    pDevice->SetStreamSource(0, Cache.pUVSquareVertexBuffer.GetPtr(),
                             0, sizeof(VertexXY16iUV));

    const VertexFormat* pvf[3];
    FSManager.MapVertexFormat(PrimFill_Texture, &VertexXY16iUV::Format, &pvf[0], &pvf[1], &pvf[2] );

    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);
    // Overlay mode isn't actually supported, it contains the blend mode for filter sub-targets.
    applyBlendMode(Blend_Overlay, true, false);

    // Render filter(s).
    unsigned shaders[FilterShaderManager::MaximumFilterPasses];
    for ( unsigned i = 0; i < filterCount; ++i )
    {
        filter = filters->GetFilter(i);
        passes = FSManager.GetFilterPasses(filter, FillFlags, shaders);

        // All shadows (except those hiding the object) need the original texture.
        bool requireSource = false;
        if ( filter->GetFilterType() >= Filter_Shadow &&
             filter->GetFilterType() <= Filter_Blur_End &&
             !(((BlurFilterImpl*)filter)->GetParams().Mode & BlurFilterParams::Mode_HideObject) )
        {
            temporaryTextures[Target_Original] = temporaryTextures[Target_Source];
            requireSource = true;
        }

        // Now actually render the filter.
        for (pass = 0; pass < passes; ++pass)
        {
            // Render the final pass directly to the target surface.
            if (pass == passes-1 && i == filterCount-1)
                break;

            // Create a destination texture if required.
            if ( !temporaryTextures[1] )
            {
                temporaryTextures[1] = *CreateTempRenderTarget(size, false);
            }

            IDirect3DSurface9* psurface = ((D3D9::HALData*)temporaryTextures[1]->GetHALData())->pRenderSurface;
            pDevice->SetRenderTarget(0, psurface);
            pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f,  0);
            ++AccumulatedStats.RTChanges;
            
            // Scale to the size of the destination.
            RenderTarget* prt = temporaryTextures[1];
            const Rect<int>& viewRect = prt->GetRect(); // On the render texture, might not be the entire surface.
            const ImageSize& bs = prt->GetBufferSize();
            VP = Viewport(bs.Width, bs.Height, viewRect.x1, viewRect.y1, viewRect.Width(), viewRect.Height());    
            ViewRect = Rect<int>(viewRect.x1, viewRect.y1, viewRect.x2, viewRect.y2);
            HALState |= HS_ViewValid;
            updateViewport();

            Matrix2F mvp = Matrix2F::Scaling(2,-2) * Matrix2F::Translation(-0.5f, -0.5f);                          
            mvp.Tx() -= 1.0f/size.Width;   // D3D9 1/2 pixel center offset
            mvp.Ty() += 1.0f/size.Height;

            FSManager.SetFilterFill(mvp, Cxform::Identity, filter, temporaryTextures, shaders, pass, passes, pvf[0], &ShaderData);
            pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
            pDevice->SetTexture(0,0);
            pDevice->SetTexture(1,0);

            // If we require the original source, create a new target for the source.
            if ( requireSource && pass == 0)
                temporaryTextures[0] = *CreateTempRenderTarget(size, false);

            // Setup for the next pass.
            Alg::Swap(temporaryTextures[0], temporaryTextures[1]);
        }

        AccumulatedStats.Primitives += passes;
        AccumulatedStats.Meshes     += passes;
    }

    // Cache the 2nd last step so it might be available as a cached filter next time.
    if ( Profiler.IsFilterCachingEnabled() )
    {
        RenderTarget* cacheResults[2] = { temporaryTextures[0], temporaryTextures[2] };
        e.pPrimitive->SetCacheResults(FilterPrimitive::Cache_PreTarget, cacheResults, 2);
        ((D3D9::HALData*)cacheResults[0]->GetHALData())->CacheID = reinterpret_cast<UPInt>(e.pPrimitive.GetPtr());
        if ( cacheResults[1] )
            ((D3D9::HALData*)cacheResults[1]->GetHALData())->CacheID = reinterpret_cast<UPInt>(e.pPrimitive.GetPtr());
    }

    // Pop the temporary target, begin rendering to the previous surface.
    PopRenderTarget();

    // Re-[en/dis]able masking from previous target, if available.
    if ( MaskStackTop != 0 )
    {
        if ( StencilAvailable )
            pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE );
        else if ( DepthBufferAvailable )
            pDevice->SetRenderState(D3DRS_ZENABLE, TRUE );
    }

    // Now actually draw the filtered sub-scene to the target below.
    const Matrix2F& mvp = Matrices.UserView * e.pPrimitive->GetFilterAreaMatrix().GetMatrix2D();
    const Cxform&   cx  = e.pPrimitive->GetFilterAreaMatrix().GetCxform();
    FSManager.SetFilterFill(mvp, cx, filter, temporaryTextures, shaders, pass, passes, pvf[0], &ShaderData);
    applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, true, true);
    pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, false, (HALState&HS_InRenderTarget) != 0);

    // Re-[en/dis]able masking from previous target, if available.
    if ( HALState & HS_DrawingMask )
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0 );

    // Cleanup.
    for ( unsigned i = 0; i < MaxTemporaryTextures; ++i )
    {
        pDevice->SetTexture(i, 0);
        if (temporaryTextures[i])
            temporaryTextures[i]->SetInUse(false);
    }
    AccumulatedStats.Filters += filters->GetFilterCount();
}

void HAL::drawCachedFilter(FilterPrimitive* primitive)
{
    setLinearStreamSource(PrimitiveBatch::DP_Batch);
    pDevice->SetStreamSource(0, Cache.pUVSquareVertexBuffer.GetPtr(),
                             0, sizeof(VertexXY16iUV));

    const int MaxTemporaryTextures = 3;
    switch(primitive->GetCacheState())
    {
        // We have one-step from final target. Render it to a final target now.
        case FilterPrimitive::Cache_PreTarget:
        {
            const FilterSet* filters = primitive->GetFilters();
            UPInt filterIndex = filters->GetFilterCount()-1;
            const Filter* filter = filters->GetFilter(filterIndex);
            unsigned shaders[FilterShaderManager::MaximumFilterPasses];
            unsigned passes = FSManager.GetFilterPasses(filter, FillFlags, shaders);
            
            // Fill out the temporary textures from the cached results.
            Ptr<RenderTarget> temporaryTextures[MaxTemporaryTextures];
            memset(temporaryTextures, 0, sizeof temporaryTextures);
            RenderTarget* results[2];
            primitive->GetCacheResults(results, 2);
            temporaryTextures[0] = results[0];
            ImageSize size = temporaryTextures[0]->GetSize();
            temporaryTextures[1] = *CreateTempRenderTarget(size, false);
            temporaryTextures[2] = results[1];
            PushRenderTarget(RectF((float)size.Width,(float)size.Height), temporaryTextures[1]);

            // Render to the target.
            const VertexFormat* pvf[3];
            SManager.MapVertexFormat(PrimFill_Texture, &VertexXY16iUV::Format, &pvf[0], &pvf[1], &pvf[2] );
            Matrix2F mvp = Matrix2F::Scaling(2,-2) * Matrix2F::Translation(-0.5f, -0.5f);
            const Cxform & cx = primitive->GetFilterAreaMatrix().GetCxform();
            mvp.Tx() -= 1.0f/size.Width;   // D3D9 1/2 pixel center offset
            mvp.Ty() += 1.0f/size.Height;
            FSManager.SetFilterFill(mvp, cx, filter, temporaryTextures, shaders, passes-1, passes, pvf[0], &ShaderData);
            applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, true, true);
            pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
            pDevice->SetTexture(0,0);
            pDevice->SetTexture(1,0);
            PopRenderTarget();

            // Re-[en/dis]able masking from previous target, if available.
            if ( MaskStackTop != 0 )
            {
                if ( StencilAvailable )
                    pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE );
                else if ( DepthBufferAvailable )
                    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE );
            }

            // Re-[en/dis]able masking from previous target, if available.
            if ( HALState & HS_DrawingMask )
                pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0 );

            // Set this as the final cache result, and then render it.
            RenderTarget* prt = temporaryTextures[1];
            primitive->SetCacheResults(FilterPrimitive::Cache_Target, &prt, 1);
            ((D3D9::HALData*)prt->GetHALData())->CacheID = reinterpret_cast<UPInt>(primitive);
            drawCachedFilter(primitive);

            // Cleanup.
            for ( unsigned i = 0; i < MaxTemporaryTextures; ++i )
            {
                if (temporaryTextures[i])
                    temporaryTextures[i]->SetInUse(false);
            }
            break;
        }

        // We have a final filtered texture. Just apply it to a screen quad.
        case FilterPrimitive::Cache_Target:
        {
            const VertexFormat* pvf[3];
            SManager.MapVertexFormat(PrimFill_Texture, &VertexXY16iUV::Format, &pvf[0], &pvf[1], &pvf[2] );
            SManager.PrimitivePSType = PS_Texture;
            if ( !SManager.ApplyPrimitiveBatchFill(PrimFill_Texture, UInt16((FillFlags & ~FF_Cxform)), pvf[0], PrimitiveBatch::DP_Single) )
                return;

            RenderTarget* results;
            primitive->GetCacheResults(&results, 1);
            Texture* ptexture = (D3D9::Texture*)results->GetTexture();
            ptexture->ApplyTexture(0, ImageFillMode(Wrap_Clamp, Sample_Linear));
            const Matrix2F& mvp = primitive->GetFilterAreaMatrix().GetMatrix2D();
            const Rect<int>& srect = results->GetRect();
            Matrix2F texgen;
            texgen.AppendTranslation((float)srect.x1, (float)srect.y1);
            texgen.AppendScaling((float)srect.Width() / ptexture->GetSize().Width, (float)srect.Height() / ptexture->GetSize().Height);
            applyRawMatrixConstants(mvp, Cxform::Identity, texgen);
            applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, true, true);
            pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
            applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, false, (HALState&HS_InRenderTarget)!=0);
            pDevice->SetTexture(0,0);
            pDevice->SetTexture(1,0);            

            // Cleanup.
            results->SetInUse(false);
            if ( !Profiler.IsFilterCachingEnabled() )
                primitive->SetCacheResults(FilterPrimitive::Cache_Uncached, 0, 0);
            break;
        }

        // Should have been one of the other two caching types.
        default: SF_ASSERT(0); break;
    }
}

}}} // Scaleform::Render::D3D9

