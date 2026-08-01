# Triaging a `sema` BRANCH-COUNT row: real source shape vs a core-header inline wall

- **confidence**: 9/10
- **tags**: cpp:inline cpp:template cpp:operator | asm:call asm:jcc | topic:inline-vs-call topic:sema topic:triage
- **symptoms**: `sema sweep` says `BRANCH-COUNT`, the target has many more blocks
  than the base, and the extra target blocks form repeating little clusters that
  all sit around one `call` the base still makes.

`BRANCH-COUNT` is the productive `sema` class (batch B7: 5 real bugs in 9 opened),
but on an optimized module most of its rows are **not** caller-shape bugs. Batch
B8 opened 14 render/core rows; the split was 3 source-shape, 11 inline-vs-call.
The two are distinguishable in one read of `blocks --diff`, without touching the
source.

## The tell

Read the **base** side of the divergent block, not the target side.

* **Inline wall** - the base block ends in a `call` to a *known, named, small*
  callee and the target's extra blocks are that callee's body, expanded in place.
  The caller's own statement records match; only the callee's blocks are new. The
  target's extra blocks repeat once per call site.
* **Real source bug** - the target's extra blocks contain something with **no
  counterpart in the base at all**: a literal the base never loads, a call the
  base never makes, an extra member store. There is nothing to "expand into".

## Worked examples, render/core, batch B8

Inline walls (do not touch the caller - fix the callee's ticket, or bank it):

| function | base calls | target inlines |
|---|---|---|
| `res_declaration::get`, `effect_compiler::end_technique`, `backend::~backend` | `single_threading_policy::increment/decrement`, `~signature_layout_pair`, `~textures_handler<N>` | the refcount `dec [eax]; cmp [eax],0; jne` and the member releases |
| `effect_compiler::begin_pass` (29 vs 77 blocks!) | `intrusive_ptr<res_xs_hw<T>>::operator=`, `xs_descriptor<T>::reset` x3 | both, x3 |
| `textures_handler<1>::set_overwrite`, `resource_manager::find_registered_sampler` | `vostok::operator==(buffer_string const&, char const*)` | `detail::strcmp_s` + the `strcmp` intrinsic's 2-byte-unrolled loop |
| `textures_handler<1>::set_overwrite` | (nothing - base inlines) | the *reverse*: the target CALLS `math::max(u32,u32)`, a non-template overload we do not declare |

A 48-block deficit therefore proved to be **zero** caller work. Note the last row:
the wall runs in both directions, so "the target has more blocks" is not the
signal - "the extra blocks are a known callee's body" is.

Real source-shape bugs from the same sweep (each had a target-only literal or
call with no base counterpart):

* `res_render_output::present` - three `cmp eax,887A00xxh; je` against
  `DXGI_ERROR_DEVICE_REMOVED/_DEVICE_RESET/_DRIVER_INTERNAL_ERROR` and a
  `device::on_device_removed()` arm; base only had `CHECK_RESULT`.
* `constants_handler<N>::update_buffers` - the inlined
  `shader_constant_buffer::update()` reads a *different* API
  (`UpdateSubresource` under a `backend::ref().disabled_shader_constansts_set`
  guard) than our `Map`/`CopyMemory`/`Unmap` body. An inline wall whose callee
  body is also wrong: fixing the callee fixes both.
* `texture_options_binary_cooker::on_binary_config_loaded` - a stack-built
  `fs_new::virtual_path_string` plus `buffer_string::replace("resources/",
  "resources.sources/")`, a call and two literals absent from the base.

## Corollary: `TOPOLOGY` on equal block counts can still be an optimizer artifact

`backend::flush` is 63 blocks / 35 branches on **both** sides with one retargeted
`jmp` (base `jmp B33`, target `jmp B36`). That single edge is **cross-jumping**:
both `IASetVertexBuffers` call sites end in the identical four-instruction
indirect-call suffix (`mov ecx,[eax]; push eax; mov eax,[ecx+48h]; call eax`), and
the target tail-merged them into one shared block while the base emitted both.
Nothing about the source differs - the two arms even push different argument
counts. Do not reopen it.
