# vostok/scaleform/sources/factory.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 20 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x0ae120 size 0x38    void* survarium::scaleform_engine::xrSysAllocMalloc::Alloc(unsigned int, unsigned int)
    ?Alloc@xrSysAllocMalloc@scaleform_engine@survarium@@UAEPAXII@Z
0x0ae160 size 0x13    void survarium::scaleform_engine::xrSysAllocMalloc::Free(void*, unsigned int, unsigned int)
    ?Free@xrSysAllocMalloc@scaleform_engine@survarium@@UAEXPAXII@Z
0x0ae180 size 0x53    void* survarium::scaleform_engine::xrSysAllocMalloc::Realloc(void*, unsigned int, unsigned int, unsigned int)
    ?Realloc@xrSysAllocMalloc@scaleform_engine@survarium@@UAEPAXPAXIII@Z
0x0ae1e0 size 0x56    Scaleform::File* survarium::vostok_file_opener::OpenFile(char const*, int, int)
    ?OpenFile@vostok_file_opener@survarium@@UAEPAVFile@Scaleform@@PBDHH@Z
0x0ae240 size 0xa3    void survarium::vostok_scaleform_log::LogMessageVarg(Scaleform::LogMessageId, char const*, char*)
    ?LogMessageVarg@vostok_scaleform_log@survarium@@EAEXVLogMessageId@Scaleform@@PBDPAD@Z
0x0ae2f0 size 0x23    void* Scaleform::Log::`scalar deleting destructor'(unsigned int)
    ??_GLog@Scaleform@@UAEPAXI@Z
0x0ae2f0 size 0x23    void* survarium::vostok_scaleform_log::`scalar deleting destructor'(unsigned int)
    ??_GLog@Scaleform@@UAEPAXI@Z
0x5bb9c0 size 0x1     void survarium::flash_factory::tick()
    ?tick@flash_factory@survarium@@QAEXXZ
0x5bb9d0 size 0x12    void survarium::flash_factory::destroy_movie(survarium::flash_movie*)
    ?destroy_movie@flash_factory@survarium@@QAEXPAUflash_movie@2@@Z
0x5bb9f0 size 0x32    void survarium::flash_factory::~flash_factory()
    ??1flash_factory@survarium@@QAE@XZ
0x5bba30 size 0x5     void survarium::scaleform_engine::destroy()
    ?destroy@scaleform_engine@survarium@@YAXXZ
0x5bba40 size 0x161   survarium::flash_movie* survarium::flash_factory::build_movie(void*, unsigned int, char const*)
    ?build_movie@flash_factory@survarium@@QAEPAUflash_movie@2@PAXIPBD@Z
0x5bbbb0 size 0x58    void survarium::scaleform_engine::initialize(void* (*)(unsigned int), void (*)(void*), void (*)(unsigned char, char const*))
    ?initialize@scaleform_engine@survarium@@YAXP6APAXI@ZP6AXPAX@ZP6AXEPBD@Z@Z
0x5bbc10 size 0x26    survarium::flash_text_manager* survarium::flash_factory::create_text_manager()
    ?create_text_manager@flash_factory@survarium@@QAEPAUflash_text_manager@2@XZ
0x5bbc40 size 0x287   survarium::flash_factory::flash_factory(survarium::scaleform_game_engine&)
    ??0flash_factory@survarium@@QAE@AAVscaleform_game_engine@1@@Z
0x7d9330 size 0x3b    void survarium::`dynamic initializer for 'g_file_opener''()
    survarium::`dynamic initializer for 'g_file_opener''
0x7d9370 size 0x20    void survarium::`dynamic initializer for 'g_vostok_logger''()
    survarium::`dynamic initializer for 'g_vostok_logger''
0x7f0a80 size 0xb     void `survarium::scaleform_engine::initialize'::`2'::`dynamic atexit destructor for 'scaleform_alloc''()
    `survarium::scaleform_engine::initialize'::`2'::`dynamic atexit destructor for 'scaleform_alloc''
0x7f0a90 size 0x14    void survarium::`dynamic atexit destructor for 'g_file_opener''()
    survarium::`dynamic atexit destructor for 'g_file_opener''
0x7f0ab0 size 0xa     void survarium::`dynamic atexit destructor for 'g_vostok_logger''()
    survarium::`dynamic atexit destructor for 'g_vostok_logger''
