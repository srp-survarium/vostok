# vostok/scaleform/sources/command_queue.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 5 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x5bb940 size 0xf     void survarium::scaleform_render_command_queue::~scaleform_render_command_queue()
    ??1scaleform_render_command_queue@survarium@@QAE@XZ
0x5bb950 size 0x9     void survarium::scaleform_render_command_queue_impl::PushThreadCommand(Scaleform::Render::ThreadCommand*)
    ?PushThreadCommand@scaleform_render_command_queue_impl@survarium@@UAEXPAVThreadCommand@Render@Scaleform@@@Z
0x5bb960 size 0x1e    void survarium::scaleform_render_command_queue_impl::GetRenderInterfaces(Scaleform::Render::Interfaces*)
    ?GetRenderInterfaces@scaleform_render_command_queue_impl@survarium@@UAEXPAUInterfaces@Render@Scaleform@@@Z
0x5bb980 size 0x9     void survarium::scaleform_render_command::execute()
    ?execute@scaleform_render_command@survarium@@QAEXXZ
0x5bb990 size 0x2b    survarium::scaleform_render_command_queue::scaleform_render_command_queue(survarium::scaleform_game_engine&)
    ??0scaleform_render_command_queue@survarium@@QAE@AAVscaleform_game_engine@1@@Z
