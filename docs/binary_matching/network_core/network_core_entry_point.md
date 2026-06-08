# network_core_entry_point.cpp - matching trail

Unit: 5 free functions in `vostok::network_core` (entry_point.cpp).
Branch: `match/network_core-entry_point` off `match/network_core-async_connector-match`.

## Setup
- Enabled the TU: cleared `ExcludedFromBuild` in `network_core.vcproj`, `regen_ninja.py`.
- Created `sources/.../network_core_entry_point.h` declaring the 5 fns.
- Anchored `use_network_core_entry_point()` in game_core temp_include_all.cpp.

## Target symbols (rich index binaries/rich/target/index.jsonl)
- 0x576f80 `?finalize@network_core@vostok@@YAXXZ`              - 2 stmts, 5 bytes (empty)
- 0x576f90 `?initialize@network_core@vostok@@YAXXZ`            - 2 stmts, 5 bytes (empty)
- 0x576fa0 `?memory_allocator@...@@YAXAAVbase_allocator@memory@2@@Z` - 4 stmts, 0x1d
- 0x576fc0 `?get_connection_info_from_string@...@@YA_NPBDQADAAG@Z`  - 10 stmts, 0x7b
- 0x577040 `?get_ip_address@...@@YA?AV?$basic_string@...@AAVio_service@asio@boost@@@Z` - 13 stmts, 0x2a4 (heavy boost.asio)

## memory_allocator
asm: `mov byte[ebp-1],0; lea eax,[ebp-1]; call finalize_impl` (<0xc> at L18) = compiled-out ASSERT
(assembly_patterns.md L221). Then `mov ecx,[ebp+8]; mov [g_allocator],ecx` = `g_allocator = &allocator`.
Source: `ASSERT( UNKNOWN_EXPRESSION ); g_allocator = &allocator;`

## get_connection_info_from_string
LOCALS: delim (char const*, [ebp-4]), result (s32, [ebp-0Ch]), port (u32, [ebp-8]).
- L157 delim = strchr(buffer, ':')   (0x3A)
- L158 if (delim) { ... }            (cmp [ebp-4],0; je return-false)  [braced block]
- L160 strncpy_s(dest_host, 0x40=64, buffer, delim - buffer)
- L162 result = sscanf_s(delim+1, "%d", &port)
- L163 if (strings::length(dest_host) && result == 1)  (length!=0; je; cmp result,1; jne)
- L165 dest_port = (u16)port
- L166 return true
- L169 return false

## get_ip_address
LOCALS: query, end, resolver, iter, addr<1>. Heavy boost.asio template-inlined.
- L135 resolver(io_service)  [basic_io_object ctor]
- L136 query(host_name(), "")  [string temp + query ctor + temp string dtors + alloc finalize]
- L137 iter = resolver.resolve(query)
- L138 end  [default iterator ctor / shared_ptr]
- L142 for(;iter!=end;++iter)  loop: increment / equal
- L144 addr = (*iter).endpoint().address()  [rep movsd 7 dwords = 0x1c copy of endpoint, then address()]
- L145 if (!addr.is_loopback() && addr.is_v4())  [is_loopback; cmp [addr],0 sete = is_v4 tag==0]
- L147 return ...address().to_string()  [v6 if tag==1 else v4 to_string; release shared_ptrs; dtors]
- L150 end loop
- L152 return "unknown"  [string("unknown") + finalize + releases + dtors]

NOTE: addr discriminator: `cmp dword ptr [ebp-94h],0; sete` -> type tag 0 means v4. At L147
`cmp [ebp-11Ch],1; jne v4` -> tag 1 = v6. So addr.type()/family selects v4 vs v6 to_string.
This obj is ~0x2a4 bytes of inlined boost; like async_connector it may not load in objdiff.
Score read from `--view diff` (objdiff backend) and report.json if present.

## Results (report.json fuzzy_match_percent)
- finalize                       100%  DONE
- initialize                     100%  DONE
- memory_allocator               100%  DONE  (ASSERT(UNKNOWN_EXPRESSION) recovered the empty_stub)
- get_connection_info_from_string 99.98% PARTIAL
- get_ip_address                 99.01% PARTIAL

### get_connection_info_from_string (99.98%)
First pass used `dest_port = (u16)port` -> base emitted `mov cx,[port]; mov [eax],cx` (<0xa>),
target is `mov eax,[port]; and eax,0FFFFh; mov [ecx],ax` (<0xe>). Source is `port & 0xffff`.
After that the ONLY diffs (strict instr diff) are:
- `sub esp,10h` (target) vs `sub esp,0Ch` (base) - a single extra 4-byte /Od frame slot. LOCALS
  lists exactly 3 (delim/result/port = 0x0C); the 0x10 has one unattributed dword. Not chased
  further (1 dword, <0.02%).
- `call strncpy_s`/`sscanf_s` vs `_strncpy_s`/`_sscanf_s` - secure-CRT reloc decoration only.

### get_ip_address (99.01%)
Body byte-correct. Strict instr diff shows only ICF empty-fn fold-winner naming:
- target `...fixed_size_allocator<...>::finalize_impl` vs base `unreferenced_parameter_helper`
  (both the 0x3f210 empty stub) at the two allocator-temp finalize sites (L136, L152).
- target `boost::shared_ptr<vector<resolver_entry>>::shared_ptr()` vs base
  `hash_map<...>::bucket_type::bucket_type()` - both empty default ctors folded together.
None source-fixable; this is which COMDAT name the linker kept for byte-identical empties.

## ICF churn from enabling this TU (one-time, accepted)
The FIRST rebuild that enabled the TU regressed 57 / improved 42 boost+threading template
COMDATs (bind_t/binder2/bucket_type/event::~event/weapon vector-deleting-dtor thunks),
100% <-> 0% flips. These are ICF fold-winner reassignments: get_ip_address instantiates more
boost.asio empties, so the linker's chosen representative for a fold group changes and the
delinker re-attributes the original address to a different mangled name. Net real bytes ~neutral
(57 lost-names vs 42 gained-names). Subsequent rebuilds show 0 regressed/0 improved - stable.
Same accepted cost async_connector took.

## Commands log
- pdb_rich_query --index binaries/rich/target/index.jsonl --list --function network_core
- pdb_fetch --target-index ... --rva {0x576f80,90,a0,c0,0x577040} --view target,structure,callees,info
- pdb_fetch --base-index binaries/rich/base/index.jsonl --function ... --view base   (strict instr diff)
- python3 scripts/regen_ninja.py ; python3 scripts/rebuild.py  (x3)
