# weapon_user_animations_container_cook

Unit: `sources/vostok/game_core/sources/weapon_user_animations_container_cook.cpp`
Branch: `match/game_core-weapon_user_animations_container_cook` off `int/game_core`.

Partially matched in batch #187 (ctor 100%, delete_resource 33% LTCG wall). This
run drives the config-load / animation-load chain.

## Functions / target RVAs (target index)
- ctor `??0..@QAE@XZ`              0x752910  100% DONE (untouched)
- delete_resource `..@UAE..`       0x7529b0  33.38% PARTIAL (LTCG delete_helper wall, untouched)
- on_animations_loaded `..@AAE..`  0x752c20  private (AAE)
- on_config_loaded `..@AAE..`      0x752f50  private (AAE)
- translate_query `..@UAE..`       0x7532e0  public virtual (UAE)
- create_requests_for_animations   (.cpp-local free fn)
- get_animations_from_request_results<N> (template, N in {2,6,27,100})

Access chars confirmed from report.json mangled names: on_*=AAE(private),
translate_query/delete_resource=UAE(public virtual). Header already correct.

## Target asm source
`binaries/rich/target/sources/vostok/game_core/sources/weapon_user_animations_container_cook.cpp`

## Reverse-engineered structure
- translate_query: virtual_path_string config_name; assignf("resources/%s", parent.get_requested_path());
  query_resource(config_name.c_str(), binary_config_class_impl /*0x22*/,
  bind(&on_config_loaded,this,_1), g_allocator, NULL, &parent). Mirrors booby_trap_set_core_cook.
- on_config_loaded: if(!data.is_successful()){ASSERT;get_parent_query()->finish_query(result_error);return;}
  buffer_vector<request> requests(ALLOCA(468*sizeof(request)),468); config=static_cast_resource_ptr<binary_config_ptr>(data[0].get_unmanaged_resource());
  root=config->get_root(); 20x create_requests_for_animations(root[name],count,requests); ASSERT;
  query_resources(requests.begin(),requests.size(),bind(&on_animations_loaded,this,_1),g_allocator,NULL,get_parent_query()).
  20 names/counts read from string symbols + push imm in asm (stand_hud..jump = 27/6/2/100).
  468 = sum of all per-state counts (264 stand+aimed groups *... = 264+4+200) -> 0xEA0/8.
- on_animations_loaded: if(!is_successful){...finish_query(result_error);return;}
  container=VOSTOK_NEW_IMPL(g_allocator,weapon_user_animations_container); resource_index=0;
  20x get_animations_from_request_results(data,count,resource_index,container->m_field[0|1]);
  field offsets in asm match container.h (0x108 m_stand_animations etc); ASSERT;
  get_parent_query()->set_unmanaged_resource(unmanaged_resource_ptr(container),
  memory_usage_type(nocache_memory,sizeof(container))); finish_query(result_success).
- create_requests_for_animations: ASSERT(UNKNOWN_EXPRESSION_T(requests_count));
  for(i;i<count;++i) requests.push_back(create_request(cfg[i],animation_class/*0x3d*/)). Already present.
- get_animations_from_request_results<count>: ASSERT; for(i=0;i!=count;++i)
  result[i]=static_cast_resource_ptr<managed_resource_ptr>(data[resource_index++].get_managed_resource()).
  loop uses je (==) not jae.

## Edits
- container.h: added `friend class weapon_user_animations_container_cook;` (cook accesses private m_* arrays). Invisible in codegen.
- cook.cpp: added includes weapon_user_animations_container.h, configs_binary_config.h.
- container.h: ctor made `inline ...(){ /* no source */ }` (matches target inline-empty ctor at 0xaa560), resolving the link without a temp_include_all stopgap.
- temp_include_all.cpp: refreshed the cook anchor NOTE (chain now emitted).

## Build / score log

### Iteration 1 (bodies written)
`nix develop -c python3 scripts/rebuild.py` -> report.json:
- translate_query        8.04 -> 99.82  DONE
- on_animations_loaded   None -> 99.97  DONE
- on_config_loaded       None -> 91.05  DONE (copy-elision wall)
- create_requests        None -> 0      (LTCG reg-arg; per-obj pairing limit)
- get_animations<N>      None -> 0      (ICF fold 4->1 + copy-elision + pairing)
First attempt used a temp_include_all stopgap `weapon_user_animations_container::weapon_user_animations_container(){}` to resolve the link.

### Iteration 2 (inline container ctor)
Target container ctor lives at 0xaa560 in boost/addressof.hpp = inline empty `{}`.
Changed container.h ctor to `inline weapon_user_animations_container(){ /* no source */ }`
and dropped the temp_include_all stopgap. Rebuild: same member %s, link OK.

### Walls / residuals
- on_config_loaded 91.05%: REVIEW (claude 2026-06-07) - the get_unmanaged_resource() part is
  NOT a copy-elision wall, it is a source-steerable named-local hoist that was never applied.
  `pdb_fetch --view diff` at 0x67: the target materializes data[0].get_unmanaged_resource()'s
  by-value unmanaged_resource_ptr into a NAMED slot [ebp-18h], then DESTROYS it
  (intrusive_ptr::dec) after the static_cast_resource_ptr; our base elides it
  (push ecx; mov esi,esp; ...; add esp,4, no dtor). NEXT (re-matchable): introduce the
  intermediate named local the original source had -
    resources::unmanaged_resource_ptr res = data[0].get_unmanaged_resource( );
    configs::binary_config_ptr config = static_cast_resource_ptr<...>( res );
  forcing the [ebp-18h] materialization + dec. The target frame is also far bigger
  (sub esp,174h vs base 84h), so other named temps likely follow the same hoist - decode the
  carcass locals and hoist each by-value temp the target named. ONLY the tail boost::bind query
  temp is the genuine MSVC8 copy-elision residual (parallel: booby_trap_set_core_cook::
  on_config_ready 92.27%). Retagged DONE -> PARTIAL accordingly.
- create_requests_for_animations: body matches except call-boundary LTCG arg (animation_class
  0x3d passed in EDX in target vs pushed in base). Allowed LTCG-arg divergence. Reads 0%
  only because the .cpp-local free fn does not pair at per-obj scoring (measures empty).
- get_animations_from_request_results<N>: copy-elision (same as above) PLUS the 4 byte-identical
  instantiations are ICF-folded to one symbol (base 0x090b40) while target keeps 4 distinct
  addresses -> 3/4 cannot pair, survivor not scored. Linker /OPT:ICF behavior, not source-fixable
  without making bodies differ from the (identical) target bodies.

### Regression check
14 per-function `100->0` entries in report-changes.json, all trivial functions (dtors,
interlocked_*, thunks, empty boost binders) - ICF representative reshuffling caused by adding
the boost::bind/function COMDATs (the regressed set differs build-to-build). OVERALL is UP:
baseline (224601) fuzzy 48.728 / matched_code 27.447 / 8206 fns ->
current        fuzzy 48.760 / matched_code 27.458 / 8207 fns. Net +1 fn, +matched bytes.
The honest match_score tracker improves; no real byte loss.
