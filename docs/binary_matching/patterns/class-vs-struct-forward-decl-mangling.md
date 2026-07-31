# A `class` vs `struct` forward-declaration mangles a param as PAV vs PAU

tags: cpp:struct cpp:class | topic:mangling topic:buildability

## Symptom
A consumer TU references a member function and the link fails with
`LNK2001: unresolved external symbol` even though the callee's `.obj` clearly
contains a same-named symbol. Diffing the mangled names shows they differ in ONE
letter at the parameter's type tag:

```
reference (consumer): ?process_keyboard@...@@QAE_NPAUworld@input@vostok@@...   ; PAU = ptr-to-struct
definition (callee) : ?process_keyboard@...@@QAE_NPAVworld@input@vostok@@...   ; PAV = ptr-to-class
```

`PAU` = pointer to a *struct*, `PAV` = pointer to a *class*. MSVC mangles the
class-key (`struct`/`class`) of a type into pointer/reference parameters, so the
two are DIFFERENT symbols and do not join.

## Cause
The two TUs see the type through different forward declarations:

- the callee's header forward-declares `class world;` (→ `PAV`),
- the consumer pulls in the type's REAL header where it is `struct world { ... };`
  (→ `PAU`, matching the shipped/target symbol).

Whichever declaration the definition's TU compiled against wins for the DEFINITION;
the reference uses the consumer's. They never meet.

## Fix
Make the forward declaration agree with the type's real class-key (and with the
target's mangling). Check the rich index for which the SHIPPED build used:

```
grep -o '<fn>@<class>@<ns>@@[^"]*' binaries/rich/target/index.jsonl   # PAU vs PAV
```

then correct the wrong forward-decl (here `class world;` → `struct world;` in the
callee's header). This is a buildability + correctness fix: it also makes the
callee's own symbol pair against the target. Precedent: swf_input_translator.h's
`class world;` broke `process_keyboard`'s link the moment chat_handler (which
includes the struct `<vostok/input/world.h>`) referenced it; the target mangles
`PAUworld`.

## Variant: the class-key is wrong in the DEFINITION, so the function never pairs
The same one-letter split silently costs a match even when the link succeeds: if
BOTH sides of our build agree on `class T` but the target used `struct T`, every
signature mentioning `T*`/`T&`/`T` as a template argument mangles `V` where the
target mangles `U`, and objdiff reports the target function as unpaired with a
size but no `fuzzy_match_percent` (the base symbol sits in the same `.obj` under
the other spelling - `strings -a binaries/objdiff/base/<unit>.obj | grep <fn>`
shows it).

Sweep it per module instead of one symbol at a time - the target index answers
the class-key for every type at once:

```
for t in ambient_volume clouds environment_probe material_effects_instance ...; do
  u=$(grep -c "U$t@render@vostok@@" binaries/rich/target/index.jsonl)
  v=$(grep -c "V$t@render@vostok@@" binaries/rich/target/index.jsonl)
  echo "$t: struct(U)=$u class(V)=$v"
done
```

Compiler warning **C4099** (`type name first seen using 'struct' now seen using
'class'`) is a free pre-filter: every C4099 in the build log names a type whose
declarations disagree inside our own tree, and the target index says which
spelling is right. Fixing the class-key is codegen-neutral (only the mangle
moves), so it is always safe. Precedent: render/engine had `render_surface_instance`,
`material_effects_instance`, `tracer_model_instance`, `ambient_volume`, `clouds`,
`environment_probe`, `geometry_batch`, `grass_patch`, `post_process_parameters`
all declared `class` against a `struct` target - `scene::set_sky_material` went
from unpaired to 100% on the class-key fix alone.

## Related
- `param-pointer-const-mangling.md` (QAV vs PAV - a CONST mismatch, not class-key).
- `mangled-join-failure.md` (the general "0%/unpaired = check the mangle first").
