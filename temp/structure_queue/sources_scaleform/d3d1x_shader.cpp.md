# vostok/scaleform/sources/d3d1x_shader.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 17 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x0b3790 size 0x74    void Scaleform::Render::D3D1x::ShaderConstantRange::Update(int, int, int)
    ?Update@ShaderConstantRange@D3D1x@Render@Scaleform@@QAEXHHH@Z
0x5b6170 size 0x1     void Scaleform::Render::D3D1x::ShaderManager::EndScene()
    ?EndScene@ShaderManager@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6180 size 0x1     void Scaleform::Render::D3D1x::ShaderManager::BeginScene()
    ?BeginScene@ShaderManager@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6190 size 0xd     bool Scaleform::Render::D3D1x::ShaderManager::HasInstancingSupport() const
    ?HasInstancingSupport@ShaderManager@D3D1x@Render@Scaleform@@QBE_NXZ
0x5b61a0 size 0x6e    void Scaleform::Render::D3D1x::ShaderInterface::BeginScene()
    ?BeginScene@ShaderInterface@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6210 size 0x2a    void Scaleform::Render::D3D1x::ShaderInterface::SetTexture(const Scaleform::Render::D3D1x::ShaderPair, unsigned int, Scaleform::Render::Texture*, Scaleform::Render::ImageFillMode, unsigned int)
    ?SetTexture@ShaderInterface@D3D1x@Render@Scaleform@@QAEXUShaderPair@234@IPAVTexture@34@UImageFillMode@34@I@Z
0x5b6240 size 0x17    void Scaleform::Render::D3D1x::FragShader::Shutdown()
    ?Shutdown@FragShader@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6260 size 0x84    bool Scaleform::Render::D3D1x::FragShader::Init(ID3D11Device*, Scaleform::Render::D3D1x::FragShaderDesc const*)
    ?Init@FragShader@D3D1x@Render@Scaleform@@QAE_NPAUID3D11Device@@PBUFragShaderDesc@234@@Z
0x5b62f0 size 0x17    void Scaleform::Render::D3D1x::VertexShader::Shutdown()
    ?Shutdown@VertexShader@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6310 size 0x84    bool Scaleform::Render::D3D1x::VertexShader::Init(ID3D11Device*, Scaleform::Render::D3D1x::VertexShaderDesc const*)
    ?Init@VertexShader@D3D1x@Render@Scaleform@@QAE_NPAUID3D11Device@@PBUVertexShaderDesc@234@@Z
0x5b63a0 size 0x10a   bool Scaleform::Render::D3D1x::ShaderManager::Initialize(Scaleform::Render::D3D1x::HAL*)
    ?Initialize@ShaderManager@D3D1x@Render@Scaleform@@QAE_NPAVHAL@234@@Z
0x5b64b0 size 0xb0    void Scaleform::Render::D3D1x::ShaderManager::Reset()
    ?Reset@ShaderManager@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6560 size 0xe1    void Scaleform::Render::D3D1x::ShaderManager::MapVertexFormat(Scaleform::Render::PrimitiveFillType, Scaleform::Render::VertexFormat const*, Scaleform::Render::VertexFormat const**, Scaleform::Render::VertexFormat const**, Scaleform::Render::VertexFormat const**)
    ?MapVertexFormat@ShaderManager@D3D1x@Render@Scaleform@@QAEXW4PrimitiveFillType@34@PBUVertexFormat@34@PAPBU634@22@Z
0x5b6650 size 0x70    Scaleform::Render::D3D1x::ShaderManager::ShaderManager(Scaleform::Render::ProfileViews*)
    ??0ShaderManager@D3D1x@Render@Scaleform@@QAE@PAVProfileViews@23@@Z
0x5b66c0 size 0x1b0   void Scaleform::Render::D3D1x::ShaderInterface::Finish(unsigned int)
    ?Finish@ShaderInterface@D3D1x@Render@Scaleform@@QAEXI@Z
0x5b6870 size 0x14d   Scaleform::Render::D3D1x::SysVertexFormat::SysVertexFormat(ID3D11Device*, Scaleform::Render::VertexFormat const*, Scaleform::Render::D3D1x::VertexShaderDesc const*)
    ??0SysVertexFormat@D3D1x@Render@Scaleform@@QAE@PAUID3D11Device@@PBUVertexFormat@23@PBUVertexShaderDesc@123@@Z
0x5b69c0 size 0xf3    bool Scaleform::Render::D3D1x::ShaderInterface::SetStaticShader(Scaleform::Render::D3D1x::ShaderDesc::ShaderType, Scaleform::Render::VertexFormat const*)
    ?SetStaticShader@ShaderInterface@D3D1x@Render@Scaleform@@QAE_NW4ShaderType@ShaderDesc@234@PBUVertexFormat@34@@Z
