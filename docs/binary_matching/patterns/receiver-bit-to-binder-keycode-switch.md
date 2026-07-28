# A `convert_to_binder_X` switch maps device bits to unified binder keycodes

tags: cpp:enum cpp:switch cpp:member | asm:movzx asm:jmp asm:mov-imm | topic:structure-shape topic:enum-pinning
confidence: 8/10

## Symptom

A per-device conversion method compiles to a sparse jump-table switch. Each case moves a
constant into the return register, and the constants form a range above the keyboard scan
codes:

```text
movzx ecx, byte ptr [eax + table]  ; argument is 1, 2, 4, ... 128
jmp   dword ptr [ecx*4 + table]
...
mov   eax, 151h                    ; case 1 << 0
ret
...
mov   eax, 158h                    ; case 1 << 7
ret
```

The byte-index table is MSVC's sparse-switch form for non-contiguous case labels.

## Source model

The receiver stores raw button bits, while the handler and key binder use a unified keycode
space. Keep those representations separate:

```cpp
enum mouse_button {
	mouse_button_left      = 0x151,
	// ...
	mouse_button_extended4 = 0x158,
	mouse_button_count     = 8,
};

mouse_button convert_to_binder_mouse_button( s32 receiver_button )
{
	switch ( receiver_button ) {
		case 1 << 0: return mouse_button_left;
		// ...
		case 1 << 7: return mouse_button_extended4;
	}
	NODEFAULT();
}
```

Pin the enum values from the per-case `mov eax, <immediate>` instructions. Cross-check the
binder consumer to confirm that it compares the same enum values as raw keycodes.

## Caller shape

The receiver can iterate set bits with
`for ( u16 i = bits; i; i &= i - 1 )` and pass the lowest bit,
`i & ~( i - 1 )`, directly to the conversion method. Check PDB locals before introducing a
named converted-button temporary: a source temporary can add a local that the target does
not record.
