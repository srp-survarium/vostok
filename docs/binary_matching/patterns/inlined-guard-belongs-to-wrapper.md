# An inlined guard the callee's own COMDAT lacks belongs to a WRAPPER above it

- **confidence**: 9/10
- **tags**: cpp:operator cpp:template cpp:inline cpp:const | asm:cmp asm:je | topic:inline-vs-call topic:mangling topic:structure-shape
- **symptoms**: an inlined assignment shows `mov reg,[obj]; cmp reg,<src>; je` that our
  base does not emit; the obvious owner of the guard (the innermost callee) is
  ALSO present as a standalone COMDAT in the target - and that COMDAT has **no**
  guard.

## The trap

Batch B7 read `render_cc_bool::fill_macro`'s target asm, saw

    mov  eax, [esi+114h]      ; definition.m_begin
    lea  ecx, [esi+114h]
    cmp  eax, edx             ; ... vs the source pointer
    je   short <skip>
    mov  [ecx+4], eax         ; clear(): m_end = m_begin
    mov  byte ptr [eax], 0    ; clear(): *m_end = NULL
    call vostok::buffer_string::operator+=

and concluded `buffer_string::operator=(pcstr)` was missing a `m_begin != s`
self-assign guard. The reasoning is sound - `clear(); *this += s;` IS
`buffer_string::operator=(pcstr)`, and the guard sits immediately in front of it.

It is still wrong. Adding the guard there cost a **measured regression**: the
target's standalone `buffer_string::operator=(char const*)` COMDAT is

    mov eax, [ecx]
    mov [ecx+4], eax
    mov byte ptr [eax], 0
    jmp vostok::buffer_string::operator+=

- four instructions, **no guard** - and our guardless source had been matching it
byte-for-byte at 100%. The guard turned that 100% row into an unpaired one.

## The rule

**Before you attribute an inlined guard to a function, look up that function's
own out-of-line COMDAT in the target index.** If the COMDAT lacks the guard, the
guard is not in that function - it is in a *wrapper* one level up that the
compiler also inlined, and whose own COMDAT you have not looked at yet.

Here the wrapper was `fixed_string<Size>::operator=`, and the target has it:

    ??4?$fixed_string@$0BAE@@vostok@@QAEABV01@QBD@Z
      vostok::fixed_string<260> const& vostok::fixed_string<260>::operator=(char const* const)

      mov  eax, [esi]        ; begin()
      cmp  eax, ecx          ; ... != src
      je   short .1
      push ecx
      mov  [esi+4], eax      ;  \
      mov  ecx, esi          ;   > (buffer_string&)*this = src;  (the callee, INLINED)
      mov  byte ptr [eax], 0 ;  /
      call vostok::buffer_string::operator+=
    .1:
      mov  eax, esi          ; return *this;
      ret

with `; locals (1): char const* const src` and three consecutive line records -
i.e.

```cpp
template <int Size>
fixed_string<Size> const&   fixed_string<Size>::operator = (value_type const* const src)
{
	if ( begin() != src )
		(buffer_string&)*this		=	src;
	return								*this;
}
```

## Two corroborating tells

1. **A missing overload announces itself in the mangling.** The demangled
   parameter reads `char const* const`, not `char const*`. MSVC encodes a
   top-level `const` on a pointer parameter as `Q` where a plain pointer is `P`
   (`...@QAEABV01@QBD@Z` vs `...PBD@Z`), so the demangler is telling you the
   source really wrote `T const* const src`. Our tree only had the catch-all
   `template <class src_type> operator = (src_type const& s)`, which mangles
   completely differently - the target symbol was simply absent from our build.
2. **A call-site temporary is the by-value/by-reference tell.** Where the target
   inlined the pcstr overload directly, our base emitted
   `mov [esp+0Ch], ecx; lea edx, [esp+0Ch]; push edx; call ...operator=<char const*>`
   - the stack slot exists only to materialise a *reference* to a pointer. A
   spurious `lea` of a freshly stored pointer at an assignment site means the
   real overload takes the pointer **by value**.

Overload resolution stays correct after adding it: a string *literal* argument
(`char const[N]`) still binds to the template by identity, while a `pcstr`
lvalue prefers the non-template. The target confirms both -
`path_string_impl::operator=<char const [1]>` and the pcstr overload coexist.

## Payoff

One overload, measured repo-wide (`vostok derive diff`), 36 improvements against
11 regressions, eight functions moved to byte-exact:
`core_debug_engine::create_folder_r` 58.4 -> 100,
`engine_world::get_user_data_directory` 48.8 -> 100,
`native_path_string::convert` 78.7 -> 100 (plus `native_path_string::operator=`
newly paired at 100), `memory::writer::save_to` 92.3 -> 100,
`ui_text::set_text` 83.7 -> 100, `npc_stats::set_stats` 87.3 -> 100,
`static_render_model_instance::get_surface_stats` 30.2 -> 100.
