# File-static console command (cc_bool/cc_float): dynamic initializer, objdiff-UNSCORABLE
tags: cpp:static cpp:ctor | asm:push asm:call | topic:codegen-idiom topic:scoring-artifact
symptoms: dynamic initializer for 's_*_cc', dynamic atexit destructor, ??__E/??__F, scores None, push atexit
confidence: 9/10

A file-static console command compiles into a `dynamic initializer for 's_NAME_cc'` +
matching atexit destructor. Both score None: the per-TU init/atexit thunks are LTCG/
ICF-folded with no standalone symbol in the base index (and objdiff does not pair the
base ??__E/??__F mangles with the demangled target names). The bytes still match - mirror
a matched sibling and mark None|DONE; do not chase the symbol.

```cpp
static console_commands::cc_bool s_NAME_cc( "NAME", s_NAME_value, true, command_type );
// header <vostok/console_command.h>
// cc_bool args:  (name, bool& value, bool serializable, command_type, execution_filter=general)
// cc_float args: (name, float& value, float min, float max, bool serializable, command_type, execution_filter=general)
```
```asm
push 1                       ; serializable
push &s_NAME_value; push ??_C@..NAME
mov eax,<command_type>       ; engine_internal=0, user_specific=1
xor ecx,ecx                  ; execution_filter_general default
mov esi,&s_NAME_cc; call cc_TYPE::cc_TYPE
push <atexit dtor>; call atexit
; cc_float: min arrives in xmm0, max via fld [const]; fstp [esp]
```
Evidence: dispersion_calculator.cpp s_dispersion_enabled_cc (None|DONE), bullet.cpp; cc_float byte-identical, cc_bool's only diff = ctor passing command_type/filter in registers under LTCG.

## Sibling idiom: file-static `command_line::key` (the protected_call form)

A file-static `command_line::key` (header <vostok/command_line_extensions.h>) registers a
command-line switch. Its `key` ctor sets string-pointer / fixed_string512 members that are
all **constant-initializable**, so MSVC puts them in `.data` and the dynamic initializer
contains ONLY the ctor's non-constant tail - `debug::protected_call( protected_key_construct,
this )`. The whole `dynamic initializer for 's_NAME'` is then just 0x13 bytes and scores None
for the SAME delinker reason: target names it the friendly `\`dynamic initializer for 's_NAME''`
(0x13b), base emits the raw `??__Es_NAME@@YAXXZ`, objdiff cannot bridge them.

```cpp
static vostok::command_line::key  s_NAME( "full_name", "short", "category", "description" );
// key( pcstr full, pcstr short, pcstr category, pcstr description, pcstr arg_desc="" )
```
```asm
push  s_NAME                                          ; this
push  vostok::command_line::protected_key_construct
call  vostok::debug::protected_call
add   esp, 8
ret                                                  ; 0x13 bytes, 0 PDB statements
```
DECL SCOPE IS LOAD-BEARING: the target delinks `s_NAME` WITH its namespace prefix only when
it really had one - a stripped `\`dynamic initializer for 's_NAME''` (no `survarium::`) means
the static was at GLOBAL/file scope; put it above the namespace block. Mark None|DONE; the
0x13-byte init is byte-correct, the pairing waits on a delinker naming fix.
Evidence: game/match_client.cpp s_flow_emulator/s_lost_packets/s_min_ping/s_max_ping (global
scope, None); command_line.cpp s_show_help is the matched-TU precedent (key ctor inlines there).
