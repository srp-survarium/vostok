# float4x4/float3 operator push order: first-push = RIGHT operand, hidden sret last
tags: cpp:operator cpp:float | asm:push asm:lea asm:rep-movsd | topic:codegen-idiom topic:convention
symptoms: push push lea push edx call operator* add esp 0Ch, rep movsd 0x10 result chains
confidence: 9/10
variants: cxyz-reference-hoist.md, matrix-index-no-hoist.md

For a __cdecl free operator returning a struct by value, args push right-to-left and the
hidden return pointer pushes LAST: first-push -> right operand, second-push -> left,
last-push -> &result. So `result = A * B` = (2nd-pushed) * (1st-pushed). A run of
operator* calls each `rep movsd 0x10` into a fresh 0x40 slot, each result feeding the
next call, is a forward-kinematics object-space chain.

```cpp
up_leg_obj = matrices[ up_leg_idx ] * hip;
knee_obj   = matrices[ knee_idx ]   * up_leg_obj;   // chain: result is the next right/left operand
```
```asm
push eax       ; 1st push = RIGHT operand
push ecx       ; 2nd push = LEFT
lea edx,[ebp-XX]; push edx   ; hidden sret
call vostok::math::operator*
add esp, 0Ch
```
float3 `operator-`/`operator^` use ecx=A, edx=B - decode subtraction ORDER from which operand lands in ecx; backwards negates the float3 and diverges downstream.
Evidence: get_bone_matrix_in_object_space_impl (matrices[i] * recursive_result: first push = recursive_result); legs_ik FK chains.
