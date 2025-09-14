/**************************************************************************

Filename    :   GL_HAL.cpp
Content     :   GL Renderer HAL Prototype implementation.
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#include "Kernel/SF_Debug.h"
#include "Kernel/SF_Random.h"
#include "Kernel/SF_HeapNew.h"

#include "GL_HAL.h"

namespace Scaleform { namespace Render { namespace GL {


// ***** RenderHAL_GL

HAL::HAL(ThreadCommandQueue* commandQueue)
:   Render::HAL(commandQueue),
    MultiBitStencil(1),
    FillFlags(0),
    EnabledVertexArrays(-1),
    Cache(Memory::GetGlobalHeap(), MeshCacheParams::PC_Defaults),
    SManager(&Profiler),
    ShaderData(GetHAL()),
    PrevBatchType(PrimitiveBatch::DP_None)
{
    Caps = 0;
    memset(StaticShaders, 0, sizeof(StaticShaders));
}

HAL::~HAL()
{
    ShutdownHAL();
}

void   HAL::MapVertexFormat(PrimitiveFillType fill, const VertexFormat* sourceFormat,
                            const VertexFormat** single,
                            const VertexFormat** batch, const VertexFormat** instanced, unsigned)
{
    SManager.MapVertexFormat(fill, sourceFormat, single, batch, instanced, Caps & MVF_Align);
#if defined(GL_ES_VERSION_2_0)
    if (Caps & Cap_NoBatching)
        *batch = 0;
#endif
}

PrimitiveFill*  HAL::CreatePrimitiveFill(const PrimitiveFillData &data)
{
    return SF_HEAP_NEW(pHeap) PrimitiveFill(data);
}

// Draws a range of pre-cached and preprocessed primitives
void        HAL::DrawProcessedPrimitive(Primitive* pprimitive,
                                        PrimitiveBatch* pstart, PrimitiveBatch *pend)
{
    SF_AMP_SCOPE_RENDER_TIMER("HAL::DrawProcessedPrimitive", Amp_Profile_Level_High);
    if (!checkState(HS_InDisplay, __FUNCTION__) ||
        !pprimitive->GetMeshCount() )
        return;

    // If in overdraw profile mode, and this primitive is part of a mask, draw it in color mode.
    static bool drawingMask = false;
    if ( !Profiler.ShouldDrawMask() && !drawingMask && (HALState & HS_DrawingMask) )
    {
        drawingMask = true;
        glColorMask(1,1,1,1);
        glDisable(GL_STENCIL_TEST);
        DrawProcessedPrimitive(pprimitive, pstart, pend );
        glColorMask(0,0,0,0);
        glEnable(GL_STENCIL_TEST);
        drawingMask = false;
    }

    SF_ASSERT(pend != 0);
    
    PrimitiveBatch* pbatch = pstart ? pstart : pprimitive->Batches.GetFirst();

    ShaderData.BeginPrimitive();

    unsigned bidx = 0;
    while (pbatch != pend)
    {        
        // pBatchMesh can be null in case of error, such as VB/IB lock failure.
        MeshCacheItem* pmesh = (MeshCacheItem*)pbatch->GetCacheItem();
        unsigned       meshIndex = pbatch->GetMeshIndex();
        unsigned       batchMeshCount = pbatch->GetMeshCount();

        if (pmesh)
        {
            Profiler.SetBatch((UPInt)pprimitive, bidx);

            UInt32 fillFlags = FillFlags;
            if ( batchMeshCount > 0 )
                fillFlags |= pprimitive->Meshes[0].M.Has3D() ? FF_3DProjection : 0;

            const ShaderManager::Shader& pShader =
                SManager.SetPrimitiveFill(pprimitive->pFill, fillFlags, pbatch->Type, pbatch->pFormat, batchMeshCount, Matrices,
                                          &pprimitive->Meshes[meshIndex], &ShaderData);

            if ((HALState & HS_ViewValid) && pShader &&
                SetVertexArray(pbatch->pFormat, pmesh->pVertexBuffer->GetBuffer(),
                               pmesh->pVertexBuffer->GetBufferBase() + pmesh->VBAllocOffset))
            {
                SF_ASSERT((pbatch->Type != PrimitiveBatch::DP_Failed) &&
                          (pbatch->Type != PrimitiveBatch::DP_Virtual));

                // Draw the object with cached mesh.
                if (pbatch->Type != PrimitiveBatch::DP_Instanced)
                {
                    AccumulatedStats.Meshes += pmesh->MeshCount;
                    AccumulatedStats.Triangles += pmesh->IndexCount / 3;
                }
                else
                {
                    SF_DEBUG_WARNING(1, "instanced draw");
                    AccumulatedStats.Meshes += batchMeshCount;
                    AccumulatedStats.Triangles += (pmesh->IndexCount / 3) * batchMeshCount;
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pmesh->pIndexBuffer->GetBuffer());
                glDrawElements(GL_TRIANGLES, pmesh->IndexCount, GL_UNSIGNED_SHORT, pmesh->pIndexBuffer->GetBufferBase() + pmesh->IBAllocOffset);

                AccumulatedStats.Primitives++;
            }

            pmesh->MoveToCacheListFront(MCL_ThisFrame);
        }

        pbatch = pbatch->GetNext();
        bidx++;
    }
}


void HAL::DrawProcessedComplexMeshes(ComplexMesh* complexMesh,
                                           const StrideArray<HMatrix>& matrices)
{    
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
        glColorMask(1,1,1,1);
        glDisable(GL_STENCIL_TEST);
        DrawProcessedComplexMeshes(complexMesh, matrices);
        glColorMask(0,0,0,0);
        glEnable(GL_STENCIL_TEST);
        drawingMask = false;
    }

    const FillRecord* fillRecords = complexMesh->GetFillRecords();
    unsigned    fillCount     = complexMesh->GetFillRecordCount();
    unsigned    instanceCount = (unsigned)matrices.GetSize();
    BatchType   batchType = PrimitiveBatch::DP_Single;

    const Matrix2F* textureMatrices = complexMesh->GetFillMatrixCache();

    for (unsigned fillIndex = 0; fillIndex < fillCount; fillIndex++)
    {
        const FillRecord& fr = fillRecords[fillIndex];

        Profiler.SetBatch((UPInt)complexMesh, fillIndex);
        ShaderData.BeginPrimitive();

        unsigned fillFlags = FillFlags;
        unsigned startIndex = 0;
        if ( instanceCount > 0 )
        {
            const HMatrix& hm = matrices[0];
            fillFlags |= hm.Has3D() ? FF_3DProjection : 0;

            for (unsigned i = 0; i < instanceCount; i++)
            {
                const HMatrix& hm = matrices[startIndex + i];
                if (!(Profiler.GetCxform(hm.GetCxform()) == Cxform::Identity))
                    fillFlags |= FF_Cxform;
            }
        }

        // Apply fill.
        PrimitiveFillType fillType = Profiler.GetFillType(fr.pFill->GetType());
        const ShaderManager::Shader& pso = SManager.SetFill(fillType, fillFlags, batchType, fr.pFormats[0], &ShaderData);
        SetVertexArray(fr.pFormats[0], pmesh->pVertexBuffer->GetBuffer(), pmesh->pVertexBuffer->GetBufferBase() + pmesh->VBAllocOffset + fr.VertexByteOffset);

        UByte textureCount = fr.pFill->GetTextureCount();
        bool solid = (fillType == PrimFill_None || fillType == PrimFill_Mask || fillType == PrimFill_SolidColor);

        for (unsigned i = 0; i < instanceCount; i++)
        {            
            const HMatrix& hm = matrices[startIndex + i];

            ShaderData.SetMatrix(pso, Uniform::SU_mvp, complexMesh->GetVertexMatrix(), hm, Matrices);
            if (solid)
                ShaderData.SetColor(pso, Uniform::SU_cxmul, Profiler.GetColor(fr.pFill->GetSolidColor()));
            else if (fillFlags & FF_Cxform)
                ShaderData.SetCxform(pso, Profiler.GetCxform(hm.GetCxform()));

            for (unsigned tm = 0, stage = 0; tm < textureCount; tm++)
            {
                ShaderData.SetMatrix(pso, Uniform::SU_texgen, textureMatrices[fr.FillMatrixIndex[tm]], tm);
                Texture* ptex = (Texture*)fr.pFill->GetTexture(tm);
                ShaderData.SetTexture(pso, stage, ptex, fr.pFill->GetFillMode(tm));
                stage += ptex->GetPlaneCount();
            }

            ShaderData.Finish(0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pmesh->pIndexBuffer->GetBuffer());
            glDrawElements(GL_TRIANGLES, fr.IndexCount, GL_UNSIGNED_SHORT,
                           pmesh->pIndexBuffer->GetBufferBase() + pmesh->IBAllocOffset + sizeof(IndexType) * fr.IndexOffset);
        }

        AccumulatedStats.Triangles += (fr.IndexCount / 3) * instanceCount;
        AccumulatedStats.Meshes += instanceCount;
        AccumulatedStats.Primitives += instanceCount;

    } // for (fill record)
  
    pmesh->MoveToCacheListFront(MCL_ThisFrame);
}


//--------------------------------------------------------------------
// Background clear helper, expects viewport coordinates.
void HAL::clearSolidRectangle(const Rect<int>& r, Color color)
{
    color = Profiler.GetClearColor(color);

    if (color.GetAlpha() == 0xFF)
    {
        glEnable(GL_SCISSOR_TEST);

        PointF tl((float)(VP.Left + r.x1), (float)(VP.Top + r.y1));
        PointF br((float)(VP.Left + r.x2), (float)(VP.Top + r.y2));
        tl = Matrices.Orient2D * tl;
        br = Matrices.Orient2D * br;
        Rect<int> scissor((int)Alg::Min(tl.x, br.x), (int)Alg::Min(tl.y,br.y), (int)Alg::Max(tl.x,br.x), (int)Alg::Max(tl.y,br.y));
        glScissor(scissor.x1, scissor.y1, scissor.Width(), scissor.Height());
        glClearColor(color.GetRed() * 1.f/255.f, color.GetGreen() * 1.f/255.f, color.GetBlue() * 1.f/255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (VP.Flags & Viewport::View_UseScissorRect)
        {
            glEnable(GL_SCISSOR_TEST);
            glScissor(VP.ScissorLeft, VP.BufferHeight-VP.ScissorTop-VP.ScissorHeight, VP.ScissorWidth, VP.ScissorHeight);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }
    else
    {
        float colorf[4];
        color.GetRGBAFloat(colorf, colorf+1, colorf+2, colorf+3);
        Matrix2F m((float)r.Width(), 0.0f, (float)r.x1,
                   0.0f, (float)r.Height(), (float)r.y1);

        Matrix2F  mvp(m, Matrices.UserView);

        unsigned fillflags = 0;
        const ShaderManager::Shader& pso = SManager.SetFill(PrimFill_SolidColor, fillflags, PrimitiveBatch::DP_Single, &VertexXY16iInstance::Format, &ShaderData);
        ShaderData.SetMatrix(pso, Uniform::SU_mvp, mvp);
        ShaderData.SetUniform(pso, Uniform::SU_cxmul, colorf, 4);
        ShaderData.Finish(1);

        SetVertexArray(&VertexXY16iInstance::Format, Cache.MaskEraseBatchVertexBuffer, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
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
    if (!checkState(HS_InDisplay, __FUNCTION__))
        return;

    Profiler.SetDrawMode(1);

    glColorMask(0,0,0,0);                       // disable framebuffer writes
    glEnable(GL_STENCIL_TEST);

    bool viewportValid = (HALState & HS_ViewValid) != 0;

    // Erase previous mask if it existed above our current stack top.
    if (MaskStackTop && (MaskStack.GetSize() > MaskStackTop) && viewportValid && MultiBitStencil)
    {
        glStencilFunc(GL_LEQUAL, MaskStackTop, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        MaskPrimitive* erasePrim = MaskStack[MaskStackTop].pPrimitive;
        drawMaskClearRectangles(erasePrim->GetMaskAreaMatrices(), erasePrim->GetMaskCount());
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
            glClearStencil(0);
            glClear(GL_STENCIL_BUFFER_BIT);
        }
    }
    else
        if ((MaskStackTop == 1) && viewportValid)
    {
        glStencilFunc(GL_ALWAYS, 0, 0xff);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

        drawMaskClearRectangles(prim->GetMaskAreaMatrices(), prim->GetMaskCount());

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    if (MultiBitStencil)
    {
        glStencilFunc(GL_EQUAL, MaskStackTop-1, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    }
    else if (MaskStackTop == 1)
    {
        glStencilFunc(GL_ALWAYS, 1, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

    glColorMask(1,1,1,1);
    glStencilFunc(GL_LEQUAL, MaskStackTop, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}


void HAL::PopMask()
{
    if (!checkState(HS_InDisplay, __FUNCTION__))
        return;

    SF_ASSERT(MaskStackTop);
    MaskStackTop--;

    if (MaskStack[MaskStackTop].pPrimitive->IsClipped())
    {
        // Restore viewport
        ViewRect      = MaskStack[MaskStackTop].OldViewRect;

        if (MaskStack[MaskStackTop].OldViewportValid)
            HALState |= HS_ViewValid;
        else
            HALState &= ~HS_ViewValid;
        updateViewport();
    }

    if (MaskStackTop == 0)
        glDisable(GL_STENCIL_TEST);
    else
        glStencilFunc(GL_LEQUAL, MaskStackTop, 0xff);
}


void HAL::drawMaskClearRectangles(const HMatrix* matrices, UPInt count)
{
    // This operation is used to clear bounds for masks.
    // Potential issue: Since our bounds are exact, right/bottom pixels may not
    // be drawn due to HW fill rules.
    //  - This shouldn't matter if a mask is tessellated within bounds of those
    //    coordinates, since same rules are applied to those render shapes.
    //  - EdgeAA must be turned off for masks, as that would extrude the bounds.

    unsigned fillflags = 0;
    const ShaderManager::Shader& pso = SManager.SetFill(PrimFill_SolidColor, fillflags, PrimitiveBatch::DP_Batch, &VertexXY16iInstance::Format, &ShaderData);

    unsigned drawRangeCount = 0;
    for (UPInt i = 0; i < count; i+= (UPInt)drawRangeCount)
    {
        drawRangeCount = Alg::Min<unsigned>((unsigned)count, MeshCache::MaxEraseBatchCount);

        for (unsigned j = 0; j < drawRangeCount; j++)
        {
            ShaderData.SetMatrix(pso, Uniform::SU_mvp, Matrix2F::Identity, matrices[i+j], Matrices, 0, j);
        }
        ShaderData.Finish(drawRangeCount);

        SetVertexArray(&VertexXY16iInstance::Format, Cache.MaskEraseBatchVertexBuffer, 0);
        glDrawArrays(GL_TRIANGLES, 0, drawRangeCount * 6);

        AccumulatedStats.Meshes += drawRangeCount;
        AccumulatedStats.Triangles += drawRangeCount * 2;
        AccumulatedStats.Primitives++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



//--------------------------------------------------------------------
// *** BlendMode Stack support
//--------------------------------------------------------------------

// Structure describing color combines applied for a given blend mode.
struct BlendModeDesc
{
    GLenum op, src, dest;
};

struct BlendModeDescAlpha
{
    GLenum op, srcc, srca, destc, desta;
};

void HAL::applyBlendMode(BlendMode mode, bool sourceAc, bool forceAc)
{    
    static BlendModeDesc modes[15] =
    {
        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // None
        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Normal
        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Layer

        { GL_FUNC_ADD,              GL_DST_COLOR,           GL_ZERO                 }, // Multiply
        // (For multiply, should src be pre-multiplied by its inverse alpha?)

        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Screen *??

        { GL_MAX,                   GL_SRC_ALPHA,           GL_ONE                  }, // Lighten
        { GL_MIN,                   GL_SRC_ALPHA,           GL_ONE                  }, // Darken

        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Difference *??

        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE                  }, // Add
        { GL_FUNC_REVERSE_SUBTRACT, GL_SRC_ALPHA,           GL_ONE                  }, // Subtract

        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Invert *??

        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Alpha *??
        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Erase *??
        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // Overlay *??
        { GL_FUNC_ADD,              GL_SRC_ALPHA,           GL_ONE_MINUS_SRC_ALPHA  }, // HardLight *??
    };

    // Blending into alpha textures with premultiplied colors
    static BlendModeDescAlpha acmodes[15] =
    {
        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // None
        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // Normal
        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // Layer

        { GL_FUNC_ADD,              GL_DST_COLOR,  GL_DST_ALPHA,  GL_ZERO,                GL_ZERO                 }, // Multiply

        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // Screen *??

        { GL_MAX,                   GL_SRC_ALPHA,  GL_SRC_ALPHA,  GL_ONE,                 GL_ONE                  }, // Lighten *??
        { GL_MIN,                   GL_SRC_ALPHA,  GL_SRC_ALPHA,  GL_ONE,                 GL_ONE                  }, // Darken *??

        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // Difference

        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ZERO,       GL_ONE,                 GL_ONE                  }, // Add
        { GL_FUNC_REVERSE_SUBTRACT, GL_SRC_ALPHA,  GL_ZERO,       GL_ONE,                 GL_ONE                  }, // Subtract

        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // Invert *??

        { GL_FUNC_ADD,              GL_ZERO,       GL_ZERO,       GL_ONE,                 GL_ONE                  }, // Alpha *??
        { GL_FUNC_ADD,              GL_ZERO,       GL_ZERO,       GL_ONE,                 GL_ONE                  }, // Erase *??
        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // Overlay *??
        { GL_FUNC_ADD,              GL_SRC_ALPHA,  GL_ONE,        GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  }, // HardLight *??
    };

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
        BlendModeDescAlpha ms = acmodes[mode];
        if ( sourceAc && acmodes[mode].srcc == GL_SRC_ALPHA )
            ms.srcc = GL_ONE;
        glBlendFuncSeparate(ms.srcc, ms.destc, ms.srca, ms.desta);
        glBlendEquation(ms.op);
    }
    else
    {
        BlendModeDesc ms = modes[mode];
        if ( sourceAc && modes[mode].src == GL_SRC_ALPHA )
            ms.src = GL_ONE;
        glBlendFunc(ms.src, ms.dest);
        glBlendEquation(ms.op);
    }
}

RenderTarget* HAL::CreateRenderTarget(GLuint fbo)
{
    GLint currentFbo;
    RenderTarget* prt = pRenderBufferManager->CreateRenderTarget(getFboInfo(fbo, currentFbo, false), RBuffer_User, Image_R8G8B8A8, 0);
    if ( !prt )
        return 0;
    if ( prt->GetHALData() != 0 )
        return prt;

    HALData::UpdateData(prt, this, fbo, 0);
    return prt;
}

RenderTarget* HAL::CreateRenderTarget(Render::Texture* texture, bool needsStencil)
{
    GL::Texture* pt = (GL::Texture*)texture;

    if ( !pt || pt->TextureCount != 1 )
        return 0;

    GLuint fboID = 0, dsbID = 0;
    RenderTarget* prt = pRenderBufferManager->CreateRenderTarget(
        texture->GetSize(), RBuffer_Texture, texture->GetFormat(), texture);
    if ( !prt )
        return 0;
    Ptr<DepthStencilBuffer> pdsb;

    // Cannot render to textures which have multiple HW representations.
    SF_ASSERT(pt->TextureCount == 1); 
    GLuint colorID = pt->pTextures[0].TexId;

    glGenFramebuffersEXT(1, &fboID);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
    
#if defined(GL_ES_VERSION_2_0)
    // If on GLES2, and it has NPOT limitations, then we need to ensure that the texture
    // uses clamping mode without mipmapping, otherwise the glCheckFramebufferStatus will 
    // return that the target is unsupported.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
#endif
    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colorID, 0);
    if ( needsStencil )
    {
        pdsb = *pRenderBufferManager->CreateDepthStencilBuffer(texture->GetSize());
        if ( pdsb )
        {
            DepthStencilSurface* surf = (GL::DepthStencilSurface*)pdsb->GetSurface();
            if ( surf )
            {
                dsbID = surf->RenderBufferID;
                glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, dsbID);
            }
        }
    }

    HALData::UpdateData(prt, this, fboID, pdsb);
    return prt;
}

RenderTarget* HAL::CreateTempRenderTarget(const ImageSize& size, bool needsStencil)
{
    RenderTarget* prt = pRenderBufferManager->CreateTempRenderTarget(size);
    if ( !prt )
        return 0;
    Texture* pt = (Texture*)prt->GetTexture();
    if ( !pt )
        return 0;

    HALData* phd = (HALData*)prt->GetHALData();
    if ( phd && (!needsStencil || phd->pDepthStencilBuffer != 0 ))
        return prt;

    Ptr<DepthStencilBuffer> pdsb = 0;
    GLuint dsbID = 0;
    if ( needsStencil )
    {
        pdsb = *pRenderBufferManager->CreateDepthStencilBuffer(size);
        DepthStencilSurface* pdss = (DepthStencilSurface*)pdsb->GetSurface();
        dsbID = pdss->RenderBufferID;
    }

    // If only a new depth stencil is required.
    GLuint fboID;
    GLuint colorID = pt->pTextures[0].TexId;

    if ( phd )
        fboID = phd->FBOID;
    else
        glGenFramebuffersEXT(1, &fboID);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
    
#if defined(GL_ES_VERSION_2_0)
    // If on GLES2, and it has NPOT limitations, then we need to ensure that the texture
    // uses clamping mode without mipmapping, otherwise the glCheckFramebufferStatus will 
    // return that the target is unsupported.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
#endif
    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colorID, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, dsbID);
    GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    SF_ASSERT( status == GL_FRAMEBUFFER_COMPLETE_EXT);
    SF_UNUSED( status );

    HALData::UpdateData(prt, this, fboID, pdsb);
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

    RenderTargetEntry entry;
    if ( setState )
    {
        HALData* phd = (HALData*)ptarget->GetHALData();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, phd->FBOID);
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
    // Setup the render target/depth stencil on the device.
    HALState |= HS_InRenderTarget;
    RenderTargetEntry entry = {prt, Matrices, ViewRect, VP};
    Matrices.Orient2D.SetIdentity();
    Matrices.Orient3D.SetIdentity();
    Matrices.SetUserMatrix(Matrix2F::Identity);
    // VP.Flags &= ~Viewport::View_Orientation_Mask;

    // Setup the render target/depth stencil.
    if ( !prt )
    {
        SF_DEBUG_WARNING(1, "HAL::PushRenderTarget - invalid render target.");
        RenderTargetStack.PushBack(entry);
        return;
    }
    HALData* phd = (GL::HALData*)prt->GetHALData();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, phd->FBOID);
    ++AccumulatedStats.RTChanges;

    glDisable(GL_SCISSOR_TEST);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Setup viewport.
    Rect<int> viewRect = prt->GetRect(); // On the render texture, might not be the entire surface.
    const ImageSize& bs = prt->GetBufferSize();
    VP = Viewport(bs.Width, bs.Height, viewRect.x1, viewRect.y1, viewRect.Width(), viewRect.Height());    
    VP.Flags |= Viewport::View_IsRenderTexture;

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
        GL::HALData* plasthd = (GL::HALData*)prt->GetHALData();
        if ( plasthd->pDepthStencilBuffer )
        {
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, plasthd->FBOID);
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
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
    GLuint fboID = 0;
    if ( RenderTargetStack.GetSize() > 0 )
    {
        RenderTargetEntry& back = RenderTargetStack.Back();
        phd = (GL::HALData*)back.pRenderTarget->GetHALData();
        fboID = phd->FBOID;
    }

    if ( RenderTargetStack.GetSize() == 1 )
        HALState &= ~HS_InRenderTarget;

    // Restore the old render target.
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
    ++AccumulatedStats.RTChanges;

    // Reset the viewport to the last render target on the stack.
    HALState |= HS_ViewValid;
    updateViewport();
}

ImageSize HAL::getFboInfo(GLint fbo, GLint& currentFBO, bool useCurrent)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &currentFBO);
    if (!useCurrent)
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

    bool validFBO = glIsFramebufferEXT( fbo ) ? true : false;
    GLint width = 0, height = 0;
    GLint type, id;

    if ( validFBO )
    {
        glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT, &type );
        glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT, &id );
        switch(type)
        {
        case GL_TEXTURE:
            {
#ifdef GL_TEXTURE_WIDTH
                glBindTexture(GL_TEXTURE_2D, id );
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );
#endif
                break;
            }
        case GL_RENDERBUFFER_EXT:
            if ( !glIsRenderbufferEXT( id ) )
                break;
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id);
            glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &width );
            glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &height );
            break;
        }
    }

    if (width == 0 || height == 0)
    {
        // Get the dimensions of the framerect from glViewport.
        GLfloat viewport[4];
        glGetFloatv(GL_VIEWPORT, viewport);
        width = (GLint)viewport[2];
        height = (GLint)viewport[3];
    }

    if (!useCurrent)
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, currentFBO);

    return ImageSize(width, height);
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
        RenderTargetEntry entry;

        GLint currentFBO;
        rtSize = getFboInfo(0, currentFBO, true);

        Ptr<RenderTarget> ptarget = *SF_HEAP_AUTO_NEW(this) RenderTarget(0, RBuffer_Default, rtSize );
        Ptr<DepthStencilBuffer> pdsb = *SF_HEAP_AUTO_NEW(this) DepthStencilBuffer(0, rtSize);
        HALData::UpdateData(ptarget, this, currentFBO, pdsb);

        if (!SetRenderTarget(ptarget))
            return false;
    }

    return pRenderBufferManager->Initialize(pTextureManager, Image_R8G8B8A8, rtSize );
}

void HAL::PushFilters(FilterPrimitive* prim)
{
    if (!checkState(HS_InDisplay, __FUNCTION__))
        return;

    FilterStackEntry e = {prim, 0};

    // Do not render filters if the profile does not support it (unfiltered content will be rendered).
    if (!shouldRenderFilters(prim))
    {
        FilterStack.PushBack(e);
        return;
    }

    // Queue the profiler off of whether masks should be draw or not.
    if ( !Profiler.ShouldDrawMask() )
    {
        Profiler.SetDrawMode(2);

        unsigned fillflags = 0;
        float colorf[4];
        Profiler.GetColor(0xFFFFFFFF).GetRGBAFloat(colorf);
        const ShaderManager::Shader& pso = SManager.SetFill(PrimFill_SolidColor, fillflags, 
            PrimitiveBatch::DP_Single, &VertexXY16iInstance::Format, &ShaderData);
        Matrix2F mvp(prim->GetFilterAreaMatrix().GetMatrix2D(), Matrices.UserView);
        ShaderData.SetMatrix(pso, Uniform::SU_mvp, mvp);
        ShaderData.SetUniform(pso, Uniform::SU_cxmul, colorf, 4);
        ShaderData.Finish(1);

        SetVertexArray(&VertexXY16iInstance::Format, Cache.MaskEraseBatchVertexBuffer, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        FilterStack.PushBack(e);
        return;
    }

    if ( (HALState & HS_CachedFilter) )
    {
        FilterStack.PushBack(e);
        return;
    }

    // Disable masking from previous target, if this filter primitive doesn't have any masking.
    if ( MaskStackTop != 0 && !prim->GetMaskPresent() && prim->GetCacheState() != FilterPrimitive::Cache_Target)
    {
        glDisable(GL_STENCIL_TEST);
    }

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
            glClearStencil(MaskStackTop);
            glClear(GL_STENCIL_BUFFER_BIT);
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
    // Do not render filters if the profile does not support it.
    if (!shouldRenderFilters(FilterStack.Back().pPrimitive))
    {
        FilterStack.Pop();
        return;
    }

    Render::HAL::PopFilters();
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
    const unsigned MaxTemporaryTextures = 3;
    Ptr<RenderTarget> temporaryTextures[MaxTemporaryTextures];
    memset(temporaryTextures, 0, sizeof temporaryTextures);

    // Fill our the source target.
    ImageSize size = e.pRenderTarget->GetSize();
    temporaryTextures[0] = e.pRenderTarget;

    const VertexFormat* pvf[3];
    SManager.MapVertexFormat(PrimFill_UVTexture, &VertexXY16iUV::Format, &pvf[0], &pvf[1], &pvf[2] );
    glColorMask(1,1,1,1);
    FilterVertexBufferSet = 0;

    glDisable(GL_SCISSOR_TEST);

    // Overlay mode isn't actually supported, it contains the blend mode for filter sub-targets.
    applyBlendMode(Blend_Overlay, true, false);

    // Render filter(s).
    unsigned shaders[ShaderManager::MaximumFilterPasses];
    BlurFilterState LEBlurState (8);

    for ( unsigned i = 0; i < filterCount; ++i )
    {
        filter = filters->GetFilter(i);
        passes = SManager.SetupFilter(filter, FillFlags, shaders, LEBlurState);

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

            GLuint fboid = ((GL::HALData*)temporaryTextures[1]->GetHALData())->FBOID;
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboid );
            ++AccumulatedStats.RTChanges;
            glClearColor(0,0,0,0);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // Scale to the size of the destination.
            RenderTarget* prt = temporaryTextures[1];
            const Rect<int>& viewRect = prt->GetRect(); // On the render texture, might not be the entire surface.
            const ImageSize& bs = prt->GetBufferSize();
            VP = Viewport(bs.Width, bs.Height, viewRect.x1, viewRect.y1, viewRect.Width(), viewRect.Height());    
            ViewRect = Rect<int>(viewRect.x1, viewRect.y1, viewRect.x2, viewRect.y2);
            HALState |= HS_ViewValid;
            updateViewport();

            Matrix2F mvp = Matrix2F::Scaling(2,2) * Matrix2F::Translation(-0.5f, -0.5f);                          
            DrawFilter(mvp, Cxform::Identity, filter, temporaryTextures, shaders, pass, passes, pvf[0], LEBlurState, false);

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
    RenderTarget* cacheResults[2] = { temporaryTextures[0], temporaryTextures[2] };
    if ( Profiler.IsFilterCachingEnabled() )
        e.pPrimitive->SetCacheResults(FilterPrimitive::Cache_PreTarget, cacheResults, 2);
    ((GL::HALData*)cacheResults[0]->GetHALData())->CacheID = reinterpret_cast<UPInt>(e.pPrimitive.GetPtr());
    if ( cacheResults[1] )
        ((GL::HALData*)cacheResults[1]->GetHALData())->CacheID = reinterpret_cast<UPInt>(e.pPrimitive.GetPtr());

    // Pop the temporary target, begin rendering to the previous surface.
    PopRenderTarget();

    // Re-[en/dis]able masking from previous target, if available.
    if ( MaskStackTop != 0 )
        glEnable(GL_STENCIL_TEST);

    // Now actually draw the filtered sub-scene to the target below.
    const Matrix2F& mvp = Matrices.UserView * e.pPrimitive->GetFilterAreaMatrix().GetMatrix2D();
    const Cxform&   cx  = e.pPrimitive->GetFilterAreaMatrix().GetCxform();
    applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, true, true);
    DrawFilter(mvp, cx, filter, temporaryTextures, shaders, pass, passes, pvf[0], LEBlurState, true);
    applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, false, (HALState&HS_InRenderTarget) != 0);

    if ( HALState & HS_DrawingMask )
        glColorMask(0,0,0,0);

    // Cleanup.
    for ( unsigned i = 0; i < MaxTemporaryTextures; ++i )
    {
        if (temporaryTextures[i])
            temporaryTextures[i]->SetInUse(false);
    }
    AccumulatedStats.Filters += filters->GetFilterCount();

}

void HAL::drawCachedFilter(FilterPrimitive* primitive)
{
    FilterVertexBufferSet = 0;
    BlurFilterState LEBlurState (8);

    const unsigned MaxTemporaryTextures = 3;
    switch(primitive->GetCacheState())
    {
        // We have one-step from final target. Render it to a final target now.
        case FilterPrimitive::Cache_PreTarget:
        {
            const FilterSet* filters = primitive->GetFilters();
            UPInt filterIndex = filters->GetFilterCount()-1;
            const Filter* filter = filters->GetFilter(filterIndex);
            unsigned shaders[ShaderManager::MaximumFilterPasses];

            unsigned passes = SManager.SetupFilter(filter, FillFlags, shaders, LEBlurState);
            
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
            Matrix2F mvp = Matrix2F::Scaling(2,2) * Matrix2F::Translation(-0.5f, -0.5f);
            const Cxform & cx = primitive->GetFilterAreaMatrix().GetCxform();

            applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, true, true);
            DrawFilter(mvp, cx, filter, temporaryTextures, shaders, passes-1, passes, pvf[0], LEBlurState, true);

            PopRenderTarget();
            
            // Re-[en/dis]able masking from previous target, if available.
            if ( MaskStackTop != 0 )
                glEnable(GL_STENCIL_TEST);

            // Re-[en/dis]able masking from previous target, if available.
            if ( HALState & HS_DrawingMask )
                glColorMask(0,0,0,0);

            // Set this as the final cache result, and then render it.
            RenderTarget* prt = temporaryTextures[1];
            primitive->SetCacheResults(FilterPrimitive::Cache_Target, &prt, 1);
            ((GL::HALData*)prt->GetHALData())->CacheID = reinterpret_cast<UPInt>(primitive);
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
            unsigned fillFlags = (FillFlags & ~FF_Cxform);
            const ShaderManager::Shader& pso = SManager.SetFill(PrimFill_Texture, fillFlags, PrimitiveBatch::DP_Single, pvf[0], &ShaderData);

            RenderTarget* results;
            primitive->GetCacheResults(&results, 1);
            Texture* ptexture = (GL::Texture*)results->GetTexture();
            const Matrix2F& mvp = Matrices.UserView * primitive->GetFilterAreaMatrix().GetMatrix2D();
            const Rect<int>& srect = results->GetRect();
            Matrix2F texgen;
            texgen.AppendTranslation((float)srect.x1, (float)srect.y1);
            texgen.AppendScaling((float)srect.Width() / ptexture->GetSize().Width, (float)srect.Height() / ptexture->GetSize().Height);

            ShaderData.SetUniform(pso, Uniform::SU_mvp, &mvp.M[0][0], 8 );
            ShaderData.SetUniform(pso, Uniform::SU_texgen, &texgen.M[0][0], 8 );
            ShaderData.SetTexture(pso, 0, ptexture, ImageFillMode(Wrap_Clamp, Sample_Linear));
            ShaderData.Finish();

            if (!FilterVertexBufferSet)
            {
                SetVertexArray(&VertexXY16iUV::Format, Cache.UVSquareVertexBuffer, 0);
                FilterVertexBufferSet = 1;
            }

            applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, true, true);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            applyBlendMode(BlendModeStack.GetSize()>=1 ? BlendModeStack.Back() : Blend_Normal, false, (HALState&HS_InRenderTarget)!=0);

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

//--------------------------------------------------------------------
#if defined(GL_ES_VERSION_2_0)
#include "Render/GL/GLES_ExtensionMacros.h"
#else
#include "Render/GL/GL_ExtensionMacros.h"
#endif

HALData::~HALData()
{
    if (pBuffer->GetType() != RBuffer_Default && pBuffer->GetType() != RBuffer_User)
        glDeleteFramebuffersEXT(1, &FBOID);
}

class VertexBuilder
{
public:
    UByte*    vertexOffset;
    unsigned  Size;
    HAL*      pHal;

    VertexBuilder(HAL* phal, UByte* v, unsigned s) : vertexOffset(v), Size(s), pHal(phal) {}
    HAL* GetHAL() { return pHal; }

    void Add(int vi, int attr, int ac, int offset)
    {
        GLenum vet; bool norm;

        switch (attr & VET_CompType_Mask)
        {
        case VET_U8:  vet = GL_UNSIGNED_BYTE; norm = false; break;
        case VET_U8N: vet = GL_UNSIGNED_BYTE; norm = true; break;
        case VET_U16: vet = GL_UNSIGNED_SHORT; norm = false; break;
        case VET_S16: vet = GL_SHORT; norm = false; break;
        case VET_U32: vet = GL_UNSIGNED_INT; norm = false; break;
        case VET_F32: vet = GL_FLOAT; norm = false;  break;

        default: SF_ASSERT(0); vet = GL_FLOAT; norm = false; break;
        }

        if ( pHal->EnabledVertexArrays < vi )
        {
            glEnableVertexAttribArray(vi);
            pHal->EnabledVertexArrays++;
        }
        glVertexAttribPointer(vi, ac, vet, norm, Size, vertexOffset + offset);
    }

    void Finish(int vi)
    {
        int newEnabledCount = vi-1;
        for (int i = vi; i < pHal->EnabledVertexArrays; i++)
        {
            glDisableVertexAttribArray(i);
        }
        pHal->EnabledVertexArrays = newEnabledCount;
    }
};

bool HAL::SetVertexArray(const VertexFormat* pFormat, GLuint buffer, UByte* vertexOffset)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    VertexBuilder vb (this, vertexOffset, pFormat->Size);
    BuildVertexArray(pFormat, vb);

    return true;
}

const ShaderObject* HAL::GetStaticShader( unsigned index, bool is3d )
{
    if ( index >= FragShaderDesc::FS_Count*2 )
        return 0;

    ShaderObject* shader = 0;
    if ( is3d )
        shader = &StaticShaders[index + FragShaderDesc::FS_Count];
    else
        shader = &StaticShaders[index];

    // Initialize the shader if it hasn't already been initialized.
    if ( (VMCFlags & HALConfig_DynamicShaderCompile) && shader->Prog == 0 )
    {
        shader->Init(this, (FragShaderType)index, is3d ? VertexShaderDesc::VS_base_Position3d : 0 );
    }
    return shader;
}

bool HAL::initializeShaders(bool force)
{
    if ( (VMCFlags & HALConfig_DynamicShaderCompile) == 0 || force)
    {
        for (unsigned i = 0; i < FragShaderDesc::FS_Count; i++)
        {
            // If the platform does not support dynamic loops, do not initialize shadow or blur shaders,
            // because they will fail.
            bool filterShader = false;
            if (( i >= FragShaderDesc::FS_start_shadows && i <= FragShaderDesc::FS_end_shadows) ||
                ( i >= FragShaderDesc::FS_start_blurs   && i <= FragShaderDesc::FS_end_blurs) )
            {
                filterShader = true;
            }

            // If the platform does not support dynamic loops, do not initialize shadow or blur shaders,
            // because they will fail.
            if (filterShader && (Caps & Cap_NoDynamicLoops))                
                continue;

            if ( i >= FragShaderDesc::FS_start_cmatrix && i <= FragShaderDesc::FS_end_cmatrix)
                filterShader = true;

            if (FragShaderDesc::Descs[i])
            {
                if ( !StaticShaders[i].Init(this, (FragShaderType)i, 0) )
                    return false;

                // Filter shaders do not have 3D versions.
                if ( !filterShader && !StaticShaders[i + FragShaderDesc::FS_Count].Init(
                        this, (FragShaderType)i, VertexShaderDesc::VS_base_Position3d) )
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool HAL::shouldRenderFilters(const FilterPrimitive*) const
{
    return true;
}

void    HAL::DrawFilter(const Matrix2F& mvp, const Cxform & cx, const Filter* filter, Ptr<RenderTarget> * targets, 
                        unsigned* shaders, unsigned pass, unsigned passCount, const VertexFormat* pvf, 
                        BlurFilterState& leBlur, bool isLastPass)
{
    if (leBlur.Passes > 0)
    {
        leBlur.SetPass(pass);

        const BlurFilterShader* pShader = ShaderData.GetBlurShader(leBlur);
        if (!pShader)
            return;

        Rect<int> srcrect = targets[Target_Source]->GetRect();
        Rect<int> destrect = Rect<int>(0,0,1,1);

        glUseProgram(pShader->Shader);

        if (!isLastPass)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
        }
        // else do nothing, dest blend mode was set before calling DrawFilter

        glUniform4fv(pShader->mvp, 2, &mvp.M[0][0]);

        if (pShader->cxadd >= 0)
        {
            float cxform[2][4];
            cx.GetAsFloat2x4(cxform);
            glUniform4fv(pShader->cxmul, 1, cxform[0]);
            glUniform4fv(pShader->cxadd, 1, cxform[1]);
        }

        for (int i = 0; i < 2; i++)
            if (pShader->scolor[i] >= 0)
            {
                float color[4];
                leBlur.CurPass->Colors[i].GetRGBAFloat(color);
                glUniform4fv(pShader->scolor[i], 1, color);
            }

        if (pShader->samples >= 0)
        {
            glUniform1f(pShader->samples, 1.0f/leBlur.Samples);
        }

        if (pShader->tex[1] >= 0)
        {
            GL::Texture *ptexture = (GL::Texture*) targets[Target_Original]->GetTexture();
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, ptexture->pTextures[0].TexId);
            glUniform1i(pShader->tex[1], 1);
            glUniform2f(pShader->texscale[1], 1.0f/ptexture->GetSize().Width, 1.0f/ptexture->GetSize().Height);
        }

        GL::Texture *ptexture = (GL::Texture*) targets[Target_Source]->GetTexture();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ptexture->pTextures[0].TexId);
        glUniform1i(pShader->tex[0], 0);
        glUniform2f(pShader->texscale[0], 1.0f/ptexture->GetSize().Width, 1.0f/ptexture->GetSize().Height);

        float* pvertices = (float*) alloca(sizeof(float) * leBlur.GetVertexBufferSize());
        VertexFunc_Buffer vout (pvertices);
        leBlur.GetVertices(srcrect, destrect, vout);
        int vbstride = leBlur.VertexAttrs * 2 * sizeof(float);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        if (EnabledVertexArrays < 0)
            glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, 0, vbstride, pvertices);
        for (int i = 0; i < leBlur.TotalTCs; i++)
        {
            if (EnabledVertexArrays < i + 1)
                glEnableVertexAttribArray(i + 1);
            glVertexAttribPointer(i + 1, 2, GL_FLOAT, 0, vbstride, pvertices+(2+i*2));
        }
        for (int i = leBlur.TotalTCs+2; i < EnabledVertexArrays; i++)
           glDisableVertexAttribArray(i);
        EnabledVertexArrays = leBlur.TotalTCs;

        glDrawArrays(GL_TRIANGLES, 0, 6 * leBlur.Quads);

        FilterVertexBufferSet = 0;
    }
    else
    {
        if (!FilterVertexBufferSet)
        {
            SetVertexArray(&VertexXY16iUV::Format, Cache.UVSquareVertexBuffer, 0);
            FilterVertexBufferSet = 1;
        }

        SManager.SetFilterFill(mvp, cx, filter, targets, shaders, pass, passCount, pvf, &ShaderData);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}


}}} // Scaleform::Render::GL

