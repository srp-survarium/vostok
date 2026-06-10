# LOG expansion: base schedules the boost::function1 ctor at the append site (target: block entry)
tags: cpp:macro | asm:lea asm:call asm:push | topic:logging topic:convention
symptoms: lea eax [fn_slot] call fn-ctor at block entry vs lea esi at append, guard-bit cleanups swapped
confidence: 6/10
variants: log-line-verbosity-immediates.md, boost-function-assign-inline.md

Inside each inlined LOG block the target constructs the log-callback boost::function
FIRST (block entry, this in eax), then evaluates message(), then pushes &fn at the
append; the /Od+LTCG base emits the ctor AT the append site (this in esi, push eax),
which also SWAPS the two guard-bit cleanups (`[ebp-guard] & 2` destroys the function1 in
base vs the message std::string in target). Slots/structure identical; a few bytes per
LOG + cascaded register renames. Non-steerable whole-program class (http_client::on_error
precedent) - mark PARTIAL.

Evidence: network_core/udp_match_client::handle_receive (93.54%, 23/23 stmts).
