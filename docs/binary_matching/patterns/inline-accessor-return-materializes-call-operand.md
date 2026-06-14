# Inline accessor RETURN materializes a double-store the raw expression doesn't
tags: cpp:inline cpp:call cpp:temporary | asm:mov | topic:inline-helper topic:codegen-idiom
symptoms: a call/min operand with a `[ebp-A]->[ebp-B]` double-store and NO `call` between, the raw inline expression scores ~14% short
confidence: 8/10
variants: chained-temporary-params-object.md, inline-temp-vs-named-temp-ctor-order.md, array-walk-zero-locals-is-for-each.md

When a call/`min` operand is an INLINE accessor's RETURN value (e.g. `min(len, total_ammo())`),
the accessor inlines (no `call` appears) but its return value materializes into a temp, which
is then copied into the consuming call's by-value param - a `[ebp-2]->[ebp-6]` double-store.
Writing the operand as the raw inline EXPRESSION instead (`min(len, u16(a+b))`) yields only ONE
temp (the cast IS the arg) and scores short. **The double-store with no call between is the
tell**: the operand is an accessor's return, not the expression. Add/find the inline accessor
(it emits no standalone symbol, so the NAME is reconstructed - the bytes confirm the shape).
Verify empirically: add the helper and rebuild; a real one snaps to MATCH.

```cpp
m_bullets_in_queue = math::min( fire_queue_length(), total_ammo() );  // total_ammo() inline, NOT min(len, u16(a+b))
```
```asm
; ... compute a+b into a temp ...
mov [ebp-2], cx ; mov cx,[ebp-2] ; mov [ebp-6], cx   ; return-temp -> min by-value param (NO call between)
```
Steerable, byte-confirmed: `weapon_core::reset_fire_queue` raw-expr 85.8% -> accessor 99.81% MATCH (0xbc==0xbc) (#372). **CAUTION - this is an accessor RETURN feeding a call operand, NOT a free-function predicate.** A by-value free-function predicate does NOT reproduce a CSE single-load temp: tried on `booby_trap_core::on_state_timer_finished` and it got WORSE (86%->75%, bytes grew) - that one is a genuine CSE residual, left as the direct expression.
