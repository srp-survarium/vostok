# An unexplained entry guard over the whole body = a file-scope debug switch (cc_bool / command_line::key)
tags: cpp:if cpp:global cpp:static | asm:cmp asm:je asm:call | topic:reconstruct-global topic:data-recovery
symptoms: cmp byte ptr [<data addr>], 0 / je <end> as the FIRST statement; `dynamic initializer for 's_xxx'` in the target's file dump; call iterate_keys<key_initializator>

A render/engine function whose whole body sits under a compare against a lone
`.data` byte is guarded by a file-scope debug switch that the carcass never had.
Two flavours, both recoverable in full (name, limits, default) without guessing:

**console command** - `cmp byte ptr [s_x_value], 0; je end`. The target's
`binaries/structure/target/sources/.../<tu>.cpp` dump lists a matching
`dynamic initializer for 's_x_cc'`; disassemble it to read the object layout back
out (`console_command`: `+0x08 m_next`, `+0x0c m_prev`, `+0x10 m_name`,
`+0x14 command_type`, `+0x18 execution_filter`, `+0x1c need_args`,
`+0x1d serializable`; `cc_value<T>`: `+0x40 &value`, `+0x44 min`, `+0x48 max`).
The initializer writes only the dynamic fields - `m_name`, `command_type` and
`serializable` sit pre-initialized in `.data`, so read them out of the image at
the object address. The `mov dword ptr [obj+0x40], <addr>` operand IS the address
of the backing value global: read its initial bytes for the default.

**command_line::key** - `cmp [k+0x218],0 / jne .1 / call iterate_keys<key_initializator> /
mov [k+0x218],1 / .1: cmp [k+0x218],1 / jne <skip>` is an inlined `key::is_set()`
(`m_type == type_uninitialized -> initialize(); return m_type != type_unset`).
`jne <skip>` after the second compare = the body runs when the key is NOT set, i.e.
`if ( s_key ) return;`. The out-of-line `dynamic initializer for 's_key'` is
`push &s_key; push protected_key_construct; call debug::protected_call`; the ctor
strings are in `.data` at `key+0x10..0x20` (full_name, short_name, category,
description, argument_description).

```cpp
static u32 s_max_triagles_per_dip_value = 1000000;
static console_commands::cc_u32 s_max_triagles_per_dip(
	"max_triagles_per_dip", s_max_triagles_per_dip_value, 0, 100000, false,
	console_commands::command_type_user_specific );

static command_line::key s_no_effects_initialize( "no_effects_initialize", "", "", "" );
```
Initialize the backing value to the byte found in the image (`true` for the two
clearing switches) - a `static bool` that nothing writes would otherwise let the
optimizer fold the guard away, and the cc object is what makes its address escape.

Evidence: render backend::clear_depth_stencil 88.6 -> 99.4, clear_render_targets(f,f,f,f)
91.5 -> 99.7, render_indexed 54 -> 99.9 (s_max_triagles_per_dip_value);
effect_manager::on_effect_created 51.7 -> 99.8, on_async_effect_created 54.3 -> 99.8
(s_no_effects_initialize).
