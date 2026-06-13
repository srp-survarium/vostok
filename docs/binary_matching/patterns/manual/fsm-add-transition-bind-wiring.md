# `add_transition(from, to, boost::bind(...))` = bind temp -> boost::function assign_to -> push(to,from) call add_transition -> clear
tags: cpp:template cpp:member | asm:call asm:push asm:lea | topic:codegen-idiom topic:structure-shape
symptoms: call boost::bind<...>, basic_vtable1<>::assign_to, push to, push from, call fsm::add_transition, call boost::function1<>::clear
confidence: 8/10

Every FSM transition is wired by one statement that (1) builds a `boost::bind` temp on the stack,
(2) copies it into a `boost::function` slot via `assign_to<bind_t<...>>` + the stored-vtable
tag dance, (3) pushes `to` then `from` then the predicate and `call fsm::add_transition`, (4) tears
the temp down with `boost::function::clear`. A free predicate is `boost::bind<bool>(&fn)` (2 args,
`add esp,8`); a bound member is `boost::bind(&Class::pred, this)` (3 args, `add esp,0Ch`).

```cpp
m_logic->add_transition( inactive, start, boost::bind< bool >( &true_predicate ) );
m_logic->add_transition( start, landing, boost::bind( &jump_logic::landing_predicate, this ) );
```
```asm
push  survarium::true_predicate / call boost::bind<bool> / add esp,8
call  ...basic_vtable1<...>::assign_to<...bind_t<...>>   ; pack into boost::function
push  eax / push edx                  ; from / to
call  vostok::ai::fsm::add_transition
call  boost::function1<void,char const*>::clear          ; temp teardown
```
Wall-ish: the bind<>/assign_to/clear names are ICF-folded and delinker-misnamed (boost-bind-icf-sibling-fold.md) — match the call SHAPE, not the names. cite: jump_logic::initialize_logic jump_logic.cpp:110-111.
