# Match queue — durable diff→0 worklist (pdb_divergence)

The persistent, regenerable worklist for the ongoing [member]/[fn-order]/[size]/
[values]/[const] matching + %-recovery. Raw CodeView line-table counts are an
opt-in forensic diagnostic, not a default work category. Snapshot below;
**regenerate fresh** before working (the chain moves):

    cd <vostok-pdb-parser clone>   # or any worktree with pdb_divergence on PATH
    pdb_divergence --base-pdb <wt>/binaries/Win32/survarium-dx11-win32-gold.pdb \
      --base-engine-path 'z:\home\...\<wt>\sources\' \
      --target-pdb $SURVARIUM_BIN/survarium.pdb --target-engine-path 'c:\survarium\sources' \
      --skip bullet --skip opcode --skip stlport --skip vorbis --skip ogg --skip zlib \
      --skip render --skip sound --skip scaleform

## Deferred / excluded
- **sound** — under dedicated manual reconstruction (wip/sound-reconstruct), skip here.
- **render / scaleform** — matched last / vendor.
- **`/* no source */`** — PARKED (usage unverifiable; resume later via non-100%-caller + access scoping).
- **[visibility]** — DROPPED (base PDB doesn't encode access; not source-steerable).

## Yield tiers (where +exact comes from)
- **[member] / [size]** — HIGH: layout fixes ripple (math::color→light +, input::keyboard +11, particle +17, engine +38). Numeric/reliable. WORK THESE FIRST.
- **Legacy `[stmt]` snapshot rows** — these were raw line-table entry counts and
  are no longer emitted by default. Re-query them only with
  `--raw-line-table-counts` (reported as `[line-table]`), then use
  `pdb_fetch --view structure-diff`, named locals, and assembly to establish any
  real init-list/guard/braces/ternary divergence. Never line-pack source to make
  the aggregate count agree.
- **[fn-order]** — LOW: definition/decl reorder, structure-faithful, byte-neutral mostly.
- **[values] / [const]** — small, genuine (enum value-set / const renames; overlaps enum_queue.md).

## %-recovery
Functions that are STRUCTURE MATCH but low fuzzy % (e.g. dipped from a faithful layout fix) — retry the codegen now the structure prerequisite is correct (insert_char_action 24→100 was this). Animation n_ary cluster especially.

## Snapshot (tip 18fec6512 / #544, [visibility] dropped)

```
[setup] Wine/ninja setup already complete.
================ HEADERS ================

struct IOleItemContainer
  [fn-order]
    only-base  HRESULT GetObjectA(wchar_t*, unsigned long, IBindCtx*, _GUID const&, void**)
    only-tgt   HRESULT GetObject(wchar_t*, unsigned long, IBindCtx*, _GUID const&, void**)

struct IRunningObjectTable
  [fn-order]
    only-base  HRESULT GetObjectA(IMoniker*, IUnknown**)
    only-tgt   HRESULT GetObject(IMoniker*, IUnknown**)

struct _TOKEN_GROUPS
  [member]
    changed  Groups: base(_SID_AND_ATTRIBUTES[1] @0x4)  target(_SID_AND_ATTRIBUTES[] @0x4)

struct _tiddata
  [member]
    changed  _wtoken: base(wchar_t* @0x1C)  target(u16* @0x1C)
    changed  _werrmsg: base(wchar_t* @0x28)  target(u16* @0x28)
    changed  _wnamebuf0: base(wchar_t* @0x30)  target(u16* @0x30)
    changed  _wnamebuf1: base(wchar_t* @0x38)  target(u16* @0x38)
    changed  _wasctimebuf: base(wchar_t* @0x40)  target(u16* @0x40)

class btCharacterControllerInterface
  [fn-order]
    only-base  void preStep(btCollisionWorld*)
    only-tgt   void preStep(btCollisionWorld*, float)

struct btCollisionWorld::objectQuerySingle::__l45::input_params
  [member]
    only-base  m_castShape, m_convexFromTrans, m_convexToTrans, m_collisionObject, m_collisionShape, m_colObjWorldTransform, m_resultCallback, m_allowedPenetration
    only-tgt   castShape, convexFromTrans, convexToTrans, collisionObject, collisionShape, colObjWorldTransform, resultCallback, allowedPenetration

struct btCollisionWorld::objectQuerySingle::__l47::VolumeTester
  [member]
    only-base  m_params
    only-tgt   m_input_params

class btDiscreteDynamicsWorld

class btIDebugDraw
  [fn-order]
    only-tgt   bool debugDrawObject(btTransform const&, btCollisionShape const*, btVector3 const&)

class btKinematicCharacterController
  [size]   base=0xD0  target=0xE0
  [member]
    changed  m_velocityTimeInterval: base(float @0xC4)  target(float @0xC8)
    changed  m_upAxis: base(s32 @0xC8)  target(s32 @0xCC)
    only-tgt   m_WalkDirectionApplied, m_has_updates

struct btSoftBody::Config
  [member]
    changed  m_vsequence: base(btAlignedObjectArray< btSoftBody::eVSolver::_ > @0x68)  target(btAlignedObjectArray< enum btSoftBody::eVSolver::_ > @0x68)
    changed  m_psequence: base(btAlignedObjectArray< btSoftBody::ePSolver::_ > @0x7C)  target(btAlignedObjectArray< enum btSoftBody::ePSolver::_ > @0x7C)
    changed  m_dsequence: base(btAlignedObjectArray< btSoftBody::ePSolver::_ > @0x90)  target(btAlignedObjectArray< enum btSoftBody::ePSolver::_ > @0x90)

struct btSolverConstraint
  [member]
    changed  m_appliedPushImpulse: base(float @0x50)  target(btSimdScalar @0x50)
    changed  m_appliedImpulse: base(float @0x54)  target(btSimdScalar @0x60)
    changed  m_friction: base(float @0x58)  target(float @0x70)
    changed  m_jacDiagABInv: base(float @0x5C)  target(float @0x74)
    changed  m_numConsecutiveRowsPerKernel: base(s32 @0x60)  target(s32 @0x78)
    changed  m_unusedPadding0: base(float @0x60)  target(float @0x78)
    changed  m_frictionIndex: base(s32 @0x64)  target(s32 @0x7C)
    changed  m_unusedPadding1: base(float @0x64)  target(float @0x7C)
    changed  m_solverBodyA: base(btRigidBody* @0x68)  target(btRigidBody* @0x80)
    changed  m_companionIdA: base(s32 @0x68)  target(s32 @0x80)
    changed  m_solverBodyB: base(btRigidBody* @0x6C)  target(btRigidBody* @0x84)
    changed  m_companionIdB: base(s32 @0x6C)  target(s32 @0x84)
    changed  m_originalContactPoint: base(void* @0x70)  target(void* @0x88)
    changed  m_unusedPadding4: base(float @0x70)  target(float @0x88)
    changed  m_rhs: base(float @0x74)  target(float @0x8C)
    changed  m_cfm: base(float @0x78)  target(float @0x90)
    changed  m_lowerLimit: base(float @0x7C)  target(float @0x94)
    changed  m_upperLimit: base(float @0x80)  target(float @0x98)
    changed  m_rhsPenetration: base(float @0x84)  target(float @0x9C)

class btVector3
  [member]
    only-tgt   mVec128
  [fn-order]
    only-tgt   __m128 get128() const, void set128(__m128)

struct codebook
  [size]   base=0x2C  target=0x38
  [member]
    only-tgt   quantvals, minval, delta

class delayed_packets_predicate
  [member]
    only-base  m_delayed_packets_to_appear
    only-tgt   <base> boost::noncopyable, m_packets
  [fn-order]
    only-tgt   delayed_packets_predicate(delayed_packets_predicate const&), void ~delayed_packets_predicate(), delayed_packets_predicate& operator=(delayed_packets_predicate const&)

struct highlevel_encode_setup
  [member]
    changed  setup: base(pcvoid @0x0)  target(pcvoid @0x4)
    changed  set_in_stone: base(s32 @0x4)  target(s32 @0x0)
    changed  impulse_noisetune: base(double @0x20)  target(double @0x10)
    changed  managed: base(s32 @0x28)  target(s32 @0x1C)
    changed  bitrate_min: base(long @0x2C)  target(long @0x20)
    changed  bitrate_av: base(long @0x30)  target(long @0x24)
    changed  bitrate_av_damp: base(double @0x38)  target(double @0x28)
    changed  bitrate_max: base(long @0x40)  target(long @0x30)
    changed  bitrate_reservoir: base(long @0x44)  target(long @0x34)
    changed  bitrate_reservoir_bias: base(double @0x48)  target(double @0x38)
    changed  impulse_block_p: base(s32 @0x50)  target(s32 @0x40)
    changed  noise_normalize_p: base(s32 @0x54)  target(s32 @0x44)
    changed  stereo_point_setting: base(double @0x58)  target(double @0x50)
    changed  lowpass_kHz: base(double @0x60)  target(double @0x58)
    only-base  long_setting, short_setting
    only-tgt   req, coupling_p, lowpass_altered
    reordered  setup

struct lsfit_acc
  [size]   base=0x20  target=0x38
  [member]
    changed  x0: base(long @0x0)  target(s32 @0x0)
    changed  x1: base(long @0x4)  target(s32 @0x4)
    changed  xa: base(long @0x8)  target(s32 @0x8)
    changed  ya: base(long @0xC)  target(s32 @0xC)
    changed  x2a: base(long @0x10)  target(s32 @0x10)
    changed  y2a: base(long @0x14)  target(s32 @0x14)
    changed  xya: base(long @0x18)  target(s32 @0x18)
    changed  an: base(long @0x1C)  target(s32 @0x1C)
    only-tgt   xb, yb, x2b, y2b, xyb, bn

struct messaging::send_message_params
  [size]   base=0x150  target=0x54
  [member]
    changed  message_body: base(char[256] @0x50)  target(char[256]* @0x50)

class mutex_mt_raii
  [size]   base=0x4  target=0x8
  [member]
    only-tgt   m_is_tasks_aware

class packets_in_list_predicate
  [size]   base=0x2  target=0x1
  [member]
    changed  m_sequence_id: base(vostok::network_core::sequence_number< u16 > @0x0)  target(vostok::network_core::sequence_number< u8 > @0x0)
  [fn-order]
    only-base  packets_in_list_predicate(vostok::network_core::sequence_number<unsigned short> const&)
    only-tgt   packets_in_list_predicate(vostok::network_core::sequence_number<unsigned char> const&), packets_in_list_predicate(packets_in_list_predicate const&), packets_in_list_predicate& operator=(packets_in_list_predicate const&)

struct regions_filler
  [size]   base=0x4  target=0x8
  [member]
    only-tgt   m_high_memory_regions
  [fn-order]
    only-base  regions_filler(vostok::buffer_vector<vostok::memory::platform::region>&)
    only-tgt   regions_filler(vostok::buffer_vector<vostok::memory::platform::region>&, vostok::buffer_vector<vostok::memory::platform::region>&)

class remove_all_predicate

class sequence_id_predicate
  [member]
    changed  m_sequence_id: base(const vostok::network_core::sequence_number< u16 > @0x8)  target(const vostok::network_core::sequence_number< u8 > @0x8)
  [fn-order]
    only-base  sequence_id_predicate(vostok::memory::single_size_buffer_allocator<300,vostok::threading::single_threading_policy>&, const vostok::network_core::sequence_number<unsigned short>, char const* const)
    only-tgt   sequence_id_predicate(vostok::memory::single_size_buffer_allocator<300,vostok::threading::single_threading_policy>&, const vostok::network_core::sequence_number<unsigned char>, char const* const)

struct singletons_on_initialize
  [size]   base=0x1C70  target=0x42EC
  [member]
    changed  material_manager: base(vostok::render::material_manager @0x1C04)  target(vostok::render::material_manager @0x4284)
    changed  particle_shader_constants: base(vostok::render::particle_shader_constants @0x1C34)  target(vostok::render::particle_shader_constants @0x42B0)
    changed  decal_shader_constants_and_geometry: base(vostok::render::decal_shader_constants_and_geometry @0x1C54)  target(vostok::render::decal_shader_constants_and_geometry @0x42D0)
    only-base  environment

struct singletons_on_preinitialize
  [size]   base=0x11970  target=0x12010
  [member]
    changed  device: base(vostok::render::device @0x200)  target(vostok::render::device @0x2D0)
    changed  backend: base(vostok::render::backend @0x338)  target(vostok::render::backend @0x424)
    changed  scene_manager: base(vostok::render::scene_manager @0x6A8)  target(vostok::render::scene_manager @0xD20)
    changed  shader_macros: base(vostok::render::shader_macros @0x6CC)  target(vostok::render::shader_macros @0xD44)
    changed  effect_manager: base(vostok::render::effect_manager @0x118DC)  target(vostok::render::effect_manager @0x11F54)
    changed  effect_constant_storage: base(vostok::render::effect_constant_storage @0x11960)  target(vostok::render::effect_constant_storage @0x12000)
  [fn-order]
    only-base  singletons_on_preinitialize()
    only-tgt   singletons_on_preinitialize(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> const&, bool)

struct static_codebook
  [size]   base=0x34  target=0x28
  [member]
    changed  allocedp: base(s32 @0x30)  target(s32 @0x24)
    only-base  nearest_tree, thresh_tree, pigeon_tree

struct survarium::affect_event_predicate
  [member]
    changed  affect_type: base(hit_affects_type_enum @0x4)  target(const hit_affects_type_enum @0x4)
    changed  event_type: base(affect_event_type_enum @0x8)  target(const affect_event_type_enum @0x8)
  [fn-order]
    only-base  affect_event_predicate(char const*, survarium::hit_affects_type_enum, survarium::affect_event_type_enum), void operator()(survarium::affect_subscriber*) const
    only-tgt   affect_event_predicate(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum), void operator()(survarium::affect_subscriber* const) const

struct survarium::affect_subscriber
  [member]
    changed  subscription_callback: base(boost::function< void( pcstr, hit_affects_type_enum, affect_event_type_enum ) > @0x0)  target(boost::function< void( pcstr, enum hit_affects_type_enum, enum affect_event_type_enum ) > @0x0)
  [fn-order]
    only-base  affect_subscriber(boost::function<void __cdecl(char const *,survarium::hit_affects_type_enum,survarium::affect_event_type_enum)> const&)
    only-tgt   affect_subscriber(boost::function<void __cdecl(char const *,enum survarium::hit_affects_type_enum,enum survarium::affect_event_type_enum)> const&)

class survarium::affects_threshold
  [member]
    changed  m_bodypart: base(body_part_parameters* @0xC)  target(body_part_parameters* const @0xC)
  [fn-order]
    only-base  affects_threshold(float, unsigned int, survarium::body_part_parameters* const), survarium::body_part_parameters* bodypart() const
    only-tgt   affects_threshold(const float, const unsigned int, survarium::body_part_parameters* const), survarium::body_part_parameters* const bodypart() const

class survarium::animated_model_instance_cook
  [fn-order]
    moved      void ~animated_model_instance_cook()

class survarium::animation_analysis_result
  [fn-order]
    only-base  animation_analysis_result(unsigned int)
    only-tgt   animation_analysis_result(const unsigned int)

class survarium::animation_analyzer
  [member]
    changed  m_legs_count: base(u32 @0x4)  target(const u32 @0x4)
  [fn-order]
    only-base  animation_analyzer(survarium::leg_info const*, unsigned int, vostok::animation::skeleton const&), void analyse_samples(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, unsigned int, survarium::leg_key_times*), float get_max_range(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, unsigned int, unsigned int) const, unsigned int get_stance_index(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, unsigned int, unsigned int, float) const, vostok::math::float3 get_cycle_center(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, unsigned int, unsigned int) const, float find_contact_time(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, unsigned int, unsigned int, bool, unsigned int, int, float, float), float find_swing_change_time(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, unsigned int, unsigned int, unsigned int, int, float, vostok::math::float3 const&)
    only-tgt   animation_analyzer(survarium::leg_info const*, const unsigned int, vostok::animation::skeleton const&), void analyse_samples(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, const unsigned int, survarium::leg_key_times*), float get_max_range(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, const unsigned int, const unsigned int) const, unsigned int get_stance_index(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, const unsigned int, const unsigned int, const float) const, vostok::math::float3 get_cycle_center(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, const unsigned int, const unsigned int) const, float find_contact_time(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, const unsigned int, const unsigned int, bool, unsigned int, int, float, float), float find_swing_change_time(vostok::buffer_vector<survarium::animation_analyzer::leg_cycle_sample> const&, const unsigned int, const unsigned int, unsigned int, int, float, vostok::math::float3 const&)

class survarium::animation_space_graph

class survarium::animation_space_graph_cook
  [fn-order]
    moved      void ~animation_space_graph_cook()

struct survarium::animation_space_graph_wrapper

class survarium::animation_space_heuristics
  [fn-order]
    moved      static bool metric_euclidian()

class survarium::animations_search_service

class survarium::animations_selector
  [fn-order]
    moved      void ~animations_selector()

struct survarium::anomaly_state
  [fn-order]
    only-base  void execute(unsigned int, unsigned int)
    only-tgt   void execute(const unsigned int, const unsigned int)

class survarium::artefact_container_core

class survarium::artefact_lifebone_core
  [fn-order]
    only-base  float reduce_damage(char const*, char const*, float, float), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    only-tgt   float reduce_damage(char const*, char const*, const float, const float), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)

struct survarium::base_animation_controller

class survarium::base_game_object

class survarium::base_game_scene

class survarium::base_network_client

struct survarium::base_player
  [member]
    changed  m_recoil_params: base(character_recoil_params @0x94)  target(const character_recoil_params @0x94)
    changed  m_dispersion_params: base(character_dispersion_params @0xA4)  target(const character_dispersion_params @0xA4)
    changed  m_breath_holding_params: base(breath_holding_params @0xDC)  target(const breath_holding_params @0xDC)
  [fn-order]
    only-base  void subscribe_animation_player(vostok::animation::reserved_channel_ids_enum, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const*), void subscribe_animation_player(char const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, unsigned char, void const*), unsigned int local_time(unsigned int) const, bool get_animation_playback_state(void const*, unsigned int, vostok::animation::animation_playback_state&) const
    only-tgt   void subscribe_animation_player(char const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const), void subscribe_animation_player(vostok::animation::reserved_channel_ids_enum, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const* const), unsigned int local_time(const unsigned int) const, bool get_animation_playback_state(void const* const, const unsigned int, vostok::animation::animation_playback_state&) const
    moved      survarium::inventory_holder const* cast_to_inventory_holder() const, void unsubscribe_animation_player(vostok::animation::reserved_channel_ids_enum, void const*), vostok::animation::animation_player const& animation_player() const

class survarium::base_project

class survarium::body_part_parameters
  [member]
    changed  m_affects: base(fixed_vector< std::pair< hit_affects_type_enum, u32 >, 8 > @0x24)  target(fixed_vector< std::pair< enum hit_affects_type_enum, u32 >, 8 > @0x24)
    changed  m_assignable: base(bool @0xA4)  target(const bool @0xA4)
    changed  m_damage_group: base(u8 @0xA5)  target(const u8 @0xA5)
  [fn-order]
    only-base  body_part_parameters(char const*, float, float, float, bool, survarium::damage_model&, unsigned char), void hit_by_type(char const*, unsigned int, float, float, bool, survarium::damage_protector*), void increase_health(float), void decrease_health(float), void regenerate(unsigned int, unsigned int), void dump_state(boost::function<void __cdecl(unsigned int,float,float,char const *)>, unsigned int) const, void dump_state(survarium::damage_info_type&, unsigned int) const, void dump_state(vostok::ai::npc_statistics&, unsigned int) const, void apply_affect_by_force(survarium::hit_affects_type_enum, survarium::affect_event_type_enum, unsigned int), bool has_affect_protector(survarium::hit_affects_type_enum), void cancel_affect_by_force(survarium::hit_affects_type_enum), bool is_affect_applied(survarium::hit_affects_type_enum), void check_affects(unsigned int), void update_affects(unsigned int), void apply_affects(survarium::affects_threshold const*, unsigned int)
    only-tgt   body_part_parameters(char const*, const float, const float, const float, const bool, survarium::damage_model&, const unsigned char), void hit_by_type(char const*, const unsigned int, const float, const float, const bool, survarium::damage_protector*), void increase_health(const float), void decrease_health(const float), void regenerate(const unsigned int, const unsigned int), void dump_state(vostok::ai::npc_statistics&, const unsigned int) const, void dump_state(survarium::damage_info_type&, const unsigned int) const, void dump_state(boost::function<void __cdecl(unsigned int,float,float,char const *)>, const unsigned int) const, void apply_affect_by_force(const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum, const unsigned int), bool has_affect_protector(const survarium::hit_affects_type_enum), void cancel_affect_by_force(const survarium::hit_affects_type_enum), bool is_affect_applied(const survarium::hit_affects_type_enum), void check_affects(const unsigned int), void update_affects(const unsigned int), void apply_affects(survarium::affects_threshold const*, const unsigned int)

class survarium::booby_trap

class survarium::booby_trap_cook
  [fn-order]
    moved      void ~booby_trap_cook()

class survarium::booby_trap_core
  [fn-order]
    only-base  void tick(unsigned int, unsigned int)
    only-tgt   void tick(const unsigned int, const unsigned int)
    moved      survarium::booby_trap_set_core* owner()

class survarium::booby_trap_core_cook

class survarium::booby_trap_set_cook
  [fn-order]
    moved      void ~booby_trap_set_cook()

class survarium::booby_trap_set_core
  [fn-order]
    only-base  vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    only-tgt   vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    moved      vostok::buffer_vector<vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base> > const& traps() const

class survarium::booby_trap_set_core_cook

struct survarium::booster_damage_protector
  [fn-order]
    only-base  float reduce_damage(char const*, char const*, float, float)
    only-tgt   float reduce_damage(char const*, char const*, const float, const float)

class survarium::breath_state
  [fn-order]
    only-base  void tick(float)
    only-tgt   void tick(const float)

class survarium::breath_state_holding

class survarium::breath_state_normal

class survarium::breath_state_shortbreathing

class survarium::breath_vibration_calculator
  [fn-order]
    only-base  void tick(unsigned int, float), void set_max_breath_delay_time(float), void set_user(survarium::base_player const*), void set_character_multiplier(float)
    only-tgt   void tick(const unsigned int, const float), void set_max_breath_delay_time(const float), void set_user(survarium::base_player const* const), void set_character_multiplier(const float)

class survarium::camera_director

class survarium::character_dispersion_calculator
  [fn-order]
    only-base  void tick(survarium::weapon_user_state_enum, bool, bool, unsigned char, bool, unsigned int), void set_aiming_speed(float), float get_target_koef(survarium::weapon_user_state_enum, bool, bool) const, float get_broken_hands_penalty(unsigned char, bool) const
    only-tgt   void tick(const survarium::weapon_user_state_enum, const bool, const bool, const unsigned char, const bool, const unsigned int), void set_aiming_speed(const float), float get_target_koef(const survarium::weapon_user_state_enum, const bool, const bool) const, float get_broken_hands_penalty(const unsigned char, const bool) const

class survarium::character_recoil_calculator
  [fn-order]
    only-base  void tick(survarium::weapon_user_state_enum, bool, unsigned int, float)
    only-tgt   void tick(const survarium::weapon_user_state_enum, const bool, const unsigned int, const float)

class survarium::chat_handler

class survarium::circular_buffer<survarium::client_player_history_item>
  [fn-order]
    only-base  circular_buffer<survarium::client_player_history_item>(vostok::memory::base_allocator&, unsigned int), void resize(unsigned int), survarium::client_player_history_item& insert(unsigned int), survarium::client_player_history_item const& operator[](unsigned int) const, survarium::client_player_history_item& operator[](unsigned int)
    only-tgt   circular_buffer<survarium::client_player_history_item>(vostok::memory::base_allocator&, const unsigned int), void resize(const unsigned int), survarium::client_player_history_item& insert(const unsigned int), survarium::client_player_history_item& operator[](const unsigned int), survarium::client_player_history_item const& operator[](const unsigned int) const
    moved      survarium::client_player_history_item const& oldest() const, survarium::client_player_history_item const& newest() const

struct survarium::client_player_history_item

class survarium::collision_geometry
  [fn-order]
    moved      bool contact_test()

class survarium::collision_sensor
  [fn-order]
    only-base  void tick(unsigned int, unsigned int)
    only-tgt   void tick(const unsigned int, const unsigned int)
    moved      bool contact_test(vostok::physics::base_physics_object*)

class survarium::collision_user

struct survarium::compare_body_parts_predicate
  [member]
    only-base  m_body_part
    only-tgt   m_name

class survarium::console_command_bind

class survarium::damage_model
  [fn-order]
    only-base  damage_model(survarium::affects_applying_type_enum), bool hit_body_part(unsigned char, char const*, char const*, float, float, unsigned int, survarium::bullet* const), void apply_med_kit(char const*, float), void tick(unsigned int, unsigned int), void fill_stats(survarium::damage_info_type&, unsigned int) const, void fill_stats(vostok::ai::npc_statistics&, unsigned int) const, void apply_affect(char const*, survarium::hit_affects_type_enum, survarium::affect_event_type_enum), void cancel_affect(char const*, survarium::hit_affects_type_enum), void subscribe_on_affect(survarium::hit_affects_type_enum, survarium::affect_subscriber* const), void unsubscribe_from_affect(survarium::hit_affects_type_enum, survarium::affect_subscriber* const), void notify_on_affect_event(char const*, survarium::hit_affects_type_enum, survarium::affect_event_type_enum), char const* get_body_part_name(unsigned char) const, void on_broken_limb_affect(char const*, survarium::hit_affects_type_enum, survarium::affect_event_type_enum)
    only-tgt   damage_model(const survarium::affects_applying_type_enum), bool hit_body_part(const unsigned char, char const*, char const*, const float, const float, const unsigned int, survarium::bullet* const), void apply_med_kit(char const*, const float), void tick(const unsigned int, const unsigned int), void fill_stats(vostok::ai::npc_statistics&, const unsigned int) const, void fill_stats(survarium::damage_info_type&, const unsigned int) const, void apply_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum), void cancel_affect(char const*, const survarium::hit_affects_type_enum), void subscribe_on_affect(const survarium::hit_affects_type_enum, survarium::affect_subscriber* const), void unsubscribe_from_affect(const survarium::hit_affects_type_enum, survarium::affect_subscriber* const), void notify_on_affect_event(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum), char const* get_body_part_name(const unsigned char) const, void on_broken_limb_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)

class survarium::damage_model_cook
  [fn-order]
    moved      void on_hit_params_received(vostok::resources::queries_result&)

class survarium::damage_model_stats

struct survarium::damage_protector
  [member]
    changed  protect_affect_functor: base(boost::function< bool( pcstr, hit_affects_type_enum ) > @0x28)  target(boost::function< bool( pcstr, enum hit_affects_type_enum ) > @0x28)

class survarium::damage_zone_cook

class survarium::damage_zone_core
  [fn-order]
    only-base  void tick(unsigned int, unsigned int), void hit_on_enter(unsigned int, unsigned int), void hit_on_inside(unsigned int, unsigned int), void hit_on_motion_inside(unsigned int, unsigned int)
    only-tgt   void tick(const unsigned int, const unsigned int), void hit_on_enter(const unsigned int, const unsigned int), void hit_on_inside(const unsigned int, const unsigned int), void hit_on_motion_inside(const unsigned int, const unsigned int)

class survarium::dispersion_calculator
  [fn-order]
    only-base  void tick(survarium::weapon_user_state_enum, bool, bool, unsigned char, bool, unsigned int), void set_shooting_skill_coeff(float), void set_aiming_speed_coeff(float)
    only-tgt   void tick(const survarium::weapon_user_state_enum, const bool, const bool, const unsigned char, const bool, const unsigned int), void set_shooting_skill_coeff(const float), void set_aiming_speed_coeff(const float)

class survarium::double_barreled_weapon_core_aimed_fire_state
  [fn-order]
    only-base  double_barreled_weapon_core_aimed_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   double_barreled_weapon_core_aimed_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::double_barreled_weapon_core_aimed_idle_state
  [fn-order]
    only-base  double_barreled_weapon_core_aimed_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   double_barreled_weapon_core_aimed_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::double_barreled_weapon_core_fire_state
  [fn-order]
    only-base  double_barreled_weapon_core_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   double_barreled_weapon_core_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::double_barreled_weapon_core_hide_state
  [member]
    changed  m_time_scale: base(float @0x188)  target(const float @0x188)
  [fn-order]
    only-base  double_barreled_weapon_core_hide_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   double_barreled_weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::double_barreled_weapon_core_idle_state
  [fn-order]
    only-base  double_barreled_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   double_barreled_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::double_barreled_weapon_core_reload_state
  [fn-order]
    only-base  double_barreled_weapon_core_reload_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   double_barreled_weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::double_barreled_weapon_core_show_state
  [member]
    changed  m_time_scale: base(float @0x188)  target(const float @0x188)
  [fn-order]
    only-base  double_barreled_weapon_core_show_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   double_barreled_weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

struct survarium::dump_npc_body_part_state_predicate
  [fn-order]
    only-base  dump_npc_body_part_state_predicate(vostok::ai::npc_statistics&, unsigned int), void operator()(survarium::body_part_parameters*) const
    only-tgt   dump_npc_body_part_state_predicate(vostok::ai::npc_statistics&, const unsigned int), void operator()(survarium::body_part_parameters* const) const

struct survarium::dz_bone_data_contact_test_predicate
  [fn-order]
    only-base  float add_single_result(void*, vostok::collision::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::collision::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&)
    only-tgt   float add_single_result(void*, vostok::physics::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::physics::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&)

class survarium::empty_hands

class survarium::empty_hands_cook
  [fn-order]
    moved      void ~empty_hands_cook()

struct survarium::find_body_part_by_name_predicate
  [fn-order]
    only-base  bool operator()(survarium::body_part_parameters*) const
    only-tgt   bool operator()(survarium::body_part_parameters* const) const

struct survarium::find_by_damage_type_predicate

struct survarium::find_hit_parameters_by_type_predicate
  [fn-order]
    only-base  bool operator()(survarium::hit_type_parameters*) const
    only-tgt   bool operator()(survarium::hit_type_parameters* const) const

class survarium::fingers_to_weapon_corrector

class survarium::flash_external_handler
  [size]   base=0xC  target=0x8
  [member]
    only-base  <base> Scaleform::GFx::ExternalInterface
    only-tgt   impl
  [fn-order]
    only-base  void Callback(Scaleform::GFx::Movie*, char const*, Scaleform::GFx::Value const*, unsigned int)
    only-tgt   void callback(survarium::flash_movie*, char const*, survarium::flash_value const*, unsigned int)

struct survarium::flash_external_handler_impl

class survarium::flash_factory
  [member]
    only-base  <base> boost::noncopyable, m_game
    only-tgt   m_render_thread_queue
  [fn-order]
    only-base  flash_factory(survarium::game&), flash_factory(survarium::flash_factory const&), survarium::flash_movie* create_movie(char*), void destroy_movie(char*), survarium::flash_factory& operator=(survarium::flash_factory const&)
    only-tgt   flash_factory(survarium::scaleform_game_engine&), survarium::flash_movie* build_movie(void*, unsigned int, char const*), void destroy_movie(survarium::flash_movie*), survarium::flash_text_manager* create_text_manager(), void destroy_text_manager(survarium::flash_text_manager*)

struct survarium::flash_function_handler_impl

struct survarium::flash_movie
  [fn-order]
    only-base  void ~flash_movie()

class survarium::free_fly_camera
  [size]   base=0x90  target=0x88
  [member]
    changed  <base> input::handler: base(input::handler @0x5C)  target(input::handler @0x54)
    changed  m_camera_director: base(camera_director_ptr& @0x60)  target(camera_director& @0x58)
    changed  m_prev_time_ms: base(u32 @0x64)  target(u32 @0x5C)
    changed  m_prev_delta_sec: base(float @0x68)  target(float @0x60)
    changed  m_keyb_events: base(vector< int > @0x6C)  target(vector< int > @0x64)
    changed  m_mouse_events: base(vector< int > @0x78)  target(vector< int > @0x70)
    changed  m_mouse_move: base(float3 @0x84)  target(float3 @0x7C)
  [fn-order]
    only-base  free_fly_camera(survarium::game_scene&, vostok::intrusive_ptr<survarium::camera_director,vostok::resources::unmanaged_intrusive_base,vostok::threading::single_threading_policy>&)
    only-tgt   free_fly_camera(survarium::base_game_scene&, survarium::camera_director&)

class survarium::game
  [fn-order]
    only-base  survarium::base_network_client& network_client() const

class survarium::game_camera

class survarium::game_material

class survarium::game_material_manager
  [fn-order]
    moved      bool material_exist(char const*, unsigned short*) const

class survarium::game_material_manager_cook

struct survarium::game_material_manager_cook::query_ext_data
  [member]
    changed  type: base(game_material_manager_cook::query_ext_data::res_type @0x4)  target(game_material_manager_cook::res_type @0x4)

class survarium::game_object_
  [member]
    changed  m_game_scene: base(game_scene& @0x10C)  target(base_game_scene& @0x108)
    only-base  m_loaded_cnt
  [fn-order]
    only-base  game_object_(survarium::game_scene&), void load(vostok::configs::binary_config_value const&), void on_scene_start(), void load_contents(), void unload_contents(), survarium::game_scene& get_game_scene()
    only-tgt   game_object_(survarium::base_game_scene&), void load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&), void insert(), void remove(), survarium::base_game_scene& get_game_scene()

class survarium::game_object_static
  [fn-order]
    only-base  game_object_static(survarium::game_scene&), void load(vostok::configs::binary_config_value const&)
    only-tgt   game_object_static(survarium::base_game_scene&), void load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&)

class survarium::game_options
  [member]
    changed  m_conflicted_action_ids: base(vector< game_action_id > @0x44)  target(vector< enum game_action_id > @0x44)

class survarium::game_scene
  [size]   base=0x9C  target=0xC
  [member]
    only-base  <base> physics::engine, <base> boost::noncopyable, m_is_active, m_scene, m_scene_view, m_game, m_sound_scene, m_inverted_view_matrix, m_projection_matrix, m_physics_world
    only-tgt   m_render_scene, m_render_scene_view
  [fn-order]
    only-base  game_scene(survarium::game&), void init_physics(), void tick(), bool is_active() const, void apply_camera(vostok::intrusive_ptr<survarium::camera_director,vostok::resources::unmanaged_intrusive_base,vostok::threading::single_threading_policy>), survarium::game& get_game() const, vostok::render::game::renderer& renderer(), vostok::physics::world* get_physics_world(), vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const& get_render_scene() const, vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const& get_render_scene_view() const, vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>& get_sound_scene()
    only-tgt   void tick(const unsigned int, const unsigned int, const bool), void on_after_tick(), vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const& render_scene(), vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const& render_scene_view(), game_scene()
    moved      game_scene(survarium::game_scene const&), void ~game_scene()

class survarium::game_world
  [fn-order]
    moved      static void kill_npc(vostok::resources::resource_ptr<survarium::human_npc,vostok::resources::unmanaged_intrusive_base>&)

class survarium::game_world_object
  [fn-order]
    moved      survarium::inventory_item* owner()

class survarium::game_world_ui
  [member]
    changed  m_slots_to_update: base(vector< profile_slot_enum > @0x34)  target(vector< enum profile_slot_enum > @0x34)

class survarium::generate_shaders_world
  [fn-order]
    only-base  void load(char const*, char const*)
    moved      void ~generate_shaders_world()

class survarium::generic_anomaly_core
  [member]
    reordered  <base> base_game_object
  [fn-order]
    only-base  void tick(unsigned int, unsigned int)
    only-tgt   void tick(const unsigned int, const unsigned int)

struct survarium::get_first_npc_in_camera_direction_predicate

class survarium::global_input_handler

class survarium::hand_to_weapon_ik_processor
  [fn-order]
    only-base  void process(unsigned int, vostok::math::float4x4 const*, vostok::math::float4x4*) const, void activate_hand(survarium::hand_to_weapon_ik_processor::hands_enum, bool, unsigned int), float get_hand_coefficient(survarium::hand_to_weapon_ik_processor::hand const&, unsigned int) const, static bool hand_need_correction(survarium::hand_to_weapon_ik_processor::hand const&, unsigned int), static bool hand_need_interpolation(survarium::hand_to_weapon_ik_processor::hand const&, unsigned int), static unsigned int get_hand_new_start_transition_time(survarium::hand_to_weapon_ik_processor::hand const&, unsigned int)
    only-tgt   void process(const unsigned int, vostok::math::float4x4 const*, vostok::math::float4x4*) const, void activate_hand(const survarium::hand_to_weapon_ik_processor::hands_enum, const bool, const unsigned int), static bool hand_need_correction(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int), static bool hand_need_interpolation(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int), static unsigned int get_hand_new_start_transition_time(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int), float get_hand_coefficient(survarium::hand_to_weapon_ik_processor::hand const&, const unsigned int) const

struct survarium::hit_info
  [fn-order]
    only-base  hit_info(unsigned char, unsigned char, char const*, char const*, float, float, survarium::bullet*)
    only-tgt   hit_info(const unsigned char, const unsigned char, char const* const, char const* const, const float, const float, survarium::bullet* const)

struct survarium::hit_initiator
  [member]
    changed  id: base(u8 @0x4)  target(const u8 @0x4)
    changed  is_local: base(bool @0x5)  target(const bool @0x5)
  [fn-order]
    only-base  hit_initiator(unsigned char, bool)
    only-tgt   hit_initiator(const unsigned char, const bool)

struct survarium::hit_receiver
  [fn-order]
    moved      hit_receiver(), hit_receiver(survarium::hit_receiver const&)

struct survarium::hit_receiver_info
  [fn-order]
    only-base  bool operator==(survarium::hit_receiver const*) const
    only-tgt   bool operator==(survarium::hit_receiver const* const) const

class survarium::hit_type_parameters
  [member]
    changed  m_type: base(fixed_string< 16 > @0x4)  target(const fixed_string< 16 > @0x4)
    changed  m_bdb_count: base(u32 @0x2C)  target(const u32 @0x2C)
  [fn-order]
    only-base  hit_type_parameters(char const*, float, float, float, unsigned int), void apply_damage(float, unsigned int)
    only-tgt   hit_type_parameters(char const*, const float, const float, float, const unsigned int), void apply_damage(const float, const unsigned int)

class survarium::human_npc
  [fn-order]
    only-base  bool is_playing_animation() const, bool is_moving() const, void stop_animation_playing(), vostok::ai::collision_object* get_collision_object() const, void set_filter(stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*)
    only-tgt   void set_filter(stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*)

class survarium::human_npc_cook

class survarium::ik_processor
  [fn-order]
    only-base  vostok::animation::skeleton const& get_skeleton() const

class survarium::interactive_object
  [fn-order]
    only-base  vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    only-tgt   vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    moved      survarium::weapon_core const* cast_weapon_core() const

class survarium::inventory
  [fn-order]
    only-base  void set_item(survarium::profile_slot_enum, vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const&), vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base>& item_in_slot(survarium::profile_slot_enum), vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const& item_in_slot(survarium::profile_slot_enum) const, bool action(survarium::profile_slot_enum, bool)
    only-tgt   void set_item(const survarium::profile_slot_enum, vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const&), vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const& item_in_slot(const survarium::profile_slot_enum) const, vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base>& item_in_slot(const survarium::profile_slot_enum), bool action(const survarium::profile_slot_enum, bool)

class survarium::inventory_cook

class survarium::inventory_holder
  [fn-order]
    moved      survarium::inventory const& inventory() const

class survarium::inventory_item
  [member]
    changed  m_action_behaviuor: base(inventory_item::action_behaviour_type @0x108)  target(const inventory_item::action_behaviour_type @0x108)
  [fn-order]
    only-base  void set_amount(unsigned short)
    only-tgt   void set_amount(const unsigned short)
    moved      survarium::weapon_core const* cast_weapon_core() const

struct survarium::inventory_item_instance
  [fn-order]
    only-base  bool operator==(const unsigned int) const

class survarium::items_cook

class survarium::items_dictionary

class survarium::items_dictionary_cook

class survarium::jump_logic
  [member]
    changed  m_animated_object: base(pcvoid @0xC)  target(pcvoid const @0xC)
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_animation(survarium::jump_animation_parts, bool) const, char const* get_animation_caption(survarium::jump_animation_parts) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_move_animation(bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_move_look_animation(bool) const
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_animation(const survarium::jump_animation_parts, const bool) const, char const* get_animation_caption(const survarium::jump_animation_parts) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_move_animation(const bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_move_look_animation(const bool) const

class survarium::jump_logic_base_state
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)

class survarium::jump_logic_state_inactive
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&)

class survarium::jump_logic_state_landing
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&), vostok::animation::mixing::animation_lexeme get_main_lexeme(vostok::mutable_buffer&, bool, vostok::animation::body_part_masks_enum), vostok::animation::mixing::animation_lexeme get_look_lexeme(vostok::mutable_buffer&, bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&), vostok::animation::mixing::animation_lexeme get_main_lexeme(vostok::mutable_buffer&, const bool, const vostok::animation::body_part_masks_enum), vostok::animation::mixing::animation_lexeme get_look_lexeme(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
    moved      vostok::animation::callback_return_type_enum on_interval_end(vostok::animation::animation_callback_params&)

class survarium::jump_logic_state_start
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&), vostok::animation::mixing::animation_lexeme get_main_lexeme(vostok::mutable_buffer&, bool, vostok::animation::body_part_masks_enum), vostok::animation::mixing::animation_lexeme get_look_lexeme(vostok::mutable_buffer&, bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, survarium::weapon_animation_parameters const&), vostok::animation::mixing::animation_lexeme get_main_lexeme(vostok::mutable_buffer&, const bool, const vostok::animation::body_part_masks_enum), vostok::animation::mixing::animation_lexeme get_look_lexeme(vostok::mutable_buffer&, const bool, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, vostok::animation::mixing::animation_lexeme&)
    moved      bool is_ready_for_transition() const

class survarium::key_binder

class survarium::ladder
  [fn-order]
    only-base  survarium::landing_point const* select_landing_point(vostok::math::float3 const&, bool) const
    only-tgt   survarium::landing_point const* select_landing_point(vostok::math::float3 const&, const bool) const

class survarium::landing_point
  [member]
    changed  m_position: base(float3 @0x4)  target(const float3 @0x4)
    changed  m_rotation: base(float3 @0x10)  target(const float3 @0x10)

struct survarium::left_objects_predicate

class survarium::legs_ik_drawer
  [fn-order]
    only-base  void draw_cross(vostok::math::float3 const&, float, vostok::math::color const&, bool), void draw_origin(vostok::math::float4x4 const&, float, bool)
    only-tgt   void draw_cross(vostok::math::float3 const&, const float, vostok::math::color const&, bool), void draw_origin(vostok::math::float4x4 const&, const float, bool)

class survarium::legs_ik_processor
  [fn-order]
    only-base  void tick(unsigned int)
    only-tgt   void tick(const unsigned int)

struct survarium::legs_ik_processor::leg_params
  [fn-order]
    only-base  void set_heel_on_ground(bool), void set_toe_on_ground(bool)
    only-tgt   void set_heel_on_ground(const bool), void set_toe_on_ground(const bool)

class survarium::lobby_camera

class survarium::lobby_client
  [fn-order]
    only-base  char const* account_name() const

class survarium::lobby_menu

class survarium::lobby_menu_external_handler

class survarium::login_menu_external_handler

class survarium::main_menu
  [size]   base=0xD8  target=0xC0
  [member]
    changed  <base> input::handler: base(input::handler @0x9C)  target(input::handler @0xBC)
    only-base  <base> game_scene, m_main_menu_ui, m_timer, m_main_menu_ui_last_time, m_mouse_pos, m_window_size, m_game_world
    only-tgt   <base> base_game_scene
  [fn-order]
    only-base  main_menu(survarium::game&, survarium::game_world&), void tick(), void create_main_menu_ui()
    only-tgt   main_menu(survarium::game&), void tick(const unsigned int, const unsigned int, const bool), void clear_resources(), survarium::bullet_manager& get_bullet_manager() const

class survarium::main_menu_external_handler
  [size]   base=0x14  target=0xC
  [member]
    changed  <base> boost::noncopyable: base(boost::noncopyable @0xC)  target(boost::noncopyable @0x8)
    changed  m_game: base(game& @0xC)  target(game& @0x8)
    only-base  <base> Scaleform::GFx::ExternalInterface, m_game_world
    only-tgt   <base> flash_external_handler
  [fn-order]
    only-base  main_menu_external_handler(survarium::game&, survarium::game_world&), void Callback(Scaleform::GFx::Movie*, char const*, Scaleform::GFx::Value const*, unsigned int)
    only-tgt   main_menu_external_handler(survarium::game&), void callback(survarium::flash_movie*, char const*, survarium::flash_value const*, unsigned int)

class survarium::map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >(survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::base_point_stats>,stlp_std::less<unsigned int> > const&), survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::base_point_stats>,stlp_std::less<unsigned int> >& operator=(survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::base_point_stats>,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >(survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::base_point_stats>,stlp_std::less<unsigned int> > const&), survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::base_point_stats>,stlp_std::less<unsigned int> >& operator=(survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::base_point_stats>,stlp_std::less<unsigned int> > const&)

class survarium::map<unsigned int,survarium::dictionary_item,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,survarium::dictionary_item,stlp_std::less<unsigned int> >(survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::dictionary_item>,stlp_std::less<unsigned int> > const&), survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::dictionary_item>,stlp_std::less<unsigned int> >& operator=(survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::dictionary_item>,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,survarium::dictionary_item,stlp_std::less<unsigned int> >(survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::dictionary_item>,stlp_std::less<unsigned int> > const&), survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::dictionary_item>,stlp_std::less<unsigned int> >& operator=(survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::dictionary_item>,stlp_std::less<unsigned int> > const&)

class survarium::map<unsigned int,survarium::respawn_point_core *,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,survarium::respawn_point_core *,stlp_std::less<unsigned int> >(survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::respawn_point_core *>,stlp_std::less<unsigned int> > const&), survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::respawn_point_core *>,stlp_std::less<unsigned int> >& operator=(survarium::map<unsigned int,stlp_std::pair<unsigned int const,survarium::respawn_point_core *>,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,survarium::respawn_point_core *,stlp_std::less<unsigned int> >(survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::respawn_point_core *>,stlp_std::less<unsigned int> > const&), survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::respawn_point_core *>,stlp_std::less<unsigned int> >& operator=(survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::respawn_point_core *>,stlp_std::less<unsigned int> > const&)

class survarium::map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >
  [fn-order]
    only-base  map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >(survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::game_material const *>,stlp_std::less<unsigned short> > const&), survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::game_material const *>,stlp_std::less<unsigned short> >& operator=(survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::game_material const *>,stlp_std::less<unsigned short> > const&)
    only-tgt   map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >(survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::game_material const *>,stlp_std::less<unsigned short> > const&), survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::game_material const *>,stlp_std::less<unsigned short> >& operator=(survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::game_material const *>,stlp_std::less<unsigned short> > const&)

class survarium::map<unsigned short,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> >,stlp_std::less<unsigned short> >
  [fn-order]
    only-base  map<unsigned short,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> >,stlp_std::less<unsigned short> >(survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > >,stlp_std::less<unsigned short> > const&), survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > >,stlp_std::less<unsigned short> >& operator=(survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > >,stlp_std::less<unsigned short> > const&)
    only-tgt   map<unsigned short,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> >,stlp_std::less<unsigned short> >(survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > >,stlp_std::less<unsigned short> > const&), survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > >,stlp_std::less<unsigned short> >& operator=(survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > >,stlp_std::less<unsigned short> > const&)

class survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> >
  [fn-order]
    only-base  map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> >(survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::material_pair const *>,stlp_std::less<unsigned short> > const&), survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::material_pair const *>,stlp_std::less<unsigned short> >& operator=(survarium::map<unsigned short,stlp_std::pair<unsigned short const,survarium::material_pair const *>,stlp_std::less<unsigned short> > const&)
    only-tgt   map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> >(survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::material_pair const *>,stlp_std::less<unsigned short> > const&), survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::material_pair const *>,stlp_std::less<unsigned short> >& operator=(survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::material_pair const *>,stlp_std::less<unsigned short> > const&)

class survarium::map<vostok::fixed_string<16>,survarium::body_part_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >
  [fn-order]
    only-base  map<vostok::fixed_string<16>,survarium::body_part_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const,survarium::body_part_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&), survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const,survarium::body_part_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > >& operator=(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const,survarium::body_part_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&)
    only-tgt   map<vostok::fixed_string<16>,survarium::body_part_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::body_part_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&), survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::body_part_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > >& operator=(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::body_part_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&)

class survarium::map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >
  [fn-order]
    only-base  map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&), survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > >& operator=(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&)
    only-tgt   map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&), survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > >& operator=(survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > > const&)

class survarium::map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >
  [fn-order]
    only-base  map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >(survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > > const&), survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > >& operator=(survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > > const&)
    only-tgt   map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >(survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const ,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > > const&), survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const ,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > >& operator=(survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const ,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > > const&)

class survarium::match_client
  [member]
    changed  m_packets_orderer: base(network_packets_orderer< match_client_message_types_enum, match_server_message_types_enum > @0xF8)  target(network_packets_orderer< enum match_client_message_types_enum, enum match_server_message_types_enum > @0xF8)
  [fn-order]
    only-base  void connect(char const*, unsigned short, unsigned int, unsigned int, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::lobby_server_message_types_enum)> const&), void set_on_disconnect(boost::function<void __cdecl(vostok::network_core::disconnect_event_types_enum)> const&)
    only-tgt   void connect(char const*, unsigned short, unsigned int, unsigned int, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::lobby_server_message_types_enum)> const&), void set_on_disconnect(boost::function<void __cdecl(enum vostok::network_core::disconnect_event_types_enum)> const&)

class survarium::material_pair

class survarium::max_angular_velocity_command

class survarium::medkit
  [fn-order]
    only-base  void active_tick(unsigned int), float reduce_damage(char const*, char const*, float, float), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    only-tgt   void active_tick(const unsigned int), float reduce_damage(char const*, char const*, const float, const float), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)

class survarium::messaging_client

class survarium::network_client
  [fn-order]
    only-base  void process_sync_response(vostok::network_core::packet_reader&)
    only-tgt   void process_sync_response()

class survarium::normal_random
  [fn-order]
    only-base  void set_seed(int)
    only-tgt   void set_seed(const int)
    moved      normal_random(int), int randI(int, int), int randI(int), int randIs(int, int), float randF(float, float), float randF(float), float randFs(float, float)

class survarium::npc_stats

class survarium::object_ambient_volume

class survarium::object_decal

class survarium::object_environment_probe

class survarium::object_light
  [size]   base=0x208  target=0x248
  [member]
    changed  m_props: base(render::light_props @0x158)  target(render::light_props @0x150)
    changed  m_light_id: base(u32 @0x200)  target(u32 @0x240)
    only-base  <base> object_controlled, m_current_state
  [fn-order]
    only-base  object_light(survarium::game_scene&), void load(vostok::configs::binary_config_value const&), void load_contents(), void unload_contents(), bool current_state() const, void add_to_scene(), void remove_from_scene()
    only-tgt   object_light(survarium::base_game_scene&), void load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&), void insert(), void remove()

class survarium::object_lpv_occluder

class survarium::object_particle_visual
  [size]   base=0x268  target=0x158
  [member]
    changed  m_particle_system_instance_ptr: base(resources::unmanaged_resource_ptr @0x260)  target(resources::unmanaged_resource_ptr @0x150)
    only-base  <base> object_visual, m_playing
    only-tgt   <base> game_object_static
  [fn-order]
    only-base  object_particle_visual(survarium::game_scene&), void load_contents(), void unload_contents(), void on_visual_ready(vostok::resources::queries_result&)
    only-tgt   object_particle_visual(survarium::base_game_scene&), void load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&), void insert(), void remove(), void on_visual_ready(vostok::resources::queries_result&, boost::function<void __cdecl(survarium::game_object_ &)>&)

class survarium::object_sky

class survarium::object_sky_ambient_occlusion

class survarium::object_vegetation

class survarium::object_volume_fog

class survarium::object_weapon
  [fn-order]
    only-base  vostok::ai::collision_object* get_collision_object() const

class survarium::object_wire
  [fn-order]
    only-base  object_wire(survarium::game_scene&), void load(vostok::configs::binary_config_value const&), void load_contents(), void unload_contents(), void resources_ready(vostok::resources::queries_result&)
    only-tgt   object_wire(survarium::base_game_scene&), void load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&), void insert(), void remove(), void resources_ready(vostok::resources::queries_result&, boost::function<void __cdecl(survarium::game_object_ &)>&)

struct survarium::objects_filter_predicate

class survarium::options_item_base

class survarium::options_item_bool

class survarium::options_tab

class survarium::oxygen_tank
  [fn-order]
    only-base  void active_tick(unsigned int), float reduce_damage(char const*, char const*, float, float), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4*, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    only-tgt   void active_tick(const unsigned int), float reduce_damage(char const*, char const*, const float, const float), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)

class survarium::pistol_weapon_core_aimed_fire_state
  [fn-order]
    only-base  pistol_weapon_core_aimed_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   pistol_weapon_core_aimed_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::pistol_weapon_core_aimed_idle_state
  [fn-order]
    only-base  pistol_weapon_core_aimed_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   pistol_weapon_core_aimed_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::pistol_weapon_core_fire_state
  [fn-order]
    only-base  pistol_weapon_core_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   pistol_weapon_core_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::pistol_weapon_core_hide_state
  [member]
    changed  m_time_scale: base(float @0x178)  target(const float @0x178)
  [fn-order]
    only-base  pistol_weapon_core_hide_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   pistol_weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::pistol_weapon_core_idle_state
  [fn-order]
    only-base  pistol_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   pistol_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::pistol_weapon_core_reload_state
  [fn-order]
    only-base  pistol_weapon_core_reload_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   pistol_weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::pistol_weapon_core_show_state
  [member]
    changed  m_time_scale: base(float @0x178)  target(const float @0x178)
  [fn-order]
    only-base  pistol_weapon_core_show_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   pistol_weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::player
  [fn-order]
    only-base  void subscribe_animation_player(char const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const), void subscribe_animation_player(vostok::animation::reserved_channel_ids_enum, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const* const)
    only-tgt   void subscribe_animation_player(char const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const), void subscribe_animation_player(vostok::animation::reserved_channel_ids_enum, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const* const)

class survarium::player_actions_subscriber

class survarium::player_cook

struct survarium::player_death_subscriber
  [fn-order]
    moved      player_death_subscriber()

class survarium::player_input_handler
  [member]
    changed  m_game_actions: base(fixed_vector< std::pair< game_action_id, player_input_handler::action_state_enum >, 32 > @0x58)  target(fixed_vector< std::pair< enum game_action_id, enum player_input_handler::action_state_enum >, 32 > @0x58)
  [fn-order]
    moved      bool action_present(const survarium::game_action_id, survarium::player_input_handler::action_state_enum&) const

class survarium::player_logic_base_state
  [member]
    changed  m_weapon_user_state_id: base(weapon_user_state_enum @0x20)  target(const weapon_user_state_enum @0x20)
  [fn-order]
    only-base  player_logic_base_state(survarium::weapon_user_animations_selector&, survarium::weapon_user_state_enum), stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const
    only-tgt   player_logic_base_state(survarium::weapon_user_animations_selector&, const survarium::weapon_user_state_enum), stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
    moved      void ~player_logic_base_state()

class survarium::player_logic_crouch_state
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const, vostok::animation::mixing::animation_lexeme movement_lexeme(vostok::mutable_buffer&, unsigned int, vostok::animation::body_part_masks_enum, bool, bool, bool) const, vostok::animation::mixing::expression get_recoil_animation_lexeme(survarium::animation_type_enum, bool, float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, bool, unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const, vostok::animation::mixing::expression look_expression(vostok::mutable_buffer&, unsigned int, bool, bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const, vostok::animation::mixing::animation_lexeme movement_lexeme(vostok::mutable_buffer&, const unsigned int, const vostok::animation::body_part_masks_enum, const bool, const bool, const bool) const, vostok::animation::mixing::expression get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const, vostok::animation::mixing::expression look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const

class survarium::player_logic_jump_state
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const

class survarium::player_logic_sprint_state
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const

class survarium::player_logic_stand_state
  [fn-order]
    only-base  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const, vostok::animation::mixing::animation_lexeme movement_lexeme(vostok::mutable_buffer&, unsigned int, vostok::animation::body_part_masks_enum, bool, bool, bool) const, vostok::animation::mixing::expression get_recoil_animation_lexeme(survarium::animation_type_enum, bool, float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, bool, unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const, vostok::animation::mixing::expression look_expression(vostok::mutable_buffer&, unsigned int, bool, bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const, vostok::animation::mixing::animation_lexeme movement_lexeme(vostok::mutable_buffer&, const unsigned int, const vostok::animation::body_part_masks_enum, const bool, const bool, const bool) const, vostok::animation::mixing::expression get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const, vostok::animation::mixing::expression look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const

struct survarium::player_parameters_modifyer

class survarium::player_parameters_modifyer_cook

class survarium::player_stamina
  [fn-order]
    only-base  void set_max_value(float), void set_spending_speed(float), void set_regeneration_speed(float), void set_regeneration_threshold(float), void set_max_value_factor(float), void set_spending_speed_factor(float), void set_regeneration_speed_factor(float), void tick(unsigned int, bool), void spend(float), void increase_value(float), void decrease_value(float), void regenerate(unsigned int), void sprint(unsigned int)
    only-tgt   void set_max_value(const float), void set_spending_speed(const float), void set_regeneration_speed(const float), void set_regeneration_threshold(const float), void set_max_value_factor(const float), void set_spending_speed_factor(const float), void set_regeneration_speed_factor(const float), void tick(const unsigned int, const bool), void spend(const float), void increase_value(const float), void decrease_value(const float), void regenerate(const unsigned int), void sprint(const unsigned int)
    moved      player_stamina(survarium::player_stamina const&)

struct survarium::player_stamina_subscriber
  [fn-order]
    moved      player_stamina_subscriber()

class survarium::player_stealth
  [fn-order]
    only-base  bool can_be_detected(survarium::player_stealth const&, survarium::player_input const&, float) const
    only-tgt   bool can_be_detected(survarium::player_stealth const&, survarium::player_input const&, const float) const
    moved      player_stealth(survarium::player_stealth const&)

class survarium::profile_character

class survarium::profile_player_character

class survarium::profile_skin_visual_cook
  [fn-order]
    moved      void ~profile_skin_visual_cook()

class survarium::project_cooker_simple
  [fn-order]
    moved      void ~project_cooker_simple()

struct survarium::protect_affect_predicate
  [fn-order]
    only-base  void operator()(survarium::damage_protector*)
    only-tgt   void operator()(survarium::damage_protector* const)

struct survarium::protect_damage_predicate
  [member]
    changed  m_armor_piercing: base(float @0x8)  target(const float @0x8)

class survarium::pseudo_random
  [fn-order]
    only-base  pseudo_random(float), float random_f(float), void set_time(float)
    only-tgt   pseudo_random(const float), float random_f(const float), void set_time(const float)

struct survarium::ray_query_predicate

class survarium::recoil_calculator
  [fn-order]
    only-base  void tick(survarium::weapon_user_state_enum, bool, unsigned int, float)
    only-tgt   void tick(const survarium::weapon_user_state_enum, const bool, const unsigned int, const float)

struct survarium::regenerate_body_parts_predicate
  [member]
    changed  time_delta_ms: base(u32 @0x0)  target(const u32 @0x0)
    changed  current_time_in_ms: base(u32 @0x4)  target(const u32 @0x4)
  [fn-order]
    only-base  regenerate_body_parts_predicate(unsigned int, unsigned int), void operator()(survarium::body_part_parameters*) const
    only-tgt   regenerate_body_parts_predicate(unsigned int, const unsigned int), void operator()(survarium::body_part_parameters* const) const

class survarium::relocate_item_func

struct survarium::reset_predicate
  [fn-order]
    only-base  void operator()(survarium::body_part_parameters*) const
    only-tgt   void operator()(survarium::body_part_parameters* const) const

class survarium::rifle_scope_cook
  [fn-order]
    moved      void ~rifle_scope_cook()

class survarium::scheduler
  [fn-order]
    only-base  survarium::scheduler::record& register_object(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, bool), void on_frame(unsigned int, unsigned int), void on_frame(survarium::scheduler::record&, unsigned int, unsigned int), void register_on_frame(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, bool), void register_for_update(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, bool, unsigned int, unsigned int, unsigned int)
    only-tgt   survarium::scheduler::record& register_object(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, const bool), void on_frame(survarium::scheduler::record&, const unsigned int, const unsigned int), void on_frame(const unsigned int, const unsigned int), void register_on_frame(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, const bool), void register_for_update(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, const bool, const unsigned int, const unsigned int, const unsigned int)

class survarium::shotgun_weapon_reload_state_cook

class survarium::simple_game_project

struct survarium::stamina_depletion_predicate
  [fn-order]
    only-base  void operator()(survarium::player_stamina_subscriber*) const
    only-tgt   void operator()(survarium::player_stamina_subscriber* const) const

class survarium::stats

class survarium::stats_graph

struct survarium::stats_row

struct survarium::stats_stream

class survarium::step_manager

class survarium::swf_input_translator
  [member]
    changed  char_map: base(map< input::enum_keyboard, dik_to_swf_bind, std::less< input::enum_keyboard > > @0x0)  target(map< enum input::enum_keyboard, dik_to_swf_bind, std::less< enum input::enum_keyboard > > @0x0)

class survarium::text_translator

class survarium::victory_item_core

class survarium::victory_items_container_core

class survarium::vostok_file_opener

class survarium::weapon

class survarium::weapon_ammunition
  [fn-order]
    only-base  vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
    only-tgt   vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)

class survarium::weapon_ammunition_cook

struct survarium::weapon_animation_parameters
  [member]
    changed  recoil_horizontal: base(float @0x0)  target(const float @0x0)
    changed  recoil_vertical: base(float @0x4)  target(const float @0x4)
    changed  recoil_backward: base(float @0x8)  target(const float @0x8)
    changed  body_part_mask: base(animation::body_part_masks_enum @0xC)  target(const animation::body_part_masks_enum @0xC)
  [fn-order]
    only-base  weapon_animation_parameters(float, float, float, vostok::animation::body_part_masks_enum, bool, bool)
    only-tgt   weapon_animation_parameters(const float, const float, const float, const vostok::animation::body_part_masks_enum, const bool, const bool)

class survarium::weapon_cook
  [member]
    only-base  <base> resources::translate_query_cook, <base> boost::noncopyable
    only-tgt   <base> weapon_core_cook
  [fn-order]
    only-base  void translate_query(vostok::resources::query_result_for_cook&), void on_weapon_config_ready(vostok::resources::queries_result&, vostok::resources::query_result_for_cook*), void on_weapon_parts_ready(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, vostok::resources::query_result_for_cook*)
    only-tgt   static void register_cooks_for_logic_states(), void on_weapon_subresources_ready(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*), void on_weapon_config_loaded(vostok::resources::queries_result&), unsigned int cooked_object_size(survarium::weapon_core&) const

class survarium::weapon_core
  [fn-order]
    only-base  void set_fire_queue_type(unsigned char), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, bool) const, void set_target(survarium::weapon_targets), void instant_fire(unsigned int), void set_aim_fov_factor(float), void set_aim_near_plane_factor(float), void set_double_handed(bool), void set_chamber_a_round_on_reload(bool), void set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), void set_animation_callback(char const*, void const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), bool& get_is_shown_ref(), bool target_predicate(survarium::weapon_targets) const, bool target_and_animation_ended_predicate(survarium::weapon_targets) const, void update_dispersion(bool, unsigned int), void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, unsigned int, unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&), void on_skeleton_matrices_changed(unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&), void process_finger_correction(unsigned int, vostok::math::float4x4* const), vostok::animation::mixing::expression get_weapon_and_hands_animation_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, float computed_backward_recoil_time(float, float, unsigned int, unsigned int, unsigned int, float), float computed_horizontal_recoil_time(float, float, unsigned int, unsigned int, unsigned int, float), float computed_vertical_recoil_time(float, float, unsigned int, unsigned int, unsigned int, float), vostok::animation::callback_return_type_enum on_hand_ik_event(vostok::animation::animation_callback_params&, survarium::hand_to_weapon_ik_processor::hands_enum), void update_recoil(unsigned int, float), void update_breath_vibration(bool, unsigned int, float)
    only-tgt   void set_fire_queue_type(const unsigned char), vostok::animation::mixing::expression selected_animations(vostok::mutable_buffer&, const bool) const, void set_target(const survarium::weapon_targets), void instant_fire(const unsigned int), void set_aim_fov_factor(const float), void set_aim_near_plane_factor(const float), void set_animation_callback(char const*, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), void set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), bool target_predicate(const survarium::weapon_targets) const, bool target_and_animation_ended_predicate(const survarium::weapon_targets) const, void update_recoil(unsigned int, const float), void update_dispersion(const bool, unsigned int), void update_breath_vibration(const bool, unsigned int, const float), void update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&), void on_skeleton_matrices_changed(const unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&), void process_finger_correction(const unsigned int, vostok::math::float4x4* const), vostok::animation::mixing::expression get_weapon_and_hands_animation_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, float computed_backward_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float), float computed_horizontal_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float), float computed_vertical_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float), vostok::animation::callback_return_type_enum on_hand_ik_event(vostok::animation::animation_callback_params&, const survarium::hand_to_weapon_ik_processor::hands_enum)
    moved      survarium::dispersion_calculator const& get_dispersion_calculator() const, survarium::weapon_user_animations_selector const& user_animations_selector() const, void set_bullet_pierce(float), survarium::weapon_core const* cast_weapon_core() const, void remove_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*), vostok::animation::body_part_masks_enum get_body_part_mask_for_user() const, void initialize_weapon_logic(vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&)

class survarium::weapon_core_aimed_fire_state
  [fn-order]
    only-base  weapon_core_aimed_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   weapon_core_aimed_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::weapon_core_aimed_fire_state_base
  [member]
    changed  m_animation_timescale: base(float @0x140)  target(const float @0x140)
    changed  m_playback_type: base(animation::mixing::playing_type_enum @0x144)  target(animation::mixing::playback_enum @0x144)

class survarium::weapon_core_aimed_state
  [fn-order]
    only-base  weapon_core_aimed_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   weapon_core_aimed_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::weapon_core_animation_end_aware_state

class survarium::weapon_core_base_state
  [member]
    changed  m_serialize_animation_state: base(bool @0x136)  target(const bool @0x136)
  [fn-order]
    only-base  vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_chamber_a_round_aimed_state
  [fn-order]
    only-base  weapon_core_chamber_a_round_aimed_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   weapon_core_chamber_a_round_aimed_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_chamber_a_round_aimed_state_base
  [member]
    changed  m_animation_timescale: base(float @0x140)  target(const float @0x140)
  [fn-order]
    only-base  weapon_core_chamber_a_round_aimed_state_base(survarium::weapon_core&, float)
    only-tgt   weapon_core_chamber_a_round_aimed_state_base(survarium::weapon_core&, const float)

class survarium::weapon_core_chamber_a_round_state
  [fn-order]
    only-base  weapon_core_chamber_a_round_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   weapon_core_chamber_a_round_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_chamber_a_round_state_base
  [member]
    changed  m_animation_timescale: base(float @0x140)  target(const float @0x140)
  [fn-order]
    only-base  weapon_core_chamber_a_round_state_base(survarium::weapon_core&, float)
    only-tgt   weapon_core_chamber_a_round_state_base(survarium::weapon_core&, const float)

class survarium::weapon_core_cook
  [fn-order]
    only-base  void process_loading_weapon_core(vostok::resources::query_result_for_cook*, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*), void query_weapon_states(vostok::resources::query_result_for_cook*, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*)
    only-tgt   void process_loading_weapon_core(vostok::resources::query_result_for_cook* const, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*), void query_weapon_states(vostok::resources::query_result_for_cook* const, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*)
    moved      static void register_cooks_for_logic_states()

class survarium::weapon_core_fire_state
  [fn-order]
    only-base  weapon_core_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   weapon_core_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::weapon_core_fire_state_base
  [member]
    changed  m_animation_timescale: base(float @0x140)  target(const float @0x140)
    changed  m_playback_type: base(animation::mixing::playing_type_enum @0x144)  target(animation::mixing::playback_enum @0x144)

class survarium::weapon_core_hide_state
  [member]
    changed  m_time_scale: base(float @0x168)  target(const float @0x168)
  [fn-order]
    only-base  weapon_core_hide_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_hide_state_base

class survarium::weapon_core_idle_state
  [fn-order]
    only-base  weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::weapon_core_inactive_state
  [fn-order]
    only-base  void on_animation_end(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, unsigned int), void on_specific_event(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   void on_animation_end(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned int), void on_specific_event(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_reload_state
  [fn-order]
    only-base  weapon_core_reload_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_reload_state_base
  [member]
    changed  m_animation_timescale: base(float @0x140)  target(const float @0x140)
  [fn-order]
    only-base  weapon_core_reload_state_base(survarium::weapon_core&, float)
    only-tgt   weapon_core_reload_state_base(survarium::weapon_core&, const float)

class survarium::weapon_core_shotgun_reload_base_substate
  [member]
    changed  m_animation_timescale: base(float @0x14C)  target(const float @0x14C)
    changed  m_playback_type: base(animation::mixing::playing_type_enum @0x150)  target(animation::mixing::playback_enum @0x150)
    changed  m_time_synchronization_group: base(u32 @0x154)  target(const u32 @0x154)
  [fn-order]
    only-base  weapon_core_shotgun_reload_base_substate(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, vostok::animation::mixing::playing_type_enum, unsigned int, char const*, char const*, char const*, char const*), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, void set_animation_playback_state_ptr(vostok::animation::animation_playback_state*), survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const
    only-tgt   weapon_core_shotgun_reload_base_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, const vostok::animation::mixing::playback_enum, const unsigned int, char const*, char const*, char const*, char const*), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, void set_animation_playback_state_ptr(vostok::animation::animation_playback_state* const), survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const

class survarium::weapon_core_shotgun_reload_finish_substate
  [fn-order]
    only-base  weapon_core_shotgun_reload_finish_substate(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   weapon_core_shotgun_reload_finish_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_shotgun_reload_one_round_substate
  [fn-order]
    only-base  weapon_core_shotgun_reload_one_round_substate(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   weapon_core_shotgun_reload_one_round_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_shotgun_reload_start_substate
  [fn-order]
    only-base  weapon_core_shotgun_reload_start_substate(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   weapon_core_shotgun_reload_start_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_shotgun_reload_state
  [fn-order]
    only-base  vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, void set_delete_substates_on_destruction(bool)
    only-tgt   vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, void set_delete_substates_on_destruction(const bool)

class survarium::weapon_core_show_state
  [member]
    changed  m_time_scale: base(float @0x168)  target(const float @0x168)
  [fn-order]
    only-base  weapon_core_show_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
    only-tgt   weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&), vostok::animation::mixing::expression weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const, survarium::weapon_lexeme_pair get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const, vostok::animation::mixing::expression get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const

class survarium::weapon_core_show_state_base

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_fire_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_aimed_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_aimed_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_idle_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_aimed_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_aimed_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_fire_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_hide_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_hide_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_hide_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_idle_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_reload_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_reload_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_show_state>
  [fn-order]
    only-base  survarium::double_barreled_weapon_core_show_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::double_barreled_weapon_core_show_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_fire_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_aimed_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_aimed_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_idle_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_aimed_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_aimed_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_fire_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_hide_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_hide_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_hide_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_idle_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_reload_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_reload_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_reload_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_show_state>
  [fn-order]
    only-base  survarium::pistol_weapon_core_show_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::pistol_weapon_core_show_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_aimed_fire_state>
  [fn-order]
    only-base  survarium::weapon_core_aimed_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_aimed_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_aimed_state>
  [fn-order]
    only-base  survarium::weapon_core_aimed_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_aimed_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_chamber_a_round_aimed_state>
  [fn-order]
    only-base  survarium::weapon_core_chamber_a_round_aimed_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_chamber_a_round_aimed_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_chamber_a_round_state>
  [fn-order]
    only-base  survarium::weapon_core_chamber_a_round_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_chamber_a_round_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_fire_state>
  [fn-order]
    only-base  survarium::weapon_core_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_fire_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_hide_state>
  [fn-order]
    only-base  survarium::weapon_core_hide_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_hide_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_idle_state>
  [fn-order]
    only-base  survarium::weapon_core_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_idle_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_reload_state>
  [fn-order]
    only-base  survarium::weapon_core_reload_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_reload_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_core_state_cook_template<survarium::weapon_core_show_state>
  [fn-order]
    only-base  survarium::weapon_core_show_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, unsigned int)
    only-tgt   survarium::weapon_core_show_state* new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)

class survarium::weapon_dispersion_calculator
  [fn-order]
    only-base  void set_reload_dispersion_amount(float), void set_one_shoot_dispersion_amount(float), void tick(unsigned int), void set_aiming_speed(float)
    only-tgt   void set_reload_dispersion_amount(const float), void set_one_shoot_dispersion_amount(const float), void tick(const unsigned int), void set_aiming_speed(const float)

struct survarium::weapon_dispersion_params
  [fn-order]
    moved      weapon_dispersion_params(vostok::configs::binary_config_value const&)

class survarium::weapon_recoil_calculator
  [fn-order]
    only-base  void set_interpolation_time(float), void set_character_multiplier(float), void set_player_compensation_multiplier(float), void tick(unsigned int, float), void process_compensation(float), float get_random_angle(float), float get_random_amount(float)
    only-tgt   void set_interpolation_time(const float), void set_character_multiplier(const float), void set_player_compensation_multiplier(const float), void tick(const unsigned int, const float), void process_compensation(const float), float get_random_angle(const float), float get_random_amount(const float)

struct survarium::weapon_recoil_params
  [fn-order]
    moved      weapon_recoil_params(vostok::configs::binary_config_value const&)

struct survarium::weapon_state
  [fn-order]
    only-tgt   weapon_state(survarium::weapon_state const&)

struct survarium::weapon_state_creation_params
  [fn-order]
    only-base  weapon_state_creation_params(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core&, float, float, bool&)
    only-tgt   weapon_state_creation_params(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core&, const float, const float, bool&)

class survarium::weapon_user_animations_container
  [fn-order]
    only-base  vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_stand_animation(bool, unsigned int, bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_crouch_animation(bool, unsigned int, bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_sprint_animation(unsigned int, bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_jump_animation(unsigned int, bool) const, char const* get_stand_animation_caption(bool, unsigned int) const, char const* get_crouch_animation_caption(bool, unsigned int) const, char const* get_sprint_animation_caption(unsigned int) const, char const* get_jump_animation_caption(unsigned int) const
    only-tgt   vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_stand_animation(const bool, const unsigned int, const bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_crouch_animation(const bool, const unsigned int, const bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_sprint_animation(const unsigned int, const bool) const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> get_jump_animation(const unsigned int, const bool) const, char const* get_stand_animation_caption(const bool, const unsigned int) const, char const* get_crouch_animation_caption(const bool, const unsigned int) const, char const* get_sprint_animation_caption(const unsigned int) const, char const* get_jump_animation_caption(const unsigned int) const

class survarium::weapon_user_animations_selector
  [fn-order]
    only-base  void set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), void set_animation_callback(char const*, void const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool) const, void set_forced_not_to_sprint(bool), float look_time_factor_calculator(float, float, unsigned int, unsigned int, unsigned int, float) const, void on_broken_limb_affect(char const*, survarium::hit_affects_type_enum, survarium::affect_event_type_enum)
    only-tgt   void set_animation_callback(char const*, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), void set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&), stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const, void set_forced_not_to_sprint(const bool), float look_time_factor_calculator(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float) const, void on_broken_limb_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
    moved      void remove_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*)

class survarium::weapon_user_dead_state

struct survarium::zone_group
  [fn-order]
    only-base  void execute(unsigned int, unsigned int)
    only-tgt   void execute(const unsigned int, const unsigned int)

class vostok::ai::ai_world
  [size]   base=0x698  target=0x688
  [member]
    changed  m_search_service: base(planning::search @0xB0)  target(planning::search @0x98)
    changed  m_engine: base(engine& @0x14C)  target(engine& @0x138)
    changed  m_npc_characters: base(fixed_vector< std::pair< char*, u32 >, 32 > @0x150)  target(fixed_vector< std::pair< char*, u32 >, 32 > @0x13C)
    changed  m_npc_groups: base(fixed_vector< char*, 32 > @0x258)  target(fixed_vector< char*, 32 > @0x244)
    changed  m_npc_classes: base(fixed_vector< char*, 32 > @0x2E0)  target(fixed_vector< char*, 32 > @0x2CC)
    changed  m_npc_outfits: base(fixed_vector< char*, 32 > @0x368)  target(fixed_vector< char*, 32 > @0x354)
    changed  m_melee_weapons: base(fixed_vector< char*, 32 > @0x3F0)  target(fixed_vector< char*, 32 > @0x3DC)
    changed  m_sniper_weapons: base(fixed_vector< char*, 32 > @0x478)  target(fixed_vector< char*, 32 > @0x464)
    changed  m_heavy_weapons: base(fixed_vector< char*, 32 > @0x500)  target(fixed_vector< char*, 32 > @0x4EC)
    changed  m_light_weapons: base(fixed_vector< char*, 32 > @0x588)  target(fixed_vector< char*, 32 > @0x574)
    changed  m_energy_weapons: base(fixed_vector< char*, 32 > @0x610)  target(fixed_vector< char*, 32 > @0x5FC)
    only-base  m_npc_lives_timer
  [fn-order]
    only-base  void set_ignore_filter(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*)
    only-tgt   void set_ignore_filter(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*), void select_new_goal(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>)
    moved      unsigned int get_current_time_in_ms() const

class vostok::ai::behaviour
  [fn-order]
    only-base  static void delete_parameters_filters(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>*)
    only-tgt   static void delete_parameters_filters(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>*)

class vostok::ai::behaviour_cook
  [member]
    changed  m_ai_world: base(ai_world* @0x20)  target(ai_world& @0x20)
    only-tgt   <base> boost::noncopyable
  [fn-order]
    only-base  behaviour_cook(vostok::ai::ai_world*)
    only-tgt   behaviour_cook(vostok::ai::ai_world&)

class vostok::ai::brain_unit
  [fn-order]
    only-base  void set_filter(stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*), bool is_playing_animation() const, bool is_playing_sound() const, bool is_moving() const
    only-tgt   void set_filter(stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*)

class vostok::ai::brain_unit_cook
  [member]
    changed  m_ai_world: base(ai_world* @0x20)  target(ai_world& @0x20)
    only-tgt   <base> boost::noncopyable
  [fn-order]
    only-base  brain_unit_cook(vostok::ai::ai_world*)
    only-tgt   brain_unit_cook(vostok::ai::ai_world&)

class vostok::ai::fsm
  [fn-order]
    moved      vostok::intrusive_list<vostok::ai::fsm_state,vostok::ai::fsm_state *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> const& states() const

struct vostok::ai::game_object
  [fn-order]
    only-base  vostok::ai::collision_object* get_collision_object() const

class vostok::ai::map<unsigned int,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int>,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int>,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int> >,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int> >,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int> >,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int>,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int> >,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int> >,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,stlp_std::pair<vostok::ai::planning::pddl_predicate const *,unsigned int> >,stlp_std::less<unsigned int> > const&)

class vostok::ai::map<unsigned int,vostok::ai::planning::oracle *,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,vostok::ai::planning::oracle *,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::ai::planning::oracle *>,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::ai::planning::oracle *>,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::ai::planning::oracle *>,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,vostok::ai::planning::oracle *,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::ai::planning::oracle *>,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::ai::planning::oracle *>,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::ai::planning::oracle *>,stlp_std::less<unsigned int> > const&)

class vostok::ai::map<unsigned int,vostok::ai::planning::pddl_predicate *,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,vostok::ai::planning::pddl_predicate *,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::ai::planning::pddl_predicate *>,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::ai::planning::pddl_predicate *>,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::ai::planning::pddl_predicate *>,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,vostok::ai::planning::pddl_predicate *,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::ai::planning::pddl_predicate *>,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::ai::planning::pddl_predicate *>,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::ai::planning::pddl_predicate *>,stlp_std::less<unsigned int> > const&)

class vostok::ai::map<unsigned int,vostok::fixed_vector<vostok::ai::planning::object_instance,16>,stlp_std::less<unsigned int> >
  [fn-order]
    only-base  map<unsigned int,vostok::fixed_vector<vostok::ai::planning::object_instance,16>,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::fixed_vector<vostok::ai::planning::object_instance,16> >,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::fixed_vector<vostok::ai::planning::object_instance,16> >,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const,vostok::fixed_vector<vostok::ai::planning::object_instance,16> >,stlp_std::less<unsigned int> > const&)
    only-tgt   map<unsigned int,vostok::fixed_vector<vostok::ai::planning::object_instance,16>,stlp_std::less<unsigned int> >(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::fixed_vector<vostok::ai::planning::object_instance,16> >,stlp_std::less<unsigned int> > const&), vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::fixed_vector<vostok::ai::planning::object_instance,16> >,stlp_std::less<unsigned int> >& operator=(vostok::ai::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::fixed_vector<vostok::ai::planning::object_instance,16> >,stlp_std::less<unsigned int> > const&)

class vostok::ai::navigation::navigation_world
  [fn-order]
    only-base  unsigned int get_node_id_at(vostok::math::float3 const&), bool find_path(unsigned int, vostok::math::float3 const&, unsigned int, vostok::math::float3 const&, float, vostok::vectora<vostok::math::float3>&)
    only-tgt   unsigned int get_node_id_at(vostok::math::float3 const&) const, bool find_path(unsigned int, vostok::math::float3 const&, unsigned int, vostok::math::float3 const&, float, vostok::vectora<vostok::math::float3>&) const

struct vostok::ai::navigation::world
  [fn-order]
    only-base  unsigned int get_node_id_at(vostok::math::float3 const&), bool find_path(unsigned int, vostok::math::float3 const&, unsigned int, vostok::math::float3 const&, float, vostok::vectora<vostok::math::float3>&)
    only-tgt   unsigned int get_node_id_at(vostok::math::float3 const&) const, bool find_path(unsigned int, vostok::math::float3 const&, unsigned int, vostok::math::float3 const&, float, vostok::vectora<vostok::math::float3>&) const

struct vostok::ai::npc
  [fn-order]
    only-base  bool is_playing_animation() const, bool is_moving() const, void stop_animation_playing()

class vostok::ai::planning::action_instance
  [member]
    changed  m_filters_set: base(intrusive_list_item< intrusive_list< intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, debug_policy >* >, intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, debug_policy >* >*, 4, threading::single_threading_policy, size_policy, debug_policy >* >_list @0x18)  target(intrusive_list_item< intrusive_list< intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, no_debug_policy >* >, intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, no_debug_policy >* >*, 4, threading::single_threading_policy, size_policy, no_debug_policy >* >_list @0x18)
  [fn-order]
    only-base  void add_filters_list(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>*), vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>* pop_filter_list(), static bool is_object_suitable(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&, void const* const&)
    only-tgt   void add_filters_list(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>*), vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>* pop_filter_list(), static bool is_object_suitable(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&, void const* const&)

class vostok::ai::planning::goal
  [member]
    changed  m_filters_set: base(intrusive_list_item< intrusive_list< intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, debug_policy >* >, intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, debug_policy >* >*, 4, threading::single_threading_policy, size_policy, debug_policy >* >_list @0x20)  target(intrusive_list_item< intrusive_list< intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, no_debug_policy >* >, intrusive_list_item< intrusive_list< intrusive_list_item< base_filter* >, intrusive_list_item< base_filter* >*, 4, threading::single_threading_policy, size_policy, no_debug_policy >* >*, 4, threading::single_threading_policy, size_policy, no_debug_policy >* >_list @0x20)
  [fn-order]
    only-base  void add_filters_list(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>*), vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>* pop_filter_list()
    only-tgt   void add_filters_list(vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>*), vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>* pop_filter_list()

struct vostok::ai::planning::parameters_suitability_checker_predicate
  [fn-order]
    only-base  bool operator()(vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy> *>*) const
    only-tgt   bool operator()(vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>,vostok::ai::planning::intrusive_list_item<vostok::intrusive_list<vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *>,vostok::ai::planning::intrusive_list_item<vostok::ai::planning::base_filter *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *> *,4,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy> *>*) const

class vostok::ai::planning::search
  [size]   base=0x9C  target=0xA0
  [member]
    changed  m_search_backward: base(search_backward @0x44)  target(search_backward @0x48)
    changed  m_search_forward: base(search_forward @0x6C)  target(search_forward @0x70)
    changed  m_previous_search: base(search* @0x94)  target(search* @0x98)
    changed  m_next_search: base(search* @0x98)  target(search* @0x9C)

class vostok::ai::planning::search_base
  [size]   base=0x38  target=0x3C
  [member]
    changed  m_vertex_manager: base(search_base::vertex_manager_impl_type @0xC)  target(search_base::vertex_manager_impl_type @0x10)
    changed  m_priority_queue: base(search_base::priority_queue_impl_type @0x28)  target(search_base::priority_queue_impl_type @0x2C)

struct vostok::ai::planning::search_base::vertex_allocator_impl_type
  [size]   base=0xC  target=0x10

class vostok::ai::pre_perceptors_filter
  [member]
    changed  m_ignorable_objects: base(fixed_vector< std::pair< game_object const*, ignorance_types_enum >, 16 > @0x0)  target(fixed_vector< std::pair< game_object const*, enum ignorance_types_enum >, 16 > @0x0)
  [fn-order]
    only-base  void ignore(stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*), stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum>* find_ignored_object(vostok::ai::game_object const* const), stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const* find_ignored_object(vostok::ai::game_object const* const) const
    only-tgt   void ignore(stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*), stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum>* find_ignored_object(vostok::ai::game_object const* const), stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const* find_ignored_object(vostok::ai::game_object const* const) const

struct vostok::ai::sensors::cleaning_not_in_frustum_predicate
  [fn-order]
    only-base  cleaning_not_in_frustum_predicate(vostok::intrusive_list<vostok::ai::sensed_visual_object,vostok::ai::sensed_visual_object *,28,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&)
    only-tgt   cleaning_not_in_frustum_predicate(vostok::intrusive_list<vostok::ai::sensed_visual_object,vostok::ai::sensed_visual_object *,28,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&)

class vostok::ai::sensors::vision_sensor
  [fn-order]
    only-base  static vostok::ai::sensed_visual_object* find_visual_object_in_list(vostok::ai::game_object const* const, vostok::intrusive_list<vostok::ai::sensed_visual_object,vostok::ai::sensed_visual_object *,28,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&)
    only-tgt   static vostok::ai::sensed_visual_object* find_visual_object_in_list(vostok::ai::game_object const* const, vostok::intrusive_list<vostok::ai::sensed_visual_object,vostok::ai::sensed_visual_object *,28,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&)

class vostok::ai::vertex_manager::fixed_count_hash::impl<survarium::animations_search_service::vertex_allocator_impl_type,survarium::animations_search_service::look_up_cell_type>
  [fn-order]
    only-tgt   survarium::animations_search_service::vertex_allocator_impl_type& allocator() const

class vostok::ai::vertex_manager::fixed_count_hash::impl<vostok::ai::planning::search_base::vertex_allocator_impl_type,vostok::ai::planning::search_base::look_up_cell_type>
  [fn-order]
    only-tgt   vostok::ai::planning::search_base::vertex_allocator_impl_type& allocator() const

struct vostok::ai::world
  [fn-order]
    only-base  void set_ignore_filter(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,vostok::ai::ignorance_types_enum> const*), unsigned int get_current_time_in_ms() const
    only-tgt   void set_ignore_filter(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*, stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum> const*), void select_new_goal(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>)

struct vostok::animation::animation_callback
  [member]
    changed  callback: base(boost::function< callback_return_type_enum( animation_callback_params& ) > @0x0)  target(boost::function< enum callback_return_type_enum( animation_callback_params& ) > @0x0)
  [fn-order]
    only-base  animation_callback(boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const* const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const)
    only-tgt   animation_callback(boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const* const, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const)

struct vostok::animation::animation_callback_params

class vostok::animation::animation_collection
  [member]
    changed  m_type: base(collection_playing_types @0x118)  target(collection_playback_types @0x118)
  [fn-order]
    only-base  animation_collection(vostok::animation::collection_playing_types, bool, bool, void*, unsigned int, unsigned int)
    only-tgt   animation_collection(vostok::animation::collection_playback_types, bool, bool, void*, unsigned int, unsigned int)

class vostok::animation::animation_event_channels
  [fn-order]
    only-tgt   static unsigned int count_internal_memory_size(vostok::animation::bi_spline_event_channel_baked const*, unsigned int), static unsigned int count_internal_memory_size(vostok::animation::animation_event_channels const&), void create_in_place_internals(vostok::animation::bi_spline_event_channel_baked const*, unsigned int, void*)

class vostok::animation::animation_player
  [fn-order]
    only-base  void subscribe(char const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const), void subscribe(vostok::animation::reserved_channel_ids_enum, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const* const)
    only-tgt   void subscribe(char const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const), void subscribe(vostok::animation::reserved_channel_ids_enum, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const* const)
    moved      static void serialize_empty_state(void*, unsigned int), static void destroy_state(void*), static void destroy_subscriptions(vostok::animation::subscribed_channel const*)

struct vostok::animation::base_interpolator
  [fn-order]
    moved      vostok::animation::base_interpolator* clone(vostok::animation::mixing::n_ary_tree_transition_tree_constructor&) const, void visit(vostok::animation::interpolator_comparer&, vostok::animation::linear_interpolator const&) const

class vostok::animation::bi_spline_skeleton_animation_baked

class vostok::animation::bi_spline_skeleton_animation_impl_cook
  [fn-order]
    moved      static void on_resources_ready(vostok::resources::queries_result&, vostok::resources::query_result_for_cook* const)

class vostok::animation::bone_animation
  [fn-order]
    only-tgt   static unsigned int count_internal_memory_size(vostok::animation::bi_spline_bone_animation const&), static unsigned int count_internal_memory_size(vostok::animation::bi_spline_bone_animation_baked const&), void create_internals_in_place(vostok::animation::bi_spline_bone_animation_baked const&, void*)

class vostok::animation::bone_matrices_computer

class vostok::animation::bone_names
  [fn-order]
    only-base  void read(vostok::configs::binary_config_value const&), void write(_iobuf*&) const

class vostok::animation::cubic_spline_skeleton_animation
  [fn-order]
    only-base  cubic_spline_skeleton_animation(), void create_in_place_internals(void*)
    only-tgt   static unsigned int count_memory_size(vostok::animation::bi_spline_skeleton_animation_baked const&), static vostok::animation::cubic_spline_skeleton_animation* new_animation(void*, vostok::animation::bi_spline_skeleton_animation_baked const&), cubic_spline_skeleton_animation(vostok::animation::bi_spline_skeleton_animation_baked const&), void create_in_place_internals(vostok::animation::bi_spline_skeleton_animation_baked const&, void*)

class vostok::animation::event_channel
  [fn-order]
    only-tgt   static unsigned int count_internal_memory_size(vostok::animation::bi_spline_event_channel_baked const&), static unsigned int count_internal_memory_size(vostok::animation::event_channel const&), void create_in_place_internals(vostok::animation::bi_spline_event_channel_baked const&, void*)

class vostok::animation::fermi_interpolator
  [fn-order]
    moved      vostok::animation::fermi_interpolator* clone(vostok::animation::mixing::n_ary_tree_transition_tree_constructor&) const

struct vostok::animation::fixed_joint_camera_effector

class vostok::animation::i_editor_mixer
  [fn-order]
    only-base  void render(vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const&, vostok::render::scene_renderer&, vostok::render::debug::renderer&, vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base>, bool), void set_model_transform(vostok::math::float4x4&)
    only-tgt   void render(vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const&, vostok::render::scene_renderer&, vostok::render::debug::renderer&, vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base>&, bool), void set_model_transform(vostok::math::float4x4&, void const*)

class vostok::animation::instant_interpolator
  [fn-order]
    moved      vostok::animation::instant_interpolator* clone(vostok::animation::mixing::n_ary_tree_transition_tree_constructor&) const

class vostok::animation::linear_interpolator
  [fn-order]
    moved      vostok::animation::linear_interpolator* clone(vostok::animation::mixing::n_ary_tree_transition_tree_constructor&) const, void accept(vostok::animation::interpolator_comparer&, vostok::animation::base_interpolator const&) const, void visit(vostok::animation::interpolator_comparer&, vostok::animation::fermi_interpolator const&) const, void visit(vostok::animation::interpolator_comparer&, vostok::animation::linear_interpolator const&) const

class vostok::animation::mixing::addition_lexeme
  [fn-order]
    only-base  addition_lexeme(vostok::animation::mixing::addition_lexeme const&, bool)

class vostok::animation::mixing::animated_object_holder
  [member]
    changed  animated_object: base(pcvoid @0x80)  target(pcvoid const @0x80)
  [fn-order]
    only-base  animated_object_holder(void const*), bool operator==(void const*) const
    only-tgt   animated_object_holder(void const* const), bool operator==(void const* const) const, vostok::animation::mixing::animated_object_holder& operator=(vostok::animation::mixing::animated_object_holder const&)

class vostok::animation::mixing::animation_comparer_equal_predicate
  [fn-order]
    only-tgt   vostok::animation::mixing::animation_comparer_equal_predicate& operator=(vostok::animation::mixing::animation_comparer_equal_predicate const&)

class vostok::animation::mixing::animation_comparer_less_predicate
  [fn-order]
    only-tgt   vostok::animation::mixing::animation_comparer_less_predicate& operator=(vostok::animation::mixing::animation_comparer_less_predicate const&)

class vostok::animation::mixing::animation_comparer_predicate
  [fn-order]
    only-tgt   vostok::animation::mixing::animation_comparer_predicate& operator=(vostok::animation::mixing::animation_comparer_predicate const&)

class vostok::animation::mixing::animation_lexeme
  [fn-order]
    only-base  animation_lexeme(vostok::animation::mixing::animation_lexeme&, bool)

class vostok::animation::mixing::animation_lexeme_parameters
  [member]
    changed  m_playback_type: base(playing_type_enum @0x3C)  target(playback_enum @0x3C)
    only-base  <base> noncopyable
    only-tgt   <base> core::noncopyable
  [fn-order]
    only-base  vostok::animation::mixing::animation_lexeme_parameters& playback_type(const vostok::animation::mixing::playing_type_enum), vostok::animation::mixing::playing_type_enum playback_type() const
    only-tgt   vostok::animation::mixing::animation_lexeme_parameters& playback_type(const vostok::animation::mixing::playback_enum), vostok::animation::mixing::playback_enum playback_type() const
    moved      vostok::animation::mixing::animation_interval const* animation_intervals() const, void create_animation_intervals(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&), void ~animation_lexeme_parameters()

struct vostok::animation::mixing::animation_state
  [fn-order]
    moved      vostok::animation::mixing::animation_state& operator=(vostok::animation::mixing::animation_state const&)

class vostok::animation::mixing::base_lexeme
  [fn-order]
    only-base  base_lexeme(vostok::mutable_buffer&, const bool), base_lexeme(vostok::animation::mixing::base_lexeme const&, bool)
    only-tgt   base_lexeme(vostok::mutable_buffer&), base_lexeme(vostok::animation::mixing::base_lexeme const&)

class vostok::animation::mixing::binary_operation_lexeme
  [fn-order]
    only-base  binary_operation_lexeme(vostok::animation::mixing::base_lexeme const&, bool), static vostok::animation::mixing::binary_tree_base_node* cloned_in_buffer(vostok::animation::mixing::expression const&)
    only-tgt   binary_operation_lexeme(vostok::animation::mixing::base_lexeme const&), binary_operation_lexeme(vostok::animation::mixing::binary_operation_lexeme const&)

class vostok::animation::mixing::binary_tree_animation_node
  [member]
    changed  m_playback_type: base(const playing_type_enum @0x5C)  target(const playback_enum @0x5C)
  [fn-order]
    only-base  vostok::animation::mixing::playing_type_enum playback_type() const
    only-tgt   vostok::animation::mixing::playback_enum playback_type() const

class vostok::animation::mixing::multiplication_lexeme
  [fn-order]
    only-base  multiplication_lexeme(vostok::animation::mixing::multiplication_lexeme const&, bool)

class vostok::animation::mixing::n_ary_tree
  [fn-order]
    moved      static bool dispatch_callbacks(vostok::animation::mixing::callback_generator_info const*, vostok::animation::subscribed_channel*&, unsigned int, bool&), static void update_animation_time(vostok::animation::mixing::animation_state&)

class vostok::animation::mixing::n_ary_tree_animation_event_iterator
  [fn-order]
    only-tgt   void invert_times(unsigned int)

class vostok::animation::mixing::n_ary_tree_animation_node
  [member]
    changed  m_playback_type: base(const playing_type_enum @0x44)  target(const playback_enum @0x44)
  [fn-order]
    only-base  n_ary_tree_animation_node(vostok::animation::mixing::animation_interval const*, vostok::animation::mixing::animation_interval const*, unsigned char, unsigned int, vostok::animation::base_interpolator const&, void const* const, vostok::animation::mixing::playing_type_enum, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, unsigned int, unsigned int, bool, bool, bool, unsigned int, unsigned int, unsigned int, bool), n_ary_tree_animation_node(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::animation_interval const*, vostok::animation::mixing::animation_interval const*, unsigned char, unsigned int, void const* const, vostok::animation::mixing::playing_type_enum, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, unsigned int, bool, bool, const bool, unsigned int, unsigned int, unsigned int, bool), vostok::animation::mixing::playing_type_enum playback_type() const
    only-tgt   n_ary_tree_animation_node(vostok::animation::mixing::animation_interval const*, vostok::animation::mixing::animation_interval const*, unsigned char, unsigned int, vostok::animation::base_interpolator const&, void const* const, vostok::animation::mixing::playback_enum, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, unsigned int, unsigned int, bool, bool, bool, unsigned int, unsigned int, unsigned int, bool), n_ary_tree_animation_node(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::animation_interval const*, vostok::animation::mixing::animation_interval const*, unsigned char, unsigned int, void const* const, vostok::animation::mixing::playback_enum, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&, unsigned int, bool, bool, const bool, unsigned int, unsigned int, unsigned int, bool), vostok::animation::mixing::animation_state const* get_animation_state() const, char const* identifier() const, vostok::animation::mixing::playback_enum playback_type() const

class vostok::animation::mixing::n_ary_tree_dumper
  [member]
    changed  m_verbosity: base(logging::verbosity @0x10)  target(const logging::verbosity @0x10)
  [fn-order]
    only-base  n_ary_tree_dumper(const unsigned int)
    only-tgt   n_ary_tree_dumper(const vostok::logging::verbosity, const unsigned int)

class vostok::animation::mixing::n_ary_tree_event_iterator
  [fn-order]
    only-base  n_ary_tree_event_iterator(vostok::animation::mixing::animation_state&, vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int, unsigned int, vostok::animation::subscribed_channel*&)
    only-tgt   n_ary_tree_event_iterator(vostok::animation::mixing::animation_state&, vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int, unsigned short, vostok::animation::subscribed_channel*&)

class vostok::animation::mixing::n_ary_tree_node_comparer
  [member]
    changed  result: base(n_ary_tree_node_comparer::enum_result @0x4)  target(comparison_result_enum @0x4)
  [fn-order]
    only-base  vostok::animation::mixing::n_ary_tree_node_comparer::enum_result compare(vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::mixing::n_ary_tree_base_node&)
    only-tgt   vostok::animation::comparison_result_enum compare(vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::mixing::n_ary_tree_base_node&)

class vostok::animation::mixing::n_ary_tree_time_scale_calculator
  [member]
    changed  m_animation: base(n_ary_tree_animation_node const* const @0x4)  target(n_ary_tree_animation_node* const @0x4)
  [fn-order]
    only-base  n_ary_tree_time_scale_calculator(unsigned int, float, unsigned int, vostok::animation::mixing::n_ary_tree_animation_node const*)
    only-tgt   n_ary_tree_time_scale_calculator(unsigned int, float, unsigned int, vostok::animation::mixing::n_ary_tree_animation_node*)

class vostok::animation::mixing::n_ary_tree_time_scale_transition_node
  [fn-order]
    only-base  n_ary_tree_time_scale_transition_node(vostok::animation::mixing::n_ary_tree_weight_transition_node const&), vostok::animation::mixing::n_ary_tree_time_scale_transition_node& operator=(vostok::animation::mixing::n_ary_tree_weight_transition_node const&)
    moved      void ~n_ary_tree_time_scale_transition_node()

class vostok::animation::mixing::n_ary_tree_transition_tree_constructor
  [fn-order]
    only-base  n_ary_tree_transition_tree_constructor(vostok::mutable_buffer&, vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&, unsigned int, unsigned int, unsigned int, vostok::animation::subscribed_channel*&, boost::function<vostok::math::float4x4 __cdecl(void const *)> const&), vostok::animation::mixing::n_ary_tree_base_node* new_time_scale_transition(float, float, vostok::animation::mixing::n_ary_tree_base_node&), void add_operands(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node**, vostok::animation::mixing::n_ary_tree_base_node**, bool), vostok::animation::mixing::n_ary_tree_animation_node* add_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node*), vostok::animation::mixing::n_ary_tree_animation_node* remove_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node*, bool), void change_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node*, bool), void merge_weight_asynchronous_groups(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*), void advance_buffer(unsigned int)
    only-tgt   n_ary_tree_transition_tree_constructor(vostok::mutable_buffer&, vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&, const unsigned int, const unsigned int, const unsigned int, vostok::animation::subscribed_channel*&, boost::function<vostok::math::float4x4 __cdecl(void const *)> const&), vostok::animation::mixing::n_ary_tree_base_node* new_time_scale_transition(const float, float, vostok::animation::mixing::n_ary_tree_base_node&), void add_operands(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node**, vostok::animation::mixing::n_ary_tree_base_node**, const bool), vostok::animation::mixing::n_ary_tree_animation_node* add_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const), vostok::animation::mixing::n_ary_tree_animation_node* remove_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const, bool), void change_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const, bool), void merge_weight_asynchronous_groups(vostok::animation::mixing::n_ary_tree_animation_node* const, vostok::animation::mixing::n_ary_tree_animation_node* const, vostok::animation::mixing::n_ary_tree_animation_node* const, vostok::animation::mixing::n_ary_tree_animation_node* const), void advance_buffer(const unsigned int)

class vostok::animation::mixing::n_ary_tree_weight_event_iterator
  [fn-order]
    only-base  vostok::animation::mixing::animation_event operator*()
    only-tgt   vostok::animation::mixing::animation_event operator*() const

class vostok::animation::mixing::n_ary_tree_weight_transition_node
  [member]
    changed  m_start_time_in_ms: base(const u32 @0x10)  target(u32 @0x10)

class vostok::animation::mixing::subtraction_lexeme
  [fn-order]
    only-base  subtraction_lexeme(vostok::animation::mixing::subtraction_lexeme const&, bool)

class vostok::animation::mixing::weight_lexeme
  [fn-order]
    only-base  weight_lexeme(vostok::animation::mixing::weight_lexeme const&, bool), weight_lexeme(vostok::mutable_buffer&, const float, vostok::animation::base_interpolator const* const, const bool)
    only-tgt   weight_lexeme(vostok::mutable_buffer&, const float, vostok::animation::base_interpolator const* const)

class vostok::animation::poly_curve<vostok::animation::poly_curve_order3_domain<float,1> >
  [fn-order]
    only-tgt   static unsigned int count_internal_memory_size(vostok::animation::bi_spline_channel_animation_baked const&), void create_in_place_internals(vostok::animation::bi_spline_channel_animation_baked const&, void*)

class vostok::animation::skeleton_animation_cook
  [size]   base=0x24  target=0x20
  [member]
    only-base  m_world
  [fn-order]
    only-base  skeleton_animation_cook(vostok::animation::world&)
    only-tgt   skeleton_animation_cook()
    moved      void translate_query(vostok::resources::query_result_for_cook&), void delete_resource(vostok::resources::resource_base*)

class vostok::animation::skeleton_bone
  [fn-order]
    only-base  skeleton_bone(char const* const, vostok::animation::skeleton_bone const* const, vostok::animation::skeleton_bone const* const, vostok::animation::skeleton_bone const* const), unsigned int mask() const
    only-tgt   skeleton_bone(char const* const, vostok::animation::skeleton_bone const* const, vostok::animation::skeleton_bone const* const, vostok::animation::skeleton_bone const* const, const unsigned int), const unsigned int mask() const

class vostok::collision::animated_object
  [fn-order]
    only-base  void draw_collision(vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const&, vostok::render::debug::renderer&, vostok::math::float4x4 const&) const
    only-tgt   animated_object(vostok::configs::binary_config_value const&, vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, const unsigned int, vostok::memory::stack_allocator&, const bool)

struct vostok::collision::primitive
  [fn-order]
    only-base  primitive(vostok::collision::cylinder const&), vostok::collision::cylinder cylinder() const
    only-tgt   primitive(vostok::collision::cylinder_x const&), primitive(vostok::collision::cylinder_y const&), vostok::collision::cylinder_x cylinder_x() const, vostok::collision::cylinder_y cylinder_y() const

class vostok::collision::sphere_geometry_instance
  [fn-order]
    only-base  float radius_unit() const

class vostok::configs::binary_config_value
  [member]
    changed  id: base(platform_pointer_selector< char const, 1 >::helper @0x8)  target(platform_pointer_selector< char const , 1 >::helper @0x8)

class vostok::console_commands::cc_bool
  [member]
    only-base  <base> console_command, m_value
    only-tgt   <base> cc_value< bool >

class vostok::console_commands::cc_string
  [fn-order]
    only-base  char const* get_value() const
    only-tgt   char const* get_value()

struct vostok::console_commands::starts_from_predicate
  [member]
    changed  starts_from: base(pcstr @0x0)  target(pcstr const @0x0)
  [fn-order]
    only-base  starts_from_predicate(char const*)
    only-tgt   starts_from_predicate(char const* const), starts_from_predicate(vostok::console_commands::starts_from_predicate const&), vostok::console_commands::starts_from_predicate& operator=(vostok::console_commands::starts_from_predicate const&)

class vostok::core::configs::binary_config_cook
  [fn-order]
    only-base  binary_config_cook()
    only-tgt   binary_config_cook(vostok::resources::class_id_enum), void register_object_to_delete(vostok::core::configs::binary_config*, unsigned int)

struct vostok::editor::engine
  [fn-order]
    only-base  void load_level(char const*, char const*)
    only-tgt   void load_level(char const*), void on_fullscreen_alttab(bool)

class vostok::engine::engine_world
  [fn-order]
    only-base  void load_level(char const*, char const*), void initialize_render(), void create_render()
    only-tgt   void on_fullscreen_alttab(bool), void load_level(char const*), void initialize_render(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> const&, bool), void create_render(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> const&, bool)
    moved      void exit(int)

struct vostok::engine_user::world
  [fn-order]
    only-base  void load(char const*, char const*)
    only-tgt   void load(char const*), bool is_loading() const, void on_fullscreen_alttab(bool)

class vostok::fixed_string<1024>
  [fn-order]
    only-tgt   vostok::fixed_string<1024> const& operator=(char const* const)

class vostok::fixed_string<11>
  [fn-order]
    only-tgt   vostok::fixed_string<11> const& operator=(char const* const)

class vostok::fixed_string<128>
  [fn-order]
    only-tgt   vostok::fixed_string<128> const& operator=(char const* const)

class vostok::fixed_string<16>
  [fn-order]
    only-tgt   vostok::fixed_string<16> const& operator=(char const* const)

class vostok::fixed_string<2048>
  [fn-order]
    only-tgt   vostok::fixed_string<2048> const& operator=(char const* const)

class vostok::fixed_string<20>
  [fn-order]
    only-tgt   vostok::fixed_string<20> const& operator=(char const* const)

class vostok::fixed_string<24>
  [fn-order]
    only-tgt   vostok::fixed_string<24> const& operator=(char const* const)

class vostok::fixed_string<256>
  [fn-order]
    only-tgt   vostok::fixed_string<256> const& operator=(char const* const)

class vostok::fixed_string<260>
  [fn-order]
    only-tgt   vostok::fixed_string<260> const& operator=(char const* const)

class vostok::fixed_string<32>
  [fn-order]
    only-tgt   vostok::fixed_string<32> const& operator=(char const* const)

class vostok::fixed_string<4096>
  [fn-order]
    only-tgt   vostok::fixed_string<4096> const& operator=(char const* const)

class vostok::fixed_string<42>
  [fn-order]
    only-tgt   vostok::fixed_string<42> const& operator=(char const* const)

class vostok::fixed_string<46>
  [fn-order]
    only-tgt   vostok::fixed_string<46> const& operator=(char const* const)

class vostok::fixed_string<512>
  [fn-order]
    only-tgt   vostok::fixed_string<512> const& operator=(char const* const)

class vostok::fixed_string<520>
  [fn-order]
    only-tgt   vostok::fixed_string<520> const& operator=(char const* const)

class vostok::fixed_string<64>
  [fn-order]
    only-tgt   vostok::fixed_string<64> const& operator=(char const* const)

class vostok::fixed_string<8192>
  [fn-order]
    only-tgt   vostok::fixed_string<8192> const& operator=(char const* const)

class vostok::fixed_string<8>
  [fn-order]
    only-tgt   vostok::fixed_string<8> const& operator=(char const* const)

class vostok::game_test_suite
  [size]   base=0x38  target=0x30
  [member]
    only-base  m_rtp_world
  [fn-order]
    only-base  void set_rtp_world(vostok::rtp::world*), vostok::rtp::world* rtp_world() const

struct vostok::input::handler
  [fn-order]
    only-base  void on_before_processing(vostok::input::world*)
    only-tgt   void on_before_processing(vostok::input::world*, const unsigned int)

class vostok::input::input_world
  [fn-order]
    only-base  void tick()
    only-tgt   void tick(unsigned int)

class vostok::input::receiver::keyboard
  [member]
    changed  m_current_events_count: base(u32 @0x404)  target(unsigned long @0x404)

struct vostok::input::world
  [fn-order]
    only-base  void tick()
    only-tgt   void tick(unsigned int)

class vostok::math::color
  [member]
    changed  b: base(u8 @0x0)  target(u8 @0x2)
    changed  r: base(u8 @0x2)  target(u8 @0x0)
    reordered  b, g

class vostok::math::convex
  [fn-order]
    only-base  convex(stlp_std::vector<vostok::math::aabb_plane,stlp_std::allocator<vostok::math::aabb_plane> >, stlp_std::vector<vostok::fixed_vector<unsigned char,8>,stlp_std::allocator<vostok::fixed_vector<unsigned char,8> > >), stlp_std::vector<vostok::math::aabb_plane,stlp_std::allocator<vostok::math::aabb_plane> > get_planes() const, stlp_std::vector<vostok::fixed_vector<unsigned char,8>,stlp_std::allocator<vostok::fixed_vector<unsigned char,8> > > get_adjacencies() const, static stlp_std::vector<unsigned char,stlp_std::allocator<unsigned char> > get_plane_flags(stlp_std::vector<vostok::fixed_vector<unsigned char,8>,stlp_std::allocator<vostok::fixed_vector<unsigned char,8> > >), static void build_original_to_new_adjacency_dict(stlp_std::vector<unsigned char,stlp_std::allocator<unsigned char> >, vostok::buffer_vector<unsigned char>&)
    only-tgt   convex(stlp_std::vector<vostok::math::aabb_plane,stlp_std::allocator<vostok::math::aabb_plane> > const&, stlp_std::vector<vostok::fixed_vector<unsigned char,8>,stlp_std::allocator<vostok::fixed_vector<unsigned char,8> > > const&), stlp_std::vector<vostok::math::aabb_plane,stlp_std::allocator<vostok::math::aabb_plane> > const& get_planes() const, stlp_std::vector<vostok::fixed_vector<unsigned char,8>,stlp_std::allocator<vostok::fixed_vector<unsigned char,8> > > const& get_adjacencies() const, static stlp_std::vector<unsigned char,stlp_std::allocator<unsigned char> > get_plane_flags(stlp_std::vector<vostok::fixed_vector<unsigned char,8>,stlp_std::allocator<vostok::fixed_vector<unsigned char,8> > > const&), static void build_original_to_new_adjacency_dict(stlp_std::vector<unsigned char,stlp_std::allocator<unsigned char> > const&, vostok::buffer_vector<unsigned char>&)
    moved      void sort_adjacencies()

struct vostok::math::curve_line_points<float,0>
  [fn-order]
    only-base  vostok::math::curve_line_points<float,0>& operator=(vostok::math::curve_line_points<float,0> const&)
    only-tgt   void operator=(vostok::math::curve_line_points<float,0> const&)
    moved      curve_line_points<float,0>()

struct vostok::math::curve_line_points<vostok::math::float3_pod,0>
  [fn-order]
    only-base  vostok::math::curve_line_points<vostok::math::float3_pod,0>& operator=(vostok::math::curve_line_points<vostok::math::float3_pod,0> const&)
    only-tgt   void operator=(vostok::math::curve_line_points<vostok::math::float3_pod,0> const&)
    moved      curve_line_points<vostok::math::float3_pod,0>()

struct vostok::math::curve_line_points<vostok::math::float4_pod,0>
  [fn-order]
    only-base  vostok::math::curve_line_points<vostok::math::float4_pod,0>& operator=(vostok::math::curve_line_points<vostok::math::float4_pod,0> const&)
    only-tgt   void operator=(vostok::math::curve_line_points<vostok::math::float4_pod,0> const&)
    moved      curve_line_points<vostok::math::float4_pod,0>()

struct vostok::math::curve_line_points<vostok::math::float4_pod,1>
  [fn-order]
    only-base  vostok::math::curve_line_points<vostok::math::float4_pod,1>& operator=(vostok::math::curve_line_points<vostok::math::float4_pod,1> const&)
    only-tgt   void operator=(vostok::math::curve_line_points<vostok::math::float4_pod,1> const&)
    moved      curve_line_points<vostok::math::float4_pod,1>()

class vostok::math::float4x4
  [fn-order]
    only-tgt   void compute_characteristic_polynom(float[5]&) const

class vostok::math::frustum
  [fn-order]
    only-tgt   frustum(const vostok::math::plane[6]&)

class vostok::math::quaternion
  [fn-order]
    only-tgt   vostok::math::quaternion operator-() const

class vostok::math::random32
  [fn-order]
    only-base  unsigned int seed()
    only-tgt   unsigned int seed() const, unsigned int random(const unsigned int, const unsigned int), float random_f(const float, const float)

class vostok::math::random32_with_count
  [fn-order]
    only-tgt   unsigned int random(const unsigned int, const unsigned int)

class vostok::memory::base_allocator

class vostok::memory::doug_lea_mt_allocator
  [fn-order]
    only-base  doug_lea_mt_allocator(const bool, const bool, bool)
    only-tgt   doug_lea_mt_allocator(const bool, const bool, bool, bool)
    moved      void initialize(void*, unsigned long long, char const*)

class vostok::memory::fixed_size_allocator<vostok::particle::base_particle,vostok::threading::mutex>
  [member]
    changed  m_allocator: base(uninitialized_reference< single_size_buffer_allocator< 176, threading::mutex > > @0x18)  target(uninitialized_reference< single_size_buffer_allocator< 208, threading::mutex > > @0x18)

class vostok::memory::single_size_buffer_allocator<12,vostok::threading::mutex>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<12,vostok::threading::mutex>()

class vostok::memory::single_size_buffer_allocator<120,vostok::threading::mutex_tasks_unaware>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<120,vostok::threading::mutex_tasks_unaware>()

class vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<128,vostok::threading::simple_lock>()

class vostok::memory::single_size_buffer_allocator<16,vostok::threading::single_threading_policy>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<16,vostok::threading::single_threading_policy>()

class vostok::memory::single_size_buffer_allocator<208,vostok::threading::mutex>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<208,vostok::threading::mutex>()

class vostok::memory::single_size_buffer_allocator<300,vostok::threading::single_threading_policy>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<300,vostok::threading::single_threading_policy>()

class vostok::memory::single_size_buffer_allocator<44,vostok::threading::single_threading_policy>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<44,vostok::threading::single_threading_policy>()

class vostok::memory::single_size_buffer_allocator<44200,vostok::threading::single_threading_policy>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<44200,vostok::threading::single_threading_policy>()

class vostok::memory::single_size_buffer_allocator<536,vostok::threading::multi_threading_policy>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<536,vostok::threading::multi_threading_policy>()

class vostok::memory::single_size_buffer_allocator<8,vostok::threading::single_threading_policy>
  [fn-order]
    only-tgt   unsigned int allocated_count() const, unsigned int max_count() const, bool is_memory_available() const
    moved      void ~single_size_buffer_allocator<8,vostok::threading::single_threading_policy>()

class vostok::network::connect_order
  [member]
    only-base  <base> boost::noncopyable, m_functor, m_allocator, m_port
    only-tgt   m_connector, m_packets_allocator, m_strings_allocator, m_packet
  [fn-order]
    only-base  connect_order(boost::function<void __cdecl(char const *,unsigned short)> const&, char const* const, const unsigned short, vostok::memory::base_allocator&)
    only-tgt   connect_order(vostok::memory::base_allocator&, char const* const, vostok::intrusive_ptr<vostok::network_core::udp_match_packets_allocator,vostok::network_core::udp_match_packets_allocator,vostok::threading::multi_threading_policy> const&, vostok::network_core::udp_match_packet const*, boost::function<void __cdecl(char const *,vostok::network_core::udp_match_packet const *)> const&)

class vostok::network::http_client

class vostok::network::login_client
  [member]
    changed  m_on_sign_up: base(boost::function< void( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, sign_up_info const& ) > @0xB8)  target(boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, sign_up_info const& ) > @0xB8)
    changed  m_on_sign_in: base(boost::function< void( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > @0xD8)  target(boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > @0xD8)
    changed  m_on_sign_out: base(boost::function< void( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > @0xF8)  target(boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > @0xF8)
  [fn-order]
    only-base  void sign_up(char const*, unsigned short, vostok::sign_up_info const&, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&), void sign_in(char const*, unsigned short, char const*, char const*, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&), void sign_out(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&)
    only-tgt   void sign_up(char const*, unsigned short, vostok::sign_up_info const&, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&), void sign_in(char const*, unsigned short, char const*, char const*, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&), void sign_out(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&)

class vostok::network::login_client_impl
  [fn-order]
    only-base  void sign_up(char const*, unsigned short, vostok::sign_up_info const&, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&), void sign_in(char const*, unsigned short, char const*, char const*, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&), void sign_out(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&), void on_resolved(boost::asio::ip::basic_resolver<boost::asio::ip::tcp,boost::asio::ip::resolver_service<boost::asio::ip::tcp> >* const, const unsigned int, boost::function<void __cdecl(vostok::resolve_error_types_enum,boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>)> const&, boost::system::error_code const&, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>), void resolve(boost::function<void __cdecl(vostok::resolve_error_types_enum,boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>)> const&, const unsigned int), void on_connected(const unsigned int, boost::function<void __cdecl(vostok::connection_error_types_enum)> const&, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>, boost::system::error_code const&, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>), void connect(vostok::resolve_error_types_enum, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>, const unsigned int, boost::function<void __cdecl(vostok::connection_error_types_enum)> const&), void on_handshake_connected(const vostok::connection_error_types_enum, boost::function<void __cdecl(vostok::handshaking_error_types_enum)> const&, const unsigned int, bool), void on_handshaked(boost::system::error_code const&, boost::function<void __cdecl(vostok::handshaking_error_types_enum)> const&, unsigned int, bool), void handshake(boost::function<void __cdecl(vostok::handshaking_error_types_enum)> const&, unsigned int, bool), void establish_connection(boost::function<void __cdecl(vostok::connection_error_types_enum)> const&, const unsigned int, const unsigned int), void on_sign_in_answer_received(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_in_password_written(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_in_handshaked(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, const vostok::handshaking_error_types_enum), void on_user_name_answer_received(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_in_written(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, unsigned int), void sign_in_on_connected(vostok::connection_error_types_enum, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&), void on_sign_out_password_written(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_out_handshaked(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, const vostok::handshaking_error_types_enum), void on_sign_out_written(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, unsigned int), void sign_out_on_connected(vostok::connection_error_types_enum, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum)> const&), void on_sign_up_answer_received(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void on_sign_up_info_written(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void sign_up_on_handshaked(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, const vostok::handshaking_error_types_enum), void on_sign_up_account_answer_received(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void on_sign_up_written(boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void sign_up_on_connected(const vostok::connection_error_types_enum, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&)
    only-tgt   void sign_up(char const*, unsigned short, vostok::sign_up_info const&, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&), void sign_in(char const*, unsigned short, char const*, char const*, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&), void sign_out(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&), void on_resolved(boost::asio::ip::basic_resolver<boost::asio::ip::tcp,boost::asio::ip::resolver_service<boost::asio::ip::tcp> >* const, const unsigned int, boost::function<void __cdecl(enum vostok::resolve_error_types_enum,boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>)> const&, boost::system::error_code const&, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>), void resolve(boost::function<void __cdecl(enum vostok::resolve_error_types_enum,boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>)> const&, const unsigned int), void on_connected(const unsigned int, boost::function<void __cdecl(enum vostok::connection_error_types_enum)> const&, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>, boost::system::error_code const&, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>), void connect(vostok::resolve_error_types_enum, boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>, const unsigned int, boost::function<void __cdecl(enum vostok::connection_error_types_enum)> const&), void on_handshake_connected(const vostok::connection_error_types_enum, boost::function<void __cdecl(enum vostok::handshaking_error_types_enum)> const&, const unsigned int, bool), void on_handshaked(boost::system::error_code const&, boost::function<void __cdecl(enum vostok::handshaking_error_types_enum)> const&, unsigned int, bool), void handshake(boost::function<void __cdecl(enum vostok::handshaking_error_types_enum)> const&, unsigned int, bool), void establish_connection(boost::function<void __cdecl(enum vostok::connection_error_types_enum)> const&, const unsigned int, const unsigned int), void on_sign_in_answer_received(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_in_password_written(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_in_handshaked(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, const vostok::handshaking_error_types_enum), void on_user_name_answer_received(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_in_written(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, unsigned int), void sign_in_on_connected(vostok::connection_error_types_enum, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&), void on_sign_out_password_written(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, const unsigned int), void on_sign_out_handshaked(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, const vostok::handshaking_error_types_enum), void on_sign_out_written(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&, boost::system::error_code const&, unsigned int), void sign_out_on_connected(vostok::connection_error_types_enum, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum)> const&), void on_sign_up_answer_received(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void on_sign_up_info_written(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void sign_up_on_handshaked(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, const vostok::handshaking_error_types_enum), void on_sign_up_account_answer_received(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void on_sign_up_written(boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&, boost::system::error_code const&, const unsigned int), void sign_up_on_connected(const vostok::connection_error_types_enum, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::login_server_message_types_enum,vostok::sign_up_info const &)> const&, vostok::sign_up_info const&)

class vostok::network::match_client
  [member]
    changed  m_on_connected: base(boost::function< void( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, lobby_server_message_types_enum ) > @0x88)  target(boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > @0x88)
    changed  m_on_disconnected: base(boost::function< void( network_core::disconnect_event_types_enum ) > @0xC8)  target(boost::function< void( enum network_core::disconnect_event_types_enum ) > @0xC8)
  [fn-order]
    only-base  void connect(char const*, unsigned short, unsigned int, vostok::network_core::udp_match_packet*, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::lobby_server_message_types_enum)> const&), void set_on_disconnect(boost::function<void __cdecl(vostok::network_core::disconnect_event_types_enum)> const&)
    only-tgt   void connect(char const*, unsigned short, unsigned int, vostok::network_core::udp_match_packet*, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::lobby_server_message_types_enum)> const&), void set_on_disconnect(boost::function<void __cdecl(enum vostok::network_core::disconnect_event_types_enum)> const&)

class vostok::network::match_client_impl
  [size]   base=0x258BA8  target=0x96BA0
  [member]
    changed  m_packets_storage: base(boost::array< char [300], 8192 > @0x0)  target(boost::array< char [300], 2048 > @0x0)
    changed  m_packets_allocator: base(memory::single_size_buffer_allocator< 300, threading::single_threading_policy > @0x258000)  target(memory::single_size_buffer_allocator< 300, threading::single_threading_policy > @0x96000)
    changed  m_on_disconnect: base(boost::function< void( network_core::disconnect_event_types_enum ) > @0x258010)  target(boost::function< void( enum network_core::disconnect_event_types_enum ) > @0x96010)
    changed  m_network_flow_emulator: base(network_core::udp_network_flow_emulator* @0x258030)  target(network_core::udp_network_flow_emulator* @0x96030)
    changed  m_client: base(network_core::udp_match_client @0x258038)  target(network_core::udp_match_client @0x96038)
    changed  m_on_connected: base(boost::function< void( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, lobby_server_message_types_enum ) > @0x258B60)  target(boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > @0x96B58)
    changed  m_on_packet_received: base(boost::function< void( u8, network_core::packet_reader& ) > @0x258B80)  target(boost::function< void( u8, network_core::packet_reader& ) > @0x96B78)
    changed  m_state: base(match_client_impl::state @0x258BA0)  target(match_client_impl::state @0x96B98)
  [fn-order]
    only-base  void connect(char const*, unsigned short, unsigned int, vostok::network_core::udp_match_packet const*, boost::function<void __cdecl(vostok::connection_error_types_enum,vostok::handshaking_error_types_enum,vostok::socket_error_types_enum,vostok::lobby_server_message_types_enum)> const&), void set_on_disconnect(boost::function<void __cdecl(vostok::network_core::disconnect_event_types_enum)> const&)
    only-tgt   void connect(char const*, unsigned short, unsigned int, vostok::network_core::udp_match_packet const*, boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::lobby_server_message_types_enum)> const&), void set_on_disconnect(boost::function<void __cdecl(enum vostok::network_core::disconnect_event_types_enum)> const&)
    moved      static void construct_packet(vostok::network_core::udp_match_packets_orderer&, vostok::network_core::udp_match_packet&, unsigned char)

class vostok::network::network_world
  [size]   base=0x12C  target=0x124
  [member]
    changed  m_io_service: base(boost::asio::io_service @0x4)  target(boost::asio::io_service* @0x4)
    changed  m_channel: base(two_way_threads_channel @0x10)  target(two_way_threads_channel @0x8)
    changed  m_engine: base(engine& @0x128)  target(engine& @0x120)
  [fn-order]
    only-base  void tick(), vostok::network::packet* new_packet()
    only-tgt   void tick(bool), vostok::network_core::tcp_packet* new_packet()

class vostok::network::order

class vostok::network::receive_response
  [member]
    changed  m_receiver: base(boost::function< void( packet const& ) > @0x8)  target(boost::function< void( network_core::packet_reader& ) > @0x8)
    changed  m_packet: base(packet const& @0x2C)  target(network_core::tcp_packet const& @0x2C)
  [fn-order]
    only-base  receive_response(boost::function<void __cdecl(vostok::network::packet const &)> const&, vostok::memory::doug_lea_allocator&, vostok::network::packet const&)
    only-tgt   receive_response(boost::function<void __cdecl(vostok::network_core::packet_reader &)> const&, vostok::memory::doug_lea_allocator&, vostok::network_core::tcp_packet const&)

class vostok::network::receive_udp_response

class vostok::network::send_order
  [member]
    changed  m_sender: base(boost::function< void( packet const& ) > @0x8)  target(boost::function< void( network_core::tcp_packet const& ) > @0x8)
    changed  m_packet: base(packet const& @0x28)  target(network_core::tcp_packet const& @0x28)
    only-base  <base> boost::noncopyable
  [fn-order]
    only-base  send_order(boost::function<void __cdecl(vostok::network::packet const &)> const&, vostok::network::packet const&, vostok::memory::base_allocator&)
    only-tgt   send_order(boost::function<void __cdecl(vostok::network_core::tcp_packet const &)> const&, vostok::network_core::tcp_packet const&, vostok::memory::base_allocator&)

class vostok::network::string_response

class vostok::network::tcp_packet_client
  [member]
    changed  m_on_error: base(boost::function< void( network_core::client_error_codes_enum, boost::system::error_code ) > @0x60)  target(boost::function< void( enum network_core::client_error_codes_enum, boost::system::error_code ) > @0x60)
  [fn-order]
    only-base  void set_on_error(boost::function<void __cdecl(vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)
    only-tgt   void set_on_error(boost::function<void __cdecl(enum vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)

struct vostok::network::world
  [fn-order]
    only-base  void tick()
    only-tgt   void tick(bool)

class vostok::network_core::async_connector
  [member]
    changed  m_on_error: base(boost::function< void( client_error_codes_enum, boost::system::error_code ) > @0x30)  target(boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > @0x30)
  [fn-order]
    only-base  void connect(boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> >&, char const*, unsigned short, boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)
    only-tgt   void connect(boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> >&, char const*, unsigned short, boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(enum vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)

class vostok::network_core::base_packet
  [fn-order]
    only-base  base_packet(unsigned char*, unsigned int)
    only-tgt   base_packet(unsigned char* const, const unsigned int)
    moved      unsigned char* buffer()

class vostok::network_core::handler_allocator

class vostok::network_core::http_client

class vostok::network_core::move_to_list_predicate
  [fn-order]
    only-base  move_to_list_predicate(vostok::intrusive_list<vostok::network_core::udp_match_packet,vostok::network_core::udp_match_packet *,28,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&, char const*, unsigned int, unsigned int), bool operator()(vostok::network_core::udp_match_packet*) const
    only-tgt   move_to_list_predicate(vostok::intrusive_list<vostok::network_core::udp_match_packet,vostok::network_core::udp_match_packet *,28,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&, char const* const, const unsigned int, const unsigned int), bool operator()(vostok::network_core::udp_match_packet* const) const

class vostok::network_core::packet<vostok::network_core::tcp_packet>
  [fn-order]
    only-base  void append(bool), void append(unsigned char), void append(signed char), void append(unsigned short), void append(short), void append(unsigned int), void append(int), void append(unsigned long long), void append(long long), void append(float)
    only-tgt   void append(const bool), void append(const unsigned char), void append(const signed char), void append(const unsigned short), void append(const short), void append(const unsigned int), void append(const int), void append(const unsigned long long), void append(const long long), void append(const float)

class vostok::network_core::packet<vostok::network_core::udp_match_packet>
  [fn-order]
    only-base  void append(bool), void append(unsigned char), void append(signed char), void append(unsigned short), void append(short), void append(unsigned int), void append(int), void append(unsigned long long), void append(long long), void append(float)
    only-tgt   void append(const bool), void append(const unsigned char), void append(const signed char), void append(const unsigned short), void append(const short), void append(const unsigned int), void append(const int), void append(const unsigned long long), void append(const long long), void append(const float)

class vostok::network_core::packet_reader
  [fn-order]
    only-base  void r(void*, unsigned int, const unsigned int)
    only-tgt   void r(void*, unsigned int, unsigned int), vostok::network_core::packet_reader& operator=(vostok::network_core::packet_reader const&)

class vostok::network_core::process_packet_predicate
  [fn-order]
    only-base  void operator()(unsigned char, vostok::network_core::packet_reader&) const
    only-tgt   void operator()(const unsigned char, vostok::network_core::packet_reader&) const

class vostok::network_core::sequence_number<unsigned char>
  [fn-order]
    only-base  sequence_number<unsigned char>(), sequence_number<unsigned char>(unsigned char)
    only-tgt   sequence_number<unsigned char>(const unsigned char), sequence_number<unsigned char>(vostok::network_core::sequence_number<unsigned char> const&), vostok::network_core::sequence_number<unsigned char>& operator=(vostok::network_core::sequence_number<unsigned char> const&)
    moved      static vostok::network_core::sequence_number<unsigned char> deserialize(vostok::network_core::packet_reader&)

class vostok::network_core::sequence_number<unsigned short>
  [fn-order]
    only-base  sequence_number<unsigned short>(), sequence_number<unsigned short>(unsigned short)
    only-tgt   sequence_number<unsigned short>(const unsigned short), sequence_number<unsigned short>(vostok::network_core::sequence_number<unsigned short> const&), vostok::network_core::sequence_number<unsigned short>& operator=(vostok::network_core::sequence_number<unsigned short> const&)
    moved      static vostok::network_core::sequence_number<unsigned short> deserialize(vostok::network_core::packet_reader&)

class vostok::network_core::tcp_packet

class vostok::network_core::tcp_packet_client
  [member]
    changed  m_on_error: base(boost::function< void( client_error_codes_enum, boost::system::error_code ) > @0x960)  target(boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > @0x960)
  [fn-order]
    only-base  void set_on_error(boost::function<void __cdecl(vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)
    only-tgt   void set_on_error(boost::function<void __cdecl(enum vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)

class vostok::network_core::tcp_packet_socket<boost::asio::basic_stream_socket<boost::asio::ip::tcp,boost::asio::stream_socket_service<boost::asio::ip::tcp> > >
  [member]
    changed  m_on_error: base(boost::function< void( client_error_codes_enum, boost::system::error_code ) > @0x20)  target(boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > @0x20)
  [fn-order]
    only-base  void set_on_error(boost::function<void __cdecl(vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)
    only-tgt   void set_on_error(boost::function<void __cdecl(enum vostok::network_core::client_error_codes_enum,boost::system::error_code)> const&)

class vostok::network_core::udp_match_client
  [size]   base=0xB28  target=0xB20
  [member]
    changed  m_on_packet_received: base(boost::function< void( u8, packet_reader& ) > @0x538)  target(boost::function< void( u8, packet_reader& ) > @0x530)
    changed  m_on_disconnect: base(boost::function< void( disconnect_event_types_enum ) > @0x558)  target(boost::function< void( enum disconnect_event_types_enum ) > @0x550)
    changed  m_timer: base(timing::timer @0x578)  target(timing::timer @0x570)
    changed  m_socket: base(boost::asio::ip::udp::socket @0x590)  target(boost::asio::ip::udp::socket @0x588)
    changed  m_server_endpoint: base(boost::asio::ip::udp::endpoint @0x5D4)  target(boost::asio::ip::udp::endpoint @0x5CC)
    changed  m_remote_endpoint: base(boost::asio::ip::udp::endpoint @0x5F0)  target(boost::asio::ip::udp::endpoint @0x5E8)
    changed  m_io_service: base(boost::asio::io_service& @0x60C)  target(boost::asio::io_service& @0x604)
    changed  m_packets_allocator: base(memory::single_size_buffer_allocator< 300, threading::single_threading_policy >& @0x610)  target(memory::single_size_buffer_allocator< 300, threading::single_threading_policy >& @0x608)
    changed  m_network_flow_emulator: base(udp_network_flow_emulator* const @0x614)  target(udp_network_flow_emulator* const @0x60C)
    changed  m_time_in_ms: base(u32 @0x618)  target(u32 @0x610)
    changed  m_is_receiving: base(bool @0x61C)  target(bool @0x614)
    changed  m_receive_buffer: base(boost::array< u8, 256 > @0x61D)  target(boost::array< u8, 256 > @0x615)
    changed  m_handler_allocator: base(handler_allocator @0x720)  target(handler_allocator @0x718)
  [fn-order]
    only-base  void set_on_disconnect(boost::function<void __cdecl(vostok::network_core::disconnect_event_types_enum)> const&), vostok::network_core::udp_match_packet* new_packet(unsigned char), void handle_send(boost::system::error_code const&, unsigned int), void on_error(vostok::network_core::client_error_codes_enum, boost::system::error_code), void on_disconnect(vostok::network_core::disconnect_event_types_enum)
    only-tgt   void set_on_disconnect(boost::function<void __cdecl(enum vostok::network_core::disconnect_event_types_enum)> const&), vostok::network_core::udp_match_packet* new_packet(const unsigned char), void handle_send(boost::system::error_code const&, const unsigned int), void on_error(const vostok::network_core::client_error_codes_enum, const boost::system::error_code), void on_disconnect(const vostok::network_core::disconnect_event_types_enum)
    moved      static void construct_packet(vostok::network_core::udp_match_packets_orderer&, vostok::network_core::udp_match_packet&, unsigned char)

class vostok::network_core::udp_match_client_session
  [size]   base=0x570  target=0x568
  [member]
    changed  m_client_endpoint: base(boost::asio::ip::udp::endpoint @0x540)  target(boost::asio::ip::udp::endpoint @0x538)
    changed  set_member_hook: base(boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none > @0x55C)  target(boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none > @0x554)
    changed  next_in_destroy_list: base(udp_match_client_session* @0x56C)  target(udp_match_client_session* @0x564)
  [fn-order]
    only-base  vostok::network_core::udp_match_packet* new_packet(unsigned char), void on_error(vostok::network_core::client_error_codes_enum, boost::system::error_code)
    only-tgt   vostok::network_core::udp_match_packet* new_packet(const unsigned char), void on_error(const vostok::network_core::client_error_codes_enum, const boost::system::error_code)

class vostok::network_core::udp_match_connection
  [size]   base=0x538  target=0x530
  [member]
    changed  m_on_disconnect: base(boost::function< void( disconnect_event_types_enum ) > @0xC8)  target(boost::function< void( enum disconnect_event_types_enum ) > @0xC8)
    changed  m_local_sequence_id: base(sequence_number< u16 > @0x124)  target(sequence_number< u8 > @0x124)
    changed  m_remote_sequence_id: base(sequence_number< u16 > @0x126)  target(sequence_number< u8 > @0x125)
    changed  m_received_local_sequence_id: base(sequence_number< u16 > @0x128)  target(sequence_number< u8 > @0x126)
    changed  m_disconnection_local_sequence_id: base(sequence_number< u16 > @0x12A)  target(sequence_number< u8 > @0x127)
    changed  m_handler_allocator: base(handler_allocator @0x130)  target(handler_allocator @0x128)
  [fn-order]
    only-base  void set_on_disconnect(boost::function<void __cdecl(vostok::network_core::disconnect_event_types_enum)> const&), vostok::network_core::udp_match_packet* new_packet(unsigned char), void set_max_packet_wait_time_in_ms(unsigned int), void update_acknowledgements(vostok::network_core::sequence_number<unsigned short>, vostok::network_core::sequence_number<unsigned short>, unsigned short), void dump(char const* const, unsigned int), void process_low_level_message(vostok::network_core::packet_reader&, unsigned int)
    only-tgt   void set_on_disconnect(boost::function<void __cdecl(enum vostok::network_core::disconnect_event_types_enum)> const&), vostok::network_core::udp_match_packet* new_packet(const unsigned char), void set_max_packet_wait_time_in_ms(const unsigned int), void update_acknowledgements(vostok::network_core::sequence_number<unsigned char>, vostok::network_core::sequence_number<unsigned char>, unsigned short), void dump(char const* const, const unsigned int), void process_low_level_message(vostok::network_core::packet_reader&, const unsigned int)
    moved      static void construct_packet(vostok::network_core::udp_match_packets_orderer&, vostok::network_core::udp_match_packet&, unsigned char), static bool is_low_level_packet(vostok::network_core::base_packet const&)

struct vostok::network_core::udp_match_connection::comparer
  [fn-order]
    only-base  bool operator()(vostok::network_core::sequence_number<unsigned short>, vostok::network_core::udp_match_packet const&) const, bool operator()(vostok::network_core::udp_match_packet const&, vostok::network_core::sequence_number<unsigned short>) const
    only-tgt   bool operator()(const vostok::network_core::sequence_number<unsigned short>, vostok::network_core::udp_match_packet const&) const, bool operator()(vostok::network_core::udp_match_packet const&, const vostok::network_core::sequence_number<unsigned short>) const

struct vostok::network_core::udp_match_items_stats
  [fn-order]
    only-base  void operator/=(unsigned int)
    only-tgt   void operator/=(const unsigned int)

struct vostok::network_core::udp_match_message_type_info
  [fn-order]
    only-base  udp_match_message_type_info(bool, bool, unsigned char)
    only-tgt   udp_match_message_type_info(const bool, const bool, const unsigned char)

class vostok::network_core::udp_match_packet
  [fn-order]
    only-base  void reallocate(unsigned int), vostok::network_core::udp_match_packet& operator=(vostok::network_core::udp_match_packet const&)
    only-tgt   void reallocate(const unsigned int), void operator=(vostok::network_core::udp_match_packet const&)

class vostok::network_core::udp_match_packet::helper

class vostok::network_core::udp_match_server
  [member]
    changed  m_clients: base(boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, 1372 >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > @0x88)  target(boost::intrusive::set< udp_match_client_session, boost::intrusive::member_hook< udp_match_client_session, boost::intrusive::set_member_hook< boost::intrusive::none, boost::intrusive::none, boost::intrusive::none, boost::intrusive::none >, 1364 >, boost::intrusive::compare< udp_match_server::comparer >, boost::intrusive::none, boost::intrusive::none > @0x88)
  [fn-order]
    only-base  void handle_receive(boost::system::error_code const&, unsigned int), void on_error(vostok::network_core::client_error_codes_enum, boost::system::error_code)
    only-tgt   void handle_receive(boost::system::error_code const&, const unsigned int), void on_error(const vostok::network_core::client_error_codes_enum, const boost::system::error_code)

struct vostok::network_core::udp_match_server::destroy_predicate

struct vostok::network_core::udp_match_stats
  [fn-order]
    only-base  void dump(char const*) const, void operator/=(unsigned int)
    only-tgt   void dump(char const* const) const, void operator/=(const unsigned int)

struct vostok::network_core::udp_match_stream_stats
  [fn-order]
    only-base  void dump(char const*, char const*) const, void operator/=(unsigned int)
    only-tgt   void dump(char const* const, char const* const) const, void operator/=(const unsigned int)

class vostok::network_core::udp_network_flow_emulator

struct vostok::particle::engine
  [fn-order]
    only-base  vostok::particle::render_particle_emitter_instance* create_render_emitter_instance(vostok::particle::particle_emitter_instance&, vostok::intrusive_list<vostok::particle::base_particle,vostok::particle::base_particle *,104,vostok::threading::mutex,vostok::size_policy,vostok::debug_policy> const&, vostok::particle::billboard_parameters*, vostok::particle::beamtrail_parameters*, vostok::particle::enum_particle_locked_axis, vostok::particle::enum_particle_screen_alignment, vostok::math::float4x4 const&, vostok::math::float4 const&)
    only-tgt   vostok::particle::render_particle_emitter_instance* create_render_emitter_instance(vostok::particle::particle_emitter_instance&, vostok::intrusive_list<vostok::particle::base_particle,vostok::particle::base_particle *,128,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy> const&, vostok::particle::billboard_parameters*, vostok::particle::beamtrail_parameters*, vostok::particle::enum_particle_locked_axis, vostok::particle::enum_particle_screen_alignment, vostok::math::float4x4 const&, vostok::math::float4 const&), vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> get_scene(vostok::particle::world&)

class vostok::particle::particle_action_random_velocity
  [fn-order]
    only-tgt   vostok::particle::particle_domain_complex& domain()

class vostok::particle::particle_beam_emitter_instance
  [fn-order]
    only-base  particle_beam_emitter_instance(vostok::particle::particle_world&, vostok::particle::particle_emitter&, vostok::particle::engine&, bool), void create_render_particle_emitter_instance(vostok::particle::engine&)
    only-tgt   particle_beam_emitter_instance(vostok::particle::particle_emitter&, bool, bool), void create_render_particle_emitter_instance(vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const&, vostok::particle::engine&)

class vostok::particle::particle_emitter
  [fn-order]
    only-tgt   vostok::particle::particle_action_random_velocity* get_target_action() const

class vostok::particle::particle_emitter_instance
  [member]
    changed  m_engine: base(engine& @0xA0)  target(engine* @0xA4)
    changed  m_subuv_pos_uv: base(math::uint2 @0xA4)  target(math::uint2 @0xA8)
    changed  m_subuv_size_uv: base(float2 @0xAC)  target(float2 @0xB0)
    changed  m_material: base(resources::unmanaged_resource_ptr @0xB4)  target(resources::unmanaged_resource_ptr @0xB8)
    changed  m_particle_world: base(particle_world& @0xB8)  target(particle_world* @0xBC)
    changed  m_particle_system_instance_ptr: base(resources::unmanaged_resource_ptr @0xBC)  target(resources::unmanaged_resource_ptr @0xC0)
    changed  m_particle_system_instance: base(particle_system_instance_impl* @0xC0)  target(particle_system_instance_impl* @0xC4)
    changed  m_render_instance: base(render_particle_emitter_instance* @0xC4)  target(render_particle_emitter_instance* @0xC8)
    changed  m_data_type_action: base(particle_action_data_type* @0xC8)  target(particle_action_data_type* @0xCC)
    changed  m_cook_data_to_delete: base(render::material_effects_instance_cook_data* @0xCC)  target(render::material_effects_instance_cook_data* @0xD0)
    changed  m_emitter: base(particle_emitter& @0xD0)  target(particle_emitter& @0xD4)
    changed  m_billboard_parameters: base(billboard_parameters* @0xD4)  target(billboard_parameters* @0xD8)
    changed  m_beamtrail_parameters: base(beamtrail_parameters* @0xD8)  target(beamtrail_parameters* @0xDC)
    changed  m_next: base(particle_emitter_instance* @0xDC)  target(particle_emitter_instance* @0xE0)
    changed  m_num_live_particles: base(u32 @0xE0)  target(u32 @0xE4)
    changed  m_num_created_particles: base(u32 @0xE4)  target(u32 @0xE8)
    changed  m_delay_time: base(float @0xE8)  target(float @0xEC)
    changed  m_emitter_time: base(float @0xEC)  target(float @0xF0)
    changed  m_emitter_time_numerator: base(float @0xF0)  target(float @0xF4)
    changed  m_current_loop: base(u32 @0xF4)  target(u32 @0xF8)
    changed  m_time_to_create_new_one: base(float @0xF8)  target(float @0xFC)
    changed  m_num_particles_to_create: base(u32 @0xFC)  target(u32 @0x100)
    changed  m_current_max_num_particles: base(u32 @0x100)  target(u32 @0x104)
    changed  m_current_calc_num_max_particles: base(u32 @0x104)  target(u32 @0x108)
    changed  m_create_rate: base(float @0x108)  target(float @0x10C)
    changed  m_current_create_rate: base(float @0x10C)  target(float @0x110)
    changed  m_current_duration: base(float @0x110)  target(float @0x114)
    changed  m_subimage_index: base(float @0x114)  target(float @0x118)
    changed  m_max_num_particles: base(u32 @0x118)  target(u32 @0x11C)
    changed  m_is_child_emitter_instance: base(bool @0x11C)  target(bool @0x120)
    changed  m_waiting_for_end: base(bool @0x11D)  target(bool @0x121)
    changed  m_delayed: base(bool @0x11E)  target(bool @0x122)
    changed  m_visible: base(bool @0x11F)  target(bool @0x123)
    changed  m_particle_added: base(bool @0x120)  target(bool @0x124)
    changed  m_world_space: base(bool @0x121)  target(bool @0x125)
    only-tgt   m_scene
  [fn-order]
    only-base  particle_emitter_instance(vostok::particle::particle_world&, vostok::particle::particle_emitter&, vostok::particle::engine&, bool), void create_render_particle_emitter_instance(vostok::particle::engine&)
    only-tgt   particle_emitter_instance(vostok::particle::particle_emitter&, bool, bool), void create_render_particle_emitter_instance(vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base> const&, vostok::particle::engine&), void reset(), vostok::render::enum_vertex_input_type get_vertex_input_type() const

class vostok::particle::particle_system_instance
  [member]
    changed  m_particle_world: base(particle_world& @0x248)  target(particle_world* @0x248)
  [fn-order]
    only-base  particle_system_instance(vostok::particle::particle_world&), void play(), void play(vostok::math::float4x4 const&)
    only-tgt   particle_system_instance(), void play(vostok::particle::particle_world*), void play(vostok::particle::particle_world*, vostok::math::float4x4 const&)

class vostok::particle::particle_system_instance_cook
  [fn-order]
    only-base  void on_sub_resources_loaded(vostok::resources::queries_result&, vostok::particle::world*)
    only-tgt   void on_materials_loaded(vostok::resources::queries_result&, vostok::particle::particle_system_instance_impl*, vostok::particle::material_query_data*, vostok::render::material_effects_instance_cook_data*), void on_sub_resources_loaded(vostok::resources::queries_result&)

class vostok::particle::particle_system_instance_impl
  [size]   base=0x2B8  target=0x2C0
  [member]
    changed  m_next: base(particle_system_instance_impl* @0x290)  target(particle_system_instance_impl_ptr @0x290)
    changed  m_current_lod: base(u32 @0x294)  target(u32 @0x298)
    changed  m_old_lod: base(u32 @0x298)  target(u32 @0x29C)
    changed  m_num_lods: base(u32 @0x29C)  target(u32 @0x2A0)
    changed  m_particle_system_time: base(float @0x2A0)  target(float @0x2A8)
    changed  m_lods_lerp_alpha: base(float @0x2A4)  target(float @0x2AC)
    changed  m_lerped: base(bool @0x2A8)  target(bool @0x2B0)
    changed  m_use_lods: base(bool @0x2A9)  target(bool @0x2B1)
    changed  m_always_looping: base(bool @0x2AA)  target(bool @0x2B2)
    changed  m_child_played: base(bool @0x2AB)  target(bool @0x2B3)
    changed  m_pinned: base(bool @0x2B0)  target(bool @0x2B4)
    changed  m_is_playing: base(bool @0x2B1)  target(volatile long @0x2A4)
    changed  m_no_more_create: base(bool @0x2B2)  target(bool @0x2B5)
    changed  m_paused: base(bool @0x2B3)  target(bool @0x2B6)
    changed  m_visible: base(bool @0x2B4)  target(bool @0x2B7)
    changed  m_ticked: base(bool @0x2B5)  target(bool @0x2B8)
    only-base  self_ptr
    only-tgt   m_scene
    reordered  m_is_playing
  [fn-order]
    only-base  particle_system_instance_impl(vostok::particle::particle_world&), void play_impl(), void play_impl(vostok::math::float4x4 const&)
    only-tgt   particle_system_instance_impl(), void prepare_render_resources(), bool is_playing() const, void play_impl(vostok::particle::particle_world*), void play_impl(vostok::particle::particle_world*, vostok::math::float4x4 const&), void reset()

struct vostok::particle::particle_system_lod
  [member]
    changed  m_emitters_array: base(platform_pointer_selector< particle_emitter, 1 >::helper @0x0)  target(platform_pointer_selector< particle_emitter, 1 >::helper @0x8)
    changed  m_num_emitters: base(u32 @0x8)  target(u32 @0x10)
    changed  m_visibility: base(bool @0xC)  target(bool @0x18)
    changed  m_total_length: base(float @0x10)  target(float @0x14)
    changed  m_parent: base(platform_pointer_selector< particle_system, 1 >::helper @0x18)  target(platform_pointer_selector< particle_system, 1 >::helper @0x0)
    reordered  m_visibility, m_parent

class vostok::particle::particle_world
  [fn-order]
    only-base  vostok::particle::particle_emitter_instance* create_emitter_instance(vostok::particle::particle_emitter&, bool)
    only-tgt   static vostok::particle::particle_emitter_instance* create_emitter_instance(vostok::particle::particle_emitter&, bool, bool)

class vostok::physics::animated_model_instance_cook

class vostok::physics::base_physics_object

class vostok::physics::btBvhTriangleMeshShapeResource

class vostok::physics::bt_animated_rigid_body
  [fn-order]
    only-base  unsigned short get_triangle_material(int, bool) const, void update_bone_matrix(unsigned int, vostok::math::float4x4 const&, bool), vostok::math::float4x4 get_bone_transform(unsigned int) const
    only-tgt   unsigned short get_triangle_material(const int, const bool) const, void update_bone_matrix(const unsigned int, vostok::math::float4x4 const&, bool), vostok::math::float4x4 get_bone_transform(const unsigned int) const
    moved      void ~bt_animated_rigid_body()

class vostok::physics::bt_character_controller
  [fn-order]
    only-base  void update_action(unsigned int)
    only-tgt   void update_action(const unsigned int)
    moved      bool adjust_foot_transform(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3&, vostok::math::float3&) const

class vostok::physics::bt_collision_shape
  [fn-order]
    only-base  unsigned short get_triangle_material(int, bool)
    only-tgt   unsigned short get_triangle_material(const int, const bool)

class vostok::physics::bt_constraint

class vostok::physics::bt_ghost_object
  [fn-order]
    moved      bool contact_test(vostok::physics::world*)

class vostok::physics::bt_rigid_body_base
  [fn-order]
    only-base  unsigned short get_triangle_material(int, bool) const
    only-tgt   unsigned short get_triangle_material(const int, const bool) const

class vostok::physics::bt_soft_body_rope

class vostok::physics::bt_static_rigid_body
  [fn-order]
    only-base  unsigned short get_triangle_material(int, bool) const, vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base> get_collision_shape() const
    only-tgt   unsigned short get_triangle_material(const int, const bool) const, const vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base> get_collision_shape() const

class vostok::physics::character_move_test_callback
  [member]
    changed  m_up_vector: base(btVector3 @0x60)  target(const btVector3 @0x60)
    changed  m_minSlopeDot: base(float @0x74)  target(const float @0x74)
  [fn-order]
    only-base  character_move_test_callback(btCollisionObject*, btVector3 const&, float)
    only-tgt   character_move_test_callback(btCollisionObject*, btVector3 const&, const float)

class vostok::physics::collision_shape_cook

struct vostok::physics::contact_result_callback
  [fn-order]
    only-base  contact_result_callback(vostok::physics::contact_test_predicate*)
    only-tgt   contact_result_callback(vostok::physics::contact_test_predicate&)

struct vostok::physics::contact_test_predicate
  [fn-order]
    only-base  float add_single_result(void*, vostok::collision::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::collision::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&)
    only-tgt   float add_single_result(void*, vostok::physics::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::physics::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&)

struct vostok::physics::distance_predicate

struct vostok::physics::world
  [fn-order]
    only-base  void tick(unsigned int), vostok::physics::closest_ray_result ray_test(vostok::math::float3 const&, vostok::math::float3 const&, float, unsigned short, unsigned short), void ray_query(vostok::math::float3 const&, vostok::math::float3 const&, float, vostok::vectora<vostok::physics::closest_ray_result>&, unsigned short, unsigned short)
    only-tgt   void tick(const unsigned int), vostok::physics::closest_ray_result ray_test(vostok::math::float3 const&, vostok::math::float3 const&, const float, unsigned short, unsigned short), void ray_query(vostok::math::float3 const&, vostok::math::float3 const&, const float, vostok::vectora<vostok::physics::closest_ray_result>&, unsigned short, unsigned short)

class vostok::resources::base_of_intrusive_base
  [member]
    changed  m_flags: base(flags_type< base_of_intrusive_base::flag_pinned_by_enum, threading::multi_threading_policy > @0x4)  target(flags_type< enum base_of_intrusive_base::flag_pinned_by_enum, threading::multi_threading_policy > @0x4)

class vostok::resources::cook_base
  [member]
    changed  m_flags: base(flags_type< cook_base::flags_enum, threading::single_threading_policy > @0x18)  target(flags_type< enum cook_base::flags_enum, threading::single_threading_policy > @0x18)
  [fn-order]
    only-base  cook_base(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, vostok::enum_flags<vostok::resources::cook_base::flags_enum>, unsigned int, unsigned int)
    only-tgt   cook_base(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>, unsigned int, unsigned int)

class vostok::resources::device_manager
  [fn-order]
    only-base  void grab_sorted_queries(vostok::intrusive_list<vostok::resources::query_result,vostok::resources::query_result *,608,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&)
    only-tgt   void grab_sorted_queries(vostok::intrusive_list<vostok::resources::query_result,vostok::resources::query_result *,608,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&)

struct vostok::resources::game_resources_manager_data
  [member]
    changed  flags: base(flags_type< game_resources_manager_data::flags_enum, threading::simple_lock > @0x0)  target(flags_type< enum game_resources_manager_data::flags_enum, threading::simple_lock > @0x0)

class vostok::resources::game_resources_manager_tester
  [fn-order]
    only-tgt   void test(vostok::core_test_suite*)

class vostok::resources::hdd_manager
  [fn-order]
    only-base  void grab_sorted_queries(vostok::intrusive_list<vostok::resources::query_result,vostok::resources::query_result *,608,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&)
    only-tgt   void grab_sorted_queries(vostok::intrusive_list<vostok::resources::query_result,vostok::resources::query_result *,608,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&)

class vostok::resources::inplace_managed_cook
  [fn-order]
    only-base  inplace_managed_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, vostok::enum_flags<vostok::resources::cook_base::flags_enum>)
    only-tgt   inplace_managed_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>)

class vostok::resources::inplace_unmanaged_cook
  [fn-order]
    only-base  inplace_unmanaged_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, unsigned int, vostok::enum_flags<vostok::resources::cook_base::flags_enum>)
    only-tgt   inplace_unmanaged_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, unsigned int, vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>)

class vostok::resources::managed_cook
  [fn-order]
    only-base  managed_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, vostok::enum_flags<vostok::resources::cook_base::flags_enum>)
    only-tgt   managed_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>)

class vostok::resources::quality_decreasing_functionality
  [fn-order]
    only-base  vostok::resources::decrease_is_possible_bool collect_quality_resources(vostok::resources::resource_base*, vostok::intrusive_list<vostok::resources::resource_base,vostok::resources::resource_base *,184,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>*)
    only-tgt   vostok::resources::decrease_is_possible_bool collect_quality_resources(vostok::resources::resource_base*, vostok::intrusive_list<vostok::resources::resource_base,vostok::resources::resource_base *,184,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>*)

class vostok::resources::quality_increase_functionality
  [fn-order]
    only-base  void select_to_increase_quality(vostok::intrusive_list<vostok::resources::resource_quality,vostok::resources::resource_base *,104,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>*), void select_resource_to_increase_quality(vostok::resources::resource_base*, vostok::intrusive_list<vostok::resources::resource_quality,vostok::resources::resource_base *,104,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>*), void schedule_to_increase_quality(vostok::intrusive_list<vostok::resources::resource_quality,vostok::resources::resource_base *,104,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&)
    only-tgt   void select_to_increase_quality(vostok::intrusive_list<vostok::resources::resource_quality,vostok::resources::resource_base *,104,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>*), void select_resource_to_increase_quality(vostok::resources::resource_base*, vostok::intrusive_list<vostok::resources::resource_quality,vostok::resources::resource_base *,104,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>*), void schedule_to_increase_quality(vostok::intrusive_list<vostok::resources::resource_quality,vostok::resources::resource_base *,104,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&)

class vostok::resources::queries_result

class vostok::resources::resource_children
  [fn-order]
    only-base  vostok::intrusive_list<vostok::resources::resource_link,vostok::resources::resource_link *,4,vostok::threading::simple_lock,vostok::size_policy,vostok::debug_policy>& get_parents(), vostok::intrusive_list<vostok::resources::resource_link,vostok::resources::resource_link *,4,vostok::threading::simple_lock,vostok::size_policy,vostok::debug_policy> const& get_parents() const, vostok::intrusive_list<vostok::resources::resource_link,vostok::resources::resource_link *,4,vostok::threading::simple_lock,vostok::size_policy,vostok::debug_policy>& get_children()
    only-tgt   vostok::intrusive_list<vostok::resources::resource_link,vostok::resources::resource_link *,4,vostok::threading::simple_lock,vostok::size_policy,vostok::no_debug_policy>& get_parents(), vostok::intrusive_list<vostok::resources::resource_link,vostok::resources::resource_link *,4,vostok::threading::simple_lock,vostok::size_policy,vostok::no_debug_policy> const& get_parents() const, vostok::intrusive_list<vostok::resources::resource_link,vostok::resources::resource_link *,4,vostok::threading::simple_lock,vostok::size_policy,vostok::no_debug_policy>& get_children()

class vostok::resources::resource_flags
  [member]
    changed  m_flags: base(flags_type< resource_flags_enum, threading::simple_lock > @0x8)  target(flags_type< enum resource_flags_enum, threading::simple_lock > @0x8)

class vostok::resources::resources_manager
  [size]   base=0x41010  target=0x407F0
  [member]
    changed  m_resources_to_create: base(query_result_list @0x20C08)  target(query_result_list @0x203E8)
    changed  m_dispatching_created_resources: base(volatile long @0x20C38)  target(volatile long @0x20418)
    changed  m_created_resources: base(query_result_list @0x20C40)  target(query_result_list @0x20420)
    changed  m_generated_resources_to_save_list: base(query_result_list @0x20C70)  target(query_result_list @0x20450)
    changed  m_resources_to_decompress: base(query_result_list @0x20CA0)  target(query_result_list @0x20480)
    changed  m_decompressed_resources: base(query_result_list @0x20CD0)  target(query_result_list @0x204B0)
    changed  m_mount_and_fs_iterator_dispatch_mutex: base(threading::mutex @0x20D00)  target(threading::mutex @0x204E0)
    changed  m_mount_and_fs_iterator_dispatch_mutex_hold_thread_id: base(u32 @0x20D18)  target(u32 @0x204F8)
    changed  m_transaction_mutex: base(threading::mutex @0x20D20)  target(threading::mutex @0x20500)
    changed  m_fs_tasks: base(fs_task_list @0x20D38)  target(fs_task_list @0x20518)
    changed  m_fs_sub_tasks: base(fs_task_list @0x20D68)  target(fs_task_list @0x20548)
    changed  m_compressor: base(uninitialized_reference< ppmd_compressor > @0x20D98)  target(uninitialized_reference< ppmd_compressor > @0x20578)
    changed  m_wait_and_dispatch_callbacks_mutex: base(threading::mutex @0x20DB8)  target(threading::mutex @0x20598)
    changed  m_query_finished_callback: base(boost::function< void( resource_base* ) > @0x20DD0)  target(boost::function< void( resource_base* ) > @0x205B0)
    changed  m_self_wakeup_timer: base(timing::timer @0x20DF0)  target(timing::timer @0x205D0)
    changed  m_self_wakeuping: base(bool @0x20E08)  target(bool @0x205E8)
    changed  m_sync_device: base(fs_new::synchronous_device_interface @0x20E0C)  target(fs_new::synchronous_device_interface @0x205EC)
    changed  m_hdd: base(fs_new::asynchronous_device_interface& @0x20E18)  target(fs_new::asynchronous_device_interface& @0x205F8)
    changed  m_dvd: base(fs_new::asynchronous_device_interface& @0x20E1C)  target(fs_new::asynchronous_device_interface& @0x205FC)
    changed  m_vfs: base(vfs::virtual_file_system @0x20E20)  target(vfs::virtual_file_system @0x20600)
    changed  m_mounts_ptr: base(fs_task_unmount_ptr @0x41008)  target(fs_task_unmount_ptr @0x407E8)
    changed  m_mounts_converted_ptr: base(fs_task_unmount_ptr @0x4100C)  target(fs_task_unmount_ptr @0x407EC)
    only-base  m_cooks_registry
  [fn-order]
    only-base  void register_cook(vostok::resources::cook_base*), vostok::resources::cook_base* unregister_cook(vostok::resources::class_id_enum), vostok::resources::cook_base* find_cook(vostok::resources::class_id_enum)
    only-tgt   static void register_cook(vostok::resources::cook_base*), static vostok::resources::cook_base* unregister_cook(vostok::resources::class_id_enum), static vostok::resources::cook_base* find_cook(vostok::resources::class_id_enum)

struct vostok::resources::sorting_predicate
  [fn-order]
    only-base  bool operator()(vostok::resources::resource_base const*, vostok::resources::resource_base const*)
    only-tgt   bool operator()(vostok::resources::query_result*, vostok::resources::query_result*) const

class vostok::resources::translate_query_cook
  [fn-order]
    only-base  translate_query_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, vostok::enum_flags<vostok::resources::cook_base::flags_enum>)
    only-tgt   translate_query_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>)

class vostok::resources::unmanaged_cook
  [fn-order]
    only-base  unmanaged_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, unsigned int, vostok::enum_flags<vostok::resources::cook_base::flags_enum>)
    only-tgt   unmanaged_cook(vostok::resources::class_id_enum, vostok::resources::cook_base::reuse_enum, unsigned int, unsigned int, vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>)

class vostok::resources::unmanaged_resource
  [member]
    changed  m_flags: base(flags_type< unmanaged_resource::flag_enum, threading::single_threading_policy > @0x100)  target(flags_type< enum unmanaged_resource::flag_enum, threading::single_threading_policy > @0x100)

class vostok::strings::text_tree_item
  [fn-order]
    only-base  vostok::intrusive_list<vostok::strings::text_tree_column_item,vostok::strings::text_tree_column_item *,0,vostok::threading::mutex,vostok::size_policy,vostok::debug_policy> const& get_columns(), vostok::intrusive_list<vostok::strings::text_tree_item_base,vostok::strings::text_tree_item *,0,vostok::threading::mutex,vostok::size_policy,vostok::debug_policy> const& get_sub_items()
    only-tgt   vostok::intrusive_list<vostok::strings::text_tree_column_item,vostok::strings::text_tree_column_item *,0,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy> const& get_columns(), vostok::intrusive_list<vostok::strings::text_tree_item_base,vostok::strings::text_tree_item *,0,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy> const& get_sub_items()

class vostok::tasks::task_allocator
  [size]   base=0x56F9A40  target=0x60040
  [member]
    changed  m_task_buffer: base(u8[91200000] @0x0)  target(u8[393216] @0x0)
    changed  m_free_list: base(task_allocator::pointer_and_counter @0x56F9A00)  target(task_allocator::pointer_and_counter @0x60000)

class vostok::tasks::task_manager
  [size]   base=0x56F9B80  target=0x60180
  [member]
    changed  m_cache_line_pad_$93: base(char[64] @0x56F9B00)  target(char[64] @0x60100)
    changed  m_current_thread_task_tls_key: base(u32 @0x56F9B40)  target(u32 @0x60140)
    changed  m_collecting_garbage: base(volatile long @0x56F9B44)  target(volatile long @0x60144)

class vostok::timing::timer
  [fn-order]
    only-base  void set_time_factor(float)
    only-tgt   void set_time_factor(const float)

class vostok::ui::ui_window
  [member]
    only-base  m_childs
    only-tgt   m_children
  [fn-order]
    only-base  void remove_all_childs()
    only-tgt   void remove_all_children()

struct vostok::ui::window
  [fn-order]
    only-base  void remove_all_childs()
    only-tgt   void remove_all_children()

class vostok::uninitialized_reference<singletons_on_initialize>
  [size]   base=0x1C80  target=0x42F8
  [member]
    changed  m_static_memory: base(char[7280] @0x0)  target(char[17132] @0x0)
    changed  m_variable: base(singletons_on_initialize& @0x1C70)  target(singletons_on_initialize& @0x42EC)
    changed  m_initialized: base(volatile long @0x1C74)  target(volatile long @0x42F0)
    changed  m_construction_started: base(volatile long @0x1C78)  target(volatile long @0x42F4)

class vostok::uninitialized_reference<singletons_on_preinitialize>
  [size]   base=0x11980  target=0x12020
  [member]
    changed  m_static_memory: base(char[72048] @0x0)  target(char[73744] @0x0)
    changed  m_variable: base(singletons_on_preinitialize& @0x11970)  target(singletons_on_preinitialize& @0x12010)
    changed  m_initialized: base(volatile long @0x11974)  target(volatile long @0x12014)
    changed  m_construction_started: base(volatile long @0x11978)  target(volatile long @0x12018)

class vostok::uninitialized_reference<survarium::game>
  [size]   base=0xB8  target=0x888
  [member]
    changed  m_static_memory: base(char[168] @0x0)  target(char[2168] @0x0)
    changed  m_variable: base(survarium::game& @0xA8)  target(survarium::game& @0x878)
    changed  m_initialized: base(volatile long @0xAC)  target(volatile long @0x87C)
    changed  m_construction_started: base(volatile long @0xB0)  target(volatile long @0x880)

class vostok::uninitialized_reference<vostok::engine::engine_world>
  [size]   base=0x2D8  target=0x2F8
  [member]
    changed  m_static_memory: base(char[712] @0x0)  target(char[744] @0x0)
    changed  m_variable: base(engine::engine_world& @0x2C8)  target(engine::engine_world& @0x2E8)
    changed  m_initialized: base(volatile long @0x2CC)  target(volatile long @0x2EC)
    changed  m_construction_started: base(volatile long @0x2D0)  target(volatile long @0x2F0)

class vostok::uninitialized_reference<vostok::network::network_world>
  [size]   base=0x138  target=0x130
  [member]
    changed  m_static_memory: base(char[300] @0x0)  target(char[292] @0x0)
    changed  m_variable: base(network::network_world& @0x12C)  target(network::network_world& @0x124)
    changed  m_initialized: base(volatile long @0x130)  target(volatile long @0x128)
    changed  m_construction_started: base(volatile long @0x134)  target(volatile long @0x12C)

class vostok::uninitialized_reference<vostok::resources::resources_manager>
  [size]   base=0x41020  target=0x40800
  [member]
    changed  m_static_memory: base(char[266256] @0x0)  target(char[264176] @0x0)
    changed  m_variable: base(resources::resources_manager& @0x41010)  target(resources::resources_manager& @0x407F0)
    changed  m_initialized: base(volatile long @0x41014)  target(volatile long @0x407F4)
    changed  m_construction_started: base(volatile long @0x41018)  target(volatile long @0x407F8)

class vostok::vfs::archive_folder_mount_root_node<1>
  [member]
    changed  nodes_buffer: base(platform_pointer_selector< char const, 1 >::helper @0x398)  target(platform_pointer_selector< char const , 1 >::helper @0x398)

class vostok::vfs::archive_inline_file_node_base<1>
  [member]
    changed  m_inlined_data: base(platform_pointer_selector< char const, 1 >::helper @0x0)  target(platform_pointer_selector< char const , 1 >::helper @0x0)

class vostok::vfs::archive_saver
  [fn-order]
    only-tgt   void print(vostok::vfs::base_node<1> const*, vostok::vfs::fat_node_info const*)

struct vostok::vfs::async_link_helper
  [member]
    changed  original_callback: base(boost::function< void( vfs_locked_iterator const&, result_enum ) > @0x0)  target(boost::function< void( vfs_locked_iterator const&, enum result_enum ) > @0x0)
  [fn-order]
    only-base  async_link_helper(boost::function<void __cdecl(vostok::vfs::vfs_locked_iterator const &,vostok::vfs::result_enum)>, vostok::vfs::base_node<1>*, unsigned int, vostok::vfs::find_enum, vostok::vfs::vfs_hashset*, vostok::memory::base_allocator*)
    only-tgt   async_link_helper(boost::function<void __cdecl(vostok::vfs::vfs_locked_iterator const &,enum vostok::vfs::result_enum)>, vostok::vfs::base_node<1>*, unsigned int, vostok::vfs::find_enum, vostok::vfs::vfs_hashset*, vostok::memory::base_allocator*)

class vostok::vfs::base_node<0>
  [fn-order]
    only-base  void set_flags(vostok::enum_flags<vostok::vfs::vfs_node_enum>), void add_flags(vostok::enum_flags<vostok::vfs::vfs_node_enum>), void unset_flags(vostok::enum_flags<vostok::vfs::vfs_node_enum>)
    only-tgt   void set_flags(vostok::enum_flags<enum vostok::vfs::vfs_node_enum>), void add_flags(vostok::enum_flags<enum vostok::vfs::vfs_node_enum>), void unset_flags(vostok::enum_flags<enum vostok::vfs::vfs_node_enum>), unsigned int get_expected_node_size()

class vostok::vfs::base_node<1>
  [fn-order]
    only-base  void set_flags(vostok::enum_flags<vostok::vfs::vfs_node_enum>), void add_flags(vostok::enum_flags<vostok::vfs::vfs_node_enum>), void unset_flags(vostok::enum_flags<vostok::vfs::vfs_node_enum>)
    only-tgt   void set_flags(vostok::enum_flags<enum vostok::vfs::vfs_node_enum>), void add_flags(vostok::enum_flags<enum vostok::vfs::vfs_node_enum>), void unset_flags(vostok::enum_flags<enum vostok::vfs::vfs_node_enum>), unsigned int get_expected_node_size()

class vostok::vfs::compare_nodes
  [fn-order]
    only-base  bool equal_file_nodes(vostok::vfs::base_node<1>* const, vostok::vfs::base_node<1>* const), void read_file_data(vostok::allocated_buffer* const, vostok::vfs::base_node<1>* const)
    only-tgt   bool equal_file_nodes(vostok::vfs::fat_node_info* const, vostok::vfs::fat_node_info* const), void read_file_data(vostok::allocated_buffer* const, vostok::vfs::base_node<1>* const, vostok::vfs::fat_node_info* const)

struct vostok::vfs::find_environment
  [member]
    changed  callback: base(boost::function< void( vfs_locked_iterator const&, result_enum ) > @0x18)  target(boost::function< void( vfs_locked_iterator const&, enum result_enum ) > @0x18)
    changed  find_flags: base(enum_flags< find_enum > @0x40)  target(enum_flags< enum find_enum > @0x40)

struct vostok::vfs::find_struct
  [member]
    changed  callback: base(boost::function< void( vfs_locked_iterator const&, result_enum ) > @0x8)  target(boost::function< void( vfs_locked_iterator const&, enum result_enum ) > @0x8)

class vostok::vfs::mount_root_node_base<1>
  [member]
    changed  virtual_path: base(platform_pointer_selector< char const, 1 >::helper @0x20)  target(platform_pointer_selector< char const , 1 >::helper @0x20)
    changed  physical_path: base(platform_pointer_selector< char const, 1 >::helper @0x28)  target(platform_pointer_selector< char const , 1 >::helper @0x28)
    changed  descriptor: base(platform_pointer_selector< char const, 1 >::helper @0x30)  target(platform_pointer_selector< char const , 1 >::helper @0x30)

struct vostok::vfs::pack_archive_args
  [fn-order]
    only-base  pack_archive_args(vostok::fs_new::synchronous_device_interface&, vostok::logging::log_format* const, const vostok::core::log_flags_enum)
    only-tgt   pack_archive_args(vostok::fs_new::synchronous_device_interface&, vostok::buffer_vector<stlp_std::pair<vostok::vfs::mount_root_node_base<1> *,stlp_std::pair<void * *,void * *> > > const*, vostok::logging::log_format* const, const vostok::core::log_flags_enum)

struct vostok::vfs::patch_args
  [size]   base=0x6BC  target=0x6C0
  [member]
    changed  from_sources: base(fs_new::native_path_string @0x494)  target(fs_new::native_path_string @0x498)
    changed  to_sources: base(fs_new::native_path_string @0x5A8)  target(fs_new::native_path_string @0x5AC)
  [fn-order]
    only-base  patch_args(vostok::fs_new::synchronous_device_interface&, vostok::logging::log_format* const, const vostok::core::log_flags_enum)
    only-tgt   patch_args(vostok::fs_new::synchronous_device_interface&, vostok::buffer_vector<stlp_std::pair<vostok::vfs::mount_root_node_base<1> *,stlp_std::pair<void * *,void * *> > > const*, vostok::logging::log_format* const, const vostok::core::log_flags_enum)

class vostok::vfs::physical_file_node<0>
  [member]
    changed  m_file_flags: base(flags_type< physical_file_node< 0 >::flags_enum, threading::multi_threading_policy > @0x4)  target(flags_type< enum physical_file_node< 0 >::flags_enum, threading::multi_threading_policy > @0x4)

class vostok::vfs::physical_file_node<1>
  [member]
    changed  m_file_flags: base(flags_type< physical_file_node< 1 >::flags_enum, threading::multi_threading_policy > @0x4)  target(flags_type< enum physical_file_node< 1 >::flags_enum, threading::multi_threading_policy > @0x4)

class vostok::vfs::physical_folder_node<0>
  [member]
    changed  m_folder_flags: base(flags_type< physical_folder_node< 0 >::flags_enum, threading::multi_threading_policy > @0x8)  target(flags_type< enum physical_folder_node< 0 >::flags_enum, threading::multi_threading_policy > @0x8)

class vostok::vfs::physical_folder_node<1>
  [member]
    changed  m_folder_flags: base(flags_type< physical_folder_node< 1 >::flags_enum, threading::multi_threading_policy > @0x8)  target(flags_type< enum physical_folder_node< 1 >::flags_enum, threading::multi_threading_policy > @0x8)

class vostok::vfs::physical_path_mounter
  [fn-order]
    only-base  void flatten_helper_nodes_and_merge(vostok::intrusive_list<vostok::vfs::base_node<1>,vostok::platform_pointer_selector<vostok::vfs::base_node<1>,1>::helper,24,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&, vostok::fs_new::virtual_path_string&, vostok::vfs::physical_folder_node<1>*, unsigned int, unsigned int, vostok::fs_new::native_path_string&), void free_node_list(vostok::intrusive_list<vostok::vfs::base_node<1>,vostok::platform_pointer_selector<vostok::vfs::base_node<1>,1>::helper,24,vostok::threading::single_threading_policy,vostok::size_policy,vostok::debug_policy>&)
    only-tgt   void flatten_helper_nodes_and_merge(vostok::intrusive_list<vostok::vfs::base_node<1>,vostok::platform_pointer_selector<vostok::vfs::base_node<1>,1>::helper,24,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&, vostok::fs_new::virtual_path_string&, vostok::vfs::physical_folder_node<1>*, unsigned int, unsigned int, vostok::fs_new::native_path_string&), void free_node_list(vostok::intrusive_list<vostok::vfs::base_node<1>,vostok::platform_pointer_selector<vostok::vfs::base_node<1>,1>::helper,24,vostok::threading::single_threading_policy,vostok::size_policy,vostok::no_debug_policy>&)

struct vostok::vfs::save_archive_args
  [fn-order]
    only-base  save_archive_args(vostok::fs_new::synchronous_device_interface&, vostok::fs_new::native_path_string const&, vostok::fs_new::native_path_string const&, vostok::vfs::save_flags_enum, unsigned int, vostok::memory::base_allocator*, vostok::compressor*, float, vostok::vfs::archive_platform_enum, vostok::vfs::fat_inline_data&, unsigned int, unsigned int, boost::function<void __cdecl(unsigned int,unsigned int,char const *,unsigned int)>, vostok::logging::log_format*, vostok::core::log_flags_enum, vostok::vfs::virtual_file_system*, vostok::vfs::fat_node_info*, vostok::vfs::virtual_file_system*, vostok::vfs::fat_node_info*)
    only-tgt   save_archive_args(vostok::fs_new::synchronous_device_interface&, vostok::buffer_vector<stlp_std::pair<vostok::vfs::mount_root_node_base<1> *,stlp_std::pair<void * *,void * *> > > const*, vostok::fs_new::native_path_string const&, vostok::fs_new::native_path_string const&, vostok::vfs::save_flags_enum, unsigned int, vostok::memory::base_allocator*, vostok::compressor*, float, vostok::vfs::archive_platform_enum, vostok::vfs::fat_inline_data&, unsigned int, unsigned int, boost::function<void __cdecl(unsigned int,unsigned int,char const *,unsigned int)>, vostok::logging::log_format*, vostok::core::log_flags_enum, vostok::vfs::virtual_file_system*, vostok::vfs::fat_node_info*, vostok::vfs::virtual_file_system*, vostok::vfs::fat_node_info*)

struct vostok::vfs::saving_environment
  [member]
    changed  node_info_set: base(hash_multiset< fat_node_info, fat_node_info*, 32, detail::fixed_size_policy< 32768 >, detail::null_hash< fat_node_info >, detail::null_equal< fat_node_info >, threading::single_threading_policy >* @0x128)  target(hash_multiset< fat_node_info, fat_node_info*, 20, detail::fixed_size_policy< 32768 >, detail::null_hash< fat_node_info >, detail::null_equal< fat_node_info >, threading::single_threading_policy >* @0x128)

class vostok::vfs::vfs_iterator
  [fn-order]
    only-base  unsigned int get_nodes_count() const
    only-tgt   unsigned int get_nodes_count(bool, bool) const

class vostok::vfs::virtual_file_system
  [fn-order]
    only-base  void try_find_async(vostok::fs_new::virtual_path_string const&, boost::function<void __cdecl(vostok::vfs::vfs_locked_iterator const &,vostok::vfs::result_enum)>, vostok::vfs::find_enum, vostok::memory::base_allocator*)
    only-tgt   bool strip(vostok::vfs::base_node<1>*), bool strip(bool), void print(bool), void print(vostok::vfs::base_node<1>* const, char const*), void check_consistency(bool), void check_consistency(vostok::vfs::base_node<1>* const), bool convert_links_to_duplicates(vostok::memory::base_allocator&, const bool), bool convert_links_to_duplicates(vostok::memory::base_allocator&, vostok::vfs::base_node<1>* const), void try_find_async(vostok::fs_new::virtual_path_string const&, boost::function<void __cdecl(vostok::vfs::vfs_locked_iterator const &,enum vostok::vfs::result_enum)>, vostok::vfs::find_enum, vostok::memory::base_allocator*), vostok::vfs::base_node<1>* clone(vostok::memory::base_allocator&, vostok::vfs::base_node<1>* const, vostok::vfs::base_folder_node<1>* const, char const*), vostok::vfs::base_node<1>* clone_recursive(vostok::memory::base_allocator&, vostok::vfs::base_node<1>* const, vostok::vfs::base_folder_node<1>* const)

enum lobby::query_info_types
  [values]
    misname  0x2  base=query_profile_info  target=q_profile_contents
    misname  0x6  base=query_prices_info  target=q_price_items
    only-tgt   q_client_state = 0x0
    only-tgt   q_enumerate_profiles = 0x1
    only-tgt   q_enumerate_inventory = 0x3
    only-tgt   q_profile_slots_restrictions = 0x4
    only-tgt   q_items_compatibility = 0x5
    only-tgt   q_account_money = 0x7
    only-tgt   q_player_skills = 0x8
    only-tgt   q_player_skills_tree = 0x9
    only-tgt   q_service_prices = 0xA
    only-tgt   q_player_reputations = 0xB

enum messaging::friendship_actions_enum
  [values]
    misname  0x0  base=add_friend_action  target=add_friend
    misname  0x1  base=remove_friend_action  target=remove_friend
    misname  0x2  base=add_to_ignore_action  target=add_ignorable
    misname  0x3  base=remove_from_ignore_action  target=remove_ignorable
    misname  0x4  base=find_players_action  target=find_players
    misname  0x5  base=query_friend_list_action  target=query_friend_list
    misname  0x6  base=query_ignore_list_action  target=query_ignore_list
    misname  0x7  base=query_friends_status_action  target=update_friends_status

enum survarium::keyboard_key_group
  [values]
    misname  0x1  base=_sp  target=game_world_group
    misname  0x2  base=_menu  target=lobby_group
    misname  0x4  base=_mp  target=weapon_aim_group
    misname  0x8  base=_chat  target=chat_group
    only-base  _both = 0x5
    only-tgt   player_dead_group = 0x10

enum survarium::scene_ready_type
  [values]
    misname  0x0  base=scene_ready_login  target=login_scene_ready
    misname  0x1  base=scene_ready_lobby  target=lobby_scene_ready

enum vostok::ai::predicate_types_enum
  [values]
    misname  0x9  base=predicate_type_animation_playing  target=predicate_type_sound_played
    misname  0xA  base=predicate_type_sound_played  target=predicate_type_animation_with_sound_played
    misname  0xB  base=predicate_type_sound_playing  target=predicate_type_is_at_position
    only-base  predicate_type_animation_with_sound_played = 0xC
    only-base  predicate_type_is_at_position = 0xD
    only-base  predicate_type_is_moving = 0xE

enum vostok::animation::mixing::time_event_types_enum
  [values]
    misname  0x20  base=time_event_channel_callback_should_be_fired  target=time_event_user_defined

enum vostok::animation::reserved_channel_ids_enum
  [values]
    misname  0x3  base=channel_id_max  target=channel_id_on_animation_lexeme_end
    only-tgt   channel_id_max = 0x4

enum vostok::core::log_flags_enum
  [values]
    only-tgt   log_to_stdout = 0x0

enum vostok::input::mouse_button
  [values]
    misname  0x8  base=mouse_button_extended0  target=mouse_button_count
    only-base  mouse_button_left = 0x1
    only-base  mouse_button_right = 0x2
    only-base  mouse_button_middle = 0x4
    only-base  mouse_button_count = 0x8
    only-base  mouse_button_extended1 = 0x10
    only-base  mouse_button_extended2 = 0x20
    only-base  mouse_button_extended3 = 0x40
    only-base  mouse_button_extended4 = 0x80
    only-tgt   mouse_button_left = 0x151
    only-tgt   mouse_button_right = 0x152
    only-tgt   mouse_button_middle = 0x153
    only-tgt   mouse_button_extended0 = 0x154
    only-tgt   mouse_button_extended1 = 0x155
    only-tgt   mouse_button_extended2 = 0x156
    only-tgt   mouse_button_extended3 = 0x157
    only-tgt   mouse_button_extended4 = 0x158

enum vostok::login_client_message_types_enum
  [values]
    only-base  login_client_invalid_message_type = 0x3F
    only-tgt   login_client_invalid_message_type = 0x7

enum vostok::match_client_message_types_enum
  [values]
    only-tgt   connection_request = 0x40
    only-tgt   get_startup_info = 0x41
    only-tgt   join_match = 0x42
    only-tgt   client_player_update = 0x43
    only-tgt   client_player_commit_suicide = 0x44
    only-tgt   time_synchronization_request = 0x45
    only-tgt   time_synchronization_confirmation = 0x46
    only-tgt   bullets_info_request = 0x47
    only-tgt   team_bases_initialize_info = 0x48
    only-tgt   force_finish_match = 0x49
    only-tgt   world_synchronization_confirmation = 0x4A
    only-tgt   match_client_invalid_message_type = 0x7F

enum vostok::match_server_message_types_enum
  [values]
    only-tgt   match_server_connection_successful = 0x80
    only-tgt   match_options_message_type = 0x81
    only-tgt   server_player_input = 0x82
    only-tgt   kill_player = 0x83
    only-tgt   spawn_player = 0x84
    only-tgt   team_base_capture_progress = 0x85
    only-tgt   match_time_changed = 0x86
    only-tgt   respawn_time_changed = 0x87
    only-tgt   player_kd_stats_changed = 0x88
    only-tgt   hit_player = 0x89
    only-tgt   affect_damage_model = 0x8A
    only-tgt   sync_response = 0x8B
    only-tgt   match_finished = 0x8C
    only-tgt   server_bullet_added = 0x8D
    only-tgt   server_bullet_removed = 0x8E
    only-tgt   server_bullet_moved = 0x8F
    only-tgt   server_bullet_collided = 0x90
    only-tgt   player_visibility_changed = 0x91
    only-tgt   player_profile_message_type = 0x92
    only-tgt   team_bases_message_type = 0x93
    only-tgt   initialize_victory_items = 0x94
    only-tgt   victory_item_take_or_put = 0x95
    only-tgt   trap_placed = 0x96
    only-tgt   trap_removed = 0x97
    only-tgt   trap_fired = 0x98
    only-tgt   trap_disarmed = 0x99
    only-tgt   game_status_changed = 0x9A
    only-tgt   match_wait_time_changed = 0x9B
    only-tgt   game_world_object_state = 0x9C
    only-tgt   world_synchronization_request = 0x9D
    only-tgt   damage_model_state = 0x9E
    only-tgt   match_server_invalid_message_type = 0xC0

enum vostok::network_core::udp_match_packets_count_enum
  [values]
    misname  0x0  base=udp_match_regular_packet  target=udp_match_single_packet
    misname  0x1  base=udp_match_low_level_packet  target=udp_match_multiple_packets

enum vostok::resources::class_id_enum
  [values]
    only-base  weapon_inactive_state_class = 0x103
    only-base  weapon_show_state_class = 0x104
    only-base  weapon_hide_state_class = 0x105
    only-base  weapon_idle_state_class = 0x106
    only-base  weapon_reload_state_class = 0x107
    only-base  weapon_chamber_a_round_state_class = 0x108
    only-base  weapon_chamber_a_round_aimed_state_class = 0x109
    only-base  weapon_fire_state_class = 0x10A
    only-base  weapon_aimed_state_class = 0x10B
    only-base  weapon_aimed_fire_state_class = 0x10C
    only-base  weapon_shotgun_reload_state_class = 0x10D
    only-base  weapon_shotgun_reload_start_substate_class = 0x10E
    only-base  weapon_shotgun_reload_one_substate_class = 0x10F
    only-base  weapon_shotgun_reload_finish_substate_class = 0x110
    only-base  double_barreled_weapon_show_state_class = 0x111
    only-base  double_barreled_weapon_hide_state_class = 0x112
    only-base  double_barreled_weapon_idle_state_class = 0x113
    only-base  double_barreled_weapon_reload_state_class = 0x114
    only-base  double_barreled_weapon_fire_state_class = 0x115
    only-base  double_barreled_weapon_aimed_state_class = 0x116
    only-base  double_barreled_weapon_aimed_fire_state_class = 0x117
    only-base  pistol_weapon_show_state_class = 0x118
    only-base  pistol_weapon_hide_state_class = 0x119
    only-base  pistol_weapon_idle_state_class = 0x11A
    only-base  pistol_weapon_reload_state_class = 0x11B
    only-base  pistol_weapon_fire_state_class = 0x11C
    only-base  pistol_weapon_aimed_state_class = 0x11D
    only-base  pistol_weapon_aimed_fire_state_class = 0x11E
    only-base  platformed_raw_data_class = 0x206
    only-base  converted_model_class = 0x207
    only-base  binary_config_class = 0x208
    only-base  inherits_binary_config_class = 0x209
    only-base  config_class = 0x20A
    only-base  spl_class = 0x20B
    only-base  animation_controller_class = 0x20C
    only-base  sound_stream_class = 0x20D
    only-base  game_project_class = 0x20E
    only-base  game_project_simple_class = 0x20F
    only-base  game_cell_class = 0x210
    only-base  game_object_class = 0x211
    only-base  game_object_scene_class = 0x212
    only-base  sound_player_logic_class = 0x213
    only-base  sound_player_editor_class = 0x214
    only-tgt   platformed_raw_data_class = 0x3
    only-tgt   converted_model_class = 0x3
    only-tgt   binary_config_class = 0x22
    only-tgt   inherits_binary_config_class = 0x23
    only-tgt   weapon_inactive_state_class = 0x12C
    only-tgt   weapon_show_state_class = 0x12D
    only-tgt   weapon_hide_state_class = 0x12E
    only-tgt   weapon_idle_state_class = 0x12F
    only-tgt   weapon_reload_state_class = 0x130
    only-tgt   weapon_chamber_a_round_state_class = 0x131
    only-tgt   weapon_chamber_a_round_aimed_state_class = 0x132
    only-tgt   weapon_fire_state_class = 0x133
    only-tgt   weapon_aimed_state_class = 0x134
    only-tgt   weapon_aimed_fire_state_class = 0x135
    only-tgt   weapon_shotgun_reload_state_class = 0x136
    only-tgt   weapon_shotgun_reload_start_substate_class = 0x137
    only-tgt   weapon_shotgun_reload_one_substate_class = 0x138
    only-tgt   weapon_shotgun_reload_finish_substate_class = 0x139
    only-tgt   double_barreled_weapon_show_state_class = 0x13A
    only-tgt   double_barreled_weapon_hide_state_class = 0x13B
    only-tgt   double_barreled_weapon_idle_state_class = 0x13C
    only-tgt   double_barreled_weapon_reload_state_class = 0x13D
    only-tgt   double_barreled_weapon_fire_state_class = 0x13E
    only-tgt   double_barreled_weapon_aimed_state_class = 0x13F
    only-tgt   double_barreled_weapon_aimed_fire_state_class = 0x140
    only-tgt   pistol_weapon_show_state_class = 0x141
    only-tgt   pistol_weapon_hide_state_class = 0x142
    only-tgt   pistol_weapon_idle_state_class = 0x143
    only-tgt   pistol_weapon_reload_state_class = 0x144
    only-tgt   pistol_weapon_fire_state_class = 0x145
    only-tgt   pistol_weapon_aimed_state_class = 0x146
    only-tgt   pistol_weapon_aimed_fire_state_class = 0x147

---- header summary ----
types:  6217 compared, 551 diverged (39 size, 140 member, 386 fn-order, 928 visibility); base-only 852, target-only 553
enums:  586 compared, 14 diverged; base-only 51, target-only 56

================ SOURCES ================

vostok/ai/sources/behaviour_domain.cpp
  [stmt]   void vostok::ai::behaviour::create_domain(vostok::configs::binary_config_value const&, vostok::ai::ai_world&): base=36 target=33

vostok/ai/sources/brain_unit.cpp
  [stmt]   void vostok::ai::brain_unit::tick(): base=10 target=6
  [stmt]   void vostok::ai::brain_unit::play_sound(vostok::ai::sound_item const* const): base=5 target=3

vostok/ai/sources/vision_sensor.cpp
  [stmt]   bool vostok::ai::sensors::vision_sensor::check_if_in_blind_zones(vostok::ai::game_object const* const) const: base=9 target=10

vostok/animation/sources/animation_collection_cook.cpp
  [stmt]   void vostok::animation::animation_collection_cook::delete_resource(vostok::resources::resource_base*): base=3 target=4

vostok/animation/sources/animation_player.cpp
  [stmt]   bool vostok::animation::animation_player::set_target(vostok::animation::mixing::expression const&, const unsigned int, boost::function<vostok::math::float4x4 __cdecl(void const *)> const&): base=3 target=38
  [stmt]   bool vostok::animation::animation_player::tick(unsigned int): base=9 target=8
  [stmt]   bool vostok::animation::animation_player::set_target_and_tick(vostok::animation::mixing::expression const&, const unsigned int, boost::function<vostok::math::float4x4 __cdecl(void const *)> const&): base=5 target=7
  [stmt]   vostok::math::float4x4 vostok::animation::animation_player::get_object_transform(void const* const) const: base=2 target=3
  [stmt]   void vostok::animation::animation_player::subscribe(char const*, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned char, void const* const): base=28 target=30
  [stmt]   void vostok::animation::animation_player::subscribe(vostok::animation::reserved_channel_ids_enum, boost::function<vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&, void const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const* const): base=4 target=3
  [stmt]   static void vostok::animation::animation_player::destroy_subscriptions(vostok::animation::subscribed_channel const*): base=8 target=9
  [stmt]   void vostok::animation::animation_player::compact_callbacks(): base=55 target=63
  [stmt]   void vostok::animation::animation_player::serialize_state(void*, unsigned int): base=14 target=10
  [stmt]   void vostok::animation::animation_player::deserialize_state(void*, unsigned int): base=12 target=15
  [stmt]   static void vostok::animation::animation_player::destroy_state(void*): base=3 target=5

vostok/animation/sources/bi_spline_skeleton_animation_baked_cook.cpp
  [stmt]   vostok::mutable_buffer vostok::animation::bi_spline_skeleton_animation_baked_cook::allocate_resource(vostok::resources::query_result_for_cook&, unsigned int, unsigned int&, bool): base=6 target=4
  [stmt]   fastdelegate::FastDelegate<void __cdecl(vostok::resources::query_result_for_cook &,vostok::mutable_buffer)> vostok::animation::bi_spline_skeleton_animation_baked_cook::get_create_resource_inplace_in_creation_data_delegate(): base=3 target=1

vostok/animation/sources/bi_spline_skeleton_animation_impl_cook.cpp
  [stmt]   static void vostok::animation::bi_spline_skeleton_animation_impl_cook::on_resources_ready(vostok::resources::queries_result&, vostok::resources::query_result_for_cook* const): base=9 target=10
  [stmt]   void vostok::animation::bi_spline_skeleton_animation_impl_cook::translate_query(vostok::resources::query_result_for_cook&): base=6 target=7

vostok/animation/sources/bone_matrices_computer.cpp
  [stmt]   vostok::animation::bone_transform vostok::animation::bone_matrices_computer::computed_local_bone_transform(vostok::animation::skeleton_bone const&, unsigned int, unsigned int) const: base=2 target=29
  [stmt]   vostok::math::float4x4 vostok::animation::bone_matrices_computer::computed_local_bone_matrix(vostok::animation::skeleton_bone const&, unsigned int) const: base=10 target=13
  [stmt]   void vostok::animation::bone_matrices_computer::compute_skeleton_branch(vostok::animation::skeleton_bone const&, vostok::math::float4x4*, vostok::math::float4x4 const&, unsigned int const*, unsigned int const*) const: base=7 target=6
  [stmt]   void vostok::animation::bone_matrices_computer::compute_skeleton_branch_local(vostok::animation::skeleton_bone const&, vostok::math::float4x4*, unsigned int const*, unsigned int const*) const: base=7 target=6
  [stmt]   void vostok::animation::bone_matrices_computer::convert_to_object_matrices(vostok::math::float4x4*, vostok::math::float4x4*) const: base=8 target=9

vostok/animation/sources/cubic_spline_skeleton_animation_cook.cpp
  [stmt]   void vostok::animation::cubic_spline_skeleton_animation_cook::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>): base=3 target=5

vostok/animation/sources/fermi_interpolator.cpp
  [stmt]   float vostok::animation::fermi_interpolator::interpolated_value(float) const: base=8 target=9

vostok/animation/sources/mixing_animation_lexeme.cpp
  [stmt]   vostok::animation::mixing::animation_lexeme::animation_lexeme(vostok::animation::mixing::animation_lexeme_parameters const&): base=2 target=3

vostok/animation/sources/mixing_animation_lexeme_parameters.cpp
  [stmt]   void vostok::animation::mixing::animation_lexeme_parameters::create_animation_intervals(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&): base=20 target=25
  [stmt]   static unsigned int vostok::animation::mixing::animation_lexeme_parameters::animation_intervals_count(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&): base=10 target=11
  [stmt]   static vostok::animation::mixing::animation_interval vostok::animation::mixing::animation_lexeme_parameters::create_animation_interval(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned int): base=10 target=12

vostok/animation/sources/mixing_n_ary_tree.cpp
  [stmt]   vostok::animation::mixing::n_ary_tree::n_ary_tree(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::base_interpolator const**, vostok::animation::mixing::animation_state*, vostok::animation::mixing::animation_state**, vostok::animation::mixing::animated_object_holder*, vostok::animation::mixing::n_ary_tree_intrusive_base*, unsigned int, unsigned int, unsigned int, unsigned int): base=2 target=3
  [stmt]   void vostok::animation::mixing::n_ary_tree::initialize(): base=2 target=6
  [stmt]   vostok::animation::mixing::n_ary_tree& vostok::animation::mixing::n_ary_tree::operator=(vostok::animation::mixing::n_ary_tree const&): base=18 target=17
  [stmt]   void vostok::animation::mixing::n_ary_tree::destroy(): base=13 target=14
  [stmt]   vostok::math::float4x4 vostok::animation::mixing::n_ary_tree::get_object_transform(void const*) const: base=2 target=6
  [stmt]   void vostok::animation::mixing::n_ary_tree::set_object_transform(void const*, vostok::math::float4x4 const&): base=5 target=7
  [stmt]   void vostok::animation::mixing::n_ary_tree::update_time_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node&, const unsigned int, const unsigned int): base=2 target=17
  [stmt]   void vostok::animation::mixing::n_ary_tree::update_animation_states(unsigned int, unsigned int): base=3 target=5
  [stmt]   bool vostok::animation::mixing::n_ary_tree::need_new_transform(const unsigned int) const: base=7 target=8
  [stmt]   void vostok::animation::mixing::n_ary_tree::process_event(vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int): base=2 target=87
  [stmt]   void vostok::animation::mixing::n_ary_tree::process_events(unsigned int, unsigned int): base=3 target=6
  [stmt]   bool vostok::animation::mixing::n_ary_tree::update_event_iterators_and_dispatch_callbacks(const unsigned int, vostok::animation::subscribed_channel*&, bool&): base=3 target=26
  [stmt]   void vostok::animation::mixing::n_ary_tree::adjust_animation_events_times(vostok::animation::mixing::n_ary_tree const&): base=2 target=11
  [stmt]   bool vostok::animation::mixing::n_ary_tree::tick(unsigned int, vostok::animation::subscribed_channel*&, bool&): base=16 target=27

vostok/animation/sources/mixing_n_ary_tree_animation_time_calculator.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_animation_time_calculator::fill_time(float, float, unsigned int): base=6 target=7
  [stmt]   void vostok::animation::mixing::n_ary_tree_animation_time_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_node&): base=6 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_animation_time_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_transition_node&): base=12 target=14
  [stmt]   vostok::animation::mixing::n_ary_tree_animation_time_calculator::n_ary_tree_animation_time_calculator(vostok::animation::mixing::n_ary_tree_animation_node&, const unsigned int, const float, const unsigned int, const bool): base=2 target=7

vostok/animation/sources/mixing_n_ary_tree_cloner.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_cloner::initialize(vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&): base=11 target=10
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_cloner::clone(vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::base_interpolator const*): base=6 target=5
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_cloner::clone(vostok::animation::mixing::n_ary_tree_base_node&, float, float): base=6 target=7

vostok/animation/sources/mixing_n_ary_tree_comparer.cpp
  [stmt]   vostok::animation::comparison_result_enum vostok::animation::mixing::animation_comparer_predicate::operator()(vostok::animation::mixing::n_ary_tree_animation_node const&, vostok::animation::mixing::n_ary_tree_animation_node const&) const: base=35 target=39
  [stmt]   void vostok::animation::mixing::n_ary_tree_comparer::process_interpolators(vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&): base=2 target=13
  [stmt]   void vostok::animation::mixing::n_ary_tree_comparer::remove_weight_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*): base=2 target=9
  [stmt]   void vostok::animation::mixing::n_ary_tree_comparer::add_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const): base=2 target=13
  [stmt]   void vostok::animation::mixing::n_ary_tree_comparer::add_weight_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*): base=4 target=6
  [stmt]   void vostok::animation::mixing::n_ary_tree_comparer::change_weight_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*): base=2 target=10
  [stmt]   void vostok::animation::mixing::n_ary_tree_comparer::merge_trees(vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&): base=27 target=28
  [stmt]   vostok::animation::mixing::n_ary_tree_comparer::n_ary_tree_comparer(vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&, unsigned int): base=5 target=7

vostok/animation/sources/mixing_n_ary_tree_node_comparer.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_node_comparer::dispatch(vostok::animation::mixing::n_ary_tree_time_scale_node&, vostok::animation::mixing::n_ary_tree_time_scale_node&): base=5 target=9

vostok/animation/sources/mixing_n_ary_tree_size_calculator.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::binary_tree_animation_node&): base=4 target=6
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::binary_tree_weight_node&): base=3 target=6
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::binary_tree_addition_node&): base=3 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::binary_tree_subtraction_node&): base=3 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::binary_tree_multiplication_node&): base=3 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::n_ary_tree_animation_node&): base=5 target=8
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::n_ary_tree_weight_transition_node&): base=4 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_transition_node&): base=4 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::n_ary_tree_weight_node&): base=3 target=5
  [stmt]   void vostok::animation::mixing::n_ary_tree_size_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_node&): base=3 target=5

vostok/animation/sources/mixing_n_ary_tree_time_in_ms_calculator.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_time_in_ms_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_transition_node&): base=8 target=26
  [stmt]   vostok::animation::mixing::n_ary_tree_time_in_ms_calculator::n_ary_tree_time_in_ms_calculator(vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int, float, float, unsigned short): base=3 target=10

vostok/animation/sources/mixing_n_ary_tree_time_scale_calculator.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_time_scale_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_transition_node&): base=2 target=20
  [stmt]   void vostok::animation::mixing::n_ary_tree_time_scale_calculator::visit(vostok::animation::mixing::n_ary_tree_time_scale_node&): base=5 target=4

vostok/animation/sources/mixing_n_ary_tree_transition_tree_constructor.cpp
  [stmt]   vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::add_animation_node(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::animation_state const*, unsigned int, float, bool): base=3 target=17
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale(vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int&, float&): base=3 target=58
  [stmt]   vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node*, unsigned int, unsigned int&, unsigned int&, unsigned int&, float&, bool, bool): base=34 target=48
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale_transition(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::mixing::n_ary_tree_base_node&): base=16 target=17
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale_transition(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node&, float): base=14 target=18
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale_transition(float, float, vostok::animation::mixing::n_ary_tree_base_node&): base=14 target=17
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::mixing::n_ary_tree_base_node&): base=12 target=14
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::base_interpolator const&, vostok::animation::mixing::n_ary_tree_base_node&, float): base=17 target=20
  [stmt]   vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::base_interpolator const&, float, vostok::animation::mixing::n_ary_tree_base_node&): base=17 target=19
  [stmt]   void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::add_operands(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node**, vostok::animation::mixing::n_ary_tree_base_node**, bool): base=40 target=50
  [stmt]   stlp_std::pair<unsigned int,unsigned int> vostok::animation::mixing::computed_operands_count(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&): base=35 target=33
  [stmt]   void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::change_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const, bool): base=54 target=73
  [stmt]   vostok::animation::mixing::n_ary_tree_transition_tree_constructor::n_ary_tree_transition_tree_constructor(vostok::mutable_buffer&, vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&, unsigned int, unsigned int, unsigned int, vostok::animation::subscribed_channel*&, boost::function<vostok::math::float4x4 __cdecl(void const *)> const&): base=2 target=15

vostok/animation/sources/mixing_n_ary_tree_weaver.cpp
  [stmt]   void vostok::animation::mixing::n_ary_tree_weaver::visit(vostok::animation::mixing::binary_tree_animation_node&): base=11 target=19
  [stmt]   void vostok::animation::mixing::n_ary_tree_weaver::propagate<vostok::animation::mixing::binary_tree_addition_node>(vostok::animation::mixing::binary_tree_addition_node&, vostok::animation::mixing::n_ary_tree_weaver&, vostok::animation::mixing::n_ary_tree_weaver&): base=8 target=9
  [stmt]   void update_weights(vostok::animation::mixing::binary_tree_animation_node* const, vostok::animation::mixing::binary_tree_base_node* const): base=9 target=8
  [stmt]   void vostok::animation::mixing::n_ary_tree_weaver::visit(vostok::animation::mixing::binary_tree_multiplication_node&): base=13 target=14

vostok/animation/sources/single_animation_cook.cpp
  [stmt]   void vostok::animation::single_animation_cook::on_sub_resources_loaded(vostok::resources::queries_result&): base=27 target=26
  [stmt]   void vostok::animation::single_animation_cook::delete_resource(vostok::resources::resource_base*): base=3 target=4

vostok/animation/sources/skeleton_animation_cook.cpp
  [stmt]   void vostok::animation::skeleton_animation_cook::translate_query(vostok::resources::query_result_for_cook&): base=6 target=3

vostok/animation/sources/skeleton_cook.cpp
  [stmt]   void add_bone(vostok::animation::skeleton_bone const*, vostok::animation::skeleton_bone*, unsigned int, unsigned int&, vostok::configs::binary_config_value const* const&, char*&, unsigned int&): base=23 target=20

vostok/collision/sources/animated_object.cpp
  [stmt]   vostok::collision::animated_object::animated_object(vostok::configs::binary_config_value const&, vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, const unsigned int, vostok::memory::stack_allocator&): base=13 target=14

vostok/collision/sources/box_geometry_instance.cpp
  [stmt]   bool vostok::collision::box_geometry_instance::ray_query(vostok::collision::object const*, vostok::math::float3 const&, vostok::math::float3 const&, float, float&, vostok::vectora<vostok::collision::ray_triangle_result>&, fastdelegate::FastDelegate<bool __cdecl(vostok::collision::ray_triangle_result const &)> const&) const: base=11 target=12

vostok/collision/sources/collision_cook.cpp
  [stmt]   void vostok::collision::collision_cook::query_triangle_mesh(vostok::resources::query_result_for_cook*): base=8 target=10

vostok/collision/sources/cylinder_geometry_instance.cpp
  [stmt]   vostok::math::float3 vostok::collision::cylinder_geometry_instance::get_closest_point_to(vostok::math::float3 const&, vostok::math::float4x4 const&) const: base=22 target=23

vostok/collision/sources/intersection_double_dispatcher_capsule.cpp
  [stmt]   void vostok::collision::intersection_double_dispatcher::dispatch(vostok::collision::truncated_sphere_geometry_instance const&, vostok::collision::capsule_geometry_instance const&): base=20 target=19

vostok/collision/sources/sphere_geometry_instance.cpp
  [stmt]   vostok::collision::sphere_geometry_instance::sphere_geometry_instance(vostok::math::float4x4 const&): base=1 target=2
  [stmt]   void vostok::collision::sphere_geometry_instance::~sphere_geometry_instance(): base=1 target=2

vostok/collision/sources/triangle_mesh_geometry.cpp
  [stmt]   void vostok::collision::triangle_mesh_geometry::initialize(vostok::memory::base_allocator*, vostok::math::float3 const*, unsigned int, unsigned int const*, unsigned int): base=12 target=13

vostok/core/sources/command_line.cpp
  [stmt]   bool vostok::command_line::key::is_set_as_string(vostok::buffer_string*): base=10 target=9
  [stmt]   void vostok::command_line::checker::operator()(vostok::command_line::key* const, char const*, char const*): base=8 target=5
  [stmt]   void vostok::command_line::initialize(vostok::core::engine*, char const*, vostok::command_line::contains_application_bool): base=13 target=14
  [stmt]   void vostok::command_line::show_help_and_exit(): base=28 target=32

vostok/core/sources/compressor_ppmd.cpp
  [stmt]   vostok::ppmd_compressor::ppmd_compressor(vostok::memory::base_allocator*, unsigned int, vostok::ppmd_compressor::model_restoration_enum): base=5 target=6

vostok/core/sources/configs_binary_config.cpp
  [stmt]   vostok::core::configs::binary_config::binary_config(unsigned char const*, unsigned int, vostok::memory::base_allocator*): base=3 target=4

vostok/core/sources/configs_binary_config_cook.cpp
  [stmt]   vostok::mutable_buffer vostok::core::configs::binary_config_cook_impl::allocate_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, bool): base=9 target=10

vostok/core/sources/configs_binary_config_value.cpp
  [stmt]   vostok::configs::binary_config_value const& vostok::configs::binary_config_value::operator[](char const*) const: base=8 target=9
  [stmt]   bool vostok::configs::binary_config_value::value_exists(char const*) const: base=12 target=15
  [stmt]   void vostok::configs::binary_config_value::fix_up(unsigned int): base=9 target=10

vostok/core/sources/console_command_processor.cpp
  [fn-order]
    moved      bool vostok::console_commands::starts_from_predicate::operator()(vostok::console_commands::console_command* const, vostok::console_commands::console_command* const) const
  [stmt]   vostok::console_commands::console_command* vostok::console_commands::find(char const*): base=7 target=6
  [stmt]   unsigned int vostok::console_commands::get_similar(char const*, vostok::console_commands::console_command**, unsigned int): base=13 target=14
  [stmt]   void vostok::console_commands::show_help(char const*): base=13 target=10
  [stmt]   void vostok::console_commands::save(char const*, const vostok::console_commands::command_type, vostok::memory::base_allocator&): base=3 target=4
  [stmt]   bool vostok::console_commands::execute_console_commands(vostok::fs_new::native_path_string, const vostok::console_commands::execution_filter): base=15 target=14

vostok/core/sources/core_debug_engine.cpp
  [stmt]   void vostok::core::core_debug_engine::flush_log_file(char const*) const: base=2 target=7
  [stmt]   bool vostok::core::core_debug_engine::create_folder_r(char const*, bool) const: base=3 target=4

vostok/core/sources/core_entry_point.cpp
  [stmt]   void vostok::core::preinitialize(vostok::core::engine*, vostok::logging::log_file_usage_enum, char const*, vostok::command_line::contains_application_bool, char const*, char const*): base=9 target=12
  [stmt]   void vostok::core::initialize_resources(vostok::fs_new::asynchronous_device_interface&, vostok::fs_new::asynchronous_device_interface&, vostok::resources::enable_fs_watcher_bool): base=7 target=9

vostok/core/sources/delayhlp.cpp
  [stmt]   int (*)() __delayLoadHelper2(ImgDelayDescr const*, int (**)()): base=67 target=79

vostok/core/sources/game_resman.cpp
  [stmt]   void vostok::resources::game_resources_manager::add_new_resource_to_increase_quality_tree(vostok::resources::resource_base*): base=4 target=5
  [stmt]   void vostok::resources::game_resources_manager::on_node_unmount(vostok::vfs::vfs_iterator&): base=4 target=7

vostok/core/sources/game_resman_free.cpp
  [stmt]   void vostok::resources::resource_freeing_functionality::release_sub_fat_from_parents(vostok::resources::vfs_sub_fat_resource*): base=12 target=11

vostok/core/sources/game_resman_out_of_memory.cpp
  [stmt]   void vostok::resources::game_resources_manager::tick_memory_type(vostok::resources::memory_type*): base=30 target=28

vostok/core/sources/game_resman_quality_increase.cpp
  [stmt]   vostok::resources::resource_base* vostok::resources::quality_increase_functionality::find_next_to_select_to_increase_quality(): base=17 target=16
  [stmt]   void vostok::resources::quality_increase_functionality::tick(): base=11 target=9

vostok/core/sources/managed_allocator.cpp
  [stmt]   vostok::memory::managed_allocator::managed_allocator(unsigned int, unsigned int, unsigned int): base=8 target=9

vostok/core/sources/managed_allocator_base.cpp
  [stmt]   vostok::memory::managed_allocator_base::managed_allocator_base(unsigned int): base=13 target=12

vostok/core/sources/math_cuboid.cpp
  [stmt]   vostok::math::cuboid::cuboid(vostok::math::aabb const&, vostok::math::float4x4 const&): base=24 target=20

vostok/core/sources/math_float4x4.cpp
  [stmt]   vostok::math::float3 vostok::math::float4x4::get_angles(const vostok::math::axis_rotation_order) const: base=11 target=20
  [stmt]   vostok::math::float3 vostok::math::float4x4::get_angles_xyz() const: base=7 target=19

vostok/core/sources/math_quaternion.cpp
  [stmt]   vostok::math::quaternion::quaternion(vostok::math::float4x4 const&): base=69 target=81

vostok/core/sources/memory.cpp
  [stmt]   void vostok::memory::allocate_region(const unsigned long long, const unsigned long long): base=23 target=24
  [stmt]   void vostok::memory::dump_statistics(const bool): base=17 target=18
  [stmt]   int vostok::memory::compare(vostok::const_buffer const&, vostok::const_buffer const&): base=14 target=13

vostok/core/sources/memory_doug_lea_allocator.cpp
  [stmt]   void* vostok::memory::doug_lea_allocator::malloc_impl(unsigned int): base=11 target=10

vostok/core/sources/memory_doug_lea_mt_allocator.cpp
  [stmt]   mutex_mt_raii::mutex_mt_raii(vostok::memory::doug_lea_mt_allocator const&): base=4 target=7
  [stmt]   void* vostok::memory::doug_lea_mt_allocator::malloc_impl(unsigned int): base=4 target=6
  [stmt]   void* vostok::memory::doug_lea_mt_allocator::realloc_impl(void*, unsigned int): base=4 target=6
  [stmt]   unsigned int vostok::memory::doug_lea_mt_allocator::total_size() const: base=4 target=6
  [stmt]   unsigned int vostok::memory::doug_lea_mt_allocator::allocated_size() const: base=4 target=6

vostok/core/sources/memory_stack_allocator.cpp
  [stmt]   void* vostok::memory::stack_allocator::malloc_impl(unsigned int): base=4 target=5

vostok/core/sources/memory_video_win.cpp
  [stmt]   unsigned long long vostok::platform::get_local_video_memory_size(): base=9 target=13

vostok/core/sources/memory_win.cpp
  [stmt]   bool allocate_arenas(vostok::buffer_vector<vostok::memory::platform::region>&, vostok::buffer_vector<vostok::memory::platform::region>&, const unsigned int, bool): base=90 target=106
  [stmt]   bool try_to_allocate_arenas(vostok::buffer_vector<vostok::memory::platform::region>&, vostok::memory::platform::region&, vostok::memory::platform::region&, bool): base=14 target=16
  [stmt]   unsigned long long calculate_desirable_resource_arenas(memory_stats&, const float, const unsigned long long, const unsigned long long): base=29 target=31
  [stmt]   void vostok::memory::platform::allocate_arenas(const unsigned long long, const unsigned long long, vostok::buffer_vector<vostok::memory::platform::region>&, vostok::memory::platform::region&, vostok::memory::platform::region&): base=46 target=55

vostok/core/sources/resources_cook_base.cpp
  [stmt]   void vostok::resources::cook_base::call_destroy_resource(vostok::resources::unmanaged_resource*): base=15 target=11

vostok/core/sources/resources_device_manager.cpp
  [stmt]   void vostok::resources::device_manager::on_query_processed(vostok::resources::query_result*, bool): base=17 target=16

vostok/core/sources/resources_device_manager_thread.cpp
  [stmt]   bool vostok::resources::device_manager::process_write_query(void**, vostok::resources::query_result*, vostok::fs_new::synchronous_device_interface const&): base=21 target=24
  [stmt]   bool vostok::resources::device_manager::open_file(void***, vostok::resources::query_result*, vostok::fs_new::synchronous_device_interface const&): base=24 target=23

vostok/core/sources/resources_impl.cpp
  [stmt]   unsigned int vostok::resources::query_create_resource(char const*, vostok::const_buffer, vostok::resources::class_id_enum, boost::function<void __cdecl(vostok::resources::queries_result &)> const&, vostok::memory::base_allocator*, vostok::variant<32> const*, vostok::resources::query_result_for_cook*, assert_on_fail_bool): base=4 target=5

vostok/core/sources/resources_manager.cpp
  [stmt]   vostok::resources::resources_manager::resources_manager(vostok::fs_new::asynchronous_device_interface&, vostok::fs_new::asynchronous_device_interface&, vostok::resources::enable_fs_watcher_bool): base=23 target=24
  [stmt]   void vostok::resources::resources_manager::~resources_manager(): base=12 target=14
  [stmt]   void vostok::resources::resources_manager::finalize_name_registry(): base=11 target=12

vostok/core/sources/resources_manager_allocation.cpp
  [stmt]   void vostok::resources::allocate_functionality::prepare_raw_resource_for_managed_or_unmanaged_cook(vostok::resources::query_result*, vostok::resources::reallocating_bool): base=28 target=27
  [stmt]   void vostok::resources::allocate_functionality::continue_prepare_raw_resource_for_inplace_unmanaged_cook(vostok::resources::query_result*): base=20 target=19
  [stmt]   void vostok::resources::allocate_functionality::prepare_raw_resource(vostok::resources::query_result*, vostok::resources::reallocating_bool): base=12 target=11

vostok/core/sources/resources_manager_cook.cpp
  [stmt]   static vostok::resources::cook_base* vostok::resources::resources_manager::unregister_cook(vostok::resources::class_id_enum): base=9 target=10

vostok/core/sources/resources_manager_deallocation.cpp
  [stmt]   void vostok::resources::resources_manager::delete_unmanaged_resource(vostok::resources::unmanaged_resource*): base=29 target=28

vostok/core/sources/resources_manager_helper.cpp
  [stmt]   vostok::resources::thread_local_data* vostok::resources::resources_manager::get_thread_local_data(unsigned int, bool): base=30 target=29

vostok/core/sources/resources_manager_new_queries.cpp
  [stmt]   void vostok::resources::resources_manager::continue_init_new_query(vostok::resources::query_result&): base=30 target=31

vostok/core/sources/resources_manager_user_thread.cpp
  [stmt]   static vostok::resources::queries_result* vostok::resources::resources_manager::create_queries_result(vostok::resources::query_resource_params const&): base=72 target=68
  [stmt]   long vostok::resources::resources_manager::query_resources_impl(vostok::resources::query_resource_params const&): base=64 target=62

vostok/core/sources/resources_mount_ptr.cpp
  [stmt]   void vostok::resources::fs_task_unmount::unmount_children(vostok::vfs::vfs_mount*): base=17 target=16
  [stmt]   void vostok::resources::fs_task_unmount::execute_may_destroy_this(): base=13 target=9

vostok/core/sources/resources_queries_result.cpp
  [stmt]   void vostok::resources::queries_result::query_fs_iterators(): base=8 target=9
  [stmt]   void vostok::resources::queries_result::end_and_delete_self(bool): base=9 target=10

vostok/core/sources/resources_query_fs_iterator.cpp
  [stmt]   bool vostok::resources::convert_virtual_to_physical_path(vostok::fs_new::native_path_string*, vostok::fs_new::virtual_path_string const&, char const*): base=7 target=8

vostok/core/sources/resources_query_mount.cpp
  [stmt]   vostok::resources::mount_by_config_helper::mount_by_config_helper(const vostok::resources::mount_by_config_helper::type_enum, char const* const, boost::function<void __cdecl(vostok::intrusive_ptr<vostok::resources::fs_task_unmount,vostok::resources::intrusive_fs_task_unmount_base,vostok::threading::simple_lock>)>, vostok::memory::base_allocator* const): base=9 target=3
  [stmt]   void vostok::resources::mount_by_config_helper::query_db(): base=6 target=7
  [stmt]   void vostok::resources::query_mount_archive(vostok::fs_new::virtual_path_string const&, vostok::fs_new::native_path_string const&, vostok::fs_new::native_path_string const&, char const*, boost::function<void __cdecl(vostok::intrusive_ptr<vostok::resources::fs_task_unmount,vostok::resources::intrusive_fs_task_unmount_base,vostok::threading::simple_lock>)> const&, vostok::memory::base_allocator* const, vostok::resources::fs_task_composite* const): base=7 target=6

vostok/core/sources/resources_query_result.cpp
  [stmt]   bool vostok::resources::query_result::check_file_crc(): base=10 target=14
  [stmt]   vostok::const_buffer vostok::resources::query_result::pin_compressed_file(): base=8 target=9
  [stmt]   vostok::const_buffer vostok::resources::query_result::pin_raw_buffer(): base=16 target=17
  [stmt]   vostok::const_buffer vostok::resources::query_result::pin_raw_file(): base=9 target=5
  [stmt]   void vostok::resources::query_result::unpin_compressed_or_raw_file(vostok::const_buffer const&): base=7 target=6
  [stmt]   unsigned int vostok::resources::query_result::raw_buffer_size(): base=5 target=6

vostok/core/sources/resources_query_result_allocation.cpp
  [stmt]   void vostok::resources::query_result::copy_data_to_resource(vostok::const_buffer): base=9 target=10
  [stmt]   bool vostok::resources::query_result::retry_action_that_caused_out_of_memory(): base=15 target=20

vostok/core/sources/resources_query_result_cook.cpp
  [stmt]   void vostok::resources::query_result::send_to_create_resource(): base=12 target=11
  [stmt]   void vostok::resources::query_result::do_unmanaged_create_resource(vostok::resources::unmanaged_cook*): base=13 target=14
  [stmt]   void vostok::resources::query_result::do_create_resource_impl(): base=15 target=21
  [stmt]   void vostok::resources::query_result::propogate_sub_fats_to_resource<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>&): base=4 target=5
  [stmt]   void vostok::resources::query_result::finish_translated_query(vostok::resources::cook_base::result_enum): base=15 target=11

vostok/core/sources/resources_query_result_finalization.cpp
  [stmt]   void vostok::resources::query_result::on_refered_query_ended(vostok::resources::query_result*): base=23 target=22
  [stmt]   void vostok::resources::query_result::~query_result(): base=10 target=11

vostok/core/sources/resources_query_result_path.cpp
  [stmt]   void vostok::resources::query_result::on_request_iterator_ready(vostok::vfs::vfs_iterator&, bool): base=9 target=11

vostok/core/sources/resources_resource_children.cpp
  [stmt]   void vostok::resources::resource_children::link_child_resource(vostok::resources::resource_base*, unsigned int): base=10 target=8

vostok/core/sources/testing_impl.cpp
  [stmt]   void vostok::testing::on_exception(vostok::assert_enum, char const*, _EXCEPTION_POINTERS*, bool): base=16 target=17
  [stmt]   bool vostok::testing::detail::run_tests_impl(vostok::testing::test_base*, char const*): base=29 target=34

vostok/core/sources/text_tree_item.cpp
  [stmt]   void vostok::strings::text_tree_item::set_name(char const*): base=5 target=6
  [stmt]   vostok::strings::text_tree_item* vostok::strings::text_tree_item::new_child(char const*, bool): base=9 target=8

vostok/core/sources/threading_functions.cpp
  [stmt]   void vostok::threading::initialize_core_count(): base=19 target=18

vostok/core/sources/threading_reader_writer_lock.cpp
  [stmt]   bool vostok::threading::reader_writer_lock::lock_read_impl(bool) const: base=9 target=8
  [stmt]   bool vostok::threading::reader_writer_lock::lock_write_impl(bool) const: base=10 target=9

vostok/engine/sources/console_impl.cpp
  [stmt]   vostok::console_impl::console_impl(vostok::ui::world&, vostok::memory::base_allocator&): base=52 target=53
  [stmt]   unsigned int vostok::string_color(char const*): base=9 target=6
  [stmt]   void vostok::console_impl::tick(vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const&): base=58 target=59
  [stmt]   bool vostok::console_impl::on_text_changed(vostok::ui::window*, int, int): base=26 target=27

vostok/engine/sources/editor_console.cpp
  [stmt]   void vostok::engine::editor_console::tick(vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const&): base=3 target=2

vostok/engine/sources/engine_world.cpp
  [fn-order]
    moved      void vostok::engine::engine_world::on_crash()
  [stmt]   vostok::engine::engine_world::engine_world(vostok::engine_user::module_proxy&, char const*, char const*, char const*): base=12 target=14
  [stmt]   void vostok::engine::engine_world::~engine_world(): base=4 target=6

vostok/engine/sources/engine_world_initialize.cpp
  [stmt]   void vostok::engine::engine_world::initialize(): base=51 target=55
  [stmt]   void vostok::engine::engine_world::finalize(): base=73 target=76

vostok/engine/sources/engine_world_logic.cpp
  [stmt]   void vostok::engine::engine_world::logic_tick(): base=9 target=11
  [stmt]   void vostok::engine::engine_world::logic(): base=17 target=18

vostok/engine/sources/engine_world_network.cpp
  [stmt]   void vostok::engine::engine_world::network_tick(): base=3 target=4

vostok/engine/sources/engine_world_resources.cpp
  [stmt]   void vostok::engine::engine_world::finalize_resources(): base=13 target=14

vostok/engine/sources/engine_world_win.cpp
  [stmt]   long message_processor(HWND__*, unsigned int, unsigned int, long): base=17 target=26
  [stmt]   HWND__* new_window(): base=20 target=10

vostok/engine/sources/game_console.cpp
  [stmt]   void vostok::engine::game_console::tick(vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const&): base=3 target=2

vostok/game/sources/animated_model_instance_cook.cpp
  [stmt]   void survarium::animated_model_instance_cook::on_subresources_loaded(vostok::resources::queries_result&): base=14 target=18

vostok/game/sources/animation_space_graph.cpp
  [stmt]   survarium::animation_space_vertex const* survarium::animation_space_graph::get_animation_by_path(char const*) const: base=8 target=6
  [stmt]   static survarium::animation_space_vertex_id survarium::animation_space_graph::get_movement(vostok::animation::animation_player&, survarium::animation_space_vertex const*, survarium::animation_space_vertex const*, const float): base=2 target=12
  [stmt]   float survarium::animation_space_graph::max_speed() const: base=9 target=10

vostok/game/sources/animation_space_graph_cook.cpp
  [stmt]   void survarium::animation_space_graph_cook::translate_query(vostok::resources::query_result_for_cook&): base=4 target=3
  [stmt]   void survarium::animation_space_graph_cook::delete_resource(vostok::resources::resource_base*): base=7 target=8
  [stmt]   void survarium::animation_space_graph_cook::on_options_received(vostok::resources::queries_result&): base=21 target=24
  [stmt]   void survarium::animation_space_graph_cook::on_animations_loaded(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>): base=2 target=45

vostok/game/sources/animations_search_service.cpp
  [stmt]   bool survarium::animations_search_service::search(vostok::resources::resource_ptr<survarium::animation_space_graph,vostok::resources::unmanaged_intrusive_base> const&, survarium::vector<unsigned int>*, const survarium::animation_space_vertex_id, const survarium::animation_space_vertex_id): base=3 target=10

vostok/game/sources/animations_selector.cpp
  [stmt]   void survarium::animations_selector::set_target(vostok::ai::animation_item const&): base=5 target=4
  [stmt]   void survarium::animations_selector::set_target(vostok::ai::movement_target const&): base=8 target=7

vostok/game/sources/artefact_container.cpp
  [stmt]   survarium::artefact_container::artefact_container(survarium::base_game_scene&): base=2 target=1

vostok/game/sources/base_game_scene.cpp
  [stmt]   survarium::base_game_scene::base_game_scene(survarium::game&): base=2 target=5
  [stmt]   void survarium::base_game_scene::show_movie(vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>&): base=2 target=5
  [stmt]   void survarium::base_game_scene::hide_movie(vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>&): base=2 target=4
  [stmt]   void survarium::base_game_scene::show_text_manager(survarium::flash_text_manager*): base=2 target=5
  [stmt]   void survarium::base_game_scene::hide_text_manager(survarium::flash_text_manager*): base=2 target=3
  [stmt]   void survarium::base_game_scene::init_physics(): base=2 target=3
  [stmt]   bool survarium::base_game_scene::point_to_screen(vostok::math::float3 const&, vostok::math::float2&): base=3 target=12

vostok/game/sources/base_network_client.cpp
  [stmt]   survarium::base_network_client::base_network_client(survarium::game&): base=3 target=6

vostok/game/sources/booby_trap_cook.cpp
  [stmt]   survarium::booby_trap_cook::booby_trap_cook(survarium::game_world&): base=2 target=1
  [stmt]   void survarium::booby_trap_cook::on_models_ready(vostok::resources::queries_result&, survarium::booby_trap*): base=9 target=10

vostok/game/sources/booby_trap_set.cpp
  [stmt]   void survarium::booby_trap_set::action(bool): base=7 target=9
  [stmt]   void survarium::booby_trap_set::on_trap_placed_message(unsigned char, vostok::math::float3 const&, vostok::math::float3 const&): base=6 target=8
  [stmt]   void survarium::booby_trap_set::holder_assigned(): base=3 target=4
  [stmt]   void survarium::booby_trap_set::holder_removed(): base=3 target=4
  [stmt]   void survarium::booby_trap_set::remove(): base=3 target=4

vostok/game/sources/chat_handler.cpp
  [stmt]   void survarium::chat_handler::initialize(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&): base=10 target=11
  [stmt]   void survarium::chat_handler::add_message(const messaging::message_channel_enum, wchar_t const*, wchar_t const*): base=26 target=22
  [stmt]   void survarium::chat_handler::add_to_recent_list(wchar_t const*): base=4 target=5
  [stmt]   void survarium::chat_handler::set_mode(bool): base=32 target=33
  [stmt]   void survarium::chat_handler::set_local_player_name(char const*): base=4 target=5
  [stmt]   void survarium::chat_handler::focus(bool): base=13 target=14
  [stmt]   void survarium::chat_handler::show(survarium::base_game_scene*): base=3 target=4
  [stmt]   void survarium::chat_handler::hide(survarium::base_game_scene*): base=3 target=4
  [stmt]   void survarium::chat_handler::tick(unsigned int): base=3 target=5

vostok/game/sources/damage_zone.cpp
  [stmt]   survarium::damage_zone::damage_zone(survarium::game_world&): base=2 target=1
  [stmt]   void survarium::damage_zone::load(vostok::configs::binary_config_value const&, survarium::vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >&, survarium::vector<vostok::render::light_props>&, unsigned int): base=7 target=6
  [stmt]   void survarium::damage_zone::play_particles(survarium::vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> > const&) const: base=8 target=9

vostok/game/sources/damage_zone_cook.cpp
  [stmt]   void survarium::damage_zone_cook::translate_query(vostok::resources::query_result_for_cook&): base=6 target=7

vostok/game/sources/empty_hands.cpp
  [stmt]   vostok::animation::mixing::expression survarium::empty_hands::selected_animations(vostok::mutable_buffer&, const bool) const: base=5 target=4

vostok/game/sources/empty_hands_cook.cpp
  [stmt]   void survarium::empty_hands_cook::on_empty_hands_animations_loaded(vostok::resources::queries_result&): base=9 target=10
  [stmt]   void survarium::empty_hands_cook::delete_resource(vostok::resources::resource_base*): base=3 target=8

vostok/game/sources/fingers_to_weapon_corrector.cpp
  [stmt]   float survarium::fingers_to_weapon_corrector::get_hand_coefficient(const float, const bool) const: base=3 target=4

vostok/game/sources/free_fly_camera.cpp
  [stmt]   bool survarium::free_fly_camera::on_keyboard_action(vostok::input::world*, vostok::input::enum_keyboard, vostok::input::enum_keyboard_action): base=18 target=21
  [stmt]   void survarium::free_fly_camera::tick(): base=35 target=36

vostok/game/sources/game.cpp
  [stmt]   void survarium::game::execute_scaleform_command(survarium::scaleform_render_command): base=2 target=3
  [stmt]   void survarium::game::on_configs_loaded(vostok::resources::queries_result&): base=2 target=19
  [stmt]   void survarium::game::on_render_output_window_created(vostok::resources::queries_result&): base=6 target=4
  [stmt]   void survarium::game::on_base_resources_created(vostok::resources::queries_result&): base=2 target=30
  [stmt]   void survarium::game::load_cc_script(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>, bool): base=2 target=11
  [stmt]   void survarium::game::tick(unsigned int): base=31 target=32
  [stmt]   void survarium::game::update_stats(const unsigned int): base=20 target=24
  [stmt]   void survarium::game::on_application_deactivate(): base=6 target=5
  [stmt]   void survarium::game::on_fullscreen_alttab(bool): base=2 target=3
  [stmt]   void survarium::game::draw_debug_window(): base=9 target=8
  [stmt]   void survarium::game::toggle_pause(): base=2 target=6

vostok/game/sources/game_entry_point.cpp
  [stmt]   vostok::engine_user::world* survarium::game_module::create_world(vostok::engine_user::engine&, vostok::render::world&, vostok::sound::world&, vostok::network::world&): base=4 target=19
  [stmt]   void survarium::game_module::destroy_world(vostok::engine_user::world*&): base=2 target=4

vostok/game/sources/game_generate_shaders.cpp
  [stmt]   void survarium::generate_shaders_world::generate_renderer_shaders(): base=31 target=36
  [stmt]   void survarium::generate_shaders_world::generate_materials_shaders(): base=10 target=18
  [stmt]   void survarium::generate_shaders_world::tick(unsigned int): base=11 target=13

vostok/game/sources/game_options.cpp
  [stmt]   void survarium::game_options::callback(survarium::flash_movie*, char const*, survarium::flash_value const*, unsigned int): base=42 target=43
  [stmt]   void survarium::game_options::fill_labels(): base=33 target=34
  [stmt]   void survarium::game_options::fill_menu_buttons(bool): base=25 target=26
  [stmt]   void survarium::game_options::fill_settings_data(): base=59 target=77
  [stmt]   void survarium::game_options::activate(survarium::base_game_scene*): base=5 target=7
  [stmt]   void survarium::game_options::deactivate(): base=4 target=6
  [stmt]   bool survarium::game_options::process_key_input(int): base=3 target=34
  [stmt]   void survarium::game_options::assign_binding(survarium::game_action_id, char const*): base=15 target=16
  [stmt]   void survarium::game_options::finish_binding(): base=4 target=5
  [stmt]   bool survarium::game_options::on_keyboard_action(vostok::input::world*, vostok::input::enum_keyboard, vostok::input::enum_keyboard_action): base=5 target=10
  [stmt]   bool survarium::game_options::on_mouse_key_action(vostok::input::world*, vostok::input::mouse_button, vostok::input::enum_mouse_key_action): base=9 target=11
  [stmt]   void survarium::game_options::initialize_bindings(): base=17 target=19
  [stmt]   void survarium::game_options::reset_bindings(bool): base=18 target=20
  [stmt]   void survarium::game_options::apply_key_bindings(): base=11 target=12
  [stmt]   void survarium::game_options::reset_bindings_to_defaults(): base=5 target=4

vostok/game/sources/game_project.cpp
  [stmt]   survarium::simple_game_project::simple_game_project(survarium::base_game_scene&): base=2 target=1
  [stmt]   void survarium::simple_game_project::on_npc_loaded(vostok::resources::queries_result&): base=3 target=5
  [stmt]   void survarium::simple_game_project::insert(survarium::scheduler&): base=23 target=28
  [stmt]   void survarium::simple_game_project::remove(): base=22 target=28
  [stmt]   survarium::victory_items_container_core* survarium::simple_game_project::get_items_container(unsigned char): base=6 target=9

vostok/game/sources/game_world.cpp
  [stmt]   void survarium::game_world::register_cooks(): base=16 target=17
  [stmt]   void survarium::game_world::tick(const unsigned int, const unsigned int, const bool): base=17 target=12
  [stmt]   void survarium::game_world::on_project_loaded(vostok::resources::queries_result&, unsigned int, boost::function<void __cdecl(vostok::resources::queries_result &)> const&): base=2 target=45
  [stmt]   void survarium::game_world::unload(): base=21 target=23
  [stmt]   void survarium::game_world::load(char const*, vostok::resources::request*, vostok::resources::request*, vostok::variant<32> const**, boost::function<void __cdecl(vostok::resources::queries_result &)> const&): base=48 target=50
  [stmt]   void survarium::game_world::show_ui(bool): base=10 target=13
  [stmt]   void survarium::game_world::clear_resources(): base=14 target=16
  [stmt]   void survarium::game_world::clear_player_spawn_info(): base=5 target=6

vostok/game/sources/game_world_input.cpp
  [stmt]   bool survarium::game_world::on_keyboard_action(vostok::input::world*, vostok::input::enum_keyboard, vostok::input::enum_keyboard_action): base=23 target=24

vostok/game/sources/game_world_npc.cpp
  [stmt]   static void survarium::game_world::kill_npc(vostok::resources::resource_ptr<survarium::human_npc,vostok::resources::unmanaged_intrusive_base>&): base=4 target=3

vostok/game/sources/game_world_ui.cpp
  [stmt]   void survarium::game_world_ui::initialize(survarium::match_options&): base=42 target=40
  [stmt]   void survarium::game_world_ui::set_base_capture_progress(unsigned int, unsigned int): base=23 target=20
  [stmt]   void survarium::game_world_ui::set_match_time(unsigned int): base=7 target=12
  [stmt]   void survarium::game_world_ui::set_pregame(char const*, unsigned int): base=6 target=8
  [stmt]   void survarium::game_world_ui::set_respawn_time(unsigned int): base=7 target=10
  [stmt]   void survarium::game_world_ui::update_ui(const unsigned int, const unsigned int): base=20 target=21
  [stmt]   void survarium::game_world_ui::on_unload(): base=5 target=9
  [stmt]   void survarium::game_world_ui::on_victory_item_put_take(unsigned char, bool, bool): base=2 target=40
  [stmt]   void survarium::game_world_ui::on_player_killed(unsigned char, unsigned char, bool, unsigned int): base=2 target=33
  [stmt]   void survarium::game_world_ui::on_damage_affect_applying(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum): base=14 target=13
  [stmt]   void survarium::game_world_ui::update_minimap_objects(): base=2 target=44
  [stmt]   void survarium::game_world_ui::initialize_minimap(): base=9 target=10
  [stmt]   void survarium::game_world_ui::update_minimap_players(): base=31 target=32
  [stmt]   void survarium::game_world_ui::update_minimap_local_player(): base=11 target=14
  [stmt]   void survarium::game_world_ui::show_quick_slots(bool): base=4 target=5
  [stmt]   void survarium::game_world_ui::create_slot_value(survarium::profile_slot_enum, survarium::inventory_item_props&, survarium::flash_value&): base=22 target=23
  [stmt]   void survarium::game_world_ui::fill_quick_slots(): base=28 target=29
  [stmt]   void survarium::game_world_ui::update_quick_slot(survarium::profile_slot_enum): base=19 target=21
  [stmt]   void survarium::game_world_ui::show_screen_message(char const*): base=4 target=6
  [stmt]   void survarium::game_world_ui::set_player_online_status(unsigned int, bool): base=7 target=8

vostok/game/sources/generic_anomaly.cpp
  [stmt]   survarium::generic_anomaly::generic_anomaly(survarium::base_game_scene&): base=2 target=1

vostok/game/sources/human_npc.cpp
  [stmt]   void survarium::human_npc::fill_stats(vostok::ai::npc_statistics&) const: base=13 target=14
  [stmt]   void survarium::human_npc::on_animation_end(): base=6 target=5
  [stmt]   void survarium::human_npc::hit(survarium::hit_initiator const* const, const unsigned int, char const*, const float, const float, survarium::bullet* const): base=3 target=4

vostok/game/sources/human_npc_cook.cpp
  [stmt]   void survarium::human_npc_cook::translate_query(vostok::resources::query_result_for_cook&): base=9 target=8

vostok/game/sources/key_binder.cpp
  [stmt]   void survarium::console_command_bind::save_to(vostok::console_commands::save_storage&, vostok::memory::base_allocator*) const: base=8 target=7
  [stmt]   int survarium::key_binder::get_action_dik(survarium::game_action_id, int): base=6 target=10
  [stmt]   survarium::game_action_id survarium::key_binder::get_binded_action(int, survarium::toggle_action_enum&, int) const: base=14 target=15
  [stmt]   int survarium::key_binder::get_binding_group(survarium::game_action_id): base=3 target=4

vostok/game/sources/lobby_camera.cpp
  [stmt]   bool survarium::lobby_camera::on_mouse_move(vostok::input::world*, int, int, int): base=3 target=12
  [stmt]   void survarium::lobby_camera::on_deactivate(): base=2 target=1

vostok/game/sources/lobby_client.cpp
  [stmt]   void survarium::lobby_client::on_disconnected(): base=4 target=6
  [stmt]   void survarium::lobby_client::connect(vostok::server_connection_info const&): base=4 target=5
  [stmt]   void survarium::lobby_client::sign_in_on_packet_received(vostok::network_core::packet_reader&): base=14 target=12
  [stmt]   bool survarium::lobby_client::read_status_info(vostok::network_core::packet_reader&): base=16 target=18
  [stmt]   unsigned char survarium::lobby_client::read_profile_content_info(vostok::network_core::packet_reader&): base=9 target=10
  [stmt]   bool survarium::lobby_client::read_enumerate_inventory_info(vostok::network_core::packet_reader&): base=6 target=7
  [stmt]   unsigned char survarium::lobby_client::read_price_items(vostok::network_core::packet_reader&): base=8 target=10
  [stmt]   bool survarium::lobby_client::read_profile_slots_restrictions(vostok::network_core::packet_reader&): base=7 target=8
  [stmt]   bool survarium::lobby_client::read_items_compatibility(vostok::network_core::packet_reader&): base=7 target=8
  [stmt]   bool survarium::lobby_client::read_player_skills(vostok::network_core::packet_reader&): base=15 target=17
  [stmt]   bool survarium::lobby_client::read_player_skills_tree(vostok::network_core::packet_reader&): base=3 target=4
  [stmt]   bool survarium::lobby_client::read_service_prices(vostok::network_core::packet_reader&): base=6 target=4
  [stmt]   bool survarium::lobby_client::read_player_reputations(vostok::network_core::packet_reader&): base=7 target=8

vostok/game/sources/lobby_menu.cpp
  [stmt]   survarium::lobby_menu::lobby_menu(survarium::game&): base=5 target=6
  [stmt]   void survarium::lobby_menu::~lobby_menu(): base=10 target=11
  [stmt]   void survarium::lobby_menu::show_ui(bool): base=8 target=15
  [stmt]   void survarium::lobby_menu::query_lobby_info(): base=9 target=10
  [stmt]   void survarium::lobby_menu::on_client_status_received(lobby::query_info_types): base=52 target=56
  [stmt]   void survarium::lobby_menu::on_operation_permitted_received(vostok::lobby_client_message_types_enum): base=10 target=11
  [stmt]   void survarium::lobby_menu::on_operation_denied_received(vostok::lobby_client_message_types_enum, char const*): base=10 target=28
  [stmt]   void survarium::lobby_menu::request_friends_status_from_server(unsigned int): base=4 target=5

vostok/game/sources/lobby_menu_input.cpp
  [stmt]   bool survarium::lobby_menu::on_keyboard_action(vostok::input::world*, vostok::input::enum_keyboard, vostok::input::enum_keyboard_action): base=28 target=38
  [stmt]   bool survarium::lobby_menu::on_mouse_key_action(vostok::input::world*, vostok::input::mouse_button, vostok::input::enum_mouse_key_action): base=13 target=16

vostok/game/sources/lobby_menu_scene.cpp
  [stmt]   void survarium::lobby_menu::clear_resources(): base=2 target=14
  [stmt]   void survarium::lobby_menu::query_scene_resources(): base=2 target=18
  [stmt]   void survarium::profile_character::character_animation_ready(vostok::resources::queries_result&): base=7 target=8

vostok/game/sources/lobby_menu_ui.cpp
  [stmt]   survarium::relocate_item_func::relocate_item_func(survarium::game&): base=4 target=1
  [stmt]   void survarium::relocate_item_func::call(survarium::flash_function_handler_params&): base=2 target=37
  [const]  void survarium::lobby_menu_external_handler::callback(survarium::flash_movie*, char const*, survarium::flash_value const*, unsigned int)
    only-tgt   lobby_menu_external_handler::callback::__l60::< unnamed-tag > c_min_name_len_to_search = 3
  [stmt]   void survarium::lobby_menu::show_match_making(bool): base=26 target=32
  [stmt]   void survarium::lobby_menu::update_status(): base=11 target=19
  [stmt]   void survarium::lobby_menu::on_render_scenes_ready(vostok::resources::queries_result&): base=2 target=57
  [stmt]   void survarium::lobby_menu::fill_inventory_labels(): base=7 target=8
  [stmt]   void survarium::lobby_menu::fill_items_dictionary(): base=2 target=76
  [stmt]   void survarium::lobby_menu::on_items_compatibility_arrived(): base=12 target=16
  [stmt]   void survarium::lobby_menu::on_slot_restrictions_arrived(): base=12 target=16
  [stmt]   void survarium::lobby_menu::fill_profiles(): base=12 target=16
  [stmt]   void survarium::lobby_menu::on_profile_changed(unsigned char): base=2 target=36
  [stmt]   void survarium::lobby_menu::on_price_items_arrived(unsigned char): base=2 target=32
  [stmt]   void survarium::lobby_menu::reset_account_money(): base=10 target=11
  [stmt]   void survarium::lobby_menu::fill_skills_tree(): base=2 target=81
  [stmt]   void survarium::lobby_menu::fill_character_data(): base=30 target=35
  [stmt]   void survarium::lobby_menu::fill_service_prices(): base=4 target=6
  [stmt]   void survarium::lobby_menu::fill_friend_list(): base=19 target=20
  [stmt]   void survarium::lobby_menu::fill_ignore_list(): base=15 target=18
  [stmt]   void survarium::lobby_menu::fill_found_players(): base=14 target=16
  [stmt]   void survarium::lobby_menu::on_match_message_arrived(wchar_t const*): base=27 target=33
  [stmt]   void survarium::lobby_menu::on_stats_message_arrived(wchar_t const*, wchar_t const*, messaging::message_channel_enum): base=23 target=28
  [stmt]   void survarium::lobby_menu::on_player_reputations_arrived(): base=10 target=18

vostok/game/sources/login_menu.cpp
  [stmt]   void survarium::login_menu::clear_resources(): base=2 target=4
  [stmt]   void survarium::login_menu::set_status(survarium::login_menu_status_enum): base=32 target=33

vostok/game/sources/login_menu_input.cpp
  [stmt]   bool survarium::login_menu::on_mouse_key_action(vostok::input::world*, vostok::input::mouse_button, vostok::input::enum_mouse_key_action): base=8 target=4
  [stmt]   bool survarium::login_menu::on_mouse_move(vostok::input::world*, int, int, int): base=11 target=10

vostok/game/sources/login_menu_ui.cpp
  [stmt]   void survarium::login_menu_external_handler::callback(survarium::flash_movie*, char const*, survarium::flash_value const*, unsigned int): base=2 target=20
  [stmt]   void survarium::login_menu::on_resources_ready(vostok::resources::queries_result&): base=2 target=20

vostok/game/sources/messaging_client.cpp
  [stmt]   survarium::messaging_client::messaging_client(survarium::game&): base=8 target=7

vostok/game/sources/messaging_client_process_messagess.cpp
  [stmt]   void survarium::messaging_client::on_packet_received(vostok::network_core::packet_reader&): base=32 target=42
  [stmt]   messaging::message_channel_enum survarium::messaging_client::parse_receiver_channel(wchar_t const*, const bool): base=19 target=18
  [stmt]   void survarium::messaging_client::on_message_typed(wchar_t const*, messaging::message_channel_enum): base=29 target=35
  [stmt]   bool survarium::messaging_client::accept_message_from(const unsigned int, messaging::client_type_enum): base=3 target=7
  [stmt]   void survarium::messaging_client::process_incoming_text_message(vostok::network_core::packet_reader&): base=18 target=19

vostok/game/sources/network_client.cpp
  [stmt]   survarium::network_client::network_client(survarium::game&, const bool): base=11 target=10
  [stmt]   void survarium::network_client::~network_client(): base=2 target=7
  [stmt]   void survarium::network_client::on_connected_to_match(const vostok::connection_error_types_enum, const vostok::handshaking_error_types_enum, const vostok::socket_error_types_enum, const vostok::lobby_server_message_types_enum): base=31 target=30
  [stmt]   void survarium::network_client::on_connected_to_login(const vostok::connection_error_types_enum, const vostok::handshaking_error_types_enum, const vostok::socket_error_types_enum, const vostok::login_server_message_types_enum): base=44 target=43
  [stmt]   bool survarium::network_client::http_query_server_connection_info(unsigned char): base=3 target=20
  [stmt]   void survarium::network_client::on_http_result_ready(char const*, unsigned char): base=19 target=20
  [stmt]   void survarium::network_client::draw_stats(unsigned int): base=2 target=48

vostok/game/sources/network_client_lobby.cpp
  [stmt]   void survarium::network_client::on_lobby_packet_received(vostok::network_core::packet_reader&): base=62 target=80
  [stmt]   void survarium::network_client::on_connected_to_lobby(): base=6 target=3
  [stmt]   void survarium::network_client::on_disconnected_from_lobby(): base=4 target=3
  [stmt]   void survarium::network_client::process_shop_action(vostok::network_core::packet_reader&): base=14 target=17

vostok/game/sources/network_client_processing.cpp
  [stmt]   void survarium::network_client::destroy_player_impl(const unsigned char): base=11 target=12
  [stmt]   void survarium::network_client::query_players(): base=19 target=21
  [stmt]   void survarium::network_client::process_player_hit(vostok::network_core::packet_reader&): base=6 target=7
  [stmt]   void survarium::network_client::process_affect_damage_model(vostok::network_core::packet_reader&): base=8 target=9
  [stmt]   void survarium::network_client::process_initialize_victory_items(vostok::network_core::packet_reader&): base=37 target=34
  [stmt]   void survarium::network_client::process_respawn_timer(vostok::network_core::packet_reader&): base=3 target=4
  [stmt]   void survarium::network_client::process_match_wait_timer(vostok::network_core::packet_reader&): base=4 target=5
  [stmt]   void survarium::network_client::process_player_kd_stats(vostok::network_core::packet_reader&): base=6 target=7
  [stmt]   void survarium::network_client::process_victory_item_take_or_put(vostok::network_core::packet_reader&): base=34 target=37
  [stmt]   void survarium::network_client::send_sync_request(): base=5 target=6
  [stmt]   void survarium::network_client::send_player_inputs(): base=6 target=7
  [stmt]   void survarium::network_client::tick(unsigned int, const bool): base=50 target=53
  [stmt]   void survarium::network_client::player_visibility_change(vostok::network_core::packet_reader&): base=8 target=15
  [stmt]   void survarium::network_client::game_world_object_state_arrived(vostok::network_core::packet_reader&): base=4 target=5
  [stmt]   void survarium::network_client::on_world_sync_request(): base=19 target=20
  [stmt]   void survarium::network_client::damage_model_state_arrived(vostok::network_core::packet_reader&): base=4 target=5

vostok/game/sources/network_stats.cpp
  [stmt]   void survarium::stats_row::set_text(const unsigned int, vostok::network_core::udp_match_stream_stats const&, vostok::network_core::udp_match_stream_stats const&): base=16 target=17

vostok/game/sources/object.cpp
  [stmt]   survarium::game_object_::game_object_(survarium::base_game_scene&): base=2 target=1
  [stmt]   survarium::game_object_static::game_object_static(survarium::base_game_scene&): base=2 target=1

vostok/game/sources/object_decal.cpp
  [stmt]   void survarium::object_decal::load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&): base=20 target=19

vostok/game/sources/object_environment.cpp
  [fn-order]
    moved      void survarium::object_environment::insert()
  [stmt]   void survarium::object_environment::load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&): base=5 target=4

vostok/game/sources/object_environment_probe.cpp
  [stmt]   void survarium::object_environment_probe::insert(): base=2 target=10
  [stmt]   void survarium::object_environment_probe::remove(): base=2 target=3

vostok/game/sources/object_sky.cpp
  [stmt]   survarium::object_sky::object_sky(survarium::base_game_scene&): base=2 target=1
  [stmt]   void survarium::object_sky::~object_sky(): base=2 target=1
  [stmt]   void survarium::object_sky::load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&): base=8 target=7

vostok/game/sources/object_sky_ambient_occlusion.cpp
  [stmt]   void survarium::object_sky_ambient_occlusion::insert(): base=2 target=7
  [stmt]   void survarium::object_sky_ambient_occlusion::remove(): base=2 target=3

vostok/game/sources/object_solid_visual.cpp
  [stmt]   survarium::object_particle_visual::object_particle_visual(survarium::base_game_scene&): base=2 target=1

vostok/game/sources/object_vegetation.cpp
  [stmt]   survarium::object_vegetation::object_vegetation(survarium::base_game_scene&): base=2 target=1
  [stmt]   void survarium::object_vegetation::load(vostok::configs::binary_config_value const&, char const*, boost::function<void __cdecl(survarium::game_object_ &)>&): base=2 target=8
  [stmt]   void survarium::object_vegetation::insert(): base=2 target=3
  [stmt]   void survarium::object_vegetation::remove(): base=2 target=3

vostok/game/sources/object_wire.cpp
  [stmt]   survarium::object_wire::object_wire(survarium::base_game_scene&): base=2 target=1
  [stmt]   void survarium::object_wire::resources_ready(vostok::resources::queries_result&, boost::function<void __cdecl(survarium::game_object_ &)>&): base=5 target=7

vostok/game/sources/options_items.cpp
  [stmt]   survarium::options_item_int::options_item_int(survarium::options_tab&, char const*, unsigned char, char const**, unsigned char): base=2 target=3
  [stmt]   void survarium::options_item_int::initialize(): base=7 target=8
  [stmt]   void survarium::options_item_float::initialize(): base=7 target=8
  [stmt]   void survarium::options_item_float::fill_data(survarium::flash_value&): base=7 target=8
  [stmt]   void survarium::options_item_bool::initialize(): base=7 target=8
  [stmt]   void survarium::options_item_bool::call(survarium::flash_function_handler_params&): base=3 target=4
  [stmt]   void survarium::options_gamma_selector::call(survarium::flash_function_handler_params&): base=2 target=4
  [stmt]   void survarium::options_gamma_selector::revert(): base=2 target=4
  [stmt]   survarium::options_resolution_selector::options_resolution_selector(survarium::options_tab&): base=3 target=5
  [stmt]   void survarium::options_resolution_selector::fill_resolutions(unsigned char): base=2 target=29
  [stmt]   void survarium::options_resolution_selector::initialize(): base=9 target=11
  [stmt]   survarium::options_monitor_index_selector::options_monitor_index_selector(survarium::options_tab&): base=2 target=7
  [stmt]   void survarium::options_tab::~options_tab(): base=5 target=6

vostok/game/sources/player.cpp
  [stmt]   survarium::player::player(survarium::player_creation_params const&): base=2 target=14
  [stmt]   void survarium::player::~player(): base=5 target=6
  [stmt]   void survarium::player::insert(const bool): base=2 target=28
  [stmt]   void survarium::player::remove(): base=19 target=20
  [stmt]   void survarium::player::apply_input(survarium::client_player_state&, vostok::math::float2 const&): base=6 target=7
  [stmt]   void survarium::player::compute_bones(const unsigned int): base=2 target=11
  [stmt]   void survarium::player::render(const unsigned int, const unsigned int): base=2 target=34
  [stmt]   void survarium::player::render_crosshair_info(): base=2 target=9
  [stmt]   void survarium::player::set_physics_controller_walk_vector(survarium::client_player_state&): base=5 target=6
  [stmt]   void survarium::player::detect_usable_objects(const unsigned int): base=2 target=31
  [stmt]   void survarium::player::apply_hit_directly(survarium::hit_info const&, unsigned int): base=2 target=14
  [stmt]   void survarium::player::update_camera(): base=19 target=20
  [stmt]   float survarium::player::get_speed() const: base=3 target=5
  [stmt]   void survarium::player::hide(): base=3 target=4
  [stmt]   void survarium::player::show(): base=3 target=4
  [stmt]   void survarium::player::subscribe_on_actions(survarium::player_actions_subscriber*): base=3 target=5
  [stmt]   void survarium::player::notify_actions_subscribers(): base=4 target=14
  [stmt]   void survarium::player::on_fire(): base=3 target=6
  [stmt]   bool survarium::player::set_new_active_item(vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const&): base=6 target=7
  [stmt]   void survarium::player::take_inventory_item(vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const&): base=8 target=7
  [stmt]   vostok::animation::skeleton const& survarium::player::skeleton() const: base=2 target=3
  [stmt]   void survarium::player::process_quick_slots_for_proxy_player(): base=32 target=34
  [stmt]   void survarium::player::process_quick_slots_for_current_player(): base=46 target=54
  [stmt]   void survarium::player::select_animations(const unsigned int): base=2 target=9
  [stmt]   void survarium::player::kill(const unsigned int): base=11 target=12
  [stmt]   void survarium::player::set_head_visibility(bool): base=2 target=11
  [stmt]   void survarium::player::deserialize(vostok::network_core::packet_reader&): base=21 target=22

vostok/game/sources/player_cook.cpp
  [stmt]   void survarium::player_cook::on_config_loaded(vostok::resources::queries_result&): base=3 target=47
  [stmt]   void survarium::player_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::player_creation_params*, survarium::inventory_cooker_data*, survarium::player_parameters_cooker_data*): base=9 target=19
  [stmt]   void survarium::player_cook::on_hit_params_loaded(vostok::resources::queries_result&, survarium::player_creation_params*): base=9 target=12
  [stmt]   survarium::profile_skin_visual_cook::profile_skin_visual_cook(survarium::game&): base=2 target=3
  [stmt]   void survarium::profile_skin_visual_cook::translate_query(vostok::resources::query_result_for_cook&): base=3 target=13

vostok/game/sources/player_input_handler.cpp
  [stmt]   bool survarium::player_input_handler::on_keyboard_action(vostok::input::world*, vostok::input::enum_keyboard, vostok::input::enum_keyboard_action): base=22 target=25
  [stmt]   bool survarium::player_input_handler::on_mouse_key_action(vostok::input::world*, vostok::input::mouse_button, vostok::input::enum_mouse_key_action): base=15 target=14
  [stmt]   void survarium::player_input_handler::update_inverted_view(vostok::math::float4x4 const&): base=7 target=10

vostok/game/sources/player_logic_preview_state.cpp
  [stmt]   stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_preview_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const: base=5 target=4

vostok/game/sources/player_tick.cpp
  [stmt]   void survarium::player::update_history_item_from_previous(survarium::client_player_history_item const&, survarium::client_player_history_item&, vostok::math::float4x4&): base=10 target=17
  [stmt]   void survarium::player::time_warp(survarium::server_player_update const&, unsigned int): base=25 target=28
  [stmt]   void survarium::player::apply_input_before_new_transform(survarium::client_player_state&, survarium::player_input const&, const float): base=13 target=14
  [stmt]   void survarium::player::smooth(const float): base=17 target=18
  [stmt]   void survarium::player::tick(const unsigned int): base=67 target=88

vostok/game/sources/project_cooker_simple.cpp
  [stmt]   void survarium::project_cooker_simple::on_game_project_loaded(vostok::resources::queries_result&, vostok::resources::query_result_for_cook*): base=5 target=4
  [stmt]   void survarium::project_cooker_simple::create_game_objects(vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, vostok::resources::query_result_for_cook*): base=2 target=139
  [stmt]   void survarium::project_cooker_simple::delete_resource(vostok::resources::resource_base*): base=3 target=26

vostok/game/sources/rifle_scope_cook.cpp
  [stmt]   void survarium::rifle_scope_cook::on_subresources_loaded(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> const&): base=5 target=7

vostok/game/sources/shotgun_weapon_reload_state_cook.cpp
  [stmt]   void survarium::shotgun_weapon_reload_state_cook::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::mutable_buffer): base=11 target=14

vostok/game/sources/simple_animation_controller.cpp
  [stmt]   void survarium::simple_animation_controller::query_new_target_if_needed(): base=8 target=6
  [stmt]   vostok::animation::mixing::expression survarium::simple_animation_controller::selected_animations(vostok::mutable_buffer&): base=10 target=8

vostok/game/sources/single_position_animation_controller.cpp
  [stmt]   void survarium::single_position_animation_controller::query_new_target_if_needed(): base=8 target=6
  [stmt]   vostok::animation::mixing::expression survarium::single_position_animation_controller::selected_animations(vostok::mutable_buffer&): base=3 target=48

vostok/game/sources/stats_graph.cpp
  [stmt]   void survarium::stats_graph::add_value(float, float): base=26 target=27
  [stmt]   float survarium::stats_graph::average_value() const: base=6 target=7

vostok/game/sources/step_manager.cpp
  [stmt]   void survarium::step_manager::on_step(survarium::player const&, vostok::math::float3 const&, vostok::math::float3 const&, survarium::game_world&) const: base=17 target=19

vostok/game/sources/swf_input_translator.cpp
  [stmt]   bool survarium::swf_input_translator::process_keyboard(vostok::input::world*, vostok::input::enum_keyboard, vostok::input::enum_keyboard_action, survarium::flash_movie*, unsigned int): base=27 target=30

vostok/game/sources/victory_item.cpp
  [stmt]   char const* survarium::victory_item::use_info(survarium::usable_object_user_data*): base=9 target=10

vostok/game/sources/victory_item_cooker.cpp
  [stmt]   void survarium::victory_item_cook::on_config_loaded(vostok::resources::queries_result&): base=8 target=9
  [stmt]   void survarium::victory_item_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::victory_item*): base=11 target=12

vostok/game/sources/weapon.cpp
  [stmt]   survarium::weapon::weapon(unsigned int, unsigned int, unsigned int): base=18 target=8
  [stmt]   void survarium::weapon::set_transform(vostok::math::float4x4 const&): base=2 target=3
  [stmt]   void survarium::weapon::load_weapon(vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::rifle_scope,vostok::resources::unmanaged_resource> const&): base=2 target=18
  [stmt]   vostok::math::float4x4 survarium::weapon::calculate_locator(vostok::render::model_locator_item const&, vostok::math::float4x4 const*, const unsigned int): base=6 target=8
  [stmt]   void survarium::weapon::on_hide(): base=11 target=13
  [stmt]   void survarium::weapon::set_ui_ammo(bool): base=9 target=10
  [stmt]   void survarium::weapon::on_reload(): base=2 target=3
  [stmt]   void survarium::weapon::on_chamber_a_round(): base=4 target=3
  [stmt]   void survarium::weapon::on_unload_chambered_round(): base=4 target=3
  [stmt]   void survarium::weapon::set_target(const survarium::weapon_targets): base=12 target=11
  [stmt]   void survarium::weapon::activate(survarium::base_player&, survarium::engine&): base=2 target=15
  [stmt]   vostok::animation::callback_return_type_enum survarium::weapon::on_foot_step(vostok::animation::animation_callback_params&): base=6 target=8
  [stmt]   void survarium::weapon::on_skeleton_matrices_changed(const unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&): base=2 target=48
  [stmt]   vostok::animation::callback_return_type_enum survarium::weapon::on_hand_correction_event(vostok::animation::animation_callback_params&, const survarium::fingers_to_weapon_corrector::hands_enum): base=4 target=5
  [stmt]   void survarium::weapon::on_user_sprint(bool): base=5 target=6

vostok/game/sources/weapon_cook.cpp
  [stmt]   static void survarium::weapon_cook::register_cooks_for_logic_states(): base=2 target=30
  [stmt]   void survarium::weapon_cook::on_weapon_config_loaded(vostok::resources::queries_result&): base=2 target=36
  [stmt]   void survarium::weapon_cook::on_weapon_subresources_ready(vostok::resources::queries_result&, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>, survarium::weapon_core*): base=2 target=24
  [stmt]   void survarium::weapon_cook::delete_resource(vostok::resources::resource_base*): base=10 target=11

vostok/game_core/sources/artefact_container_core.cpp
  [stmt]   survarium::artefact_container_core::artefact_container_core(): base=2 target=1

vostok/game_core/sources/base_project.cpp
  [stmt]   survarium::base_project::base_project(): base=2 target=1

vostok/game_core/sources/body_part_parameters.cpp
  [stmt]   void survarium::body_part_parameters::dump_state(vostok::ai::npc_statistics&, const unsigned int) const: base=4 target=5

vostok/game_core/sources/character_dispersion_params.cpp
  [stmt]   survarium::character_dispersion_params::character_dispersion_params(): base=2 target=1

vostok/game_core/sources/collision_geometry.cpp
  [stmt]   survarium::collision_geometry::collision_geometry(): base=2 target=1

vostok/game_core/sources/collision_sensor.cpp
  [stmt]   survarium::collision_sensor::collision_sensor(): base=2 target=1

vostok/game_core/sources/damage_zone_core.cpp
  [stmt]   survarium::hit_receiver_info::hit_receiver_info(survarium::hit_receiver*, vostok::physics::base_physics_object*): base=2 target=1
  [stmt]   survarium::damage_zone_core::damage_zone_core(): base=2 target=1
  [stmt]   void survarium::damage_zone_core::~damage_zone_core(): base=2 target=1

vostok/game_core/sources/game_material_manager_cook.cpp
  [stmt]   void survarium::game_material_manager_cook::on_decals_loaded(vostok::resources::queries_result&, survarium::vector<survarium::game_material_manager_cook::query_ext_data>*): base=22 target=23

vostok/game_core/sources/generic_anomaly_core.cpp
  [stmt]   survarium::generic_anomaly_core::generic_anomaly_core(): base=2 target=1
  [stmt]   void survarium::generic_anomaly_core::~generic_anomaly_core(): base=2 target=1

vostok/game_core/sources/inventory_item.cpp
  [stmt]   survarium::inventory_item::inventory_item(survarium::inventory_item::action_behaviour_type): base=2 target=1

vostok/game_core/sources/jump_logic.cpp
  [const]  unsigned int survarium::get_jump_animation_index(const survarium::move_direction_enum, const bool, const survarium::jump_animation_parts)
    only-tgt   get_jump_animation_index::__l2::< unnamed-tag > animations_per_simple_jump_direction = 4
    only-tgt   get_jump_animation_index::__l2::< unnamed-tag > animations_per_two_legged_jump_direction = 6

vostok/game_core/sources/material_pair.cpp
  [stmt]   void survarium::material_pair::~material_pair(): base=2 target=1

vostok/game_core/sources/medkit.cpp
  [stmt]   survarium::medkit::medkit(): base=2 target=1

vostok/game_core/sources/oxygen_tank.cpp
  [stmt]   survarium::oxygen_tank::oxygen_tank(): base=2 target=1

vostok/game_core/sources/player_logic_crouch_state.cpp
  [stmt]   vostok::animation::mixing::expression survarium::player_logic_crouch_state::get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const: base=8 target=12
  [stmt]   vostok::animation::mixing::expression survarium::player_logic_crouch_state::look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const: base=25 target=26

vostok/game_core/sources/player_logic_sprint_state.cpp
  [const]  stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_sprint_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
    only-tgt   player_logic_sprint_state::selected_animations::__l2::< unnamed-tag > main_sprint_animation_index = 0
    only-tgt   player_logic_sprint_state::selected_animations::__l2::< unnamed-tag > sprint_look_animation_index = 1

vostok/game_core/sources/player_logic_stand_state.cpp
  [stmt]   vostok::animation::mixing::expression survarium::player_logic_stand_state::get_recoil_animation_lexeme(survarium::animation_type_enum, const bool, const float, vostok::animation::base_interpolator const&, vostok::mutable_buffer&, const bool, const unsigned int, fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> const&) const: base=8 target=12
  [stmt]   vostok::animation::mixing::expression survarium::player_logic_stand_state::look_expression(vostok::mutable_buffer&, const unsigned int, const bool, const bool, survarium::weapon_animation_parameters const&, vostok::animation::mixing::animation_lexeme&) const: base=25 target=27

vostok/game_core/sources/player_parameters_cook.cpp
  [stmt]   void survarium::player_parameters_modifyer::apply(survarium::base_player*): base=51 target=52

vostok/game_core/sources/respawn_point.cpp
  [stmt]   survarium::respawn_point_core::respawn_point_core(): base=2 target=1

vostok/game_core/sources/weapon_ammunition.cpp
  [stmt]   survarium::weapon_ammunition::weapon_ammunition(): base=2 target=1

vostok/game_core/sources/weapon_core.cpp
  [stmt]   void survarium::weapon_core::on_skeleton_matrices_changed(const unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&): base=2 target=3
  [stmt]   void survarium::weapon_core::process_finger_correction(const unsigned int, vostok::math::float4x4* const): base=2 target=3

vostok/game_core/sources/weapon_core_shotgun_reload_state_cook.cpp
  [const]  void survarium::weapon_core_shotgun_reload_state_cook::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::mutable_buffer)
    only-tgt   weapon_core_shotgun_reload_state_cook::create_resource::__l65::< unnamed-tag > total_weapon_anim_count = 12
    only-tgt   weapon_core_shotgun_reload_state_cook::create_resource::__l65::< unnamed-tag > total_user_anim_count = 12

vostok/game_core/sources/weapon_dispersion_calculator.cpp
  [stmt]   void survarium::weapon_dispersion_calculator::tick(const unsigned int): base=15 target=16

vostok/game_core/sources/weapon_lexeme_pair.cpp
  [stmt]   survarium::weapon_lexeme_pair survarium::get_weapon_lexeme_pair_impl(vostok::mutable_buffer&, char const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, void const*, vostok::animation::animation_playback_state const&, const unsigned int, const float, const vostok::animation::mixing::playback_enum, vostok::animation::base_interpolator const&): base=22 target=23

vostok/game_core/sources/weapon_user_animations_selector.cpp
  [const]  vostok::animation::callback_return_type_enum survarium::weapon_user_animations_selector::on_interval_ended(vostok::animation::animation_callback_params&)
    only-base  weapon_user_animations_selector::on_interval_ended::__l2::< unnamed-tag > c_right_leg_interval_id = 0
    only-tgt   weapon_user_animations_selector::on_interval_ended::__l9::< unnamed-tag > c_right_leg_interval_id = 0

vostok/input/sources/input_world.cpp
  [stmt]   void vostok::input::input_world::add_handler(vostok::input::handler&): base=5 target=7

vostok/input/sources/receiver_keyboard.cpp
  [stmt]   void vostok::input::receiver::keyboard::process(vostok::input::vector<vostok::input::handler *>&): base=18 target=17

vostok/input/sources/receiver_keyboard_win.cpp
  [fn-order]
    moved      void vostok::input::receiver::keyboard::reset_current_state()
  [stmt]   void vostok::input::receiver::keyboard::execute(): base=27 target=29

vostok/network_core/sources/http_client.cpp
  [stmt]   vostok::network_core::http_client::http_client(boost::asio::io_service&): base=2 target=1

vostok/network_core/sources/network_core_entry_point.cpp
  [stmt]   stlp_std::basic_string<char,stlp_std::char_traits<char>,stlp_std::allocator<char> > vostok::network_core::get_ip_address(boost::asio::io_service&): base=12 target=13

vostok/particle/sources/particle_beam_emitter_instance.cpp
  [stmt]   void vostok::particle::particle_beam_emitter_instance::tick(float, bool, float): base=54 target=46

vostok/particle/sources/particle_emitter_instance.cpp
  [stmt]   void vostok::particle::particle_emitter_instance::tick(float, bool, float): base=64 target=78

vostok/particle/sources/particle_system_instance_cook.cpp
  [stmt]   void vostok::particle::particle_system_instance_cook::translate_query(vostok::resources::query_result_for_cook&): base=7 target=3

vostok/particle/sources/particle_system_instance_impl.cpp
  [stmt]   bool vostok::particle::particle_system_instance_impl::is_finished(): base=42 target=44
  [stmt]   void vostok::particle::particle_system_instance_impl::add_emitter_instance(unsigned int, vostok::particle::particle_emitter_instance*): base=5 target=7

vostok/particle/sources/particle_system_wrapper_cook.cpp
  [stmt]   void vostok::particle::particle_system_wrapper_cook::translate_query(vostok::resources::query_result_for_cook&): base=9 target=5

vostok/particle/sources/particle_world.cpp
  [stmt]   void vostok::particle::particle_world::play(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>, vostok::math::float4x4 const&, bool, bool): base=11 target=12
  [stmt]   void vostok::particle::particle_world::add_particle_system_instance(vostok::particle::particle_system_instance*): base=5 target=6

vostok/physics/sources/animated_rigid_body.cpp
  [stmt]   btCompoundShape* vostok::physics::new_bt_element_joint(vostok::configs::binary_config_value const&, vostok::memory::base_allocator*, vostok::collision::bone_collision_data*): base=12 target=11
  [stmt]   vostok::collision::animated_object* vostok::physics::new_animated_bt_hit_model(vostok::configs::binary_config_value const&, vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::memory::base_allocator*): base=9 target=7

vostok/physics/sources/collision_shape_cook.cpp
  [stmt]   void vostok::physics::collision_shape_cook::on_collision_sources_loaded(vostok::resources::queries_result&, vostok::physics::collision_shape_cook::cook_data*): base=37 target=71

vostok/physics/sources/collision_shapes.cpp
  [stmt]   vostok::physics::btBvhTriangleMeshShapeResource::btBvhTriangleMeshShapeResource(btStridingMeshInterface*, unsigned short*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&): base=2 target=1
  [stmt]   void vostok::physics::destroy_bt_shape(btCollisionShape*): base=11 target=12
  [stmt]   btCollisionShape* vostok::physics::create_bt_primitive(vostok::collision::primitive_type, vostok::math::float3 const&, vostok::math::float3 const&): base=17 target=18
  [stmt]   vostok::physics::bt_collision_shape* vostok::physics::create_compound_shape(vostok::configs::binary_config_value const&, vostok::math::float3 const&, char const*): base=25 target=23
  [stmt]   vostok::physics::bt_collision_shape* vostok::physics::create_static_triangle_mesh_shape(vostok::math::float3*, unsigned int*, unsigned int, unsigned int, unsigned short*, vostok::math::float3 const&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&): base=7 target=5

vostok/physics/sources/ghost_object.cpp
  [stmt]   vostok::physics::bt_ghost_object::bt_ghost_object(vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base>, btPairCachingGhostObject*): base=2 target=3
  [stmt]   vostok::physics::bt_ghost_object* vostok::physics::create_ghost_object(vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base>, vostok::math::float4x4 const&): base=8 target=6
  [stmt]   void vostok::physics::destroy_ghost_object(vostok::physics::bt_ghost_object*): base=5 target=4
  [stmt]   void vostok::physics::bt_ghost_object::non_compound_shapes_centers(vostok::vectora<vostok::math::float3>&) const: base=4 target=5

vostok/physics/sources/rigid_body_construction_info.cpp
  [stmt]   vostok::physics::bt_rigid_body_construction_info::bt_rigid_body_construction_info(): base=2 target=1

vostok/physics/sources/static_rigid_body.cpp
  [stmt]   vostok::physics::bt_static_rigid_body* vostok::physics::create_static_rigid_body(vostok::physics::bt_rigid_body_construction_info const&): base=15 target=16

vostok/survarium/game_module_proxy.cpp
  [stmt]   void survarium::game_module_proxy::destroy_world(vostok::engine_user::world*&): base=2 target=3

vostok/survarium/pc/sources/survarium_pc_application_win.cpp
  [stmt]   void splash_screen_main(): base=65 target=71
  [stmt]   void survarium::application::preinitialize(): base=6 target=7

vostok/survarium/pc/sources/survarium_pc_entry_point.cpp
  [stmt]   guard::guard(): base=4 target=5

vostok/ui/sources/ui_progress_bar.cpp
  [stmt]   void vostok::ui::ui_progress_bar::draw_text(vostok::render::ui::renderer&, vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const&) const: base=10 target=14

vostok/ui/sources/ui_text_edit.cpp
  [fn-order]
    moved      void vostok::ui::ui_text_edit::set_max_chars(const unsigned short)
  [stmt]   void vostok::ui::ui_text_edit::set_text(char const*): base=15 target=21
  [stmt]   void vostok::ui::ui_text_edit::draw_cursor(vostok::render::ui::renderer&, vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base> const&): base=7 target=8
  [stmt]   void vostok::ui::ui_text_edit::delete_left(): base=11 target=12

vostok/ui/sources/ui_text_edit_initialize.cpp
  [stmt]   void vostok::ui::ui_text_edit::init_internals(vostok::ui::enum_text_edit_mode): base=88 target=89

vostok/ui/sources/ui_world_factory.cpp
  [stmt]   vostok::ui::progress_bar* vostok::ui::ui_world::create_progress_bar(): base=5 target=3

vostok/vfs/sources/mount_archive.cpp
  [stmt]   void vostok::vfs::archive_mounter::mount_fat(vostok::vfs::archive_folder_mount_root_node<1>*, vostok::vfs::base_folder_node<1>*): base=27 target=31

vostok/vfs/sources/test_log.cpp
  [stmt]   void vostok::vfs::log_vfs_nodes(vostok::vfs::base_node<1>*, unsigned int, char const*): base=39 target=38

---- source summary ----
files:  675 compared, 213 diverged; base-only 60, target-only 26
        5 files w/ fn-order diff, 600 functions w/ stmt-count diff, 5 functions w/ const diff
        out-of-line presence: 551 base-only (we emit standalone; target inlines), 423 target-only (target emits standalone; we inline / no source)

```
