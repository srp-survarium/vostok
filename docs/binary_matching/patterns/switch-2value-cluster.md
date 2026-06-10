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
