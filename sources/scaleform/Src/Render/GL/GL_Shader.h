/**************************************************************************

Filename    :   GL_Shader.h
Content     :   
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_GL_Shader_H
#define INC_SF_GL_Shader_H

#include "Render/GL/GL_Common.h"

#include "Render/Render_Shader.h"
#include "Render/Render_FiltersLE.h"

#if defined(GL_ES_VERSION_2_0)
#include "Render/GL/GLES_Shaders.h"
#else
#include "Render/GL/GL_Shaders.h"
#endif

namespace Scaleform { namespace Render { namespace GL {

class HAL;
class Texture;
                              

struct ShaderObject
{
    HAL*                    pHal;
    const VertexShaderDesc* pVDesc;
    const FragShaderDesc*   pFDesc;
    GLuint                  Prog;
    GLint                   Uniforms[Uniform::SU_Count];
    UniformVar              AllUniforms[Uniform::SU_Count];
    int                     Attributes[8]; // XXX

    ShaderObject()
    {
        Prog = 0;
        pVDesc = 0;
        pFDesc = 0;
    }
    ~ShaderObject();

    bool Init(HAL* phal, FragShaderType fs, int vsoffset);
    void Shutdown( );

    inline  HAL* GetHAL() const { return pHal; }
};

struct ShaderPair
{
    const ShaderObject*     pShaderObj;
    const VertexShaderDesc* pVDesc;
    const FragShaderDesc*   pFDesc;
        
    ShaderPair() : pShaderObj(0), pVDesc(0), pFDesc(0) {}
        
    operator bool() const { return pShaderObj && pVDesc && pFDesc; }
    const ShaderPair* operator->() const { return this; }
};

typedef BlurFilterShaderDesc<GLint> BlurFilterShader;

class ShaderInterface : public ShaderInterfaceBase<Uniform,ShaderPair>
{
    HAL*       pHal;
    ShaderPair CurShader;

    inline  HAL* GetHAL() const { return pHal; }

public:
    typedef ShaderPair Shader;

    ShaderInterface(HAL* phal): DynamicLoops(-1) 
    { 
        pHal = phal; 
    }
    ~ShaderInterface() { ResetContext(); }

    const Shader&       GetCurrentShaders() const { return CurShader; }
    bool                SetStaticShader(VertexShaderDesc::ShaderType vshader, FragShaderDesc::ShaderType shader, const VertexFormat* pvf);

    void                SetTexture(Shader sp, unsigned stage, Render::Texture* ptexture, ImageFillMode fm);

    void                Finish(unsigned batchCount = 0);

    // Returns the maximum number of uniform vectors required to render a single element in a batch or instance.
    static unsigned     GetMaximumRowsPerInstance();
    // Returns the number of rows per instance for the given VertexShaderDesc (0 if not batched).
    static unsigned     GetRowsPerInstance(const VertexShaderDesc* pvdesc);
    // Returns the number of entries per instance of the given uniform type.
    static unsigned     GetCountPerInstance(const VertexShaderDesc* pvdesc, Uniform::UniformType arrayType);
    bool                GetDynamicLoopSupport();

    const BlurFilterShader* GetBlurShader(const BlurFilterShaderKey& key);

    void                ResetContext();

protected:
    int                 DynamicLoops;        // Cached value from GetDynamicLoopSupport.
    static unsigned     MaxRowsPerInstance;  // Cached value from GetMaximumRowsPerInstance.

    typedef Hash<BlurFilterShaderKey, const BlurFilterShader*> BlurShadersHash;
    BlurShadersHash BlurShaders;
};

typedef StaticShaderManager<FragShaderDesc, VertexShaderDesc, Uniform, ShaderInterface, Texture> StaticShaderManagerType;
class ShaderManager : public StaticShaderManagerType
{
public:
    ShaderManager(ProfileViews* prof) : StaticShaderManagerType(prof), Caps(0) {}
    void SetCaps(unsigned caps) { Caps = caps; }

    // Uses either standard GetFilterPasses or BlurFilterState::Setup if non-looping mode is needed.
    unsigned SetupFilter(const Filter* filter, unsigned fillFlags, unsigned* passes, BlurFilterState& leBlur) const;

protected:
    unsigned Caps;  // See Render::GL::CapFlags, set in InitHAL.
};

}}}

#endif
