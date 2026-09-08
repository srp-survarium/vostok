# Maya animation authoring recovery

The non-gold `maya_animation` project contains SDK authoring code that is not
required by the game runtime. `sources/vostok/maya_animation/sources/discrete_data.cpp`
includes the absent
`vostok/animation/sources/bi_spline_skeleton_animation.h` implementation and
therefore cannot compile from the recovered game tree.

Game-only Release and Debug graphs omit that translation unit from their
temporary project view. The authoritative project and original non-gold body
remain available unchanged for a future SDK build.

When SDK work resumes, recover the bi-spline authoring family rather than
removing this guard piecemeal. The relevant missing surface includes the
unbaked bi-spline skeleton and bone animation implementations, `bi_spline_data`,
and the Maya `discrete_data_impl` conversion/checking path. The historical
`sdk/v0100b-editor` branch contains a compatibility implementation derived from
the older STK2 SDK, but it is not evidence for exact v0.100b authoring behavior.
