/**************************************************************************

Filename    :   D3D1x_HALSetup.cpp
Content     :   
Created     :   Mar 2011
Authors     :   Bart Muzzin

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/
#include "pch.h"

#include "D3D1x_HAL.h"
#include "Kernel/SF_Debug.h"
#include "Kernel/SF_HeapNew.h"
#include "Render/Render_BufferGeneric.h"

#include <d3dx11.h>

//#if SF_CC_MSVC >= 1600 // MSVC 2010
//#include <crtdefs.h>
//#else
//#include <xutility>
//#endif // SF_CC_MSVC >= 1600 // MSVC 2010

namespace Scaleform { namespace Render { namespace D3D1x {

static PixMarker FrameMarker(0, false);
static PixMarker SceneMarker(0, false);
static PixMarker DisplayMarker(0, false);

// *** RenderHAL_D3D1x Implementation
    
bool HAL::InitHAL(const D3D1x::HALInitParams& params)
{
    if ( !Render::HAL::initHAL(params))
        return false;

    if (!params.pD3DDevice)
        return 0;

    pDevice = params.pD3DDevice;

    D3D11(
        ID3D1x(DeviceContext)* pd3dDeviceContext = params.pD3DContext;
        if (!pd3dDeviceContext)
            pDevice->GetImmediateContext(&pd3dDeviceContext);
        );

    D3D10_11( pDeviceContext = pDevice, pDeviceContext  = pd3dDeviceContext);

    pDevice->AddRef();
    pDeviceContext->AddRef();

    if ( createBlendStates() &&
         createDepthStencilStates() &&
         createRasterStates() &&
         createConstantBuffers() && 
         SManager.Initialize(pDevice, pDeviceContext, &Profiler, VMCFlags) &&
         Cache.Initialize(pDevice, pDeviceContext))
    {
        // Create Texture manager if needed.
        if (params.pTextureManager)
            pTextureManager = params.GetTextureManager();
        else
        {
            pTextureManager = 
                *SF_HEAP_AUTO_NEW(this) TextureManager(pDevice, pDeviceContext,
                                                       params.RenderThreadId, pRTCommandQueue);
            if (!pTextureManager)
            {
                Cache.Reset();
                SManager.Reset();
            }
        }

        // Create RenderBufferManager if needed.
        if (params.pRenderBufferManager)
            pRenderBufferManager = params.pRenderBufferManager;
        else
        {
            // Create the default render target, and manager.
            pRenderBufferManager = *SF_HEAP_AUTO_NEW(this) RenderBufferManagerGeneric(true);
            if ( !pRenderBufferManager || !createDefaultRenderBuffer())
            {
                ShutdownHAL();
                return false;
            }
        }

        for (unsigned i = 0; i < VertexShaderDesc::VS_Count; i++)
        {
            if (VertexShaderDesc::Descs_[i] && VertexShaderDesc::Descs_[i]->pBinary)
            {
                if (!StaticVShaders[i].Init(pDevice, VertexShaderDesc::Descs_[i]))
                    return false;
            }
        }
    
        // Only initialize the filter shaders.
        for (unsigned i = FragShaderDesc::FS_start_shadows; i < FragShaderDesc::FS_Count; i++)
        {
            if (FragShaderDesc::Descs[i] && FragShaderDesc::Descs[i]->pBinary)
            {
                if ( !StaticFShaders[i].Init(pDevice, FragShaderDesc::Descs[i]) )
                    return false;
            }
        }

        // Success.
        if ( pTextureManager && pRenderBufferManager )
        {
            HALState|= HS_ModeSet;
            notifyHandlers(HAL_Initialize);
            return true;
        }
    }

    // Failure.
    pDevice->Release();
    pDeviceContext->Release();
    pDevice = 0;
    pDeviceContext = 0;
    return false;
}

// Returns back to original mode (cleanup)
bool HAL::ShutdownHAL()
{
    if (!(HALState & HS_ModeSet))
        return true;
    notifyHandlers(HAL_Shutdown);

    for (unsigned i = 0; i < VertexShaderDesc::VS_Count; i++)
        if (VertexShaderDesc::Descs_[i] && VertexShaderDesc::Descs_[i]->pBinary)
            StaticVShaders[i].Shutdown();

    for (unsigned i = 0; i < FragShaderDesc::FS_Count; i++)
        if (FragShaderDesc::Descs[i] && FragShaderDesc::Descs[i]->pBinary)
            StaticFShaders[i].Shutdown();

    // Destroy device states.
    destroyBlendStates();
    destroyDepthStencilStates();
    destroyRasterStates();
    destroyConstantBuffers();

    // Release the views, if they exist.
    pRenderTargetView = 0;
    pDepthStencilView = 0;

    destroyRenderBuffers();
    pRenderBufferManager.Clear();


    // Do TextureManager::Reset to ensure shutdown on the current thread.
    pTextureManager->Reset();
    pTextureManager.Clear();
    SManager.Reset();
    FSManager.Reset();
    Cache.Reset();

    pDeviceContext->Release();
    pDeviceContext  = 0;

    pDevice->Release();
    pDevice         = 0;

    // Remove ModeSet and other state flags.
    HALState = 0;    
    return true;
}

void HAL::PrepareForReset()
{
    SF_ASSERT(HALState & HS_ModeSet);
    if (HALState & HS_ReadyForReset)
        return;

    // Release the default rendertarget, and depth stencil references.
    if ( RenderTargetStack.GetSize() > 0 )
    {
        RenderTargetEntry& entry = RenderTargetStack[0];
        if ( entry.pRenderTarget )
        {
            SF_ASSERT( entry.pRenderTarget->GetType() == RBuffer_Default);
            D3D1x::HALData* phd = (D3D1x::HALData*)entry.pRenderTarget->GetHALData();
            if ( phd && phd->pRenderSurface )
            {
                phd->pRenderSurface->Release();
                phd->pRenderSurface = 0;
            }
            if ( phd && phd->pDSSurface )
            {
                phd->pDSSurface->Release();
                phd->pDSSurface = 0;
            }
        }
    }
    
    notifyHandlers(HAL_PrepareForReset);
    HALState |= HS_ReadyForReset;
}

bool HAL::RestoreAfterReset()
{
    if (!IsInitialized())
        return false;
    if (!(HALState & HS_ReadyForReset))
        return true;

    // Reobtain the default rendertarget and depthstencil references.
    if ( RenderTargetStack.GetSize() > 0 )
    {
        RenderTargetEntry& entry = RenderTargetStack[0];

        ID3D1x(RenderTargetView)* rtView;
        ID3D1x(DepthStencilView)* dsView;
        pDeviceContext->OMGetRenderTargets(1, &rtView, &dsView);

        HALData* phd = 0;
        if ( entry.pRenderTarget && rtView)
        {
            SF_ASSERT( entry.pRenderTarget->GetType() == RBuffer_Default);
            phd = (D3D1x::HALData*)entry.pRenderTarget->GetHALData();
            SF_ASSERT( phd->pRenderSurface == 0 );
            phd->pRenderSurface = rtView;
            if ( dsView )
                phd->pDSSurface = dsView;
        }

        // Note: No Release called on resource views, it acts as an AddRef on them,
        // which the HALData class expects.
    }

    notifyHandlers(HAL_RestoreAfterReset);
    HALState &= ~HS_ReadyForReset;
    return true;
}

// ***** Rendering

bool HAL::BeginFrame()
{
    FrameMarker.Begin(__FUNCTIONW__);
    return Render::HAL::BeginFrame();
}

void HAL::EndFrame()
{
    Render::HAL::EndFrame();
    FrameMarker.End();
}

bool HAL::BeginScene()
{
    // Maker for the whole scene, and for all the calls within this function.
    SceneMarker.Begin(__FUNCTIONW__);
    PixMarker event(__FUNCTIONW__ L" - SetState");

    if (!checkState(HS_InFrame, __FUNCTION__))
        return false;
    Profiler.SetProfileViews(NextProfileMode);
    HALState |= HS_InScene;

    // Get the rendertarget and depth surfaces we will render the scene to.
    pDeviceContext->OMGetRenderTargets(1, &pRenderTargetView.GetRawRef(), &pDepthStencilView.GetRawRef());

    // Blending render states.
    applyBlendMode(Blend_None);      

    pDeviceContext->OMSetDepthStencilState(DepthStencilStates[StencilMode_Disabled], 0);
    CurrentConstantBuffer = 0;

    // Start the scene
    SManager.BeginScene();
    return true;
}

void HAL::EndScene()
{
    if (!checkState(HS_InFrame|HS_InScene, __FUNCTION__))
        return;    

    // Flush all rendering on EndScene.
    Flush();

    SManager.EndScene();

    // Release the views.
    pRenderTargetView = 0;
    pDepthStencilView = 0;

    HALState &= ~HS_InScene;

    SceneMarker.End();
}



void HAL::beginDisplay(BeginDisplayData* data)
{
    DisplayMarker.Begin(__FUNCTIONW__);
    pDeviceContext->RSSetState(RasterStates[RasterMode]);
    Render::HAL::beginDisplay(data);
}

void HAL::endDisplay()
{
    Render::HAL::endDisplay();
    DisplayMarker.End();
}

// Updates D3D HW Viewport and ViewportMatrix based on provided viewport
// and view rectangle.
void HAL::updateViewport()
{
    D3D1x(VIEWPORT) vp;
    Rect<int>      vpRect;

    if (HALState & HS_ViewValid)
    {
        int dx = ViewRect.x1 - VP.Left,
            dy = ViewRect.y1 - VP.Top;
        
        // Modify HW matrix and viewport to clip.
        CalcHWViewMatrix(&Matrices.View2D, ViewRect, dx, dy);
        Matrices.SetUserMatrix(Matrices.User);
        Matrices.ViewRect    = ViewRect;
        Matrices.UVPOChanged = 1;

        /*
        // TBD: Prepend UserMatrix here is incorrect for nested viewport-based
        // mask clipping; what's needed is a complex combination of viewport and
        // coordinate adjustment. Until this is done, mask viewport clipping will be
        // in the wrong place for UserMatrix.
        if (UserMatrix != Matrix2F::Identity)
        {
            Rect<int> viewportRect;
            Rect<int> userViewRect(
                ViewRect.x1 + (int)UserMatrix.Tx(),
                ViewRect.y1 + (int)UserMatrix.Ty(),
                Size<int>((int)(UserMatrix.Sx() * (float)ViewRect.Width()),
                          (int)(UserMatrix.Sy() * (float)ViewRect.Height())));

            VP.GetClippedRect(&viewportRect);
            viewportRect.IntersectRect(&vpRect, userViewRect);
        }
        */

        if ( !(HALState & HS_InRenderTarget))
            vpRect = ViewRect;
        else
            vpRect.SetRect(VP.Left, VP.Top, VP.Left + VP.Width, VP.Top + VP.Height);
    }

    vp.TopLeftX = D3D10_11((UINT),(FLOAT))vpRect.x1;
    vp.TopLeftY = D3D10_11((UINT),(FLOAT))vpRect.y1;
    vp.Width    = D3D10_11((UINT),(FLOAT))vpRect.Width();
    vp.Height   = D3D10_11((UINT),(FLOAT))vpRect.Height();

    // DX can't handle a vp with zero area.
    vp.Width  = Alg::Max<D3D10_11(UINT,FLOAT)>(vp.Width, 1);
    vp.Height = Alg::Max<D3D10_11(UINT,FLOAT)>(vp.Height, 1);

    vp.MinDepth = 0.0f;
    vp.MaxDepth = 0.0f;
    pDeviceContext->RSSetViewports(1, &vp);
}


void HAL::CalcHWViewMatrix(Matrix* pmatrix,
                           const Rect<int>& viewRect, int dx, int dy)
{
    float       vpWidth = (float)viewRect.Width();
    float       vpHeight= (float)viewRect.Height();

    pmatrix->SetIdentity();
    pmatrix->Sx() = 2.0f  / vpWidth;
    pmatrix->Sy() = -2.0f / vpHeight;
    pmatrix->Tx() = -1.0f - pmatrix->Sx() * ((float)dx); 
    pmatrix->Ty() = 1.0f  - pmatrix->Sy() * ((float)dy);
}


}}} // Scaleform::Render::D3D1x
