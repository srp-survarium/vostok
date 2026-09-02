# physics - matching notes

- Namespace `vostok::physics`; Bullet 2.79 underneath with Survarium's own
  modifications, catalogued in [../bullet/README.md](../bullet/README.md)
  (structural delta) and [../bullet/functional_handoff.md](../bullet/functional_handoff.md).
- Status lives in the ledger: `python3 -m vostok ledger report --module physics`.

## Reading the assembly

- If a function is close but the loads of vectors come out of order, something
  got inlined that the target kept out of line (or the reverse).
- `btVector3::lerp` vs `btVector3::setInterpolate3`: the second builds in place
  and is the one Vostok Games used; the two also differ slightly in body, so the
  emitted arithmetic tells them apart.
