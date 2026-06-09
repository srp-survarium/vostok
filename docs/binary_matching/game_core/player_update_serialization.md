# game_core: player_input / player_state / *_player_update / hit_info serialize chain

Branch: vostok_2 worktree, base a9fd7422.

These were all BLOCKED on "the udp_match_packet/packet_reader header cluster is
never-compiled". That cluster now compiles (network_core matched), so the
serialize/deserialize bodies can be written and scored. They were also blocked by
the empty scalar `packet<T>::append(...)` overloads (u32/s32/bool/...), which I
filled in packet_inline.h so the call sites emit.

## player_input  (player_input.cpp)
serialize (target 3 stmts):
```cpp
packet.append( angular_velocity );      // float2 @+0
packet.append( angular_acceleration );  // float2 @+8
packet.append( actions_mask );          // u32   @+0x10
```
deserialize (target 3 stmts):
```cpp
angular_velocity     = reader.r< math::float2 >();
angular_acceleration = reader.r< math::float2 >();
actions_mask         = reader.r< u32 >();
```
Residual: SIZE diffs only - the float2 read in target goes through the
`r(void*,size,size)` overload (ctor temp + Vector2::operator=) while our `r<float2>()`
direct-derefs; per user guidance not chased.

## player_state  (player_state.cpp)
serialize: `append(transform.c.xyz())`, `append(transform.get_angles(rotation_zxy).y)`,
`append(look_pitch)`.
deserialize:
```cpp
float3 const& position = packet.r< math::float3 >();
float yaw              = packet.r< float >();
look_pitch             = packet.r< float >();
transform              = math::create_rotation_y( yaw );
transform.c.xyz()      = position;
```
Translation lives in float4x4 row `c` (offset 0x30); the `.c.xyz()` idiom is the
project convention (legs_ik_processor.cpp). Structure aligned, SIZE residual.

## client_player_update::serialize
`input.serialize(packet); state.serialize(packet); packet.append(time_in_ms);`
(time_in_ms @+0x58). Needed the u32 append body to emit the 3rd statement.

## server_player_update::deserialize  -- STRUCTURE MATCH (aligned 3, 0 diffs)
`input.deserialize; state.deserialize; weapon_state.deserialize`. Switched the header
to include the real weapon_state.h (it already has deserialize) instead of the local
stub struct, which collided with weapon_state.h when both land in temp_include_all.

## hit_info::deserialize  (hit_initiator.cpp)
```cpp
hit_initiator = r<bool>(); being_hit = r<bool>();
char c_body_part_name[16]; r_string(c_body_part_name); body_part_name = c_body_part_name;
char damage_type_info[16]; r_string(damage_type_info); damage_type = damage_type_info;
amount = r<float>(); armor_piercing = r<float>(); bullet = NULL;
```
hit_info.h previously DEFINED dummy `class packet_reader {}` / `udp_match_packet {}`
(sushi@TODO) which redefinition-collided with the real headers; replaced with forward
declarations.

## Anchor
`use_game_core_serialization(udp_match_packet*, packet_reader*)` in temp_include_all.cpp
exercises all of the above so the append / r<T> / r_string primitives keep out-of-line
call sites; wired into IncludeAll::IncludeAll().
