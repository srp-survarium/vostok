
#include "Render/GL/GL_Shader.h"
#include "Render/GL/GL_Shaders.h"

namespace Scaleform { namespace Render { namespace GL {

const char* pSource_FTexTG = 
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"\n"
"}";

const char* pSource_FTexTGBatch = 
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"\n"
"}";

const char* pSource_FTexTGBatchCxform = 
"uniform sampler2D tex0;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FTexTGBatchCxformEAlpha = 
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGBatchCxformEAlphaMul = 
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGBatchCxformMul = 
"uniform sampler2D tex0;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGBatchEAlpha = 
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGBatchEAlphaMul = 
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGBatchMul = 
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FTexTGCxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGCxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGEAlpha = 
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGEAlphaMul = 
"uniform sampler2D tex0;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGMul = 
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertex = 
"varying vec4 color;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"\n"
"}";

const char* pSource_FVertexBatch = 
"varying vec4 color;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"\n"
"}";

const char* pSource_FVertexBatchCxform = 
"varying vec4 color;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FVertexBatchCxformEAlpha = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexBatchCxformEAlphaMul = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexBatchCxformMul = 
"varying vec4 color;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexBatchEAlpha = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexBatchEAlphaMul = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexBatchMul = 
"varying vec4 color;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FVertexCxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexCxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexEAlpha = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexEAlphaMul = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexMul = 
"varying vec4 color;\n"
"void main() {\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTG = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatch = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchCxform = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchCxformEAlpha = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchCxformEAlphaMul = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchCxformMul = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchEAlpha = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchEAlphaMul = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGBatchMul = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FTexTGTexTGCxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGTexTGCxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGEAlpha = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGTexTGEAlphaMul = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGTexTGMul = 
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S TexTG\n"
"  fcolor1 = texture2D(tex1,tc1);\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertex = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatch = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchCxform = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchCxformEAlpha = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchCxformEAlphaMul = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchCxformMul = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchEAlpha = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchEAlphaMul = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexBatchMul = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FTexTGVertexCxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGVertexCxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexEAlpha = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FTexTGVertexEAlphaMul = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTexTGVertexMul = 
"uniform sampler2D tex0;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"  vec4 fcolor1;\n"
"  vec4 fcolor0;\n"
"// S TexTG\n"
"  fcolor0 = texture2D(tex0,tc0);\n"
"// S Vertex\n"
"  fcolor1 = color;\n"
"  gl_FragColor = mix(fcolor1, fcolor0, factor.r);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertex = 
"varying vec4 color;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"\n"
"}";

const char* pSource_FVertexVertexBatch = 
"varying vec4 color;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchCxform = 
"varying vec4 color;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchCxformEAlpha = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchCxformEAlphaMul = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchCxformMul = 
"varying vec4 color;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchEAlpha = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchEAlphaMul = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexBatchMul = 
"varying vec4 color;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FVertexVertexCxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexVertexCxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"varying vec4 color;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexEAlpha = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FVertexVertexEAlphaMul = 
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FVertexVertexMul = 
"varying vec4 color;\n"
"void main() {\n"
"  vec4 fcolor0;\n"
"// S Vertex\n"
"  gl_FragColor = color;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_VBatchPosition3dCxformVacolorVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+2.0*(255.01*float(afactor.b)))];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+1.0)];\n"
"\n"
"}";

const char* pSource_VBatchPosition3dCxformVacolor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute float vbatch;\n"
"varying vec4 color;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+2.0*vbatch)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+2.0*vbatch+1.0)];\n"
"\n"
"}";

const char* pSource_VBatchPosition3dVacolor = 
"uniform mat4 vfmuniforms[30];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute float vbatch;\n"
"varying vec4 color;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E Vacolor\n"
"  color = acolor;\n"
"\n"
"}";

const char* pSource_VBatchPosition3dVacolorVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VBatchCxformVacolorVafactor = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b)))];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+1.0)];\n"
"\n"
"}";

const char* pSource_VBatchCxformVacolor = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute float vbatch;\n"
"varying vec4 color;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*vbatch)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*vbatch+1.0)];\n"
"\n"
"}";

const char* pSource_VBatchVacolor = 
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute float vbatch;\n"
"varying vec4 color;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(1.0))]);  \n"
"// E Vacolor\n"
"  color = acolor;\n"
"\n"
"}";

const char* pSource_VBatchVacolorVafactor = 
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(1.0))]);  \n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VPosition3dCxformVacolor = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"varying vec4 color;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"\n"
"}";

const char* pSource_VPosition3dCxformVacolorVafactor = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VPosition3dVacolor = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"varying vec4 color;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vacolor\n"
"  color = acolor;\n"
"\n"
"}";

const char* pSource_VPosition3dVacolorVafactor = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGBatchPosition3d = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"\n"
"}";

const char* pSource_VTexTGBatchPosition3dCxformVacolorVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VTexTGBatchPosition3dCxformVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VTexTGBatchPosition3dCxform = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E Cxform\n"
"  ;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*vbatch+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*vbatch+3.0)];\n"
"\n"
"}";

const char* pSource_VTexTGBatchPosition3dVacolorVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGBatchPosition3dVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGBatch = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(1.0))]);  \n"
"\n"
"}";

const char* pSource_VTexTGBatchCxformVacolorVafactor = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VTexTGBatchCxformVafactor = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VTexTGBatchCxform = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+6.0*vbatch+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+6.0*vbatch+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*vbatch+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*vbatch+4.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*vbatch+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*vbatch+3.0)];\n"
"\n"
"}";

const char* pSource_VTexTGBatchVacolorVafactor = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(1.0))]);  \n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGBatchVafactor = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(1.0))]);  \n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGPosition3d = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"\n"
"}";

const char* pSource_VTexTGPosition3dCxform = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"\n"
"}";

const char* pSource_VTexTGPosition3dCxformVacolorVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGPosition3dCxformVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGPosition3dVacolorVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGPosition3dVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGTexTGBatchPosition3dCxformVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(2.0))]);\n"
"  tc1.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(2.0+1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+5.0)];\n"
"\n"
"}";

const char* pSource_VTexTGTexTGBatchPosition3dVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(2.0))]);\n"
"  tc1.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(2.0+1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGTexTGBatchCxformVafactor = 
"uniform vec4 vfuniforms[240];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+(2.0))]);\n"
"  tc1.y = dot(pos, vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+(2.0+1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+6.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+6.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+4.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+8.0*(255.01*float(afactor.b))+5.0)];\n"
"\n"
"}";

const char* pSource_VTexTGTexTGBatchVafactor = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc0.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(2.0))]);\n"
"  tc1.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(2.0+1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(1.0))]);  \n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGTexTGPosition3dCxformVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[4];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, texgen[int(0.1+2.0)]);\n"
"  tc1.y = dot(pos, texgen[int(0.1+2.0+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGTexTGPosition3dVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[4];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, texgen[int(0.1+2.0)]);\n"
"  tc1.y = dot(pos, texgen[int(0.1+2.0+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGTexTGCxformVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[4];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, texgen[int(0.1+2.0)]);\n"
"  tc1.y = dot(pos, texgen[int(0.1+2.0+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGTexTGVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[4];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"varying vec2 tc1;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// S TexTG\n"
"\n"
"  tc1.x = dot(pos, texgen[int(0.1+2.0)]);\n"
"  tc1.y = dot(pos, texgen[int(0.1+2.0+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTG = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"\n"
"}";

const char* pSource_VTexTGCxform = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"\n"
"}";

const char* pSource_VTexTGCxformVacolorVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGCxformVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGVacolorVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VTexTGVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"\n"
"  tc0.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc0.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VCxformVacolor = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"varying vec4 color;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"\n"
"}";

const char* pSource_VCxformVacolorVafactor = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VVacolor = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"varying vec4 color;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vacolor\n"
"  color = acolor;\n"
"\n"
"}";

const char* pSource_VVacolorVafactor = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec4 afactor;\n"
"varying vec4 color;\n"
"varying vec4 factor;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vacolor\n"
"  color = acolor;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowKnockout = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor = mix(scolor, base, gl_FragColor.a) * base.a;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowKnockoutHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor.ar = clamp((1.0 - gl_FragColor.ar) - (1.0 - gl_FragColor.ra) * 0.5, 0.0, 1.0);\n"
"    gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r\n"
"              + base * (1.0 - gl_FragColor.a - gl_FragColor.r)) * base.a;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowKnockoutMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor = mix(scolor, base, gl_FragColor.a) * base.a;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowKnockoutMulHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor.ar = clamp((1.0 - gl_FragColor.ar) - (1.0 - gl_FragColor.ra) * 0.5, 0.0, 1.0);\n"
"    gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r\n"
"              + base * (1.0 - gl_FragColor.a - gl_FragColor.r)) * base.a;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2InnerShadow = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor = mix(scolor, base, gl_FragColor.a) * base.a;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor.ar = clamp((1.0 - gl_FragColor.ar) - (1.0 - gl_FragColor.ra) * 0.5, 0.0, 1.0);\n"
"    gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r\n"
"              + base * (1.0 - gl_FragColor.a - gl_FragColor.r)) * base.a;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor = mix(scolor, base, gl_FragColor.a) * base.a;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2InnerShadowMulHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E InnerShadow\n"
"  vec4 base = texture2DLod(srctex, tc * srctexscale, 0.0);\n"
"      gl_FragColor.ar = clamp((1.0 - gl_FragColor.ar) - (1.0 - gl_FragColor.ra) * 0.5, 0.0, 1.0);\n"
"    gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r\n"
"              + base * (1.0 - gl_FragColor.a - gl_FragColor.r)) * base.a;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2ShadowKnockout = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = scolor * gl_FragColor.a * (1.0-base.a) + base;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2ShadowKnockoutHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r) * (1.0-base.a) + base;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2ShadowKnockoutMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = scolor * gl_FragColor.a * (1.0-base.a) + base;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2ShadowKnockoutMulHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r) * (1.0-base.a) + base;\n"
"  // E Knockout\n"
"  gl_FragColor -= base;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2Shadow = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = scolor * gl_FragColor.a * (1.0-base.a) + base;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2ShadowHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r) * (1.0-base.a) + base;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2ShadowMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = scolor * gl_FragColor.a * (1.0-base.a) + base;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2ShadowMulHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform vec4 scolor2;\n"
"uniform sampler2D srctex;\n"
"uniform vec2 srctexscale;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadow\n"
"  vec4 base = texture2D(srctex, tc * srctexscale);\n"
"\n"
"      gl_FragColor = (scolor * gl_FragColor.a + scolor2 * gl_FragColor.r) * (1.0-base.a) + base;\n"
"  // E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2Shadowonly = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadowonly\n"
"  gl_FragColor = scolor * gl_FragColor.a;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2ShadowonlyHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadowonly\n"
"  gl_FragColor = scolor * gl_FragColor.a;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2ShadowonlyMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color += texture2DLod(tex, tc + (offset + i) * texscale, 0.0);\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadowonly\n"
"  gl_FragColor = scolor * gl_FragColor.a;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2ShadowonlyMulHighlight = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform vec2 offset;\n"
"uniform vec4 scolor;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E shadow1\n"
"  {           color.a += texture2DLod(tex, tc + (offset + i) * texscale, 0.0).a;\n"
"       color.r += texture2DLod(tex, tc - (offset + i) * texscale, 0.0).a;\n"
"      }  gl_FragColor = color * fsize.w;\n"
"  gl_FragColor.a = gl_FragColor.a * fsize.z;\n"
"// E Shadowonly\n"
"  gl_FragColor = scolor * gl_FragColor.a;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_VBox2 = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"\n"
"\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"\n"
"}";

const char* pSource_FSolid = 
"uniform vec4 cxmul;\n"
"void main() {\n"
"// S Solid\n"
"  gl_FragColor = cxmul;\n"
"\n"
"}";

const char* pSource_FSolidBatch = 
"varying vec4 fucxmul;\n"
"void main() {\n"
"// S Solid\n"
"  gl_FragColor = fucxmul;\n"
"\n"
"}";

const char* pSource_FText = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vec4 c = vcolor;\n"
"  c.a = c.a * texture2D(tex0, tc).a;\n"
"  gl_FragColor = c;\n"
"\n"
"}";

const char* pSource_FTextBatch = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vec4 c = vcolor;\n"
"  c.a = c.a * texture2D(tex0, tc).a;\n"
"  gl_FragColor = c;\n"
"\n"
"}";

const char* pSource_FTextBatchMul = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vec4 c = vcolor;\n"
"  c.a = c.a * texture2D(tex0, tc).a;\n"
"  gl_FragColor = c;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTextMul = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vec4 c = vcolor;\n"
"  c.a = c.a * texture2D(tex0, tc).a;\n"
"  gl_FragColor = c;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTextColor = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"\n"
"}";

const char* pSource_FTextColorBatch = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"\n"
"}";

const char* pSource_FTextColorBatchCxform = 
"uniform sampler2D tex0;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FTextColorBatchCxformMul = 
"uniform sampler2D tex0;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTextColorBatchMul = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTextColorCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FTextColorCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FTextColorMul = 
"uniform sampler2D tex0;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// S TextColor\n"
"  gl_FragColor = texture2D(tex0,tc);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_VBatchPosition3dCxformVatc = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vatc\n"
"  tc = atc;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+2.0*vbatch)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+2.0*vbatch+1.0)];\n"
"\n"
"}";

const char* pSource_VBatchPosition3dVatc = 
"uniform mat4 vfmuniforms[30];\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E Vatc\n"
"  tc = atc;\n"
"\n"
"}";

const char* pSource_VBatchPosition3d = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[30];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+1.0*vbatch)];\n"
"\n"
"}";

const char* pSource_VBatchCxformVatc = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vatc\n"
"  tc = atc;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*vbatch)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*vbatch+1.0)];\n"
"\n"
"}";

const char* pSource_VBatchVatc = 
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(1.0))]);  \n"
"// E Vatc\n"
"  tc = atc;\n"
"\n"
"}";

const char* pSource_VBatch = 
"uniform vec4 vfuniforms[90];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxmul;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+3.0*vbatch+1.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+3.0*vbatch+1.0+(1.0))]);  \n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+3.0*vbatch)];\n"
"\n"
"}";

const char* pSource_VPosition3d = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"\n"
"}";

const char* pSource_VPosition3dCxformVatc = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vatc\n"
"  tc = atc;\n"
"\n"
"}";

const char* pSource_VPosition3dVatc = 
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vatc\n"
"  tc = atc;\n"
"\n"
"}";

const char* pSource_VTextBatchPosition3d = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec2 atc;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vcolor = acolor * vfuniforms[int(0.1+2.0*vbatch+1.0)] + vfuniforms[int(0.1+2.0*vbatch)];\n"
"  tc = atc;\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"\n"
"}";

const char* pSource_VTextBatch = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec2 atc;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vcolor = acolor * vfuniforms[int(0.1+4.0*vbatch+1.0)] + vfuniforms[int(0.1+4.0*vbatch)];\n"
"  tc = atc;\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(1.0))]);  \n"
"\n"
"}";

const char* pSource_VTextPosition3d = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform mat4 mvp;\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec2 atc;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vcolor = acolor * cxmul + cxadd;\n"
"  tc = atc;\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"\n"
"}";

const char* pSource_VText = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 acolor;\n"
"attribute vec2 atc;\n"
"varying vec2 tc;\n"
"varying vec4 vcolor;\n"
"void main() {\n"
"// S Text\n"
"  vcolor = acolor * cxmul + cxadd;\n"
"  tc = atc;\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"\n"
"}";

const char* pSource_V = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"\n"
"}";

const char* pSource_VCxformVatc = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vatc\n"
"  tc = atc;\n"
"\n"
"}";

const char* pSource_VVatc = 
"uniform vec4 mvp[2];\n"
"attribute vec4 pos;\n"
"attribute vec2 atc;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vatc\n"
"  tc = atc;\n"
"\n"
"}";

const char* pSource_FBox1Blur = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box1\n"
"  vec4 color = vec4(0.0);\n"
"  float  i = 0.0;\n"
"  for (i = -fsize.x; i <= fsize.x; i++)\n"
"// E Blur\n"
"    color += texture2DLod(tex, tc + i * texscale, 0.0);\n"
"  gl_FragColor = color * fsize.w;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox1BlurMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box1\n"
"  vec4 color = vec4(0.0);\n"
"  float  i = 0.0;\n"
"  for (i = -fsize.x; i <= fsize.x; i++)\n"
"// E Blur\n"
"    color += texture2DLod(tex, tc + i * texscale, 0.0);\n"
"  gl_FragColor = color * fsize.w;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FBox2Blur = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E Blur\n"
"    color += texture2DLod(tex, tc + i * texscale, 0.0);\n"
"  gl_FragColor = color * fsize.w;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"\n"
"}";

const char* pSource_FBox2BlurMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform vec4 fsize;\n"
"uniform sampler2D tex;\n"
"uniform vec2 texscale;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box2\n"
"  vec4 color = vec4(0.0);\n"
"  vec2 i = vec2(0.0);\n"
"  for (i.x = -fsize.x; i.x <= fsize.x; i.x++)\n"
"    for (i.y = -fsize.y; i.y <= fsize.y; i.y++)\n"
"// E Blur\n"
"    color += texture2DLod(tex, tc + i * texscale, 0.0);\n"
"  gl_FragColor = color * fsize.w;\n"
"// E cxformac\n"
"  gl_FragColor = (gl_FragColor * vec4(cxmul.rgb,1.0)) * cxmul.a + cxadd * gl_FragColor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_VBox1 = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E Box1\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"\n"
"}";

const char* pSource_FTexTGCMatrixAc = 
"uniform vec4 cxadd;\n"
"uniform mat4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E CMatrixAc\n"
"  gl_FragColor =(gl_FragColor) * (cxmul) + cxadd * (gl_FragColor.a + cxadd.a);\n"
"\n"
"}";

const char* pSource_FTexTGCMatrixAcMul = 
"uniform vec4 cxadd;\n"
"uniform mat4 cxmul;\n"
"uniform sampler2D tex0;\n"
"varying vec2 tc0;\n"
"void main() {\n"
"// S TexTG\n"
"  gl_FragColor = texture2D(tex0,tc0);\n"
"// E CMatrixAc\n"
"  gl_FragColor =(gl_FragColor) * (cxmul) + cxadd * (gl_FragColor.a + cxadd.a);\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUV = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"\n"
"}";

const char* pSource_FYUVBatch = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"\n"
"}";

const char* pSource_FYUVBatchCxform = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FYUVBatchCxformEAlpha = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVBatchCxformEAlphaMul = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchCxformMul = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchEAlpha = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVBatchEAlphaMul = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchMul = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVA = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVACxform = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVACxformEAlpha = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVACxformEAlphaMul = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVACxformMul = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * fucxmul + fucxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVAEAlpha = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVAEAlphaMul = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVBatchYUVAMul = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVCxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FYUVCxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVCxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVCxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVEAlpha = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVEAlphaMul = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVMul = 
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVYUVA = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"\n"
"}";

const char* pSource_FYUVYUVACxform = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"\n"
"}";

const char* pSource_FYUVYUVACxformEAlpha = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVYUVACxformEAlphaMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVYUVACxformMul = 
"uniform vec4 cxadd;\n"
"uniform vec4 cxmul;\n"
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Cxform\n"
"  gl_FragColor = gl_FragColor * cxmul + cxadd;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVYUVAEAlpha = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"\n"
"}";

const char* pSource_FYUVYUVAEAlphaMul = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E EAlpha\n"
"  gl_FragColor.a *= factor.a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_FYUVYUVAMul = 
"uniform sampler2D tex_a;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"uniform sampler2D tex_y;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"  float c0 = float((texture2D(tex_y, tc).a - 16./255.) * 1.164);\n"
"  float U0 = float(texture2D(tex_u, tc).a - 128./255.);\n"
"  float V0 = float(texture2D(tex_v, tc).a - 128./255.);\n"
"  vec4 c = vec4(c0,c0,c0,c0);\n"
"  vec4 U = vec4(U0,U0,U0,U0);\n"
"  vec4 V = vec4(V0,V0,V0,V0);\n"
"  c += V * vec4(1.596, -0.813, 0.0, 0.0);\n"
"  c += U * vec4(0.0, -0.392, 2.017, 0.0);\n"
"  c.a = 1.0;\n"
"  gl_FragColor = c;\n"
"// E YUVA\n"
"  gl_FragColor.a = texture2D(tex_a, tc).a;\n"
"// E Mul\n"
"  gl_FragColor = mix(vec4(1.0), gl_FragColor, vec4(gl_FragColor.a));\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3d = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dCxformVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dCxform = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E Cxform\n"
"  ;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*vbatch+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*vbatch+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dYUVA = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+2.0*vbatch+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E YUVA\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dYUVACxformVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dYUVACxform = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*vbatch)]);\n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+4.0*vbatch+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+4.0*vbatch+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchPosition3dYUVAVafactor = 
"uniform mat4 vfmuniforms[30];\n"
"uniform vec4 vfuniforms[60];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+2.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( vfmuniforms[int(0.1+1.0*(255.01*float(afactor.b)))]);\n"
"// E YUVA\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVBatch = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(1.0))]);  \n"
"\n"
"}";

const char* pSource_VYUVBatchCxformVafactor = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchCxform = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+6.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+6.0*vbatch+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*vbatch+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*vbatch+4.0+(1.0))]);  \n"
"// E Cxform\n"
"  ;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*vbatch+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*vbatch+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchVafactor = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(1.0))]);  \n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVBatchYUVA = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*vbatch+2.0+(1.0))]);  \n"
"// E YUVA\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVBatchYUVACxformVafactor = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+4.0+(1.0))]);  \n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*(255.01*float(afactor.b))+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchYUVACxform = 
"uniform vec4 vfuniforms[180];\n"
"attribute vec4 pos;\n"
"attribute float vbatch;\n"
"varying vec4 fucxadd;\n"
"varying vec4 fucxmul;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+6.0*vbatch+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+6.0*vbatch+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+6.0*vbatch+4.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+6.0*vbatch+4.0+(1.0))]);  \n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"// E \n"
"  fucxadd = vfuniforms[int(0.1+6.0*vbatch+2.0)];\n"
"// E \n"
"  fucxmul = vfuniforms[int(0.1+6.0*vbatch+3.0)];\n"
"\n"
"}";

const char* pSource_VYUVBatchYUVAVafactor = 
"uniform vec4 vfuniforms[120];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(0.0))]);\n"
"  tc.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+(1.0))]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(0.0))]);\n"
"  gl_Position.y = dot(pos, vfuniforms[int(0.1+4.0*(255.01*float(afactor.b))+2.0+(1.0))]);  \n"
"// E YUVA\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVPosition3d = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"\n"
"}";

const char* pSource_VYUVPosition3dCxform = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVPosition3dCxformVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVPosition3dVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVPosition3dYUVA = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E YUVA\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVPosition3dYUVACxform = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVPosition3dYUVACxformVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVPosition3dYUVAVafactor = 
"uniform mat4 mvp;\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E Position3d\n"
"\n"
"  gl_Position =(pos) * ( mvp);\n"
"// E YUVA\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUV = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"\n"
"}";

const char* pSource_VYUVCxform = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVCxformVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVYUVA = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E YUVA\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVYUVACxform = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"\n"
"}";

const char* pSource_VYUVYUVACxformVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E YUVA\n"
"  ;\n"
"// E Cxform\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";

const char* pSource_VYUVYUVAVafactor = 
"uniform vec4 mvp[2];\n"
"uniform vec4 texgen[2];\n"
"attribute vec4 pos;\n"
"attribute vec4 afactor;\n"
"varying vec4 factor;\n"
"varying vec2 tc;\n"
"void main() {\n"
"// E YUV\n"
"\n"
"  tc.x = dot(pos, texgen[int(0.1+0.0)]);\n"
"  tc.y = dot(pos, texgen[int(0.1+1.0)]);\n"
"// E position\n"
"\n"
"\n"
"  gl_Position = pos;\n"
"  gl_Position.x = dot(pos, mvp[int(0.1+0.0)]);\n"
"  gl_Position.y = dot(pos, mvp[int(0.1+1.0)]);  \n"
"// E YUVA\n"
"  ;\n"
"// E Vafactor\n"
"  factor = afactor;\n"
"\n"
"}";


const char* ShaderUniformNames[Uniform::SU_Count] = {
    "cxadd",
    "cxmul",
    "fsize",
    "mvp",
    "offset",
    "scolor",
    "scolor2",
    "srctex",
    "srctexscale",
    "tex",
    "tex0",
    "tex1",
    "tex_a",
    "tex_u",
    "tex_v",
    "tex_y",
    "texgen",
    "texscale",
    "vfmuniforms",
    "vfuniforms"
};


const unsigned char Uniform::UniformFlags[Uniform::SU_Count] = {
Uniform::Uniform_Builtin, Uniform::Uniform_Builtin, 0, Uniform::Uniform_Builtin, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Uniform::Uniform_Builtin, 0, Uniform::Uniform_Builtin, Uniform::Uniform_Builtin};

static VertexShaderDesc ShaderDesc_VS_VVatc = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}},
  /* pSource */       pSource_VVatc,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp */ {0, 0, 4, 8, 4, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex */ {-1, 0, 0, 0, 0},
                        /* tex0 */ {-1, 0, 0, 0, 0},
                        /* tex1 */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VCxformVatc = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}},
  /* pSource */       pSource_VCxformVatc,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp */ {0, 0, 4, 8, 4, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex */ {-1, 0, 0, 0, 0},
                        /* tex0 */ {-1, 0, 0, 0, 0},
                        /* tex1 */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3dVatc = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}},
  /* pSource */       pSource_VPosition3dVatc,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp */ {0, 0, 16, 16, 16, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex */ {-1, 0, 0, 0, 0},
                        /* tex0 */ {-1, 0, 0, 0, 0},
                        /* tex1 */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3dCxformVatc = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}},
  /* pSource */       pSource_VPosition3dCxformVatc,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp */ {0, 0, 16, 16, 16, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex */ {-1, 0, 0, 0, 0},
                        /* tex0 */ {-1, 0, 0, 0, 0},
                        /* tex1 */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchVatc = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchVatc,
  /* Uniforms */      { /* cxadd      */ {-1, 0, 0, 0, 0},
                        /* cxmul      */ {-1, 0, 0, 0, 0},
                        /* fsize      */ {-1, 0, 0, 0, 0},
                        /* mvp        */ {-1, 0, 0, 0, 0},
                        /* offset     */ {-1, 0, 0, 0, 0},
                        /* scolor     */ {-1, 0, 0, 0, 0},
                        /* scolor2    */ {-1, 0, 0, 0, 0},
                        /* srctex     */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex        */ {-1, 0, 0, 0, 0},
                        /* tex0       */ {-1, 0, 0, 0, 0},
                        /* tex1       */ {-1, 0, 0, 0, 0},
                        /* tex_a      */ {-1, 0, 0, 0, 0},
                        /* tex_u      */ {-1, 0, 0, 0, 0},
                        /* tex_v      */ {-1, 0, 0, 0, 0},
                        /* tex_y      */ {-1, 0, 0, 0, 0},
                        /* texgen     */ {-1, 0, 0, 0, 0},
                        /* texscale   */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {0, 0, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 0, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchCxformVatc = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchCxformVatc,
  /* Uniforms */      { /* cxadd      */ {-1, 0, 0, 0, 0},
                        /* cxmul      */ {-1, 0, 0, 0, 0},
                        /* fsize      */ {-1, 0, 0, 0, 0},
                        /* mvp        */ {-1, 0, 0, 0, 0},
                        /* offset     */ {-1, 0, 0, 0, 0},
                        /* scolor     */ {-1, 0, 0, 0, 0},
                        /* scolor2    */ {-1, 0, 0, 0, 0},
                        /* srctex     */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex        */ {-1, 0, 0, 0, 0},
                        /* tex0       */ {-1, 0, 0, 0, 0},
                        /* tex1       */ {-1, 0, 0, 0, 0},
                        /* tex_a      */ {-1, 0, 0, 0, 0},
                        /* tex_u      */ {-1, 0, 0, 0, 0},
                        /* tex_v      */ {-1, 0, 0, 0, 0},
                        /* tex_y      */ {-1, 0, 0, 0, 0},
                        /* texgen     */ {-1, 0, 0, 0, 0},
                        /* texscale   */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3dVatc = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchPosition3dVatc,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3dCxformVatc = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "atc", 2 | VET_TexCoord}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchPosition3dCxformVatc,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGCxformVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGPosition3dVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGPosition3dVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGPosition3dCxformVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGPosition3dCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchCxformVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchPosition3dVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchPosition3dVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchPosition3dCxformVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchPosition3dCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVCxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVPosition3dVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dCxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVPosition3dCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVYUVAVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVYUVAVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVYUVACxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVYUVACxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dYUVAVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVPosition3dYUVAVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dYUVACxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVPosition3dYUVACxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchCxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchPosition3dVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dCxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchPosition3dCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchYUVAVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchYUVAVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchYUVACxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchYUVACxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dYUVAVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchPosition3dYUVAVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dYUVACxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VYUVBatchPosition3dYUVACxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTG = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VTexTG,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGCxform = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VTexTGCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGPosition3d = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VTexTGPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGPosition3dCxform = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VTexTGPosition3dCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatch = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VTexTGBatch,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchCxform = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VTexTGBatchCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchPosition3d = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VTexTGBatchPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchPosition3dCxform = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VTexTGBatchPosition3dCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 16, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGCxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 16, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGPosition3dVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGPosition3dVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 16, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGPosition3dCxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGPosition3dCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 16, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGBatchVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGBatchVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 4},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGBatchCxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGBatchCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 960, 4, 8}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 4, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 5, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 6, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 4},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGBatchPosition3dVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGBatchPosition3dVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 4},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGTexTGBatchPosition3dCxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGTexTGBatchPosition3dCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 4, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 5, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 4},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VText = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "atc", 2 | VET_TexCoord}},
  /* pSource */       pSource_VText,
  /* Uniforms */      { /* cxadd       */ {0, 0, 4, 4, 4, 0},
                        /* cxmul       */ {1, 4, 4, 4, 4, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {2, 8, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTextPosition3d = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "atc", 2 | VET_TexCoord}},
  /* pSource */       pSource_VTextPosition3d,
  /* Uniforms */      { /* cxadd       */ {0, 0, 4, 4, 4, 0},
                        /* cxmul       */ {1, 4, 4, 4, 4, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {2, 8, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTextBatch = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    4,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "atc", 2 | VET_TexCoord}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VTextBatch,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTextBatchPosition3d = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    4,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "atc", 2 | VET_TexCoord}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VTextBatchPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUV = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUV,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVCxform = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3d = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dCxform = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVPosition3dCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVYUVA = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVYUVA,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVYUVACxform = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVYUVACxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dYUVA = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVPosition3dYUVA,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVPosition3dYUVACxform = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VYUVPosition3dYUVACxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatch = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatch,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchCxform = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3d = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dCxform = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchPosition3dCxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchYUVA = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchYUVA,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchYUVACxform = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchYUVACxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dYUVA = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchPosition3dYUVA,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VYUVBatchPosition3dYUVACxform = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VYUVBatchPosition3dYUVACxform,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGCxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGPosition3dVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGPosition3dVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGPosition3dCxformVafactor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGPosition3dCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {4, 16, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchCxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 720, 4, 6}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 4, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchPosition3dVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchPosition3dVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VTexTGBatchPosition3dCxformVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VTexTGBatchPosition3dCxformVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 2, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 3, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_vfuniforms, 0, 2},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_V = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_V,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3d = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatch = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatch,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 360, 4, 3}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 1, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3d = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchPosition3d,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 120, 4, 1}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VCxformVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3dVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VPosition3dVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3dCxformVacolorVafactor = {
  /* Flags */         0,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VPosition3dCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VBatchVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 0, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchCxformVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VBatchCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3dVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VBatchPosition3dVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3dCxformVacolorVafactor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "afactor", 4 | VET_Color | (1 << VET_Index_Shift)}},
  /* pSource */       pSource_VBatchPosition3dCxformVacolorVafactor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VVacolor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}},
  /* pSource */       pSource_VVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VCxformVacolor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}},
  /* pSource */       pSource_VCxformVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3dVacolor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}},
  /* pSource */       pSource_VPosition3dVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VPosition3dCxformVacolor = {
  /* Flags */         0,
  /* NumAttribs */    2,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}},
  /* pSource */       pSource_VPosition3dCxformVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 16, 16, 16, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchVacolor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 0, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchCxformVacolor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchCxformVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {0, 0, 4, 480, 4, 4}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfuniforms, 2, 2},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3dVacolor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchPosition3dVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBatchPosition3dCxformVacolor = {
  /* Flags */         Shader_Batched,
  /* NumAttribs */    3,
  /* Attributes */    {{ "pos", 4 | VET_Pos}, { "acolor", 4 | VET_Color}, { "vbatch", 1 | VET_Instance8}},
  /* pSource */       pSource_VBatchPosition3dCxformVacolor,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {0, 0, 16, 480, 16, 1},
                        /* vfuniforms  */ {120, 480, 4, 240, 4, 2}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_vfuniforms, 0, 1},
                        /* cxmul       */ {Uniform::SU_vfuniforms, 1, 1},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_vfmuniforms, 0, 1},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBox1 = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VBox1,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static VertexShaderDesc ShaderDesc_VS_VBox2 = {
  /* Flags */         0,
  /* NumAttribs */    1,
  /* Attributes */    {{ "pos", 4 | VET_Pos}},
  /* pSource */       pSource_VBox2,
  /* Uniforms */      { /* cxadd       */ {-1, 0, 0, 0, 0},
                        /* cxmul       */ {-1, 0, 0, 0, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {0, 0, 4, 8, 4, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {2, 8, 4, 8, 4, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

const VertexShaderDesc* VertexShaderDesc::Descs[VS_Count] = {
    NULL,
    &ShaderDesc_VS_VVatc,
    &ShaderDesc_VS_VCxformVatc,
    &ShaderDesc_VS_VPosition3dVatc,
    &ShaderDesc_VS_VPosition3dCxformVatc,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VBatchVatc,
    &ShaderDesc_VS_VBatchCxformVatc,
    &ShaderDesc_VS_VBatchPosition3dVatc,
    &ShaderDesc_VS_VBatchPosition3dCxformVatc,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTexTGVacolorVafactor,
    &ShaderDesc_VS_VTexTGCxformVacolorVafactor,
    &ShaderDesc_VS_VTexTGPosition3dVacolorVafactor,
    &ShaderDesc_VS_VTexTGPosition3dCxformVacolorVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTexTGBatchVacolorVafactor,
    &ShaderDesc_VS_VTexTGBatchCxformVacolorVafactor,
    &ShaderDesc_VS_VTexTGBatchPosition3dVacolorVafactor,
    &ShaderDesc_VS_VTexTGBatchPosition3dCxformVacolorVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VYUVVafactor,
    &ShaderDesc_VS_VYUVCxformVafactor,
    &ShaderDesc_VS_VYUVPosition3dVafactor,
    &ShaderDesc_VS_VYUVPosition3dCxformVafactor,
    &ShaderDesc_VS_VYUVYUVAVafactor,
    &ShaderDesc_VS_VYUVYUVACxformVafactor,
    &ShaderDesc_VS_VYUVPosition3dYUVAVafactor,
    &ShaderDesc_VS_VYUVPosition3dYUVACxformVafactor,
    &ShaderDesc_VS_VYUVBatchVafactor,
    &ShaderDesc_VS_VYUVBatchCxformVafactor,
    &ShaderDesc_VS_VYUVBatchPosition3dVafactor,
    &ShaderDesc_VS_VYUVBatchPosition3dCxformVafactor,
    &ShaderDesc_VS_VYUVBatchYUVAVafactor,
    &ShaderDesc_VS_VYUVBatchYUVACxformVafactor,
    &ShaderDesc_VS_VYUVBatchPosition3dYUVAVafactor,
    &ShaderDesc_VS_VYUVBatchPosition3dYUVACxformVafactor,
    &ShaderDesc_VS_VTexTG,
    &ShaderDesc_VS_VTexTGCxform,
    &ShaderDesc_VS_VTexTGPosition3d,
    &ShaderDesc_VS_VTexTGPosition3dCxform,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTexTGBatch,
    &ShaderDesc_VS_VTexTGBatchCxform,
    &ShaderDesc_VS_VTexTGBatchPosition3d,
    &ShaderDesc_VS_VTexTGBatchPosition3dCxform,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTexTGTexTGVafactor,
    &ShaderDesc_VS_VTexTGTexTGCxformVafactor,
    &ShaderDesc_VS_VTexTGTexTGPosition3dVafactor,
    &ShaderDesc_VS_VTexTGTexTGPosition3dCxformVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTexTGTexTGBatchVafactor,
    &ShaderDesc_VS_VTexTGTexTGBatchCxformVafactor,
    &ShaderDesc_VS_VTexTGTexTGBatchPosition3dVafactor,
    &ShaderDesc_VS_VTexTGTexTGBatchPosition3dCxformVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VText,
    NULL,
    &ShaderDesc_VS_VTextPosition3d,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTextBatch,
    NULL,
    &ShaderDesc_VS_VTextBatchPosition3d,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VYUV,
    &ShaderDesc_VS_VYUVCxform,
    &ShaderDesc_VS_VYUVPosition3d,
    &ShaderDesc_VS_VYUVPosition3dCxform,
    &ShaderDesc_VS_VYUVYUVA,
    &ShaderDesc_VS_VYUVYUVACxform,
    &ShaderDesc_VS_VYUVPosition3dYUVA,
    &ShaderDesc_VS_VYUVPosition3dYUVACxform,
    &ShaderDesc_VS_VYUVBatch,
    &ShaderDesc_VS_VYUVBatchCxform,
    &ShaderDesc_VS_VYUVBatchPosition3d,
    &ShaderDesc_VS_VYUVBatchPosition3dCxform,
    &ShaderDesc_VS_VYUVBatchYUVA,
    &ShaderDesc_VS_VYUVBatchYUVACxform,
    &ShaderDesc_VS_VYUVBatchPosition3dYUVA,
    &ShaderDesc_VS_VYUVBatchPosition3dYUVACxform,
    &ShaderDesc_VS_VTexTGVafactor,
    &ShaderDesc_VS_VTexTGCxformVafactor,
    &ShaderDesc_VS_VTexTGPosition3dVafactor,
    &ShaderDesc_VS_VTexTGPosition3dCxformVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VTexTGBatchVafactor,
    &ShaderDesc_VS_VTexTGBatchCxformVafactor,
    &ShaderDesc_VS_VTexTGBatchPosition3dVafactor,
    &ShaderDesc_VS_VTexTGBatchPosition3dCxformVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_V,
    NULL,
    &ShaderDesc_VS_VPosition3d,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VBatch,
    NULL,
    &ShaderDesc_VS_VBatchPosition3d,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VVacolorVafactor,
    &ShaderDesc_VS_VCxformVacolorVafactor,
    &ShaderDesc_VS_VPosition3dVacolorVafactor,
    &ShaderDesc_VS_VPosition3dCxformVacolorVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VBatchVacolorVafactor,
    &ShaderDesc_VS_VBatchCxformVacolorVafactor,
    &ShaderDesc_VS_VBatchPosition3dVacolorVafactor,
    &ShaderDesc_VS_VBatchPosition3dCxformVacolorVafactor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VVacolor,
    &ShaderDesc_VS_VCxformVacolor,
    &ShaderDesc_VS_VPosition3dVacolor,
    &ShaderDesc_VS_VPosition3dCxformVacolor,
    NULL,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_VS_VBatchVacolor,
    &ShaderDesc_VS_VBatchCxformVacolor,
    &ShaderDesc_VS_VBatchPosition3dVacolor,
    &ShaderDesc_VS_VBatchPosition3dCxformVacolor,
    &ShaderDesc_VS_VBox1,
    &ShaderDesc_VS_VBox2,
};

VertexShaderType FragShaderDesc::VShaderForFShader[FragShaderDesc::FS_Count] = {
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolorVafactor, // FVertexVertexEAlpha
    VertexShaderDesc::VS_VCxformVacolorVafactor, // FVertexVertexCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolorVafactor, // FVertexVertexBatchEAlpha
    VertexShaderDesc::VS_VBatchCxformVacolorVafactor, // FVertexVertexBatchCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolorVafactor, // FVertexVertexEAlphaMul
    VertexShaderDesc::VS_VCxformVacolorVafactor, // FVertexVertexCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolorVafactor, // FVertexVertexBatchEAlphaMul
    VertexShaderDesc::VS_VBatchCxformVacolorVafactor, // FVertexVertexBatchCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGVacolorVafactor, // FTexTGVertex
    VertexShaderDesc::VS_VTexTGCxformVacolorVafactor, // FTexTGVertexCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatchVacolorVafactor, // FTexTGVertexBatch
    VertexShaderDesc::VS_VTexTGBatchCxformVacolorVafactor, // FTexTGVertexBatchCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGVacolorVafactor, // FTexTGVertexMul
    VertexShaderDesc::VS_VTexTGCxformVacolorVafactor, // FTexTGVertexCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatchVacolorVafactor, // FTexTGVertexBatchMul
    VertexShaderDesc::VS_VTexTGBatchCxformVacolorVafactor, // FTexTGVertexBatchCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVatc, // FTextColor
    VertexShaderDesc::VS_VCxformVatc, // FTextColorCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVatc, // FTextColorBatch
    VertexShaderDesc::VS_VBatchCxformVatc, // FTextColorBatchCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVatc, // FTextColorMul
    VertexShaderDesc::VS_VCxformVatc, // FTextColorCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVatc, // FTextColorBatchMul
    VertexShaderDesc::VS_VBatchCxformVatc, // FTextColorBatchCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolorVafactor, // FVertexEAlpha
    VertexShaderDesc::VS_VCxformVacolorVafactor, // FVertexCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolorVafactor, // FVertexBatchEAlpha
    VertexShaderDesc::VS_VBatchCxformVacolorVafactor, // FVertexBatchCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolorVafactor, // FVertexEAlphaMul
    VertexShaderDesc::VS_VCxformVacolorVafactor, // FVertexCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolorVafactor, // FVertexBatchEAlphaMul
    VertexShaderDesc::VS_VBatchCxformVacolorVafactor, // FVertexBatchCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGVacolorVafactor, // FTexTGVertexEAlpha
    VertexShaderDesc::VS_VTexTGCxformVacolorVafactor, // FTexTGVertexCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatchVacolorVafactor, // FTexTGVertexBatchEAlpha
    VertexShaderDesc::VS_VTexTGBatchCxformVacolorVafactor, // FTexTGVertexBatchCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGVacolorVafactor, // FTexTGVertexEAlphaMul
    VertexShaderDesc::VS_VTexTGCxformVacolorVafactor, // FTexTGVertexCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatchVacolorVafactor, // FTexTGVertexBatchEAlphaMul
    VertexShaderDesc::VS_VTexTGBatchCxformVacolorVafactor, // FTexTGVertexBatchCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolorVafactor, // FVertexVertex
    VertexShaderDesc::VS_VCxformVacolorVafactor, // FVertexVertexCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolorVafactor, // FVertexVertexBatch
    VertexShaderDesc::VS_VBatchCxformVacolorVafactor, // FVertexVertexBatchCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolorVafactor, // FVertexVertexMul
    VertexShaderDesc::VS_VCxformVacolorVafactor, // FVertexVertexCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolorVafactor, // FVertexVertexBatchMul
    VertexShaderDesc::VS_VBatchCxformVacolorVafactor, // FVertexVertexBatchCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VYUVVafactor, // FYUVEAlpha
    VertexShaderDesc::VS_VYUVCxformVafactor, // FYUVCxformEAlpha
    VertexShaderDesc::VS_VYUVYUVAVafactor, // FYUVYUVAEAlpha
    VertexShaderDesc::VS_VYUVYUVACxformVafactor, // FYUVYUVACxformEAlpha
    VertexShaderDesc::VS_VYUVBatchVafactor, // FYUVBatchEAlpha
    VertexShaderDesc::VS_VYUVBatchCxformVafactor, // FYUVBatchCxformEAlpha
    VertexShaderDesc::VS_VYUVBatchYUVAVafactor, // FYUVBatchYUVAEAlpha
    VertexShaderDesc::VS_VYUVBatchYUVACxformVafactor, // FYUVBatchYUVACxformEAlpha
    VertexShaderDesc::VS_VYUVVafactor, // FYUVEAlphaMul
    VertexShaderDesc::VS_VYUVCxformVafactor, // FYUVCxformEAlphaMul
    VertexShaderDesc::VS_VYUVYUVAVafactor, // FYUVYUVAEAlphaMul
    VertexShaderDesc::VS_VYUVYUVACxformVafactor, // FYUVYUVACxformEAlphaMul
    VertexShaderDesc::VS_VYUVBatchVafactor, // FYUVBatchEAlphaMul
    VertexShaderDesc::VS_VYUVBatchCxformVafactor, // FYUVBatchCxformEAlphaMul
    VertexShaderDesc::VS_VYUVBatchYUVAVafactor, // FYUVBatchYUVAEAlphaMul
    VertexShaderDesc::VS_VYUVBatchYUVACxformVafactor, // FYUVBatchYUVACxformEAlphaMul
    VertexShaderDesc::VS_VTexTGTexTGVafactor, // FTexTGTexTGEAlpha
    VertexShaderDesc::VS_VTexTGTexTGCxformVafactor, // FTexTGTexTGCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGTexTGBatchVafactor, // FTexTGTexTGBatchEAlpha
    VertexShaderDesc::VS_VTexTGTexTGBatchCxformVafactor, // FTexTGTexTGBatchCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGTexTGVafactor, // FTexTGTexTGEAlphaMul
    VertexShaderDesc::VS_VTexTGTexTGCxformVafactor, // FTexTGTexTGCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGTexTGBatchVafactor, // FTexTGTexTGBatchEAlphaMul
    VertexShaderDesc::VS_VTexTGTexTGBatchCxformVafactor, // FTexTGTexTGBatchCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolor, // FVertex
    VertexShaderDesc::VS_VCxformVacolor, // FVertexCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolor, // FVertexBatch
    VertexShaderDesc::VS_VBatchCxformVacolor, // FVertexBatchCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VVacolor, // FVertexMul
    VertexShaderDesc::VS_VCxformVacolor, // FVertexCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatchVacolor, // FVertexBatchMul
    VertexShaderDesc::VS_VBatchCxformVacolor, // FVertexBatchCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGVafactor, // FTexTGEAlpha
    VertexShaderDesc::VS_VTexTGCxformVafactor, // FTexTGCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatchVafactor, // FTexTGBatchEAlpha
    VertexShaderDesc::VS_VTexTGBatchCxformVafactor, // FTexTGBatchCxformEAlpha
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGVafactor, // FTexTGEAlphaMul
    VertexShaderDesc::VS_VTexTGCxformVafactor, // FTexTGCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatchVafactor, // FTexTGBatchEAlphaMul
    VertexShaderDesc::VS_VTexTGBatchCxformVafactor, // FTexTGBatchCxformEAlphaMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VYUV, // FYUV
    VertexShaderDesc::VS_VYUVCxform, // FYUVCxform
    VertexShaderDesc::VS_VYUVYUVA, // FYUVYUVA
    VertexShaderDesc::VS_VYUVYUVACxform, // FYUVYUVACxform
    VertexShaderDesc::VS_VYUVBatch, // FYUVBatch
    VertexShaderDesc::VS_VYUVBatchCxform, // FYUVBatchCxform
    VertexShaderDesc::VS_VYUVBatchYUVA, // FYUVBatchYUVA
    VertexShaderDesc::VS_VYUVBatchYUVACxform, // FYUVBatchYUVACxform
    VertexShaderDesc::VS_VYUV, // FYUVMul
    VertexShaderDesc::VS_VYUVCxform, // FYUVCxformMul
    VertexShaderDesc::VS_VYUVYUVA, // FYUVYUVAMul
    VertexShaderDesc::VS_VYUVYUVACxform, // FYUVYUVACxformMul
    VertexShaderDesc::VS_VYUVBatch, // FYUVBatchMul
    VertexShaderDesc::VS_VYUVBatchCxform, // FYUVBatchCxformMul
    VertexShaderDesc::VS_VYUVBatchYUVA, // FYUVBatchYUVAMul
    VertexShaderDesc::VS_VYUVBatchYUVACxform, // FYUVBatchYUVACxformMul
    VertexShaderDesc::VS_VTexTGTexTGVafactor, // FTexTGTexTG
    VertexShaderDesc::VS_VTexTGTexTGCxformVafactor, // FTexTGTexTGCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGTexTGBatchVafactor, // FTexTGTexTGBatch
    VertexShaderDesc::VS_VTexTGTexTGBatchCxformVafactor, // FTexTGTexTGBatchCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGTexTGVafactor, // FTexTGTexTGMul
    VertexShaderDesc::VS_VTexTGTexTGCxformVafactor, // FTexTGTexTGCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGTexTGBatchVafactor, // FTexTGTexTGBatchMul
    VertexShaderDesc::VS_VTexTGTexTGBatchCxformVafactor, // FTexTGTexTGBatchCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTG, // FTexTG
    VertexShaderDesc::VS_VTexTGCxform, // FTexTGCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatch, // FTexTGBatch
    VertexShaderDesc::VS_VTexTGBatchCxform, // FTexTGBatchCxform
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTG, // FTexTGMul
    VertexShaderDesc::VS_VTexTGCxform, // FTexTGCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTexTGBatch, // FTexTGBatchMul
    VertexShaderDesc::VS_VTexTGBatchCxform, // FTexTGBatchCxformMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VText, // FText
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTextBatch, // FTextBatch
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VText, // FTextMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VTextBatch, // FTextBatchMul
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_V, // FSolid
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBatch, // FSolidBatch
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadow
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowHighlight
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowKnockout
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowKnockoutHighlight
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowMul
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowMulHighlight
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowKnockoutMul
    VertexShaderDesc::VS_VBox2, // FBox2InnerShadowKnockoutMulHighlight
    VertexShaderDesc::VS_VBox2, // FBox2Shadow
    VertexShaderDesc::VS_VBox2, // FBox2ShadowHighlight
    VertexShaderDesc::VS_VBox2, // FBox2ShadowKnockout
    VertexShaderDesc::VS_VBox2, // FBox2ShadowKnockoutHighlight
    VertexShaderDesc::VS_VBox2, // FBox2ShadowMul
    VertexShaderDesc::VS_VBox2, // FBox2ShadowMulHighlight
    VertexShaderDesc::VS_VBox2, // FBox2ShadowKnockoutMul
    VertexShaderDesc::VS_VBox2, // FBox2ShadowKnockoutMulHighlight
    VertexShaderDesc::VS_VBox2, // FBox2Shadowonly
    VertexShaderDesc::VS_VBox2, // FBox2ShadowonlyHighlight
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_None,
    VertexShaderDesc::VS_VBox2, // FBox2ShadowonlyMul
    VertexShaderDesc::VS_VBox2, // FBox2ShadowonlyMulHighlight
    VertexShaderDesc::VS_VBox1, // FBox1Blur
    VertexShaderDesc::VS_VBox2, // FBox2Blur
    VertexShaderDesc::VS_VBox1, // FBox1BlurMul
    VertexShaderDesc::VS_VBox2, // FBox2BlurMul
    VertexShaderDesc::VS_VTexTG, // FTexTGCMatrixAc
    VertexShaderDesc::VS_VTexTG, // FTexTGCMatrixAcMul
};

static FragShaderDesc ShaderDesc_FS_FVertexVertexEAlpha = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex */ {-1, 0, 0, 0, 0},
                        /* tex0 */ {-1, 0, 0, 0, 0},
                        /* tex1 */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexCxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchCxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexCxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchCxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertex = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertex,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexCxform = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexCxformMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColor = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColor,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorCxform = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorCxformMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextColorBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextColorBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexEAlpha = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexCxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchCxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexCxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchCxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexCxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchCxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexCxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGVertexBatchCxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGVertexBatchCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertex = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertex,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexCxform = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexCxformMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexVertexBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexVertexBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVCxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVAEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVAEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVACxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVACxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchCxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVAEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVAEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVACxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVACxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVCxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVAEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVAEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVACxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVACxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchCxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVAEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVAEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVACxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVACxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGCxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchCxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGCxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchCxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertex = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertex,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexCxform = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexCxformMul = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FVertexBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FVertexBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGCxformEAlpha = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchCxformEAlpha = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchCxformEAlpha,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGCxformEAlphaMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchCxformEAlphaMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchCxformEAlphaMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUV = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUV,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVCxform = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVA = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVA,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVACxform = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVACxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVA = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVA,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVACxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVACxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVCxformMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVAMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVAMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVYUVACxformMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVYUVACxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        -1},
  /* pSource */       pSource_FYUVBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {0, 0, 0, 0, 0, 0},
                        /* tex_v */ {1, 0, 0, 0, 0, 0},
                        /* tex_y */ {2, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVAMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVAMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FYUVBatchYUVACxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex_y,
                        Uniform::SU_tex_u,
                        Uniform::SU_tex_v,
                        Uniform::SU_tex_a},
  /* pSource */       pSource_FYUVBatchYUVACxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {0, 0, 0, 0, 0, 0},
                        /* tex_u */ {1, 0, 0, 0, 0, 0},
                        /* tex_v */ {2, 0, 0, 0, 0, 0},
                        /* tex_y */ {3, 0, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTG = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTG,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGCxform = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGCxformMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGTexTGBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        Uniform::SU_tex1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGTexTGBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {1, 0, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTG = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTG,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGCxform = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGCxform,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchCxform = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchCxform,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGCxformMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGCxformMul,
  /* Uniforms */      { /* cxadd */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul */ {1, 1204, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGBatchCxformMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGBatchCxformMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FText = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FText,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTextBatchMul = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTextBatchMul,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {0, 0, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FSolid = {
  /* Flags */         0,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FSolid,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {0, 1200, 4, 4, 4, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FSolidBatch = {
  /* Flags */         Shader_Batched,
  /* TexParams */     { -1,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FSolidBatch,
  /* Uniforms */      { /* cxadd */ {-1, 0, 0, 0, 0},
                        /* cxmul */ {-1, 0, 0, 0, 0},
                        /* fsize */ {-1, 0, 0, 0, 0},
                        /* mvp   */ {-1, 0, 0, 0, 0},
                        /* offset */ {-1, 0, 0, 0, 0},
                        /* scolor */ {-1, 0, 0, 0, 0},
                        /* scolor2 */ {-1, 0, 0, 0, 0},
                        /* srctex */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex   */ {-1, 0, 0, 0, 0},
                        /* tex0  */ {-1, 0, 0, 0, 0},
                        /* tex1  */ {-1, 0, 0, 0, 0},
                        /* tex_a */ {-1, 0, 0, 0, 0},
                        /* tex_u */ {-1, 0, 0, 0, 0},
                        /* tex_v */ {-1, 0, 0, 0, 0},
                        /* tex_y */ {-1, 0, 0, 0, 0},
                        /* texgen */ {-1, 0, 0, 0, 0},
                        /* texscale */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadow = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadow,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowKnockout = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowKnockout,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowKnockoutHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowKnockoutHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowMulHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowMulHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowKnockoutMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowKnockoutMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2InnerShadowKnockoutMulHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2InnerShadowKnockoutMulHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2Shadow = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2Shadow,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowKnockout = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowKnockout,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowKnockoutHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowKnockoutHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowMulHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowMulHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowKnockoutMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowKnockoutMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowKnockoutMulHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        Uniform::SU_srctex,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowKnockoutMulHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {5, 1220, 4, 4, 4, 0},
                        /* srctex      */ {0, 0, 0, 0, 0, 0},
                        /* srctexscale */ {6, 1224, 2, 2, 2, 0},
                        /* tex         */ {1, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {7, 1228, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2Shadowonly = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2Shadowonly,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {5, 1220, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowonlyHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowonlyHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {5, 1220, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowonlyMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowonlyMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {5, 1220, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2ShadowonlyMulHighlight = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2ShadowonlyMulHighlight,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {3, 1212, 2, 2, 2, 0},
                        /* scolor      */ {4, 1216, 4, 4, 4, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {5, 1220, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox1Blur = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox1Blur,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {3, 1212, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2Blur = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2Blur,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {3, 1212, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox1BlurMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox1BlurMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {3, 1212, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FBox2BlurMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FBox2BlurMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 4, 4, 4, 0},
                        /* fsize       */ {2, 1208, 4, 4, 4, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {0, 0, 0, 0, 0, 0},
                        /* tex0        */ {-1, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {3, 1212, 2, 2, 2, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGCMatrixAc = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGCMatrixAc,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 16, 16, 16, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {0, 0, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

static FragShaderDesc ShaderDesc_FS_FTexTGCMatrixAcMul = {
  /* Flags */         0,
  /* TexParams */     { Uniform::SU_tex0,
                        -1,
                        -1,
                        -1},
  /* pSource */       pSource_FTexTGCMatrixAcMul,
  /* Uniforms */      { /* cxadd       */ {0, 1200, 4, 4, 4, 0},
                        /* cxmul       */ {1, 1204, 16, 16, 16, 0},
                        /* fsize       */ {-1, 0, 0, 0, 0},
                        /* mvp         */ {-1, 0, 0, 0, 0},
                        /* offset      */ {-1, 0, 0, 0, 0},
                        /* scolor      */ {-1, 0, 0, 0, 0},
                        /* scolor2     */ {-1, 0, 0, 0, 0},
                        /* srctex      */ {-1, 0, 0, 0, 0},
                        /* srctexscale */ {-1, 0, 0, 0, 0},
                        /* tex         */ {-1, 0, 0, 0, 0},
                        /* tex0        */ {0, 0, 0, 0, 0, 0},
                        /* tex1        */ {-1, 0, 0, 0, 0},
                        /* tex_a       */ {-1, 0, 0, 0, 0},
                        /* tex_u       */ {-1, 0, 0, 0, 0},
                        /* tex_v       */ {-1, 0, 0, 0, 0},
                        /* tex_y       */ {-1, 0, 0, 0, 0},
                        /* texgen      */ {-1, 0, 0, 0, 0},
                        /* texscale    */ {-1, 0, 0, 0, 0},
                        /* vfmuniforms */ {-1, 0, 0, 0, 0},
                        /* vfuniforms  */ {-1, 0, 0, 0, 0}},
  /* BatchUniforms */ { /* cxadd       */ {Uniform::SU_Count, -1, 0},
                        /* cxmul       */ {Uniform::SU_Count, -1, 0},
                        /* fsize       */ {Uniform::SU_Count, -1, 0},
                        /* mvp         */ {Uniform::SU_Count, -1, 0},
                        /* offset      */ {Uniform::SU_Count, -1, 0},
                        /* scolor      */ {Uniform::SU_Count, -1, 0},
                        /* scolor2     */ {Uniform::SU_Count, -1, 0},
                        /* srctex      */ {Uniform::SU_Count, -1, 0},
                        /* srctexscale */ {Uniform::SU_Count, -1, 0},
                        /* tex         */ {Uniform::SU_Count, -1, 0},
                        /* tex0        */ {Uniform::SU_Count, -1, 0},
                        /* tex1        */ {Uniform::SU_Count, -1, 0},
                        /* tex_a       */ {Uniform::SU_Count, -1, 0},
                        /* tex_u       */ {Uniform::SU_Count, -1, 0},
                        /* tex_v       */ {Uniform::SU_Count, -1, 0},
                        /* tex_y       */ {Uniform::SU_Count, -1, 0},
                        /* texgen      */ {Uniform::SU_Count, -1, 0},
                        /* texscale    */ {Uniform::SU_Count, -1, 0},
                        /* vfmuniforms */ {Uniform::SU_Count, -1, 0},
                        /* vfuniforms  */ {Uniform::SU_Count, -1, 0}}};

const FragShaderDesc* FragShaderDesc::Descs[FS_Count] = {
    NULL,
    &ShaderDesc_FS_FVertexVertexEAlpha,
    &ShaderDesc_FS_FVertexVertexCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertexBatchEAlpha,
    &ShaderDesc_FS_FVertexVertexBatchCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertexEAlphaMul,
    &ShaderDesc_FS_FVertexVertexCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertexBatchEAlphaMul,
    &ShaderDesc_FS_FVertexVertexBatchCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertex,
    &ShaderDesc_FS_FTexTGVertexCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexBatch,
    &ShaderDesc_FS_FTexTGVertexBatchCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexMul,
    &ShaderDesc_FS_FTexTGVertexCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexBatchMul,
    &ShaderDesc_FS_FTexTGVertexBatchCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextColor,
    &ShaderDesc_FS_FTextColorCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextColorBatch,
    &ShaderDesc_FS_FTextColorBatchCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextColorMul,
    &ShaderDesc_FS_FTextColorCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextColorBatchMul,
    &ShaderDesc_FS_FTextColorBatchCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexEAlpha,
    &ShaderDesc_FS_FVertexCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexBatchEAlpha,
    &ShaderDesc_FS_FVertexBatchCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexEAlphaMul,
    &ShaderDesc_FS_FVertexCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexBatchEAlphaMul,
    &ShaderDesc_FS_FVertexBatchCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexEAlpha,
    &ShaderDesc_FS_FTexTGVertexCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexBatchEAlpha,
    &ShaderDesc_FS_FTexTGVertexBatchCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexEAlphaMul,
    &ShaderDesc_FS_FTexTGVertexCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGVertexBatchEAlphaMul,
    &ShaderDesc_FS_FTexTGVertexBatchCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertex,
    &ShaderDesc_FS_FVertexVertexCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertexBatch,
    &ShaderDesc_FS_FVertexVertexBatchCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertexMul,
    &ShaderDesc_FS_FVertexVertexCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexVertexBatchMul,
    &ShaderDesc_FS_FVertexVertexBatchCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FYUVEAlpha,
    &ShaderDesc_FS_FYUVCxformEAlpha,
    &ShaderDesc_FS_FYUVYUVAEAlpha,
    &ShaderDesc_FS_FYUVYUVACxformEAlpha,
    &ShaderDesc_FS_FYUVBatchEAlpha,
    &ShaderDesc_FS_FYUVBatchCxformEAlpha,
    &ShaderDesc_FS_FYUVBatchYUVAEAlpha,
    &ShaderDesc_FS_FYUVBatchYUVACxformEAlpha,
    &ShaderDesc_FS_FYUVEAlphaMul,
    &ShaderDesc_FS_FYUVCxformEAlphaMul,
    &ShaderDesc_FS_FYUVYUVAEAlphaMul,
    &ShaderDesc_FS_FYUVYUVACxformEAlphaMul,
    &ShaderDesc_FS_FYUVBatchEAlphaMul,
    &ShaderDesc_FS_FYUVBatchCxformEAlphaMul,
    &ShaderDesc_FS_FYUVBatchYUVAEAlphaMul,
    &ShaderDesc_FS_FYUVBatchYUVACxformEAlphaMul,
    &ShaderDesc_FS_FTexTGTexTGEAlpha,
    &ShaderDesc_FS_FTexTGTexTGCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGTexTGBatchEAlpha,
    &ShaderDesc_FS_FTexTGTexTGBatchCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGTexTGEAlphaMul,
    &ShaderDesc_FS_FTexTGTexTGCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGTexTGBatchEAlphaMul,
    &ShaderDesc_FS_FTexTGTexTGBatchCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertex,
    &ShaderDesc_FS_FVertexCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexBatch,
    &ShaderDesc_FS_FVertexBatchCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexMul,
    &ShaderDesc_FS_FVertexCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FVertexBatchMul,
    &ShaderDesc_FS_FVertexBatchCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGEAlpha,
    &ShaderDesc_FS_FTexTGCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGBatchEAlpha,
    &ShaderDesc_FS_FTexTGBatchCxformEAlpha,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGEAlphaMul,
    &ShaderDesc_FS_FTexTGCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGBatchEAlphaMul,
    &ShaderDesc_FS_FTexTGBatchCxformEAlphaMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FYUV,
    &ShaderDesc_FS_FYUVCxform,
    &ShaderDesc_FS_FYUVYUVA,
    &ShaderDesc_FS_FYUVYUVACxform,
    &ShaderDesc_FS_FYUVBatch,
    &ShaderDesc_FS_FYUVBatchCxform,
    &ShaderDesc_FS_FYUVBatchYUVA,
    &ShaderDesc_FS_FYUVBatchYUVACxform,
    &ShaderDesc_FS_FYUVMul,
    &ShaderDesc_FS_FYUVCxformMul,
    &ShaderDesc_FS_FYUVYUVAMul,
    &ShaderDesc_FS_FYUVYUVACxformMul,
    &ShaderDesc_FS_FYUVBatchMul,
    &ShaderDesc_FS_FYUVBatchCxformMul,
    &ShaderDesc_FS_FYUVBatchYUVAMul,
    &ShaderDesc_FS_FYUVBatchYUVACxformMul,
    &ShaderDesc_FS_FTexTGTexTG,
    &ShaderDesc_FS_FTexTGTexTGCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGTexTGBatch,
    &ShaderDesc_FS_FTexTGTexTGBatchCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGTexTGMul,
    &ShaderDesc_FS_FTexTGTexTGCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGTexTGBatchMul,
    &ShaderDesc_FS_FTexTGTexTGBatchCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTG,
    &ShaderDesc_FS_FTexTGCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGBatch,
    &ShaderDesc_FS_FTexTGBatchCxform,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGMul,
    &ShaderDesc_FS_FTexTGCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FTexTGBatchMul,
    &ShaderDesc_FS_FTexTGBatchCxformMul,
    NULL,
    NULL,
    &ShaderDesc_FS_FText,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextBatch,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextMul,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_FS_FTextBatchMul,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_FS_FSolid,
    NULL,
    NULL,
    NULL,
    &ShaderDesc_FS_FSolidBatch,
    &ShaderDesc_FS_FBox2InnerShadow,
    &ShaderDesc_FS_FBox2InnerShadowHighlight,
    &ShaderDesc_FS_FBox2InnerShadowKnockout,
    &ShaderDesc_FS_FBox2InnerShadowKnockoutHighlight,
    &ShaderDesc_FS_FBox2InnerShadowMul,
    &ShaderDesc_FS_FBox2InnerShadowMulHighlight,
    &ShaderDesc_FS_FBox2InnerShadowKnockoutMul,
    &ShaderDesc_FS_FBox2InnerShadowKnockoutMulHighlight,
    &ShaderDesc_FS_FBox2Shadow,
    &ShaderDesc_FS_FBox2ShadowHighlight,
    &ShaderDesc_FS_FBox2ShadowKnockout,
    &ShaderDesc_FS_FBox2ShadowKnockoutHighlight,
    &ShaderDesc_FS_FBox2ShadowMul,
    &ShaderDesc_FS_FBox2ShadowMulHighlight,
    &ShaderDesc_FS_FBox2ShadowKnockoutMul,
    &ShaderDesc_FS_FBox2ShadowKnockoutMulHighlight,
    &ShaderDesc_FS_FBox2Shadowonly,
    &ShaderDesc_FS_FBox2ShadowonlyHighlight,
    NULL,
    NULL,
    &ShaderDesc_FS_FBox2ShadowonlyMul,
    &ShaderDesc_FS_FBox2ShadowonlyMulHighlight,
    &ShaderDesc_FS_FBox1Blur,
    &ShaderDesc_FS_FBox2Blur,
    &ShaderDesc_FS_FBox1BlurMul,
    &ShaderDesc_FS_FBox2BlurMul,
    &ShaderDesc_FS_FTexTGCMatrixAc,
    &ShaderDesc_FS_FTexTGCMatrixAcMul,
};

FragShaderType FragShaderDesc::FindStaticShader(const char* name, unsigned flags)
{
    SF_UNUSED2(name,flags);
    return FS_None;
}

}}}
