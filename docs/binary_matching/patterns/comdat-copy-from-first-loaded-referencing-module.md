# The kept COMDAT copy comes from the first pass-1-loaded module that references it: a wrong owner module means a missing reference, not a wall
tags: cpp:inline cpp:template cpp:member | asm:mov | topic:convention topic:link-layout topic:scoring-artifact
symptoms: target COMDAT frameless/this-in-eax but ours framed /Od; `vostok tool link-layout owners` shows target_owner(O2) vs base_owner(Od); parked "standalone COMDAT not retained" notes
confidence: 8/10
variants: optimized-comdat-in-od-unit.md, stale-comdat-other-module.md, inline-vs-call-template-comdat.md

Under /LTCG every TU that references an inline or template function carries its own IL copy;
the linker keeps the copy of the first module it LOADS in pass 1 (library order, then symbol
demand), and that module's /Od or /Ox flags decide the out-of-line body. Neither the DBI
module order nor the link command line changes the choice. When the retail owner (the PDB
module whose contribution holds the COMDAT) loads before ours in our own pass 1, the retail
owner's TU references the function and ours does not: restore the reference (usually an
accessor call the reconstruction spelled as a direct member read, or a definition moved
out of the header).

```cpp
// weapon_core.h (retail: one-line inline bodies, PDB lines 132/145/190/207/213/232)
inline	bool	is_aimed	( ) const	{ return m_aimed;	}
// weapon.cpp (/Ox, loaded before game_core in pass 1): call it, do not read m_aimed
m_game_ui->show_crosshair( !( s_hide_crosshair_on_aim_value && is_aimed( ) ) );
```
```asm
; retail is_aimed, kept from game\weapon.obj (/Ox):   mov al,[eax+488h] / ret
; ours, kept from game_core\weapon_core.obj (/Od):   push ebp / mov ebp,esp / ... / ret
```
List candidates with `python3 -m vostok tool link-layout owners`; confirm load order with a
`/VERBOSE` relink (`Loaded lib(obj)` lines). Evidence: weapon_core accessors (docs/binary_matching/link-layout.md).
