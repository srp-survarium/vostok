# Render legacy triage

Legacy entries are removed only after their target replacement is live or their
incompatibility with the shipped interface is recorded here.

## Obsolete predecessor interfaces

- `engine/sources/render_output_window_cook.{h,cpp}`: implemented the earlier
  `resources::unmanaged_cook` allocation interface. The shipped type derives
  from `resources::translate_query_cook` and owns `translate_query` and
  `delete_resource`; no legacy method body maps faithfully to that target
  virtual surface.
- `engine/sources/scene_view_cook.{h,cpp}`: implemented the earlier
  `resources::unmanaged_cook` allocation interface. The shipped type derives
  from `resources::translate_query_cook`; its old allocation/create/destroy
  methods do not map to the target `translate_query`/`delete_resource` surface.
- `engine/model_format.h`: the target-owned enums, model header, quantized basis,
  and skinned vertex layouts were consolidated into the live header. Legacy
  `s24_tc_base`, `vert_static`, and `vb_sw_struct` have no retail PDB record or
  live consumer and were omitted.
