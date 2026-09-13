# 2-value case cluster = jl/jle range check + switch temp; unreachable `break;` still emits its jmp
tags: cpp:switch cpp:return | asm:cmp asm:jcc asm:jmp | topic:structure-shape
symptoms: 4-row skeleton 0x14/0x4/0x2/0x2, switch temp copy, unreachable 2-byte jmp on its own line
confidence: 8/10
variants: assignment-in-condition.md, switch-default-nodefault.md

A `case A: case B:` cluster of consecutive values sharing one body lowers to a RANGE
check (`cmp [tmp],A; jl .default; cmp [tmp],B; jle .case`), not je/je; 4-5+ dense values
become a jump table. The switch value is COPIED to a temp (`mov eax,[arg]; mov [ebp-N],eax`
- a plain `if` compares the param slot directly). A `break;` written after a `return` is
unreachable but /Od still emits its 2-byte jmp (target = the label AFTER the switch) as
its own row.

```cpp
switch ( type )
{
	case hit_type_a:
	case hit_type_b:
		return true;
		break;          // unreachable - still a 0x2 row
	default:
		return false;
}
```
Evidence: artefact_lifebone_core::protect_affect 49.89 -> 100.

The same retained `break;` can occur after a return inside a loop.
`network_core::get_ip_address` (retail RVA `0x577040`) records a return on
line 147 followed by a two-byte jump on line 148, at offset `0x216`. That
unreachable jump targets the loop exit, not the backedge. Restoring `break;`
after the return gives all 11 statements / 676 bytes and 100% in full build
`bb8b533d29bb49c2ac2e6b406c16891b`. Do not attribute such a span to a generic
closing-brace breakpoint artifact before testing the source-level exit implied
by its destination. An unreachable `continue;` would have a different target.
