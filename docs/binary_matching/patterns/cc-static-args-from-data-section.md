# A `??__E<cc_static>` initializer looks argument-less: read the args out of the shipped `.data`

`cN`: 9
`cpp:` static, ctor, console-command
`asm:` mov, movss, xorps, call-atexit
`topic:` data-section, dynamic-initializer, recovery-technique

## Symptom

A `void \`dynamic initializer for '<name>'\`()` target function is `unpaired`, and the
in-source note says the console command cannot be declared because "the command-NAME
string is not recoverable from the init asm" (only the `s_console_command_root` link, the
backing-variable pointer, min/max and the vtable are stored).

Typical optimized (`/Ox` + LTCG) shape - the whole `cc_*` ctor chain is inlined:

    mov   eax, [?s_console_command_root@console_commands@vostok@@3PAVconsole_command@12@A]
    mov   [<obj+0xC>], eax          ; m_prev = root
    mov   ecx, <obj>
    test  eax, eax
    je    short .1
    mov   [eax+8], ecx              ; root->m_next = this
    .1:
    ...                             ; m_value / m_min / m_max / m_need_args stores
    mov   dword ptr [<obj>], ??_7cc_float@console_commands@vostok@@6B@
    call  atexit

`m_name`, `m_command_type` and `m_serializable` never appear: they are **constant**, so
MSVC folds them into the object's `.data` image at compile time and only the
runtime-dependent stores survive in the initializer.

## Recovery

The arguments are all still in the binary - just in `.data`, not in `.text`.

1. The first `mov [X], eax` (opcode `A3`) stores `m_prev`, so the object base is
   `X - 0xC` (target layout: `console_command` = vptr, +8 `m_next`, +0xC `m_prev`,
   +0x10 `m_name`, +0x14 `m_command_type`, +0x18 `m_execution_type`, +0x1C
   `m_need_args`, +0x1D `m_serializable`, +0x20 `boost::function`, size 0x40;
   `cc_value<T>`: +0x40 `m_value`, +0x44 `m_min`, +0x48 `m_max`).
2. Read `$SURVARIUM_BIN/survarium.exe` as a PE (image base `0x10000`, so
   `rva = printed_address - 0x10000`) and fetch:
   - `*(u32*)(obj+0x10)` -> follow it -> the **command-name string**;
   - `*(u32*)(obj+0x14)` -> `command_type`; `*(u32*)(obj+0x18)` -> `execution_filter`;
   - `*(u8*)(obj+0x1D)` -> `serializable`.
3. `min`/`max` come from the `movss xmm0,[<rdata>]` operands in the initializer
   (`xorps xmm0,xmm0` = `0.0f`); the **seed** of the backing variable is the `.data`
   word at the address the `c705 <obj+0x40> <imm32>` store writes (an address in `.bss`
   -> the seed is 0/false).

That gives the complete ctor call with no fabrication:
`cc_float( "<name>", <backing>, <min>, <max>, <serializable>, <command_type> )`.

## Note

The delinker already resolves the *backing variable's* symbol name from the target PDB
(`mov dword ptr [obj+0x40], s_crosshair_size_value`), so the local static's real name is
free; only the string literal needs the `.data` read.

## Gotcha found with this

The target `cc_bool` derives from `cc_value<bool>` (see
`binaries/structure/target/headers/vostok/console_commands/cc_bool.h`,
`STATIC_SIZE_ASSERT(cc_bool, 0x48)`), which is why every target `cc_bool` initializer
carries two extra byte stores `[obj+0x44]=0` / `[obj+0x45]=1` (`m_min=false`,
`m_max=true`). `cc_bool` ALSO re-assigns `m_need_args = true` in its own ctor body: the
redundant store from `cc_value`'s body is dead-code-eliminated, and the surviving one
lands **after** the `cc_bool` vptr store - which is exactly the target's instruction
order. Drop that body assignment and every `cc_bool` initializer in the engine caps
~93%.
