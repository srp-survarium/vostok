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
