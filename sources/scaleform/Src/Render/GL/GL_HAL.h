/**************************************************************************

Filename    :   GL_HAL.h
Content     :   Renderer HAL Prototype header.
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_Render_GL_HAL_H
#define INC_SF_Render_GL_HAL_H

#include "Render/Render_HAL.h"
#include "GL_MeshCache.h"
#include "GL_Shader.h"
#include "GL_Texture.h"

namespace Scaleform { namespace Render { namespace GL {
#define SF_RENDER_GL_SHADER_ON_DEMAND

// HALConfigFlags enumeration defines system-specific HAL configuration
// flags passed into InitHAL though HALInitParams.
enum HALConfigFlags
{
    // Only compile shaders when they are actually used. This can reduce startup-times,
    // however, compiling shaders dynamically can cause performance spikes during playback.
    HALConfig_DynamicShaderCompile  = 0x00000001,
};

// GL::HALInitParams provides OpenGL-specific rendering initialization
// parameters for HAL::InitHAL.

struct HALInitParams : public Render::HALInitParams
{
    HALInitParams(UInt32 halConfigFlags = 0,
                  ThreadId renderThreadId = ThreadId())
    : Render::HALInitParams(0, halConfigFlags, renderThreadId)
    { }

    // GL::TextureManager accessors for correct type.
    void            SetTextureManager(TextureManager* manager) { pTextureManager = manager; }
    TextureManager* GetTextureManager() const       { return (TextureManager*) pTextureManager.GetPtr(); }
};


enum CapFlags
{
    Cap_Align           = MVF_Align,

    Cap_NoBatching      = 0x10,

    // Caps for buffers in mesh cache. Static buffers are still used even if none of these caps are set.
    Cap_MapBuffer       = 0x20,
    Cap_BufferUpdate    = 0x40,
    Cap_UseMeshBuffers  = Cap_MapBuffer | Cap_BufferUpdate,

    // Caps for shaders. 
    Cap_NoDynamicLoops  = 0x80, // profile does not support dynamic looping.

    Caps_Standard       = Cap_MapBuffer
};

class HAL : public Render::HAL
#ifdef SF_GL_RUNTIME_LINK
    , public GL::Extensions
#endif
{
public:
    bool                MultiBitStencil;
    UInt32		        FillFlags;
    int                 EnabledVertexArrays;
    bool                FilterVertexBufferSet;

    MeshCache            Cache;

    ShaderManager            SManager;
    ShaderObject             StaticShaders[FragShaderDesc::FS_Count * 2];
    ShaderInterface          ShaderData;
    Ptr<TextureManager>      pTextureManager;
    
    // Previous batching mode
    PrimitiveBatch::BatchType PrevBatchType;

    unsigned                Caps;

    // Self-accessor used to avoid constructor warning.
    HAL*      GetHAL() { return this; }

public:    
    

    HAL(ThreadCommandQueue* commandQueue = 0);
    virtual ~HAL();   

    // *** HAL Initialization and Shutdown

    // Initializes HAL for rendering.
    virtual bool        InitHAL(const GL::HALInitParams& params);

    // ShutdownHAL shuts down rendering, releasing resources allocated in InitHAL.
    virtual bool        ShutdownHAL();


    // Used when the current gl context is lost (GLES on some platforms)
    // or becomes incompatible with the new video mode
    virtual bool        ResetContext();

    // *** Rendering

    virtual bool        BeginFrame();
    virtual bool        BeginScene();
    virtual void        EndScene();

    // Bracket the displaying of a frame from a movie.
    // Fill the background color, and set up default transforms, etc.
    virtual void        beginDisplay(BeginDisplayData* data);
    virtual void        endDisplay();

    void                CalcHWViewMatrix(unsigned VPFlags, Matrix* pmatrix, const Rect<int>& viewRect,
                                         int dx, int dy);

    // Updates HW Viewport and ViewportMatrix based on the current
    // values of VP, ViewRect and ViewportValid.
    virtual void        updateViewport();


    // Creates / Destroys mesh and DP data 

    virtual PrimitiveFill*  CreatePrimitiveFill(const PrimitiveFillData& data);    

    virtual void        DrawProcessedPrimitive(Primitive* pprimitive,
                                               PrimitiveBatch* pstart, PrimitiveBatch *pend);

    virtual void        DrawProcessedComplexMeshes(ComplexMesh* p,
                                                   const StrideArray<HMatrix>& matrices);
    
    // *** Mask Support
    virtual void    PushMask_BeginSubmit(MaskPrimitive* primitive);    
    virtual void    EndMaskSubmit();
    virtual void    PopMask();

    void    drawMaskClearRectangles(const HMatrix* matrices, UPInt count);
    virtual void    clearSolidRectangle(const Rect<int>& r, Color color);

    bool SetVertexArray(const VertexFormat* pFormat, GLuint buffer, UByte* vertexOffset);


    // *** BlendMode
    virtual void       applyBlendMode(BlendMode mode, bool sourceAc = false, bool forceAc = false);



    virtual Render::TextureManager* GetTextureManager() const
    {
        return pTextureManager.GetPtr();
    }

    virtual RenderTarget*   CreateRenderTarget(GLuint fbo);
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
                                    const VertexFormat** batch, const VertexFormat** instanced, unsigned meshType);

    const ShaderObject* GetStaticShader(unsigned index, bool is3d);

    void     DrawFilter(const Matrix2F& mvp, const Cxform & cx, const Filter* filter, Ptr<RenderTarget> * targets, 
                        unsigned* shaders, unsigned pass, unsigned passCount, const VertexFormat* pvf, 
                        BlurFilterState& leBlur, bool isLastPass);


protected:
    ImageSize           getFboInfo(GLint fbo, GLint& currentFBO, bool useCurrent);

    // Initializes all shaders, unless HALConfig_DynamicShaderCompile is set, or force is true.
    // If compilation of any shader fails, returns false, in which case the Prog member is set to zero.
    bool                initializeShaders(bool force = false);

    // Returns whether the profile can render any of the filters contained in the FilterPrimitive.
    // If a profile does not support dynamic looping (Cap_NoDynamicLoops), no blur/shadow type filters
    // can be rendered, in which case this may return false, however, ColorMatrix filters can still be rendered.
    bool                shouldRenderFilters(const FilterPrimitive* prim) const;

};

//--------------------------------------------------------------------
// HALData, used for both RenderTargets and DepthStencilSurface implementations.
class HALData : public RenderBuffer::HALData
{
public:
    friend class HAL;

    HAL*                    pHAL;
    GLuint                  FBOID;                  // Framebuffer object's ID.

    static void UpdateData( RenderBuffer* buffer, HAL* phal, GLuint fboID, DepthStencilBuffer* pdsb)
    {
        if ( !buffer )
            return;

        HALData* poldHD = (GL::HALData*)buffer->GetHALData();
        if ( !poldHD )
        {
            poldHD = SF_NEW HALData(buffer, phal, fboID, pdsb);
            buffer->SetHALData(poldHD);
            return;
        }
        poldHD->pDepthStencilBuffer = pdsb;
    }

    ~HALData();
    HAL* GetHAL() const     { return pHAL; }

private:
    HALData( RenderBuffer* buffer, HAL* hal, GLuint fboID, DepthStencilBuffer* pdsb ) : 
       RenderBuffer::HALData(buffer, pdsb), pHAL(hal), FBOID(fboID)
    { }
};


inline UByte*  MeshBuffer::GetBufferBase() const
{
    return (pHal->Caps & Cap_UseMeshBuffers) ? 0 : BufferData;
}

}}} // Scaleform::Render::GL

#endif
