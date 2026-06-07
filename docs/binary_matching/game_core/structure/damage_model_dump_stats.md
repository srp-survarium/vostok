# damage_model::dump_stats - structure verification

Verdict: STRUCTURE MATCH (shape ok). fuzzy 79.26% (report.json, post-rebuild).
Marker was stale at 78.97%; corrected to 79.26%.

## Structure diff (target 0x6ef920 | base 0x45d2e0), target 7 / base 7 stmts

    0x023 <0x28> | 0x022 <0x35> | body_part->dump_state( callback, body_part_index++ );   SIZE
    .. same ..
    ; aligned 6, size-diffs 1, quantity-diffs 0

Statement count and shape match exactly. The only divergence is a SIZE diff on
the `dump_state( callback, ... )` call statement.

## Cause: by-value boost::function argument copy lowered differently

`body_part_parameters::dump_state( boost::function<void(u32,float,float,pcstr)> callback, u32 index )`
takes `callback` BY VALUE, so the call constructs a temporary copy of the
`callback` passed into `dump_stats`.

Target (0x6ef920):

    0x2d: sub esp, 20h            ; allocate temp boost::function
    0x30: mov eax, esp           ; eax = &temp (no slot)
    0x32: lea ecx, [ebp+8]       ; ecx = &callback (source)
    0x35: call boost::function<...>::function<...>   ; COPY CTOR (one call)
    0x3a: mov ecx, [ebp-8]
    0x3d: call dump_state

Base (0x45d2e0):

    0x2c: sub esp, 20h
    0x2f: mov [ebp-10h], esp     ; store temp ptr in a slot
    0x32: mov eax, [ebp-10h]
    0x35: call boost::function<...>::function<...>   ; default ctor
    0x3a: lea ecx, [ebp+8]
    0x3d: push ecx
    0x3e: mov ecx, [ebp-10h]
    0x41: call assign_to_own     ; EXTRA call
    0x46: mov ecx, [ebp-8]
    0x49: call dump_state

The base default-constructs the temp then calls `assign_to_own`; the target copy-
constructs in one call. The extra call and the extra `[ebp-10h]` slot force a
larger frame (base `sub esp,20h` vs target `sub esp,14h`) and shift slot numbers,
which is what drops the byte %.

This lives entirely inside boost::function's own header inlining (copy-ctor vs
default+assign). The source already passes `callback` by value, which is the
correct and only shape available. There is no C++-source lever to choose copy-ctor
vs default+assign_to_own; it is a boost::function LTCG inline-vs-call divergence.

Non-steerable. STRUCTURE MATCH.
