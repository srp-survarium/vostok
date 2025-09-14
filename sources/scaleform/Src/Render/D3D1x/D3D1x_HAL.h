/**************************************************************************

Filename    :   D3D1x_HAL.h
Content     :   
Created     :   Mar 2011
Authors     :   Bart Muzzin

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_D3D1X_HAL_H
#define INC_SF_D3D1X_HAL_H
#pragma once

#include "Render/D3D1x/D3D1x_Config.h"
#include "Render/Render_HAL.h"
#include "Render/D3D1x/D3D1x_MeshCache.h"
#include "Render/D3D1x/D3D1x_ShaderManager.h"
#include "Render/D3D1x/D3D1x_Shader.h"
#include "Render/D3D1x/D3D1x_Texture.h"

namespace Scaleform { namespace Render { namespace D3D1x {

#define SF_RENDERER_VSHADER_PROFILE "vs_4_0"

//------------------------------------------------------------------------
enum HALConfigFlags
{
    // Only compile shaders when they are actually used. This can reduce startup-times,
    // however, compiling shaders dynamically can cause performance spikes during playback.
    HALConfig_DynamicShaderCompile  = 0x00000001,
};

// D3D1x::HALInitParems provides D3D10/D3D11 specific rendering initialization
// parameters for HAL::InitHAL.

struct HALInitParams : public Render::HALInitParams
{
    ID3D1x(Device)*               pD3DDevice;
    D3D11(ID3D1x(DeviceContext)* pD3DContext);

    HALInitParams(ID3D1x(Device)* device,
                  D3D11(ID3D1x(DeviceContext)* pD3DContext,)
                  UInt32 halConfigFlags = 0,
                  ThreadId renderThreadId = ThreadId())
    : pD3DDevice(device),
      D3D11(pD3DContext(pD3DContext),)
      Render::HALInitParams(0, halConfigFlags, renderThreadId)
    { }

    // D3D1x::TextureManager accessors for correct type.
    void SetTextureManager(TextureManager* manager) { pTextureManager = manager; }
    TextureManager* GetTextureManager() const       { return (TextureManager*) pTextureManager.GetPtr(); }
};


//------------------------------------------------------------------------

class HAL : public Render::HAL
{
public:

    // Direct3D Device we are using.
    ID3D1x(Device)*               pDevice;
    ID3D1x(DeviceContext)*        pDeviceContext;
    Ptr<ID3D1x(RenderTargetView)> pRenderTargetView;
    Ptr<ID3D1x(DepthStencilView)> pDepthStencilView;

	UInt16				FillFlags;

    MeshCache            Cache;

    ShaderManager            SManager;
    Ptr<TextureManager>      pTextureManager;
    FilterShaderManager      FSManager;
    FragShader               StaticFShaders[FragShaderDesc::FS_Count];
    VertexShader             StaticVShaders[VertexShaderDesc::VS_Count];
    ShaderInterface          ShaderData;
    
    // Previous batching mode
    PrimitiveBatch::BatchType PrevBatchType;

    // Self-accessor used to avoid constructor warning.
    HAL*      getThis() { return this; }

public:    
    
    HAL(ThreadCommandQueue* commandQueue = 0);
    virtual ~HAL();
    
    // *** HAL Initialization / Shutdown Logic

    // Initializes HAL for rendering.
    virtual bool        InitHAL(const D3D1x::HALInitParams& params);
    // ShutdownHAL shuts down rendering, releasing resources allocated in InitHAL.
    virtual bool        ShutdownHAL();

    // - PrepareForReset should be called before the device's SwapChain is resized.
    // The HAL holds a reference to the default back and depthstencil buffers, which need
    // to be released and reobtained.
    void                PrepareForReset();
    // - RestoreAfterReset called after reset to restore needed variables.
    bool                RestoreAfterReset();


    // *** Rendering

    virtual bool        BeginFrame();
    virtual void        EndFrame();
    virtual bool        BeginScene();
    virtual void        EndScene();

    // Bracket the displaying of a frame from a movie.
    // Fill the background color, and set up default transforms, etc.
    virtual void        beginDisplay(BeginDisplayData* data);
    virtual void        endDisplay();

    void                CalcHWViewMatrix(Matrix* pmatrix, const Rect<int>& viewRect,
                                         int dx, int dy);

    // Updates D3D HW Viewport and ViewportMatrix based on the current
    // values of VP, ViewRect and ViewportValid.
    virtual void        updateViewport();


    // Creates / Destroys mesh and DP data 

    virtual PrimitiveFill*  CreatePrimitiveFill(const PrimitiveFillData& data);    

    virtual void        DrawProcessedPrimitive(Primitive* pprimitive,
                                               PrimitiveBatch* pstart, PrimitiveBatch *pend);
    template< class MatrixType >
    void                drawProcessedPrimitive(Primitive* pprimitive,
                                               PrimitiveBatch* pstart, PrimitiveBatch *pend);

    virtual void        DrawProcessedComplexMeshes(ComplexMesh* complexMesh,
                                                   const StrideArray<HMatrix>& matrices);

    template< class MatrixType >
    static void         calculateTransform(const Matrix & m, const HMatrix& hm, const MatrixState & mstate, float (* dest)[4]);  

    template< class MatrixUpdateAdapter >
    void                applyMatrixConstants(const MatrixUpdateAdapter & input );
    void                applyRawMatrixConstants(const Matrix& m, const Cxform& cx);
    void                applyRawMatrixConstants(const Matrix& m, const Cxform& cx, const Matrix& tm);

    // *** Mask Support
    // This flag indicates whether we've checked for stencil after BeginDisplay or not.
    bool            StencilChecked;
    // This flag is stencil is available, after check.
    bool            StencilAvailable;
    bool            DepthBufferAvailable;    

    virtual void    PushMask_BeginSubmit(MaskPrimitive* primitive);
    virtual void    EndMaskSubmit();
    virtual void    PopMask();

    virtual void    beginMaskDisplay()
    {
        SF_ASSERT(MaskStackTop == 0);
        StencilChecked  = 0;
        StencilAvailable= 0;
        DepthBufferAvailable = 0;
        HALState &= ~HS_DrawingMask;
    }

    bool    checkMaskBufferCaps();
    void    drawMaskClearRectangles(const HMatrix* matrices, UPInt count);

    // Background clear helper, expects viewport coordinates.
    virtual void    clearSolidRectangle(const Rect<int>& r, Color color);


    // *** BlendMode
    void            applyBlendMode(BlendMode mode, bool sourceAc = false, bool forceAc = false);


    // *** Device States
    enum ColorWriteMode
    {
        Write_All,
        Write_None,
        Write_Count
    };

    static const unsigned BlendTypeCount = Blend_Count*2 + 1;
    static unsigned GetBlendType( BlendMode blendMode, ColorWriteMode writeMode, bool sourceAc = false) 
    { 
        unsigned base = 0;
        if ( writeMode == Write_None )
        {
            base = Blend_Count*2;
            blendMode = (BlendMode)0;
        }
        else if ( sourceAc )
            base += Blend_Count;
        return base + (unsigned)blendMode;
    }
    ID3D1x(BlendState)* BlendStates[BlendTypeCount];
    bool createBlendStates();
    void destroyBlendStates();

    enum StencilModes
    {
        StencilMode_Disabled,
        StencilMode_Available_ClearMasks,
        StencilMode_Available_ClearMasksAbove,
        StencilMode_Available_WriteMask,
        StencilMode_DepthOnly_WriteMask,
        StencilMode_Available_TestMask,
        StencilMode_DepthOnly_TestMask,
        StencilMode_Unavailable,
        StencilMode_Count
    };
    ID3D1x(DepthStencilState)* DepthStencilStates[StencilMode_Count];
    bool createDepthStencilStates();
    void destroyDepthStencilStates();

    enum RasterModes
    {
        RasterMode_Default,
        RasterMode_Wireframe,
        RasterMode_Count
    };
    RasterModes RasterMode;
    void SetRasterMode(RasterModes mode) { RasterMode = mode; }
    ID3D1x(RasterizerState)* RasterStates[RasterMode_Count];
    bool createRasterStates();
    void destroyRasterStates();


    ID3D1x(Device)*   GetDevice() const { return pDevice; }

    static const unsigned   ConstantBufferCount = 8;
    ID3D1x(Buffer)*         ConstantBuffers[ConstantBufferCount];
    unsigned                CurrentConstantBuffer;

    bool                  createConstantBuffers();
    void                  destroyConstantBuffers();
    ID3D1x(Buffer)*       getNextConstantBuffer();

    virtual Render::TextureManager* GetTextureManager() const
    {
        return pTextureManager.GetPtr();
    }

    virtual RenderTarget*   CreateRenderTarget(Render::Texture* texture, bool needsStencil);
    virtual RenderTarget*   CreateTempRenderTarget(const ImageSize& size, bool needsStencil);
    virtual bool            SetRenderTarget(RenderTarget* target, bool setState = 1);
    virtual void            PushRenderTarget(const RectF& frameRect, RenderTarget* prt);
    virtual void            PopRenderTarget();

    virtual bool            createDefaultRenderBuffer();

    // *** Filters
    virtual void          PushFilters(FilterPrimitive* primitive);
    virtual void          PopFilters();
    virtual void          drawUncachedFilter(const FilterStackEntry& e);
    virtual void          drawCachedFilter(FilterPrimitive* primitive);

    virtual class MeshCache&       GetMeshCache()        { return Cache; }
        
    virtual void    MapVertexFormat(PrimitiveFillType fill, const VertexFormat* sourceFormat,
                                    const VertexFormat** single,
                                    const VertexFormat** batch, const VertexFormat** instanced, unsigned)
    {
        return SManager.MapVertexFormat(fill, sourceFormat,
                                        single, batch, instanced);
    }
};

//--------------------------------------------------------------------
// HALData, used for both RenderTargets and DepthStencilSurface implementations.
class HALData : public RenderBuffer::HALData
{
public:
    friend class HAL;
    ID3D1x(View)*           pRenderSurface;         // View of the render target.
    ID3D1x(View)*           pDSSurface;             // View of the depth stencil surface (0 if not required).

    static void UpdateData( RenderBuffer* buffer, ID3D1x(View)* prt, 
                            DepthStencilBuffer* pdsb, ID3D1x(View)* pdss )
    {
        if ( !buffer )
            return;

        HALData* poldHD = (D3D1x::HALData*)buffer->GetHALData();
        if ( !poldHD )
        {
            poldHD = SF_NEW HALData(buffer, prt, pdsb, pdss);
            buffer->SetHALData(poldHD);
            return;
        }

        if ( prt )
            prt->AddRef();
        if ( pdss )
            pdss->AddRef();
        if ( poldHD->pRenderSurface ) 
            poldHD->pRenderSurface->Release();
        if ( poldHD->pDSSurface ) 
            poldHD->pDSSurface->Release();

        poldHD->pDepthStencilBuffer = pdsb;
        poldHD->pDSSurface          = pdss;
        poldHD->pRenderSurface      = prt;
    }

    virtual ~HALData()
    {
        if ( pRenderSurface )
            pRenderSurface->Release();
        if ( pDSSurface )
            pDSSurface->Release();
    }

private:
    HALData( RenderBuffer* buffer, ID3D1x(View)* prt, DepthStencilBuffer* pdsb, ID3D1x(View)* pdss) : 
      RenderBuffer::HALData(buffer, pdsb), pRenderSurface(prt), pDSSurface(pdss)
    {
        if ( pRenderSurface )
            pRenderSurface->AddRef();
        if ( pDSSurface )
            pDSSurface->AddRef();
    }
};

#if !defined(SF_BUILD_SHIPPING)
// Used to create heirarchies in PIX captures.
typedef INT (WINAPI * LPD3DPERF_BEGINEVENT)(D3DCOLOR, LPCWSTR);
typedef INT (WINAPI * LPD3DPERF_ENDEVENT)(void);
#endif // SF_BUILD_SHIPPING

class PixMarker
{
public:
    PixMarker( LPCWSTR eventName, bool trigger = true )
    {
#if !defined(SF_BUILD_SHIPPING)
        // Colors aren't shown in PIX.
        if ( trigger )
            Begin(eventName);
#else
        SF_UNUSED2(eventName, trigger);       
#endif
    }
    void Begin( LPCWSTR eventName)
    {
#if !defined(SF_BUILD_SHIPPING)
        if ( BeginEventFn )
            BeginEventFn(D3DCOLOR_XRGB(0,0,0), eventName);
#else
        SF_UNUSED(eventName);       
#endif
    }
    void End()
    {
#if !defined(SF_BUILD_SHIPPING)
        if ( EndEventFn )
            EndEventFn();
#endif
    }

    ~PixMarker( )
    {
        End();
    }

#if !defined(SF_BUILD_SHIPPING)
    static LPD3DPERF_BEGINEVENT  BeginEventFn;
    static LPD3DPERF_ENDEVENT    EndEventFn;
#endif
};

}}} // Scaleform::Render::D3D1x

#endif // INC_SF_D3D1X_HAL_H
