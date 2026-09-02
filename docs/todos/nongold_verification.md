# Non-gold verification & content-pipeline self-checks (deferred port)

Status: **excluded from all builds** so gold / Release / Debug / dynamic compile
without pulling in the authoring stack these checks depend on. Port later.

## What it is

`animation/sources/check_animation_data.{h,cpp}` (`test_data`, `compare`) is a
`!MASTER_GOLD` **cooker self-check** written by Konstantin Slipchenko (2010). Its
one caller is the animation cooker, `resource_cookers.cpp:50`, and it fires only
when a QA check-config is present. It validates three invariants of the
Maya -> baked-animation pipeline:

- **compression fidelity** - the spline-compressed animation matches the raw Maya
  per-frame samples within tolerance (logs the exact bone/channel/frame/delta);
- **serialization roundtrip** - build-from-source vs load-from-disk are identical;
- **relocatability** - the data survives a `memcpy` to a new address (no absolute
  internal pointers), so it can be streamed/moved.

It runs at asset-build time, never at game runtime, and is stripped from the
shipped product (`MASTER_GOLD`). The matched game does not need it.

## Why deferred, not ported now

There is no gold binary that contains this code, so there is nothing to
binary-match it against - it is a functional port, not a match. It also drags in
the bi_spline **authoring** subsystem (`bi_spline_skeleton_animation.h` and family),
which is likewise content-pipeline, not runtime (the *baked* bi_spline runtime form
is already in gold and matched). For a working / linkable matched game none of it is
needed, so we exclude it rather than block builds on it.

## To port later (from STK2 / the vostok-sdk-v0100b source tree)

The real source exists in the STK2 leak (same X-Ray 2.0 / Vostok lineage), mirrored
in `resource-porter/.work/vostok-sdk-v0100b/sources/vostok/animation/sources/`:
`check_animation_data.{h,cpp}` plus the ~27-file bi_spline/skeleton-animation
authoring family. Bring them in, apply whatever fixes they need to compile in this
tree, and treat "it builds and the check runs" as the bar - **not** byte-matching.

Only warranted if we want the content-authoring / editor pipeline. The managed
(.NET/WPF) editor is a separate wall (`NETFX_ABSENT`, no netfx toolchain payload).
