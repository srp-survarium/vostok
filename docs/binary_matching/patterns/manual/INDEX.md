# Manual patterns (probe-bootstrapped + real-function-mined)

`C++ construct/idiom -> asm` mappings gathered two ways, both grounded:
1. **Foundational probes** — write a minimal snippet, compile it with the real toolchain,
   read the `/FAcs` machine-code listing. Recipe below.
2. **Real-function mining** — grep the engine source for a genuinely-used idiom, then read the
   shipped TARGET asm of a real function with `pdb_fetch --view target` (cited per pattern).

These complement the diff-recovery patterns in `../` — those tell you what a *divergence*
means; these tell you what to **write** to get a shape (the matcher's "write a first
approximation" step). Same per-file schema as `../assembly_patterns.md`; same grep-by-tag
discovery; all also listed in `../INDEX.md`. NOTE the optimization level is **per-TU**
(`.vcproj` `Optimization` 0=/Od vs 3=/Ox) — see [tu-optimization-level-od-vs-ox.md](tu-optimization-level-od-vs-ox.md); patterns tagged `/Od` or `/Ox` say which world they apply to.

## How these were produced (reproducible)

A standalone `cl.exe` compile under Wine with the game_core codegen flags, minus the three
that block a per-TU listing: `/GL` (LTCG defers codegen to link), `/Zi` (spawns mspdbsrv),
and the PCH. Everything that shapes codegen is kept (`/Od /Ob2 /Oi /Oy /arch:SSE2 /fp:fast
/GF /GS- /GT /MT /TP`, defines `WIN32 NDEBUG VOSTOK_STATIC_LIBRARIES MASTER_GOLD _MBCS`),
plus `/FAcs` for a machine-code + source listing:

```
wine $MSVC_DIR/VC/bin/cl.exe /nologo /c /FAcs /Fa<n>.cod /Od /Ob2 /Oi /Oy /arch:SSE2 \
  /fp:fast /GF /GS- /GT /MT /TP /DWIN32 /DNDEBUG /DVOSTOK_STATIC_LIBRARIES /DMASTER_GOLD \
  /D_MBCS <n>.cpp
```

These are base-side `/Od` listings: faithful for per-function body lowering (the regime the
whole patterns base assumes). LTCG only perturbs cross-TU inlining and argument passing
(see MATCHING.md's "LTCG is an excuse ONLY for arguments") — it does not change the
statement-level shapes documented here.

## Patterns

### Control flow
- [if/else under /Od: the condition INVERTS, the then-block ends in a jmp over the else](od-ifelse-inversion.md) — c9 — cpp:if cpp:return | asm:cmp asm:jcc asm:jmp | topic:codegen-idiom — cmp/jge skipping then, jmp over else, else-if = chained cmp/jne .next + jmp .end
- [Early-return guard `if(!c) return;` = inverted test `jne .body` + a bare `jmp` epilogue](od-early-return-guard.md) — c9 — cpp:if cpp:return | asm:cmp asm:jcc asm:jmp | topic:structure-shape — cmp 0/jne .body/jmp .end pair, two branches where a wrapping-if has one je
- [The three /Od loop skeletons: for vs while vs do-while are distinguishable](od-loop-skeletons.md) — c9 — cpp:for cpp:while cpp:do-while cpp:loop | asm:jmp asm:cmp asm:jcc | topic:structure-shape — jmp short .test entry, increment block above the test, test-at-top no entry jmp, body-first bottom-test non-inverted back-edge
- [`break`/`continue` = an inverted guard whose taken branch jmps to the loop exit / increment](od-break-continue.md) — c8 — cpp:for cpp:while | asm:jmp asm:cmp asm:jcc | topic:structure-shape — cmp/jne .skip then jmp .end (break) or jmp .incr (continue)
- [`&&` and `||` short-circuit: && is all je .end; || flips the first to jne .body](short-circuit-and-or.md) — c9 — cpp:if cpp:bool | asm:test asm:jcc | topic:codegen-idiom — two sequential test/je for &&, first operand test/jne to body for ||, second operand only evaluated when needed

### Braces / blocks
- [Braces around a SINGLE statement are invisible — `if(c) s;` == `if(c){ s; }` byte-for-byte](braces-single-statement-invisible.md) — c10 — cpp:if cpp:for cpp:while | asm:cmp asm:jmp | topic:wall topic:convention — identical bytes with and without braces, brace presence unrecoverable
- [A `{ }` block around a non-trivial-dtor local moves its `call ~T` to the block's `}`](lexical-block-relocates-dtor.md) — c9 — cpp:dtor cpp:local | asm:call asm:lea | topic:structure-shape — call ~T emitted before a following statement, dtor-call position shifts with the brace

### Operators / calls / params
- [An expression-statement comma is invisible: `(e1, e2);` == `e1; e2;` byte-for-byte (a for-increment comma is NOT)](comma-operator-invisible.md) — c10 — cpp:comma | asm:call asm:mov | topic:wall topic:convention — no distinguishing bytes between a discarded-value comma expression and the equivalent statement sequence; for-increment ops live in the increment block
- [cdecl vs thiscall at the call site: caller add esp,N + YA/SA vs mov ecx,this + no cleanup](call-convention-cdecl-vs-thiscall.md) — c9 — cpp:call cpp:member cpp:static | asm:push asm:call asm:mov | topic:convention topic:mangling — add esp,N after the call, mov ecx,this before a member call, YA/SA vs QAE mangling
- [A qualified `obj->Base::f()` devirtualizes to a direct UAE thiscall (vs vtable dispatch)](virtual-call-vtable-dispatch.md) — c8 — cpp:virtual cpp:call | asm:mov asm:call | topic:codegen-idiom — direct call ?f@..@UAE.. when qualified vs call reg after a double-deref of this, displacement = slot*4
- [A reference parameter and a pointer parameter generate IDENTICAL bodies — only the mangle differs](reference-vs-pointer-param-mangling-only.md) — c9 — cpp:member cpp:const | asm:mov | topic:mangling topic:wall — T& and T* produce the same load-slot-then-deref body, AAH vs PAH is the sole difference

### Engine mechanisms (memory, alloca, construct/destruct)
- [`_alloca`/`ALLOCA` = mov eax,size; call __alloca_probe_16; mov slot,esp — no matching free](alloca-stack-probe.md) — c9 — cpp:local cpp:macro | asm:call asm:sub-esp asm:mov | topic:codegen-idiom topic:convention — call __alloca_probe_16, result pointer IS esp, lea esp,[ebp-N] out, no free call
- [`VOSTOK_NEW`/`VOSTOK_DELETE` family: strip_pointer feeding new_helper<T>::call / delete_helper<T>](vostok-memory-macros.md) — c9 — cpp:new-delete cpp:template cpp:macro cpp:local | asm:call asm:push asm:mov | topic:codegen-idiom topic:convention topic:structure-shape — call strip_pointer then new_helper<T>::call then a null-guarded ctor; a separate target pointer-load statement plus no zero-back store proves `T* child = member; DELETE(child)` rather than `DELETE(member)`
- [Placement `new (p) T` = a null-guarded ctor call, NO allocation; the placement operator is codeless](placement-new-null-guarded-ctor.md) — c9 — cpp:new-delete cpp:ctor | asm:cmp asm:jcc asm:call | topic:codegen-idiom — cmp slot,0 / je over the ctor, mov ecx,slot / call ??0T, no operator new
- [Explicit `p->~T()` vs `delete p` (non-virtual): same skeleton, the flag immediate (0 vs 1) decides the free](explicit-dtor-vs-delete-flag.md) — c8 — cpp:dtor cpp:new-delete | asm:call asm:and asm:jcc | topic:codegen-idiom — call ??1T then flag&1/je over operator delete; xor eax,eax (0) vs mov reg,1 (1)
- [`delete p` on a polymorphic type = push 1 + a virtual call to the ??_G scalar deleting destructor](delete-polymorphic-scalar-deleting-dtor.md) — c8 — cpp:dtor cpp:virtual cpp:new-delete | asm:call asm:mov asm:push | topic:codegen-idiom topic:fold-icf — push 1 then vtable-dispatched call, callee ??_G/??_E, no site-level ??1/operator delete

### Expressions, inlining, float & ABI (complex)
- [`dst = c ? a : b` materializes a tv temp + a final copy (bigger frame); if/else stores direct](ternary-temp-slot.md) — c9 — cpp:ternary cpp:local | asm:cmp asm:jcc asm:jmp asm:mov | topic:structure-shape topic:pdb-locals — tv<N> temp written in both arms then copied to dst, extra push ecx/sub esp, vs if/else direct stores
- [3+ operand `&&`/`||` chains and mixed precedence: the jump TARGETS encode the grouping](short-circuit-chains-precedence.md) — c9 — cpp:if cpp:bool | asm:test asm:jcc | topic:structure-shape — N-1 jne .body + 1 je .end for ||, N je .end for &&, && group's failure routes to the next || operand
- [`inline` free fns and class-body members DO inline under /Od /Ob2 — only out-of-line defs emit a call](inline-under-od-ob2.md) — c9 — cpp:inline cpp:member | asm:call asm:imul asm:mov | topic:inline-vs-call topic:codegen-idiom — small accessor/helper has NO call site, body spliced in; /Ob2 overrides /Od; LTCG inlines even more
- [Scalar float math = movss/addss/mulss on xmm, ONE component at a time, base ptr reloaded per component (/Od)](scalar-float-sse2-ops.md) — c9 — cpp:float | asm:movss asm:addss asm:mulss | topic:codegen-idiom — f3 0f 10/58/59, base pointer reloaded per component, __fltused, __real@<hex>, float global @@3MA
- [Returning a struct by value: >8 bytes = hidden return-pointer (sret); <=8-byte POD = edx:eax](struct-by-value-return-sret.md) — c8 — cpp:return cpp:member | asm:lea asm:push asm:call asm:mov | topic:convention — lea slot/push as extra arg + result via eax (sret), or mov eax/mov edx for an 8-byte POD

## Real-function-mined engine idioms (target asm via pdb_fetch)

### Build reality
- [Optimization level is PER-TU (.vcproj Optimization 0 vs 3) — read the asm to know which world you're in](tu-optimization-level-od-vs-ox.md) — c9 — cpp:for cpp:operator | asm:idiv asm:mul asm:add | topic:convention topic:ltcg — idiv vs magic-multiply, verbose iterator object vs register pointer, same source two codegens

### Engine strings (fixed_string / shared_str)
- [`strings::equal(a,b)` is a register-args helper call (NO pushes); 1st arg->EAX, 2nd->ECX](strings-equal-register-args.md) — c9 — cpp:string cpp:call | asm:call asm:mov asm:movzx asm:test | topic:codegen-idiom — mov eax literal/mov ecx other/call strings::equal/movzx/test/jcc, no push
- [`str.assignf("fmt", ...)` = cdecl varargs: push args R-to-L, push &fmt, push this(lea), call, add esp,N](buffer-string-assignf-varargs.md) — c9 — cpp:string cpp:varargs | asm:push asm:call asm:lea asm:add | topic:codegen-idiom — push vararg/push format literal/lea this/call assignf/add esp,0Ch
- [`buffer_string` accessors inline: c_str()=mov eax,[eax], length()=m_end-m_begin](buffer-string-accessors-inline.md) — c8 — cpp:string cpp:inline | asm:mov asm:sub asm:ret | topic:codegen-idiom topic:fold-icf — c_str this in EAX, length mov eax,[ecx+4];sub eax,[ecx], ICF-folded out-of-line copy

### Engine smart pointers (intrusive_ptr / resource_ptr)
- [`if ( ptr )` = mov eax,[eax]; neg; sbb eax,eax; and eax,&c_ptr (safe-bool mask)](intrusive-ptr-safe-bool-mask.md) — c8 — cpp:bool cpp:operator cpp:template | asm:neg asm:sbb asm:and | topic:codegen-idiom topic:fold-icf — neg/sbb/and against an ICF-folded c_ptr address
- [`ptr = other;` (operator=) = copy-and-swap: two call ::dec + one interlocked_increment + slot swap, ret 4](intrusive-ptr-assign-copy-swap.md) — c8 — cpp:operator cpp:template | asm:call asm:mov | topic:codegen-idiom topic:inline-vs-call — mov [tmp],0/cmp/two dec calls/interlocked_increment/swap/ret 4
- [intrusive_ptr `a == b` / `a == raw` = mov [this]; xor eax,eax; cmp; sete al leaf](intrusive-ptr-compare-sete-leaf.md) — c9 — cpp:operator cpp:bool cpp:template | asm:cmp asm:sete asm:xor | topic:codegen-idiom — xor eax/cmp/sete al, ret (self) or ret 4 (raw rhs)
- [intrusive_ptr operator->/operator* are a bare mov eax,[eax]; ret (ASSERT compiled out, ICF-folded)](intrusive-ptr-deref-bare-load.md) — c9 — cpp:operator cpp:template | asm:mov | topic:codegen-idiom topic:fold-icf — 3-byte leaf, no ASSERT, folded across instantiations

### Engine math (vector / matrix), real-function-mined
- [float4x4::transform_position/direction = 3 scalar dot-products; the 1.f*e30 translation folds to a bare addss](matrix-transform-per-component.md) — c9 — cpp:float cpp:operator | asm:movss asm:mulss asm:addss | topic:codegen-idiom — result.x = v.x*e00+v.y*e10+v.z*e20+1.f*e30, +e30 is a bare addss not mulss-by-1
- [cross_product/mul4x3 stage operands with movaps xmm,xmm (not base reloads) because source caches self=*this](vector-op-movaps-staging.md) — c9 — cpp:float cpp:operator | asm:movaps asm:mulss asm:subss | topic:codegen-idiom — float3_pod self=*this; movaps register copies before each mulss/subss
- [length()/normalize() mix SSE + x87: SSE self-square sum, then call sqrtf + fld1;fdivrp reciprocal](vector-length-normalize-x87.md) — c9 — cpp:float cpp:vector | asm:call asm:fld1 asm:fdivrp asm:mulss | topic:codegen-idiom — call sqrtf (not sqrtss), fld1/fdivrp reciprocal, mixed fmul/mulss

### STLport containers & iteration
- [STLport `++it`: map/set = call _Rb_global::_M_increment (tree walk); list = mov reg,[node]](stlport-tree-list-iterator-advance.md) — c9 — cpp:for cpp:iterator | asm:call asm:mov asm:push | topic:codegen-idiom topic:stlport — _M_increment call for tree, node->next load for list
- [STLport begin()/end() = an ICF-folded iterator copy-ctor CALL into a slot; the name is noise](stlport-begin-end-iterator-ctor-fold.md) — c8 — cpp:iterator cpp:for | asm:call asm:cmp | topic:fold-icf topic:stlport — lea slot/call folded list1<...> ctor, then cmp/setne
- [Contiguous-iterator loop under /Ox: ++it = add reg,sizeof(T); end()-begin() count = reciprocal imul](contiguous-iterator-add-sizeof.md) — c9 — cpp:iterator cpp:for | asm:add asm:imul | topic:codegen-idiom topic:ltcg — iterator is a register raw ptr, add sizeof stride, count via magic-div
- [/Od container loop re-derives end() EVERY turn and boolizes it != end (xor/cmp/setne/movzx/test/je)](od-container-loop-boolized-compare.md) — c8 — cpp:for cpp:iterator | asm:setne asm:movzx asm:cmp | topic:codegen-idiom topic:stlport — ~7-row per-turn preamble re-taking end() + boolized compare

### config / binary_config_value access
- [`(T)config["key"]` = call binary_config_value::operator[] then call operator T](config-index-then-cast.md) — c9 — cpp:operator cpp:cast cpp:member | asm:call asm:movss asm:push | topic:codegen-idiom — push key/call operator[]/call operator float/store
- [A float3/by-ref config cast returns a POINTER + a member-wise 3-dword copy](config-vector-cast-pointer-copy.md) — c8 — cpp:operator cpp:cast cpp:float | asm:call asm:mov | topic:codegen-idiom — operator[] then ICF-folded float3 cast, mov [dst]/[dst+4]/[dst+8]
- [`if ( cfg.value_exists("k") ) m = (T)cfg["k"]` looks the SAME key up TWICE](config-value-exists-double-lookup.md) — c9 — cpp:operator cpp:if cpp:member | asm:call asm:test asm:jcc asm:push | topic:codegen-idiom — value_exists call/movzx/test/je then the same key pushed again for operator[]
- [Nested config["a"]["b"] = TWO chained out-of-line operator[] calls with mov ecx,eax between](config-nested-index-chain.md) — c9 — cpp:operator cpp:cast cpp:member | asm:call asm:mov | topic:codegen-idiom — outer operator[] -> eax -> mov ecx,eax -> inner operator[] -> operator T

### Integer / comparison / static / inheritance / member-pointer (real-function-mined)
- [Integer / and % in a /Od TU = one cdq;idiv (or xor edx;div); quotient EAX, remainder EDX](integer-div-mod-idiv-od.md) — c9 — cpp:operator | asm:idiv asm:div | topic:codegen-idiom — cdq;idiv signed / xor edx;div unsigned, even by a constant; paired /,% fold to one divide
- [Integer / or % by a CONSTANT in a /Ox TU = magic-number multiply (mov eax,<magic>; mul; shr), NO idiv](integer-div-mod-magic-ox.md) — c9 — cpp:operator | asm:mul asm:shr asm:imul | topic:codegen-idiom — reciprocal magic, mul, shr edx, imul divisor back, sub for remainder
- [Signed vs unsigned comparison: the branch mnemonic IS the operand signedness (jl/jge vs jb/jae)](signed-unsigned-compare-mnemonic.md) — c9 — cpp:if cpp:for | asm:cmp asm:jcc | topic:codegen-idiom topic:mangling — jl/jge/jg/jle signed vs jb/jae/ja/jbe unsigned, same source <
- [`static T x = f();` (function-local) = a first-call guard bit in the TU's $S flags word](function-static-runtime-init-guard.md) — c9 — cpp:static cpp:local | asm:test asm:or asm:jcc | topic:codegen-idiom — test [$S],bit/jne/or [$S],bit then init once, value in the named static
- [`(this->*pmf)(args)` (single inheritance) = load pmf from slot, mov ecx,this, call eax](pointer-to-member-fn-call.md) — c9 — cpp:member cpp:call | asm:call asm:mov | topic:codegen-idiom — mov edx,[obj+N]/mov eax,[edx+slot]/mov ecx,this/call eax, args pushed first
- [`class X : public A, public B` ctor = add ecx,sizeof(A) before B::B + one vftable store per base](multiple-inheritance-this-adjust.md) — c8 — cpp:ctor cpp:inherit cpp:virtual | asm:add asm:call asm:mov | topic:codegen-idiom topic:structure-shape — call A::A/add ecx,sizeof(A)/call B::B/dual vftable stores; no thunks in this engine

### FSM state machines
- [`fsm::tick()` = guard, walk the transition list, dispatch finalize/initialize/execute through 4 vtable slots](fsm-tick-transition-loop.md) — c9 — cpp:virtual cpp:while | asm:call asm:mov | topic:codegen-idiom topic:structure-shape — [edx+24h] next walk, boost::function0<bool> predicate, call [edx+0Ch]/[edx+4]/[edx+8]
- [`add_transition(from,to,boost::bind(...))` = bind temp -> function assign_to -> add_transition -> clear](fsm-add-transition-bind-wiring.md) — c8 — cpp:template cpp:member | asm:call asm:push asm:lea | topic:codegen-idiom topic:structure-shape — call boost::bind/assign_to/push to,from/call add_transition/call clear
- [FSM consume: fsm.tick(); static_cast<Derived*>(current_state()); derived->virtual(dt)](fsm-run-static-cast-dispatch.md) — c8 — cpp:virtual cpp:cast | asm:mov asm:call | topic:codegen-idiom — current_state inlined mov [fsm+10h], static_cast no-op temp chain, slot past the 4 pures
- [Empty-bodied derived state ctor: the vtable store is sunk into the new-expression, no standalone ctor symbol](derived-state-vtable-store-at-new-site.md) — c9 — cpp:ctor cpp:virtual cpp:new-delete | asm:call asm:mov | topic:codegen-idiom topic:inline-vs-call — call Base::Base then mov [obj],??_7Derived@@6B@ at the new-site

### boost::bind / boost::function callbacks
- [Invoking a boost::function MEMBER = mov ecx,this; add ecx,<member_off>; call functionN::operator()](boost-function-member-invoke.md) — c9 — cpp:call cpp:template | asm:add asm:mov asm:call | topic:codegen-idiom — add ecx,N member offset then call operator(), siblings differ only by N
- [`boost::functionN::operator()` body = empty-guard + bad_function_call throw + manager indirection (and eax,~1; call [eax+4])](boost-function-operator-call-body.md) — c9 — cpp:template cpp:operator | asm:cmp asm:and asm:call | topic:codegen-idiom — cmp [esi],0/jne, bad_function_call thrown even w/ EH off, and 0FFFFFFFEh, call [eax+4], ret 4*N
- [`void_function_obj_invoker0<bind_t<...>>::invoke` = a frameless 3-insn tail thunk mov eax,[esp+4]; mov ecx,[eax]; jmp ecx](boost-function-invoker-tail-thunk.md) — c9 — cpp:template | asm:mov asm:jmp | topic:fold-icf — read function_buffer&, load functor field 0, jmp; ICF-folded across instantiations
- [`boost::bind(&C::m, this)` functor build (sret) = pack mf code-ptr at +0 and bound this at +4](boost-bind-functor-field-store.md) — c8 — cpp:template | asm:mov asm:lea | topic:codegen-idiom — sret bind_t in [ebp+8], mov [ecx],edx mf ptr then mov [edx+4],ecx bound this

### Network wire I/O (packet_reader)
- [`reader.r<T>()` inlined = load m_pointer(+4), deref, advance by sizeof(T), store back](packet-reader-r-template.md) — c9 — cpp:template cpp:member | asm:mov asm:add asm:inc | topic:codegen-idiom topic:wire-io — mov [this+4]/deref/add 4 (or inc)/store back, then store to a target member
- [`reader.r<float>()` = integer load to a stack slot, then movss reload (GPR->XMM bounce)](packet-reader-r-float-bounce.md) — c8 — cpp:template cpp:member cpp:float | asm:mov asm:movss | topic:codegen-idiom topic:wire-io — mov [esp],edx then movss xmm0,[esp], this in ECX
- [`reader.r_string<N>(buf)` = inlined u8 length-prefix read, memcpy, then NUL-terminate at buf[len]](packet-reader-r-string.md) — c8 — cpp:template cpp:member cpp:string | asm:movzx asm:call asm:mov | topic:wire-io topic:codegen-idiom — length byte/memcpy/add m_pointer,len/byte ptr [len+dst],0

### Listener / event dispatch
- [Event dispatch = build a stack functor holding the payload, then m_subscribers.for_each(functor) — never a raw loop](listener-for-each-dispatch.md) — c9 — cpp:template cpp:member | asm:lea asm:call asm:push | topic:codegen-idiom topic:stlport — lea slot/push/call intrusive_list::for_each<void_predicate_ref<...callback_predicate>>
- [`intrusive_list::for_each` body = empty-guard, mutex_raii, then a save-next-BEFORE-pred-call loop](intrusive-list-for-each-body.md) — c9 — cpp:for cpp:template cpp:iterator | asm:cmp asm:je asm:call | topic:codegen-idiom topic:stlport — cmp [list+24h],0 empty, mutex_raii, next=current->m_next before call, locals raii/current/next

### Threading (locks, interlocked)
- [A scoped-lock `mutex_raii raii(lock);` = ctor (acquire) at entry + dtor (release) at EVERY exit](scoped-lock-raii.md) — c9 — cpp:dtor cpp:local cpp:ctor | asm:call asm:lea | topic:structure-shape topic:codegen-idiom — call mutex_raii/::lock at entry, call ::clear/::unlock at each return, one acquire / N releases
- [`threading::mutex` lock/unlock = Enter/LeaveCriticalSection; tiny unlock inlines as __imp__LeaveCriticalSection@4](mutex-critical-section-lowering.md) — c9 — cpp:member | asm:call asm:push | topic:codegen-idiom topic:convention — call mutex::lock spins on TryEnterCriticalSection, LeaveCriticalSection with no matching Enter = inlined unlock
- [`threading::interlocked_*` = a lock-prefixed RMW: increment=lock xadd;inc, CAS=lock cmpxchg, exchange=bare xchg](interlocked-lock-prefixed-rmw.md) — c9 — cpp:call | asm:lock asm:xadd asm:cmpxchg asm:xchg | topic:codegen-idiom — mov eax,1;lock xadd;inc eax (post-value), lock cmpxchg, xchg; call site keeps a call under /Od
