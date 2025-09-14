/**************************************************************************

Filename    :   D3D9_HAL.h
Content     :   Renderer HAL Prototype header.
Created     :   May 2009
Authors     :   Michael Antonov

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_Render_D3D9_HAL_H
#define INC_SF_Render_D3D9_HAL_H

#include "Render/Render_HAL.h"
#include "Render/D3D9/D3D9_MeshCache.h"
#include "Render/D3D9/D3D9_Texture.h"
#include "Render/D3D9/D3D9_ShaderManager.h"
#include "Render/D3D9/D3D9_Shader.h" // FilterShaderManager

#include <d3d9.h>

namespace Scaleform { namespace Render { namespace D3D9 {


// HALConfigFlags enumeration defines system-specific HAL configuration
// flags passed into InitHAL though HALInitParams.

enum HALConfigFlags
{
    // Prevents BeginScene/EndScene from being called inside BeginDisplay/EndDisplay
    // - assumes that Direct3D is already in scene by the time BeginDisplay/EndDisplay are called
    // - allows user to manage their own begin/end scene calls 
    HALConfig_NoSceneCalls          = 0x00000001,

    // Use D3DUSAGE_DYNAMIC updaeable textures for compatibility with some broken drivers
    HALConfig_UseDynamicTex         = 0x00000004,

    // Only compile shaders when they are actually used. This can reduce startup-times,
    // however, compiling shaders dynamically can cause performance spikes during playback.
    HALConfig_DynamicShaderCompile  = 0x00000008,
};


// D3D9::HALInitParems provides D3D9-specific rendering initialization
// parameters for HAL::InitHAL.

struct HALInitParams : public Render::HALInitParams
{
    IDirect3DDeviceX*       pD3DDevice;
    D3DPRESENT_PARAMETERS   PresentParams;

    HALInitParams(IDirect3DDeviceX* device,
                  const D3DPRESENT_PARAMETERS& presentParams,
                  UInt32 halConfigFlags = 0,
                  ThreadId renderThreadId = ThreadId())
    : pD3DDevice(device), PresentParams(presentParams),
      Render::HALInitParams(0, halConfigFlags, renderThreadId)
    {
    }

    // D3D9::TextureManager accessors for correct type.
    void SetTextureManager(TextureManager* manager) { pTextureManager = manager; }
    TextureManager* GetTextureManager() const       { return (TextureManager*) pTextureManager.GetPtr(); }
};



class HAL : public Render::HAL
{
public:

    // Direct3D Device we are using.
    IDirect3DDeviceX*   pDevice;
	UInt16				FillFlags;

    // Presentation parameters specified to configure the mode.
    D3DPRESENT_PARAMETERS   PresentParams;

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

    // Initialize rendering
    virtual bool        InitHAL(const D3D9::HALInitParams& params);
    // Shutdown rendering (cleanup).
    virtual bool        ShutdownHAL();

    // D3D9 device Reset and lost device support.
    // - PrepareForReset should be called before IDirect3DDevice9::Reset to release
    // caches and other system-specific references.
    void                PrepareForReset();
    // - RestoreAfterReset called after reset to restore needed variables.
    bool                RestoreAfterReset();


    // *** Rendering

    virtual bool        BeginFrame();
    virtual void        EndFrame();
    virtual bool        BeginScene();
    virtual void        EndScene();

    // Bracket the displaying of a frame from a movie (executed from render queue).
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

    virtual void        DrawProcessedComplexMeshes(ComplexMesh* p,
                                                   const StrideArray<HMatrix>& matrices);

    template< class MatrixUpdateAdapter >
    void                applyMatrixConstants(const MatrixUpdateAdapter & input);

    void                applyRawMatrixConstants(const Matrix& m, const Cxform& cx);
    void                applyRawMatrixConstants(const Matrix& m, const Cxform& cx, const Matrix& tm);

    template< class MatrixType >
    static void         calculateTransform(const Matrix & m, const HMatrix& hm, 
                                           const MatrixState & mstate, float (* dest)[4]);

    
    // Stream Source modification

    inline void         setLinearStreamSource(PrimitiveBatch::BatchType type)
    {
        if (PrevBatchType >= PrimitiveBatch::DP_Instanced)
        {
            pDevice->SetStreamSource(1, NULL, 0, 0);
            pDevice->SetStreamSourceFreq(0, 1);
            pDevice->SetStreamSourceFreq(1, 1);
        }
        PrevBatchType = type;
    }

    inline void         setInstancedStreamSource(unsigned instanceCount)
    {
        SF_ASSERT(SManager.HasInstancingSupport());
        if (PrevBatchType != PrimitiveBatch::DP_Instanced)
        {
            pDevice->SetStreamSource(1, Cache.pInstancingVertexBuffer.GetPtr(),
                                     0, sizeof(Render_InstanceData));
            pDevice->SetStreamSourceFreq(1, (UINT) D3DSTREAMSOURCE_INSTANCEDATA | 1);
            PrevBatchType = PrimitiveBatch::DP_Instanced;
        }                
        pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | instanceCount);
    }


    // *** Mask Support
    // This flag indicates whether we've checked for stencil after BeginDisplay or not.
    bool            StencilChecked;
    // This flag is stencil is available, after check.
    bool            StencilAvailable;
    bool            MultiBitStencil;
    bool            DepthBufferAvailable;    
    // Increment op we need for stencil.
    D3DSTENCILOP    StencilOpInc;    

    virtual void    PushMask_BeginSubmit(MaskPrimitive* primitive);
    virtual void    EndMaskSubmit();
    virtual void    PopMask();

    virtual void    beginMaskDisplay()
    {
        SF_ASSERT(MaskStackTop == 0);
        StencilChecked  = 0;
        StencilAvailable= 0;
        MultiBitStencil = 0;
        DepthBufferAvailable = 0;
        HALState &= ~HS_DrawingMask;
    }

    bool    checkMaskBufferCaps();
    void    drawMaskClearRectangles(const HMatrix* matrices, UPInt count);

    // Background clear helper, expects viewport coordinates.
    virtual void        clearSolidRectangle(const Rect<int>& r, Color color);


    // *** BlendMode
    virtual void        applyBlendMode(BlendMode mode, bool sourceAc = false, bool forceAc = false);



    IDirect3DDeviceX*   GetDevice() const { return pDevice; }


    virtual Render::TextureManager* GetTextureManager() const
    {
        return pTextureManager.GetPtr();
    }

    virtual RenderTarget*   CreateRenderTarget(IDirect3DSurface9* pcolor, IDirect3DSurface9* pdepth);
    virtual RenderTarget*   CreateRenderTarget(Render::Texture* texture, bool needsStencil);
    virtual RenderTarget*   CreateTempRenderTarget(const ImageSize& size, bool needsStencil);
    virtual bool            SetRenderTarget(RenderTarget* target, bool setState = 1);
    virtual void            PushRenderTarget(const RectF& frameRect, RenderTarget* prt);
    virtual void            PopRenderTarget();
    
    virtual bool            createDefaultRenderBuffer();

    // *** Filters
    virtual void            PushFilters(FilterPrimitive* primitive);
    virtual void            PopFilters();
    virtual void            drawUncachedFilter(const FilterStackEntry& e);
    virtual void            drawCachedFilter(FilterPrimitive* primitive);

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
    IDirect3DSurface9*       pRenderSurface;        // Render target surface.
    IDirect3DSurface9*       pDSSurface;            // Depth stencil surface (0 if not required)

    static void UpdateData( RenderBuffer* buffer, IDirect3DSurface9* prt, 
                            DepthStencilBuffer* pdsb, IDirect3DSurface9* pdss )
    {
        if ( !buffer )
            return;

        HALData* poldHD = (D3D9::HALData*)buffer->GetHALData();
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
        poldHD->CacheID             = 0;
    }

    virtual ~HALData()
    {
        if ( pRenderSurface )
            pRenderSurface->Release();
        if ( pDSSurface )
            pDSSurface->Release();        
        pDepthStencilBuffer = 0;
    }

private:
    HALData( RenderBuffer* buffer, IDirect3DSurface9* prt, DepthStencilBuffer* pdsb, IDirect3DSurface9* pdss) : 
      RenderBuffer::HALData(buffer, pdsb), pRenderSurface(prt), pDSSurface(pdss)
    {
        if ( pRenderSurface )
            pRenderSurface->AddRef();
        if ( pDSSurface )
            pDSSurface->AddRef();        
    }
};

// Used to create heirarchies in PIX captures.
class PixMarker
{
public:
    PixMarker( LPCWSTR eventName, bool trigger = true )
    {
        // Colors aren't shown in PIX.
        if ( trigger )
            Begin(eventName);
    }
    void Begin( LPCWSTR eventName)
    {
        D3DPERF_BeginEvent(D3DCOLOR_XRGB(0,0,0), eventName);
    }
    void End()
    {
        D3DPERF_EndEvent();
    }

    ~PixMarker( )
    {
        End();
    }
};


}}} // Scaleform::Render::D3D9

#endif
