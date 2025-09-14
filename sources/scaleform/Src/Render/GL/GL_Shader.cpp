/**************************************************************************

Filename    :   GL_Shader.cpp
Content     :   
Created     :   
Authors     :   

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#include "Render/GL/GL_Shader.h"
#include "Render/GL/GL_HAL.h"
#include "Kernel/SF_Debug.h"

#if defined(GL_ES_VERSION_2_0)
#include "Render/GL/GLES_ExtensionMacros.h"
#include "Render/GL/GLES_Shaders.cpp"
#else
#include "Render/GL/GL_ExtensionMacros.h"
#include "Render/GL/GL_Shaders.cpp"
#endif

#include "Render/Render_FiltersLE.h"
#include "Kernel/SF_MsgFormat.h"

namespace Scaleform { namespace Render { namespace GL {

extern const char* ShaderUniformNames[Uniform::SU_Count];
unsigned ShaderInterface::MaxRowsPerInstance = 0;

// Overwrites the array count of a variable. Assumes that the psrcPtr is the start of the number, and there
// is an ending ], and that the number of digits will be equal to or less than the current number.
void overwriteArrayCount(char* psrcPtr, unsigned newCount)
{
    if ( !psrcPtr )
        return;
    char* endPtr = SFstrchr(psrcPtr, ']');
    if ( !endPtr )
        return;

    char tempNumber[16];
    SFsprintf(tempNumber, 16, "%d", newCount);
    unsigned c =0;
    while(psrcPtr < endPtr)
    {
        if (tempNumber[c])
            *psrcPtr++ = tempNumber[c++];
        else
            *psrcPtr++ = ' ';
    }
}

bool ShaderObject::Init(HAL* phal, FragShaderType fs, int vsoffset)
{
    pHal = phal;
    if (Prog)
    {
        glDeleteProgram(Prog);
        Prog = 0;
    }

    int vs = FragShaderDesc::VShaderForFShader[fs] + vsoffset;
    if ( vs >= VertexShaderDesc::VS_Count )
        return false;
    
    pVDesc = VertexShaderDesc::Descs[vs];
    pFDesc = FragShaderDesc::Descs[fs];

    if ( !pVDesc || !pFDesc )
        return false;

    // By default, the batch shaders are compiled with a batch count of 30. However, depending
    // on the maximum number of uniforms supported, this may not be possible, and the shader source
    // will not compile. Thus, we need to modify the incoming source, so it can compile.
    const char * vdescpSource = pVDesc->pSource;
    char modifiedShaderSource[4096];
    if ( pVDesc->Flags & Shader_Batched )
    {
        GLint maxUniforms = 0;
#if !defined(GL_ES_VERSION_2_0)
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxUniforms);
#else
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxUniforms);
#endif
        SF_ASSERT(maxUniforms >= 128); // should be guaranteed by both GL and GLES.
        unsigned maxInstances = Alg::Min<unsigned>(SF_RENDER_GL_INSTANCE_MATRICES, 
            maxUniforms / ShaderInterface::GetMaximumRowsPerInstance());
        if ( maxInstances != SF_RENDER_GL_INSTANCE_MATRICES)
        {
			static bool batchCountWarning = false;
			if ( !batchCountWarning )
			{
				SF_DEBUG_WARNING4(1, "For the default batch count of %d, %d uniforms are required."
							  "System only supports %d uniforms, batch count will be reduced to %d\n",
							  SF_RENDER_GL_INSTANCE_MATRICES, SF_RENDER_GL_INSTANCE_MATRICES *
							  ShaderInterface::GetMaximumRowsPerInstance(), maxUniforms,
							  maxInstances);
				batchCountWarning = true; // only show this warning once.
			}
							  
            vdescpSource = modifiedShaderSource;
            UPInt originalLength = SFstrlen(pVDesc->pSource);
            SF_ASSERT(originalLength < 4096);
            SFstrncpy(modifiedShaderSource, 4096, pVDesc->pSource, originalLength);
            modifiedShaderSource[originalLength] = 0;
            const char *matSearch = "vfmuniforms[", *vecSearch = "vfuniforms[";
            char * matFind = strstr(modifiedShaderSource, matSearch);
            if (matFind)
            {
                unsigned matSize = ShaderInterface::GetCountPerInstance(pVDesc, Uniform::SU_vfmuniforms) * maxInstances;
                overwriteArrayCount( matFind + SFstrlen(matSearch), matSize);
            }
            char * vecFind = strstr(modifiedShaderSource, vecSearch);
            if (vecFind)
            {
                unsigned vecSize = ShaderInterface::GetCountPerInstance(pVDesc, Uniform::SU_vfuniforms) * maxInstances;
                overwriteArrayCount( vecFind + SFstrlen(vecSearch), vecSize);
            }
        }
    }
    

    GLint result;
    GLuint vp = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vp, 1, const_cast<const char**>(&vdescpSource), 0);
    glCompileShader(vp);
    glGetShaderiv(vp, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLchar msg[512];
        glGetShaderInfoLog(vp, sizeof(msg), 0, msg);
        SF_DEBUG_ERROR2(1, "%s: %s\n", vdescpSource, msg);
        glDeleteShader(vp);
        return 0;
    }
    Prog = glCreateProgram();
    glAttachShader(Prog, vp);
    GLuint fp = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fp, 1, const_cast<const char**>(&pFDesc->pSource), 0);
    glCompileShader(fp);
    glGetShaderiv(fp, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLchar msg[1512];
        glGetShaderInfoLog(fp, sizeof(msg), 0, msg);
        SF_DEBUG_ERROR2(1, "%s: %s\n", pFDesc->pSource, msg);
        glDeleteShader(fp);
        glDeleteProgram(Prog);
        Prog = 0;
        return 0;
    }
    glAttachShader(Prog, fp);

    for (int i = 0; i < pVDesc->NumAttribs; i++)
        glBindAttribLocation(Prog, i, pVDesc->Attributes[i].Name);

    glLinkProgram(Prog);
    glDeleteShader(vp);
    glDeleteShader(fp);
    glGetProgramiv(Prog, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLchar msg[512];
        glGetProgramInfoLog(Prog, sizeof(msg), 0, msg);
        SF_DEBUG_ERROR1(1, "link: %s\n", msg);
        glDeleteProgram(Prog);
    	Prog = 0;
    }

    for (unsigned i = 0; i < Uniform::SU_Count; i++)
    {
        if (pFDesc->Uniforms[i].Location >= 0 || pVDesc->Uniforms[i].Location >= 0)
        {
            Uniforms[i] = glGetUniformLocation(Prog, ShaderUniformNames[i]);
            if (Uniforms[i] < 0)
            {
                // Some frag shaders contain scolor2, but only reference it during Highlight, so it
                // is sometimes compiled out of the constant table. This should be fixed, but is not crucial.
                if ( i == Uniform::SU_scolor2 )
                    continue;
                SF_DEBUG_MESSAGE3(1, "Shader uniform: %s expected, "
                    "but not found (fs=%d, vs=%d)\n", ShaderUniformNames[i], fs, vs);
                SF_DEBUG_MESSAGE1(1, "Vertex Shader:\n------------\n"
                    "%s\n------------\n", vdescpSource);
                SF_DEBUG_MESSAGE1(1, "Pixel Shader:\n------------\n"
                    "%s\n------------\n", pFDesc->pSource);
                continue;
            }


            if (pFDesc->Uniforms[i].Location >= 0)
            {
                SF_ASSERT(pVDesc->Uniforms[i].Location < 0);
                AllUniforms[i] = pFDesc->Uniforms[i];
            }
            else
                AllUniforms[i] = pVDesc->Uniforms[i];
        }
        else
            Uniforms[i] = -1;
    }
    return result != 0;
}

void ShaderObject::Shutdown( )
{
    if (Prog)
        glDeleteProgram(Prog);

    Prog   = 0;
    pVDesc = 0;
    pFDesc = 0;
    pHal   = 0;
}

ShaderObject::~ShaderObject()
{
    Shutdown();
}

bool ShaderInterface::SetStaticShader(VertexShaderDesc::ShaderType vshader, FragShaderDesc::ShaderType shader, const VertexFormat*)
{
    bool is3d = (vshader == FragShaderDesc::VShaderForFShader[shader] + VertexShaderDesc::VS_base_Position3d);
    CurShader.pShaderObj = pHal->GetStaticShader(shader, is3d);
    if ( !CurShader.pShaderObj || !CurShader.pShaderObj->Prog )
    {
        CurShader.pVDesc = 0;
        CurShader.pFDesc = 0;
        glUseProgram(0);
        return false;
    }
    CurShader.pVDesc = CurShader.pShaderObj->pVDesc;
    CurShader.pFDesc = CurShader.pShaderObj->pFDesc;
    glUseProgram(CurShader.pShaderObj->Prog);
    return true;
}

void ShaderInterface::SetTexture(Shader, unsigned stage, Render::Texture* ptex, ImageFillMode fm)
{
    GL::Texture* ptexture = (GL::Texture*)ptex;
    GLint minfilter = (fm.GetSampleMode() == Sample_Point) ? GL_NEAREST : (ptexture->MipLevels>1 ? GL_LINEAR_MIPMAP_LINEAR  : GL_LINEAR);
    GLint magfilter = (fm.GetSampleMode() == Sample_Point) ? GL_NEAREST : GL_LINEAR;
    GLint address = (fm.GetWrapMode() == Wrap_Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    for (unsigned plane = 0; plane < ptexture->GetTextureStageCount(); plane++)
    {
        int stageIndex = stage + plane;

        glActiveTexture(GL_TEXTURE0 + stageIndex);
        glBindTexture(GL_TEXTURE_2D, ptexture->pTextures[plane].TexId);
        SF_ASSERT(CurShader.pShaderObj->pFDesc->TexParams[stageIndex] >= 0 ); // Expected texture uniform does not exist in this shader.
        glUniform1i(CurShader.pShaderObj->Uniforms[(int)CurShader.pShaderObj->pFDesc->TexParams[stageIndex]], stageIndex);

        if (ptexture->LastMinFilter != minfilter || ptexture->LastAddress != address)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);              
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, address);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, address);

            ptexture->LastMinFilter = minfilter;
            ptexture->LastAddress = address;
        }
    }
}

void ShaderInterface::Finish(unsigned batchCount)
{
    const ShaderObject* pCurShader = CurShader.pShaderObj;
    for (int var = 0; var < Uniform::SU_Count; var++)
        if (UniformSet[var])
        {
            unsigned size;
            if (pCurShader->AllUniforms[var].BatchSize > 0)
                size = batchCount * pCurShader->AllUniforms[var].BatchSize;
            else if (pCurShader->AllUniforms[var].ElementSize)
                size = pCurShader->AllUniforms[var].Size / pCurShader->AllUniforms[var].ElementSize;
            else
                continue;

            switch (pCurShader->AllUniforms[var].ElementCount)
            {
                case 16:
                    glUniformMatrix4fv(pCurShader->Uniforms[var], size, false /* transpose */,
                        UniformData + pCurShader->AllUniforms[var].ShadowOffset);
                    break;
                case 4:
                    glUniform4fv(pCurShader->Uniforms[var], size,
                        UniformData + pCurShader->AllUniforms[var].ShadowOffset);
                    break;
                case 3:
                    glUniform3fv(pCurShader->Uniforms[var], size,
                        UniformData + pCurShader->AllUniforms[var].ShadowOffset);
                    break;
                case 2:
                    glUniform2fv(pCurShader->Uniforms[var], size,
                        UniformData + pCurShader->AllUniforms[var].ShadowOffset);
                    break;
                case 1:
                    glUniform1fv(pCurShader->Uniforms[var], size,
                        UniformData + pCurShader->AllUniforms[var].ShadowOffset);
                    break;

                default:
                    SF_ASSERT(0);
            }
        }

    memset(UniformSet, 0, Uniform::SU_Count);
}

unsigned ShaderInterface::GetMaximumRowsPerInstance()
{
    // Check for cached value. This should not change between runs. TBD: precalculate.
    if ( MaxRowsPerInstance == 0 )
    {
        // Note: this assumes that batch variables are stored in shader descs.
        MaxRowsPerInstance = 0;
        for ( unsigned desc = 0; desc < VertexShaderDesc::VS_Count; ++desc )
        {
            const VertexShaderDesc* pvdesc = VertexShaderDesc::Descs[desc];
            MaxRowsPerInstance = Alg::Max(MaxRowsPerInstance, GetRowsPerInstance(pvdesc));
        }
    }
    return MaxRowsPerInstance;
}

bool ShaderInterface::GetDynamicLoopSupport()
{
    // Check cached value. -1 indicates not calculated yet.
    if ( DynamicLoops < 0 )
    {
        // Just try to compile a shader we know has dynamic loops, and see if it fails.
        DynamicLoops = pHal->StaticShaders[FragShaderDesc::FS_start_blurs].Init(pHal, FragShaderDesc::FS_start_blurs, 0 ) ? 1 : 0;            
    }
    return DynamicLoops ? true : false;
}

unsigned ShaderInterface::GetRowsPerInstance( const VertexShaderDesc* pvdesc )
{
    // Desc doesn't exist, or isn't batched, don't consider it.
    if ( !pvdesc || (pvdesc->Flags & Shader_Batched) == 0 )
        return 0;

    unsigned currentUniforms = 0;
    for ( unsigned uniform = 0; uniform < Uniform::SU_Count; ++uniform )
    {
        if ( pvdesc->BatchUniforms[uniform].Size > 0 )
        {
            currentUniforms += pvdesc->BatchUniforms[uniform].Size *
                (pvdesc->BatchUniforms[uniform].Array == Uniform::SU_vfmuniforms ? 4 : 1);
        }
    }
    return currentUniforms;
}

// Returns the number of entries per instance of the given uniform type.
unsigned ShaderInterface::GetCountPerInstance(const VertexShaderDesc* pvdesc, Uniform::UniformType arrayType)
{
    // Desc doesn't exist, or isn't batched, don't consider it.
    if ( !pvdesc || (pvdesc->Flags & Shader_Batched) == 0 )
        return 0;

    unsigned currentUniforms = 0;
    for ( unsigned uniform = 0; uniform < Uniform::SU_Count; ++uniform )
    {
        if ( pvdesc->BatchUniforms[uniform].Size > 0 )
        {
            if ( pvdesc->BatchUniforms[uniform].Array == arrayType )
                currentUniforms += pvdesc->BatchUniforms[uniform].Size;
        }
    }
    return currentUniforms;
}


unsigned ShaderManager::SetupFilter(const Filter* filter, unsigned fillFlags, unsigned* passes, BlurFilterState& leBlur) const
{
    // If we don't support dynamic loops only allow color matrix filters (which don't have dynamic loops).
    if ( (Caps & Cap_NoDynamicLoops) == 0 || filter->GetFilterType() == Filter_ColorMatrix )
    {
        leBlur.Passes = 0;
        return StaticShaderManagerType::GetFilterPasses(filter, fillFlags, passes);
    }

    if (leBlur.Setup(filter))
        return leBlur.Passes;

    return 0;
}

void ShaderInterface::ResetContext()
{
    for (BlurShadersHash::Iterator It = BlurShaders.Begin(); It != BlurShaders.End(); ++It)
        delete It->Second;

    BlurShaders.Clear();
}

const BlurFilterShader* ShaderInterface::GetBlurShader(const BlurFilterShaderKey& params)
{
    const BlurFilterShader*const* psh = BlurShaders.Get(params);
    if (psh)
        return *psh;

    StringBuffer vsrc, fsrc;

    fsrc.AppendString(
#if defined(GL_ES_VERSION_2_0)
        "precision mediump float;\n"
#endif
        "uniform sampler2D tex;\n"
        "uniform vec2 texscale;\n"
        "uniform vec4 cxmul;\n"
        "uniform vec4 cxadd;\n"
        "uniform float samples;\n");

    vsrc.AppendString("uniform vec4 mvp[2];\nuniform vec2 texscale;\n");
    for (int i = 0; i < params.TotalTCs; i++)
    {
        SPrintF(vsrc, "attribute vec2 intc%d;\n", i);
        SPrintF(vsrc, "varying   vec2 tc%d;\n", i);
        SPrintF(fsrc, "varying   vec2 tc%d;\n", i);
    }

    if (params.Mode & Filter_Shadow)
    {
        vsrc.AppendString("uniform vec2 srctexscale;\n");

        fsrc.AppendString("uniform vec4 scolor;\n"
                          "uniform sampler2D srctex;\n");

        if (params.Mode & BlurFilterParams::Mode_Highlight)
        {
            fsrc.AppendString("uniform vec4 scolor2;\n");
        }
    }

    vsrc.AppendString("attribute vec4 pos;\n"
                      "void main(void)\n{\n"
                      "  vec4 opos = pos;\n"
                      "  opos.x = dot(pos, mvp[0]);\n"
                      "  opos.y = dot(pos, mvp[1]);\n"
                      "  gl_Position = opos;\n");
    for (int i = 0; i < params.TotalTCs-params.BaseTCs; i++)
        SPrintF(vsrc, "  tc%d = intc%d * texscale;\n", i, i);
    for (int i = params.TotalTCs-params.BaseTCs; i < params.TotalTCs; i++)
        SPrintF(vsrc, "  tc%d = intc%d * srctexscale;\n", i, i);

    vsrc.AppendString("\n}");

    fsrc.AppendString("void main(void)\n{\n"
        "  vec4 color = vec4(0);\n");

    if (params.Mode & Filter_Shadow)
    {
        const char *color;
        if (params.Mode & BlurFilterParams::Mode_Highlight)
        {
            for (int i = 0; i < params.BoxTCs; i++)
            {
                SPrintF(fsrc, "  color.a += texture2D(tex, tc%d).a;\n", i*2);
                SPrintF(fsrc, "  color.r += texture2D(tex, tc%d).a;\n", i*2+1);
            }
            color = "(scolor * color.a + scolor2 * color.r)";
        }
        else
        {
            for (int i = 0; i < params.BoxTCs; i++)
                SPrintF(fsrc, "  color += texture2D(tex, tc%d);\n", i);
            color = "(scolor * color.a)";
        }

        SPrintF(fsrc,   "  color *= samples;\n");

        if (params.Mode & BlurFilterParams::Mode_HideObject)
            SPrintF(fsrc, "  gl_FragColor = %s;}\n", color);
        else
        {
            SPrintF(fsrc,   "  vec4 base = texture2D(srctex, tc%d);\n", params.TotalTCs-1);

            if (params.Mode & BlurFilterParams::Mode_Inner)
            {
                if (params.Mode & BlurFilterParams::Mode_Highlight)
                {
                    fsrc.AppendString("  color.ar = clamp((1.0 - color.ar) - (1.0 - color.ra) * 0.5, 0.0,1.0);\n");
                    fsrc.AppendString("  color = (scolor * (color.a) + scolor2 * (color.r)\n"
                        "           + base * (1.0 - color.a - color.r)) * base.a;\n");
                }
                else if (params.Mode & BlurFilterParams::Mode_Knockout)
                {
                    fsrc.AppendString("  color = scolor * (1-color.a) * base.a;\n");
                }
                else
                    fsrc.AppendString("  color = mix(scolor, base, color.a) * base.a;\n");

                fsrc.AppendString("  gl_FragColor = color * cxmul + cxadd * color.a;\n}");
            }
            else
            {
                SPrintF  (fsrc, "  color = %s * (1.0-base.a) + base;\n", color);

                if (params.Mode & BlurFilterParams::Mode_Knockout)
                {
                    fsrc.AppendString("  color *= (1.0 - base.a);\n"
                        "  gl_FragColor = color * cxmul + cxadd * color.a;\n}");
                }
                else
                    fsrc.AppendString("  gl_FragColor = color * cxmul + cxadd * color.a;\n}");
            }
        }
    }
    else
    {
        for (int i = 0; i < params.BoxTCs; i++)
            SPrintF(fsrc, "  color += texture2D(tex, tc%d);\n", i);
        SPrintF(fsrc,       "  color = color * samples;\n");
        fsrc.AppendString(    "  gl_FragColor = color * cxmul + cxadd * color.a;\n}\n");
    }

    const GLchar* pvsource = (const GLchar*)(vsrc.ToCStr());
    const GLchar* pfsource = (const GLchar*)(fsrc.ToCStr());

    //SF_DEBUG_MESSAGE2(1, "Blur shader\n%s\n\n%s\n\n", pvsource, pfsource);

    GLuint vp = glCreateShader(GL_VERTEX_SHADER);
    GLint result;
    glShaderSource(vp, 1, &pvsource, 0);
    glCompileShader(vp);
    glGetShaderiv(vp, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLchar msg[512];
        glGetShaderInfoLog(vp, sizeof(msg), 0, msg);
        SF_DEBUG_ERROR2(1, "%s: %s\n", pvsource, msg);
        glDeleteShader(vp);
        return 0;
    }
    GLint prog = glCreateProgram();
    glAttachShader(prog, vp);
    GLuint fp = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fp, 1, &pfsource, 0);
    glCompileShader(fp);
    glGetShaderiv(fp, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLchar msg[1512];
        glGetShaderInfoLog(fp, sizeof(msg), 0, msg);
        SF_DEBUG_ERROR2(1, "%s: %s\n", pfsource, msg);
        glDeleteShader(fp);
        glDeleteProgram(prog);
        prog = 0;
        return 0;
    }
    glAttachShader(prog, fp);

    glBindAttribLocation(prog, 0, "pos");
    for (int i = 0; i < params.TotalTCs; i++)
    {
        char tc[16];
        SPrintF(tc, "intc%d", i);
        glBindAttribLocation(prog, 1+i, tc);
    }

    glLinkProgram(prog);
    glDeleteShader(vp);
    glDeleteShader(fp);
    glGetProgramiv(prog, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLchar msg[512];
        glGetProgramInfoLog(prog, sizeof(msg), 0, msg);
        SF_DEBUG_ERROR3(1, "link:\n%s\n%s %s\n", pvsource, pfsource, msg);
        glDeleteProgram(prog);
        return 0;
    }

    BlurFilterShader* pfs = new BlurFilterShader;
    pfs->Shader = prog;

    pfs->mvp = glGetUniformLocation(prog, "mvp");
    pfs->cxmul = glGetUniformLocation(prog, "cxmul");
    pfs->cxadd = glGetUniformLocation(prog, "cxadd");
    pfs->tex[0] = glGetUniformLocation(prog, "tex");
    pfs->tex[1] = glGetUniformLocation(prog, "srctex");
    pfs->texscale[0] = glGetUniformLocation(prog, "texscale");
    pfs->texscale[1] = glGetUniformLocation(prog, "srctexscale");
    pfs->samples = glGetUniformLocation(prog, "samples");

    pfs->scolor[0] = glGetUniformLocation(prog, "scolor");
    pfs->scolor[1] = glGetUniformLocation(prog, "scolor2");

    BlurShaders.Add(params, pfs);
    return pfs;
}

}}}
