# A free function mangled `?f@ns@@YA...@Z` (not `??$f@...`) that takes `T<X>` = friend DEFINED inside the class template

tags: cpp:template cpp:friend cpp:operator | topic:mangling topic:structure-shape

## Symptom
The target has a namespace-scope function whose parameter is a class-template
specialisation, but the mangling has NO template prefix:

```
target: ?compare@render@vostok@@YAHABV?$res_xs@Ugs_data@render@vostok@@@12@0@Z   ; NON-template
base  : ??$compare@Ups_data@render@vostok@@@render@vostok@@YAHABV?$res_xs@...@Z  ; template inst
```

The two never pair (objdiff reports the target symbol `unpaired`) even when the bodies
are byte-identical and the same size. The rich index shows several PDB records for
different instantiations resolving to ONE address (ICF folded the identical bodies), all
printing the same non-template name.

## Cause
Writing

```cpp
template < typename shader_data >
inline s32 compare( res_xs< shader_data > const& left, res_xs< shader_data > const& right );
```

produces a FUNCTION TEMPLATE (`??$compare@...`). The target instead defines the function
as a **friend inside the class template**:

```cpp
template < typename shader_data >
class res_xs : public resource_intrusive_base {
    ...
    friend s32 compare( res_xs<shader_data> const& left, res_xs<shader_data> const& right )
    {
        ...                              // full body, right here
    }
};
```

Each instantiation of `res_xs<X>` then injects a distinct ORDINARY (non-template) function
`compare( res_xs<X> const&, res_xs<X> const& )` into the enclosing namespace, mangled
`?compare@...@@YAH...` - exactly the target's spelling. The idiom also explains two other
observations that a namespace-scope template cannot: the body freely touches the class's
PRIVATE members with no `friend` declaration or accessor in sight, and all N instantiations
report the SAME source-line range in the PDB (one definition, N symbols).

The function is only findable by ADL, which is how it is used (`compare( *left, *right ) < 0`
inside a comparison predicate) - so a member `s32 compare( T const& ) const` reconstructed
"because the predicate needs one" is the wrong shape; delete it and point the predicate at
the free function.

## Fix
Move the definition into the class body as a `friend`, keep it at the source position the
target's line numbers indicate, and switch the call sites from `left->compare( *right )` to
`compare( *left, *right )`. Verify with the mangled name, not the score:

```
python3 - <<'EOF'
import json
for l in open('binaries/rich/base/index.jsonl'):
    if 'render::compare' in l:
        d=json.loads(l); print(d['mangled'][:110], hex(d['rva']+d['image_base']), d['size'])
EOF
```

`?compare@...` = right, `??$compare@...` = still a template.

## Precedent
`vostok::render::compare( res_xs<T> const&, res_xs<T> const& )` (render batch B4): as a
namespace-scope template it sat unpaired at 82 bytes next to an 82-byte target; converting
it to an in-class friend fixed the mangling. Same reasoning retires the invented member
`res_xs<T>::compare( res_xs<T> const& ) const` (no such target symbol) in favour of
`resource_manager::compare_shader_predicate` calling the free `compare`.

## Related
- `class-vs-struct-forward-decl-mangling.md` (the other one-token mangling split).
- `three-way-compare-family.md` (the body this idiom carries).
