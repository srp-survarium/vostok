# A single `mov al,[f0] / or al,[f1] / ... / je` is `|`, not `||` - and the loads run in REVERSE source order

- **confidence**: 9/10
- **tags**: `cpp:bool cpp:operator cpp:if | asm:or asm:mov asm:je | topic:codegen-idiom topic:structure-shape topic:condition-shape`
- **symptoms**: target has ONE conditional jump where base has N; target accumulates
  N byte loads into `al` with `or`; base emits N separate `cmp byte ptr [...],0 / jne`;
  sema verdict `BRANCH-COUNT` with base branches == N and target branches == 1.

## Asm

```
; TARGET - one branch
mov al, [esi+0A7h]
or  al, [esi+0A6h]
or  al, [esi+0A5h]
or  al, [esi+0A4h]
or  al, [esi+0A3h]
je  short <skip>

; BASE - five branches
cmp byte ptr [esi+0A7h], 0
jne short <body>
cmp byte ptr [esi+0A3h], 0
jne short <body>
cmp byte ptr [esi+0A4h], 0
jne short <body>
...
```

## Source

MSVC 8 does **not** convert a `||` chain of `bool` member loads into a bitwise
OR - it keeps one branch per term. So an OR-accumulate in the target means the
original source spelled the operator `|`:

```cpp
bool any( ) const
{
	return render_targets[0] | render_targets[1] | render_targets[2]
		| render_targets[3] | depth_stencil;
}
```

## Operand order: the loads come out REVERSED

For the left-associated chain `(((a|b)|c)|d)|e` MSVC emits the loads
**last-to-first** - `e` seeds `al` and `a` is OR'd in last. Read the target's
load order bottom-up to recover the source order; do NOT transcribe the load
order literally (that produces the unnatural `depth_stencil | rt[3] | rt[2] |
rt[1] | rt[0]`, which is the same *set* but the wrong *source*).

Verified on `vostok::render::backend::render_dirty_targets::any()`
(`backend.h`): the natural member-declaration order `rt[0..3]` then
`depth_stencil`, written with `|`, reproduces the target's `0A7,0A6,0A5,0A4,0A3`
load sequence exactly. `backend::flush_rt` went 49.7% -> **100%** on that one
operator change (batch B7).

## When NOT to reach for this

The inverse (base ORs, target branches) is the ordinary `||`; and a chain whose
terms have side effects or are expensive calls will stay short-circuited on both
sides regardless of the spelling. This pattern is specifically about a
side-effect-free predicate over adjacent `bool` members - the shape a
`dirty`/`any`/`is_valid` aggregator has.
