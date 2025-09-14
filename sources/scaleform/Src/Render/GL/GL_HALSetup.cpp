/**************************************************************************

Filename    :   GL_HALSetup.cpp
Content     :   Renderer HAL Prototype header.
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#include "GL_HAL.h"
#include "Kernel/SF_Debug.h"
#include "Kernel/SF_HeapNew.h"
#include "Render/Render_BufferGeneric.h"

namespace Scaleform { namespace Render { namespace GL {


// *** RenderHAL_GL Implementation
    
extern bool CheckExtension (const char *exts, const char *name);

bool HAL::InitHAL(const GL::HALInitParams& params)
{
    if ( !Render::HAL::initHAL(params))
        return false;

    // Clear the error stack.
    glGetError();

#ifdef SF_GL_RUNTIME_LINK
    Extensions::Init();
#endif

#if defined(GL_ES_VERSION_2_0)
    const char *glexts = (const char *) glGetString(GL_EXTENSIONS);
    const char *ren = (const char*) glGetString(GL_RENDERER);

    if (CheckExtension(glexts, "GL_OES_mapbuffer"))
        Caps |= Cap_MapBuffer;

    // Adreno GPU/driver, needs aligned vertices
    if (!strncmp(ren, "Adreno", 6))
        Caps |= MVF_AlignVertexStride|Cap_NoBatching;

    SF_DEBUG_MESSAGE1(1, "GL_VENDOR     = %s", (const char*)glGetString(GL_VENDOR));
    SF_DEBUG_MESSAGE1(1, "GL_RENDERER   = %s", (const char*)glGetString(GL_RENDERER));
    SF_DEBUG_MESSAGE1(1, "GL_EXTENSIONS = %s", (const char*)glGetString(GL_EXTENSIONS));
#else
    Caps = Caps_Standard;
#endif
    // Check if platform supports dynamic looping. If it doesn't, can't support blur-type filter shaders.
    if ( !ShaderData.GetDynamicLoopSupport() )
        Caps |= Cap_NoDynamicLoops;

    SManager.SetCaps(Caps);

    pTextureManager = params.GetTextureManager();
    if (!pTextureManager)
    {
        pTextureManager = 
            *SF_HEAP_AUTO_NEW(this) TextureManager(params.RenderThreadId, pRTCommandQueue);
    }
    pTextureManager->Initialize(this);

    pRenderBufferManager = params.pRenderBufferManager;
    if (!pRenderBufferManager)
    {
        pRenderBufferManager = *SF_HEAP_AUTO_NEW(this) RenderBufferManagerGeneric();
        if ( !pRenderBufferManager || !createDefaultRenderBuffer())
        {
            ShutdownHAL();
            return false;
        }
    }


    if (!initializeShaders() ||
        !Cache.Initialize(this))
        return false;

    HALState|= HS_ModeSet;    
    notifyHandlers(HAL_Initialize);
    return true;
}

// Returns back to original mode (cleanup)
bool HAL::ShutdownHAL()
{
    if (!(HALState & HS_ModeSet))
        return true;
    notifyHandlers(HAL_Shutdown);

    destroyRenderBuffers();
    pRenderBufferManager.Clear();
    pTextureManager.Clear();
    Cache.Reset();

    // Remove ModeSet and other state flags.
    HALState = 0;    

    // Destroy the shader programs as well.
    for (unsigned i = 0; i < FragShaderDesc::FS_Count*2; i++)
        StaticShaders[i].Shutdown();

    return true;
}

bool HAL::ResetContext()
{
    notifyHandlers(HAL_PrepareForReset);
    pTextureManager->NotifyLostContext();
    Cache.Reset(true);

    ShaderData.ResetContext();
    pTextureManager->Initialize(this);
    pTextureManager->RestoreAfterLoss();

#ifdef SF_GL_RUNTIME_LINK
    Extensions::Init();
#endif
    
    if (!initializeShaders() ||
        !Cache.Initialize(this))
        return false;

    if (pRenderBufferManager)
        pRenderBufferManager->Reset();

    notifyHandlers(HAL_RestoreAfterReset);
    return true;
}

  
// ***** Rendering

bool HAL::BeginFrame()
{
    // Clear the error state.
    glGetError();
    return Render::HAL::BeginFrame();
}

// Set states not changed in our rendering, or that are reset after changes
bool HAL::BeginScene()
{
    if (!checkState(HS_InFrame, __FUNCTION__))
        return false;
    Profiler.SetProfileViews(NextProfileMode);
    HALState |= HS_InScene;

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

#ifndef GL_ES_VERSION_2_0
    glDisable(GL_ALPHA_TEST);
#endif
    glStencilMask(0xffffffff);

    // Reset vertex array usage (in case it changed between frames).
    EnabledVertexArrays = -1;
    GLint va;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &va);
    for (int i = 0; i < va; i++)
        glDisableVertexAttribArray(i);

    return true;
}

void HAL::EndScene()
{
    if (!checkState(HS_InFrame|HS_InScene, __FUNCTION__))
        return;    

    // Flush all rendering on EndScene.
    Flush();

    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    HALState &= ~HS_InScene;
}


void HAL::beginDisplay(BeginDisplayData* data)
{
    glEnable(GL_BLEND);
    applyBlendMode(Blend_None);
    glDisable(GL_STENCIL_TEST);

    Render::HAL::beginDisplay(data);
}

void HAL::endDisplay()
{
    Render::HAL::endDisplay();
}

// Updates HW Viewport and ViewportMatrix based on provided viewport
// and view rectangle.
void HAL::updateViewport()
{
    Viewport vp;

    if (HALState & HS_ViewValid)
    {
        int dx = ViewRect.x1 - VP.Left,
            dy = ViewRect.y1 - VP.Top;
        
        // Modify HW matrix and viewport to clip.
        CalcHWViewMatrix(VP.Flags, &Matrices.View2D, ViewRect, dx, dy);
        Matrices.SetUserMatrix(Matrices.User);
        Matrices.ViewRect    = ViewRect;
        Matrices.UVPOChanged = 1;

        if ( HALState & HS_InRenderTarget )
        {
            glViewport(VP.Left, VP.Top, VP.Width, VP.Height);
            glDisable(GL_SCISSOR_TEST);
        }
        else
        {
            vp.Left     = ViewRect.x1;
            vp.Top      = ViewRect.y1;
            vp.Width    = ViewRect.Width();
            vp.Height   = ViewRect.Height();
            glViewport(vp.Left, VP.BufferHeight-vp.Top-vp.Height, vp.Width, vp.Height);
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
    }
    else
    {
        glViewport(0,0,0,0);
    }
}

void HAL::CalcHWViewMatrix(unsigned VPFlags, Matrix* pmatrix, const Rect<int>& viewRect, int dx, int dy)
{
    float       vpWidth = (float)viewRect.Width();
    float       vpHeight= (float)viewRect.Height();

    pmatrix->SetIdentity();
    if (VPFlags & Viewport::View_IsRenderTexture)
    {
        pmatrix->Sx() = 2.0f  / vpWidth;
        pmatrix->Sy() = 2.0f /  vpHeight;
        pmatrix->Tx() = -1.0f - pmatrix->Sx() * ((float)dx); 
        pmatrix->Ty() = -1.0f - pmatrix->Sy() * ((float)dy);
    }
    else
    {
        pmatrix->Sx() = 2.0f  / vpWidth;
        pmatrix->Sy() = -2.0f / vpHeight;
        pmatrix->Tx() = -1.0f - pmatrix->Sx() * ((float)dx); 
        pmatrix->Ty() = 1.0f  - pmatrix->Sy() * ((float)dy);
    }
}

}}} // Scaleform::Render::GL
