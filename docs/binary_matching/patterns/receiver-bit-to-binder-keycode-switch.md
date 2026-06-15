# A `convert_to_binder_X` switch maps internal device bits -> a unified key-binder keycode space

tags: cpp:enum cpp:switch cpp:member | asm:movzx asm:jmp asm:mov-imm | topic:structure-shape topic:enum-pinning
confidence: 8/10

## Symptom

A small per-device method (e.g. `mouse::convert_to_binder_mouse_button(int)`) compiles to a
sparse jump-table switch whose cases each `mov eax, <imm>; ret`, with the immediates a
contiguous run well ABOVE the DIK keyboard scan-code range:

```
0x00: movzx ecx, byte ptr [eax + <byte-index-table>]   ; arg = a 1<<i device bit (1,2,4,...,128)
0x07: jmp   dword ptr [ecx*4 + <jump-table>]
...
mov   eax, 151h   ; case 1<<0
ret
mov   eax, 152h   ; case 1<<1
...
mov   eax, 158h   ; case 1<<7
```

The `[arg + table]` (no `cmp max; ja default`) is the MSVC sparse-switch byte-index table for
non-contiguous case labels (here `1<<0 .. 1<<7`). The function name says "to_binder".

## Cause / source

The DEVICE receiver stores raw bit flags (`fill_state` does `result.buttons |= 1 << i`), but the
input HANDLER interface (`on_mouse_key_action(world*, mouse_button, action)`) and the key-binder
(`key_binder.cpp`: `{ "mouse1", mouse_button_left }`, `get_binded_action(int dik)`) speak a SINGLE
unified keycode space where keyboard keys are DIK scan codes (0x01..0xED) and mouse buttons live
just above at **0x151..0x158**. So the `mouse_button` enum values ARE the binder codes:

```cpp
enum mouse_button {
    mouse_button_left      = 0x151,   // NOT u8(1<<0) - that's the receiver's internal bit
    mouse_button_right     = 0x152,
    ...
    mouse_button_extended4 = 0x158,
    mouse_button_count     = 8,       // still the bit COUNT, used by fill_state's 1<<i loop
};

mouse_button convert_to_binder_mouse_button( s32 receiver_button ) {
    switch ( receiver_button ) {          // arg is the bit value 1<<i, not an index
        case 1 << 0: return mouse_button_left;    // -> 0x151
        ...
        case 1 << 7: return mouse_button_extended4;
    }
    NODEFAULT();
}
```

The structure carcass shows the case `return`s on their source lines and the per-case `ret`
re-attributed to the closing-brace line (one shared line, N times) - normal for return-per-case.

## How to recover the enum values

The immediates ARE the enum values - pin them straight from the `mov eax, <imm>` run; don't trust a
pre-existing `1<<i` enum in the header (that was the receiver bit, a different concept). Cross-check
against the binder consumer (`key_binder.cpp` maps the same `mouse_button_*` names; `get_binded_action`
compares them as raw `dik` ints), which proves they share the keycode space.

## Caller shape (the process loop)

`process` iterates set bits with `for ( u16 i = bits; i; i &= i - 1 )` and passes the lowest set bit
`i & ~( i - 1 )` to convert INLINE (no `button` local - locals-are-structure: the target records 0).
A declared `mouse_button button = convert(...)` adds a phantom local and is the wrong shape even when
it scores the same.
