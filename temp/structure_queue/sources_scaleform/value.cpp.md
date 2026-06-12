# vostok/scaleform/sources/value.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 42 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x0ae920 size 0x2f    void survarium::flash_external_handler_impl::Callback(Scaleform::GFx::Movie*, char const*, Scaleform::GFx::Value const*, unsigned int)
    ?Callback@flash_external_handler_impl@survarium@@UAEXPAVMovie@GFx@Scaleform@@PBDPBVValue@45@I@Z
0x0ae950 size 0x29    void* survarium::flash_external_handler_impl::`scalar deleting destructor'(unsigned int)
    ??_GZlibSupport@GFx@Scaleform@@UAEPAXI@Z
0x0ae980 size 0x29    void* Scaleform::GFx::FunctionHandler::`scalar deleting destructor'(unsigned int)
    ??_GFunctionHandler@GFx@Scaleform@@UAEPAXI@Z
0x0ae980 size 0x29    void* survarium::flash_function_handler_impl::`scalar deleting destructor'(unsigned int)
    ??_GFunctionHandler@GFx@Scaleform@@UAEPAXI@Z
0x5baac0 size 0x16    void survarium::flash_function_handler::~flash_function_handler()
    ??1flash_function_handler@survarium@@QAE@XZ
0x5baae0 size 0x46    void survarium::flash_function_handler_impl::Call(Scaleform::GFx::FunctionHandler::Params const&)
    ?Call@flash_function_handler_impl@survarium@@UAEXABUParams@FunctionHandler@GFx@Scaleform@@@Z
0x5bab30 size 0x16    void survarium::flash_external_handler::~flash_external_handler()
    ??1flash_external_handler@survarium@@UAE@XZ
0x5bab50 size 0x1b    void survarium::flash_movie::SetExternalInterface(survarium::flash_external_handler*)
    ?SetExternalInterface@flash_movie@survarium@@QAEXPAUflash_external_handler@2@@Z
0x5bab70 size 0x1     Scaleform::GFx::Value* survarium::gfx(survarium::flash_value const*)
    ?gfx@survarium@@YAPAVValue@GFx@Scaleform@@PBUflash_value@1@@Z
0x5bab80 size 0x1     Scaleform::GFx::Value& survarium::gfx(survarium::flash_value const&)
    ?gfx@survarium@@YAAAVValue@GFx@Scaleform@@ABUflash_value@1@@Z
0x5bab90 size 0x16    void survarium::flash_movie::CreateFunction(survarium::flash_value*, survarium::flash_function_handler*)
    ?CreateFunction@flash_movie@survarium@@QAEXPAUflash_value@2@PAUflash_function_handler@2@@Z
0x5babb0 size 0x10    void survarium::flash_movie::CreateObject(survarium::flash_value*)
    ?CreateObject@flash_movie@survarium@@QAEXPAUflash_value@2@@Z
0x5babc0 size 0xa     void survarium::flash_movie::CreateArray(survarium::flash_value*)
    ?CreateArray@flash_movie@survarium@@QAEXPAUflash_value@2@@Z
0x5babd0 size 0x1b    bool survarium::flash_movie::Invoke(char const*, survarium::flash_value*, survarium::flash_value const*, unsigned int)
    ?Invoke@flash_movie@survarium@@QAE_NPBDPAUflash_value@2@PBU32@I@Z
0x5babf0 size 0x11    void survarium::flash_movie::GetVariable(survarium::flash_value*, char const*)
    ?GetVariable@flash_movie@survarium@@QAEXPAUflash_value@2@PBD@Z
0x5bac10 size 0x13    void survarium::flash_movie::SetVariable(char const*, survarium::flash_value const&)
    ?SetVariable@flash_movie@survarium@@QAEXPBDABUflash_value@2@@Z
0x5bac30 size 0x51    void survarium::flash_movie::SetVariable(char const*, char const*)
    ?SetVariable@flash_movie@survarium@@QAEXPBD0@Z
0x5bac90 size 0x4     unsigned int survarium::flash_value::GetUInt() const
    ?GetUInt@flash_value@survarium@@QBEIXZ
0x5baca0 size 0x4     int survarium::flash_value::GetInt() const
    ?GetInt@flash_value@survarium@@QBEHXZ
0x5bacb0 size 0x4     wchar_t const* survarium::flash_value::GetStringW() const
    ?GetStringW@flash_value@survarium@@QBEPB_WXZ
0x5bacc0 size 0x26    void survarium::flash_value::SetConvertStringW()
    ?SetConvertStringW@flash_value@survarium@@QAEXXZ
0x5bacf0 size 0x15    char const* survarium::flash_value::GetString() const
    ?GetString@flash_value@survarium@@QBEPBDXZ
0x5bad10 size 0xe     unsigned int survarium::flash_value::GetArraySize()
    ?GetArraySize@flash_value@survarium@@QAEIXZ
0x5bad20 size 0x1c    void survarium::flash_value::GetElement(unsigned int, survarium::flash_value*)
    ?GetElement@flash_value@survarium@@QAEXIPAU12@@Z
0x5bad40 size 0xf     void survarium::flash_value::PushBack(survarium::flash_value&)
    ?PushBack@flash_value@survarium@@QAEXAAU12@@Z
0x5bad50 size 0x56    void survarium::flash_value::SetElement(unsigned int, char const*)
    ?SetElement@flash_value@survarium@@QAEXIPBD@Z
0x5badb0 size 0x1c    void survarium::flash_value::SetElement(unsigned int, survarium::flash_value&)
    ?SetElement@flash_value@survarium@@QAEXIAAU12@@Z
0x5badd0 size 0x31    void survarium::flash_value::GetMember(char const*, survarium::flash_value*)
    ?GetMember@flash_value@survarium@@QAEXPBDPAU12@@Z
0x5bae10 size 0x4     bool survarium::flash_value::GetBool()
    ?GetBool@flash_value@survarium@@QAE_NXZ
0x5bae20 size 0xa     float survarium::flash_value::GetNumber()
    ?GetNumber@flash_value@survarium@@QAEMXZ
0x5bae30 size 0x3e    void survarium::flash_value::SetNumber(float)
    ?SetNumber@flash_value@survarium@@QAEXM@Z
0x5bae70 size 0x29    void survarium::flash_value::SetUInt(unsigned int)
    ?SetUInt@flash_value@survarium@@QAEXI@Z
0x5baea0 size 0x29    void survarium::flash_value::SetInt(int)
    ?SetInt@flash_value@survarium@@QAEXH@Z
0x5baed0 size 0x5f    void survarium::flash_value::SetStringW(wchar_t const*)
    ?SetStringW@flash_value@survarium@@QAEXPB_W@Z
0x5baf30 size 0x5f    void survarium::flash_value::SetString(char const*)
    ?SetString@flash_value@survarium@@QAEXPBD@Z
0x5baf90 size 0x31    void survarium::flash_value::SetBoolean(bool)
    ?SetBoolean@flash_value@survarium@@QAEX_N@Z
0x5bafd0 size 0x2a    void survarium::flash_value::~flash_value()
    ??1flash_value@survarium@@QAE@XZ
0x5bb000 size 0x14    survarium::flash_value::flash_value()
    ??0flash_value@survarium@@QAE@XZ
0x5bb020 size 0x31    void survarium::flash_value::SetMember(char const*, survarium::flash_value&)
    ?SetMember@flash_value@survarium@@QAEXPBDAAU12@@Z
0x5bb060 size 0x17    survarium::flash_function_handler_impl::flash_function_handler_impl(survarium::flash_function_handler&)
    ??0flash_function_handler_impl@survarium@@QAE@AAUflash_function_handler@1@@Z
0x5bb080 size 0x3f    survarium::flash_function_handler::flash_function_handler()
    ??0flash_function_handler@survarium@@QAE@XZ
0x5bb0c0 size 0x46    survarium::flash_external_handler::flash_external_handler()
    ??0flash_external_handler@survarium@@QAE@XZ
