# Sweep a module for access/CV-char mangling mismatches - each hit pairs a function for free

tags: cpp:const cpp:member cpp:class | asm:none | topic:mangling topic:pairing topic:structure-shape

## Symptom
A target function is unpaired even though the base clearly emits "the same" function: same
class, same name, same parameter types, similar size. Diffing the two mangled names shows a
ONE-LETTER difference in the `@@<access><cv>E` group that precedes the parameter list:

```
target ?use_texture@?$xs_descriptor@Ups_data@render@vostok@@@render@vostok@@QAE_NPBD@Z
base   ?use_texture@?$xs_descriptor@Ups_data@render@vostok@@@render@vostok@@QBE_NPBD@Z
                                                              ^^^ QAE = non-const, QBE = const

target ?fill_macro@render_cc_bool@render@vostok@@UBE_NAAUshader_macro@23@@Z   U = public virtual
base   ?fill_macro@render_cc_bool@render@vostok@@EBE_NAAUshader_macro@23@@Z   E = private virtual

target ??0res_texture@render@vostok@@AAE@_N@Z    A = private
base   ??0res_texture@render@vostok@@QAE@_N@Z    Q = public
```

MSVC's `__thiscall` mangling encodes access in the first letter (`Q/R/S` public, `I/J/K`
protected, `A/B/C` private; `U/V/W` public virtual, `M/N/O` protected virtual, `E/F/G`
private virtual) and const-ness in the second (`A` non-const, `B` const). Pairing is on the
mangled name, so either mismatch makes the function invisible.

## Why it's ground truth
The access specifier and the method's const-ness are recorded in the shipped binary itself.
This is one of the few places where the target *proves* a source detail outright - it
overrides both the current source and any judgement about what "should" be const or public.

## The sweep (cheap, no build)
Blank the access/CV group and intersect the two rich indexes:

```python
import json, re, collections
def load(p):
    m = {}
    for ln in open(p):
        d = json.loads(ln); m.setdefault(d['mangled'], d.get('file') or '')
    return m
T = load('binaries/rich/target/index.jsonl'); B = load('binaries/rich/base/index.jsonl')
pat = re.compile(r'@@([A-Z])([A-Z])E')
key = lambda m: pat.sub('@@__E', m, count=1)
bk = collections.defaultdict(list)
for m in B: bk[key(m)].append(m)
for m, f in sorted(T.items()):
    if not f.startswith('vostok/<module>/') or m in B: continue
    for bm in bk.get(key(m), ()):
        if bm != m: print(f, '\n  target', m, '\n  base  ', bm)
```

One pass over `render/core` returned 11 hits (`use_texture`, four `res_texture` members,
six `render_cc_{bool,float,u32}` overrides); fixing them paired all of them - the whole
`res_texture` set landed at 100% / 96.4% / 62% / 14.6% with no body change.

## Applying the fix
* **const:** drop or add `const` on the method (decl AND out-of-line definition).
* **access:** move the declaration between `private:` / `protected:` / `public:` **without
  reordering it** - repeat the specifier instead (definition order is structure).
* A newly private ctor/dtor/mutator usually needs `friend` declarations for its real callers
  (`friend class resource_intrusive_base;` for a private `destroy_impl`, plus whichever
  classes construct it). Friends are codegen-neutral, so this costs nothing and is the only
  way the original could have compiled with that access level.

## Related
- `param-pointer-const-mangling.md` - top-level const on a pointer PARAMETER (`QAV` vs `PAV`).
- `top-level-const-value-param-od-vs-optimized.md` - const on a value param.
- `mutable-member-const-method.md` - when the target says const but the body assigns.
