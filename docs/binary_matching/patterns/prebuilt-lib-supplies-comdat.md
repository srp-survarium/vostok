# A prebuilt `.lib` supplies the COMDAT: base is unpaired AND compiled for a different ISA
tags: cpp:comdat cpp:inline cpp:ctor | asm:fld1 asm:fldz asm:movss asm:xorps | topic:pairing topic:compiler-context topic:wall
symptoms: target function has a delinked symbol and a real body, base emits NO symbol at all (its objdiff unit lacks it, `pdb_rich_query --function` finds nothing) yet the call site links; the code that IS in the base exe uses x87 (`fld1`/`fldz`/`fst`) where the target uses SSE (`xorps`/`movss`); the tree has a declaration and no definition
confidence: 9/10

When a module links a **prebuilt** library alongside its own sources, the linker can
satisfy a COMDAT from that library instead of from our compile. It looks like a
matching gap but is a **build-input** gap, and it hides two separate problems:

1. **No base symbol.** The delinker builds its `.obj`s from PDB *line* info. A
   prebuilt lib whose objects contribute no line info to our PDB (or whose source
   paths do not map through the single `--engine-path` prefix) leaves the function
   with **no base symbol at all** — the ledger scores it 0% / unpaired with no
   `struct_class`, and no source edit can move it.
2. **Different compiler context.** The vendor/other build used *their* switches, so
   the body is a different ISA or opt context than ours — the bytes could not match
   even if it did pair.

**How to prove it, in three cheap steps:**

```sh
# a) nothing in-tree defines it
grep -rn 'Class::Method' sources/
# b) a prebuilt lib does
python3 - <<'EOF'
import glob
sym=b'??0MatrixState@Render@Scaleform@@QAE@XZ'
for l in glob.glob("binaries.prebuilt/**/*.lib", recursive=True):
    if sym in open(l,'rb').read(): print(l)
EOF
# c) the linked code is outside our compile: decode the call site's E8
#    displacement out of binaries/Win32/survarium-dx11-win32-gold.exe and look the
#    resulting rva up in binaries/rich/base/index.jsonl -- NO record.
```

`strings -a <lib> | grep '\.obj$'` names the objects and usually the tree they were
built in, which tells you whether it is a vendor drop or one of our own out-of-tree
builds.

Worked example (scaleform, v0.100b). **Both** sides link a prebuilt GFx, but not the
same one:

- base: `binaries.prebuilt/Win32/libraries/shipping/libgfx*.lib` — gitignored,
  ~197 MB, built **out of tree** from the GFx **4.2.22** sources
  (`sources/scaleform/Include/GFxVersion.h`), its members named
  `Z:\...\surv\vostok_4\binaries\Win32\intermediates\gfx\libgfx\*.obj`. Only headers
  plus 46 platform/sample `.cpp` live in `sources/scaleform`; `Render_HAL.cpp` and
  friends are not in this repo at all.
- target: the vendor's GFx **4.2.21 Shipping** objects — `survarium.pdb` names
  `C:\w\42216f4658640829\Scaleform\Releases\GFx_4.2.21\Obj\Win32\Msvc90\Shipping\GFx\Render_HAL.obj`,
  `...\Render_BufferGeneric.obj`, `...\Render_Matrix4x4.obj`, and so on.

`Scaleform::Render::MatrixState::MatrixState()` / `MatrixState(HAL*)` are declared in
`Render_HAL.h` and defined nowhere in-tree, so our libgfx supplies both (1564 target
bytes — half the module's whole remaining byte gap). The target's copies delink fine
into `binaries/objdiff/target/scaleform/src/render/render_hal.h.obj`; ours produce no
base symbol (our base PDB contains zero `GFx_4.2` strings), so the rows sit at 0%.
The bodies also differ in kind: ours is x87, the target's is SSE.

Same lib is the only standalone copy of `Matrix4x4<float>::Matrix4x4()` and
`Rect<float>::Rect(float,float,const Size&)` — header-inline ctors our own TUs inline
everywhere without emitting a COMDAT — which is why their parks read "target retains
the inline ctor as a COMDAT while base emits no standalone RVA".

The library also *defines* functions we compile ourselves
(`MeshCacheItem::MoveToCacheListFront`, `GFx::Value::operator=`,
`RBGenericImpl::RenderBufferManager::*`). There our COMDAT wins and gets a base
symbol, but the target inlined those at every site and emits none — so they show up
as base-only symbols and as inline-vs-call size rows in their callers.

**Do not** chase any of this from source and **do not** add an anchor to force a
COMDAT: an anchor would emit *our* body at *our* rva and "pair" a function the
shipped build took from a different library. The real next step is a build input —
rebuild the prebuilt with `/Zi` under this tree's engine path and with the switches
the shipped build used, then re-measure.
