# A store before the first ctor body record belongs in the member initializer list
tags: cpp:ctor cpp:init-list cpp:member | asm:mov asm:movss | topic:structure-shape topic:line-records
symptoms: unaddressed member store before the first body statement; pre-body scalar load and sunk stores interleaved with default constructors in member-declaration order
confidence: 9/10

When a target constructor stores a member before the first PDB-addressed body
statement, recover that store in the constructor's initializer list. Do not add
an assignment at the start of the body: that gives the store its own source
statement and places it on the wrong side of the body's first line record.

Confirm the interpretation at an inlined construction site. If the same store
appears immediately before the code attributed to the first body statement,
and the member offset agrees with the target class layout, the source shape is:

```cpp
container::container( ) :
	m_size( 0 )
{
	memory::zero( m_buffer, sizeof(m_buffer) );
}
```

Evidence: `hash_multiset::hash_multiset` had an unaddressed `m_size = 0` store
before its sole `memory::zero` statement, and `strings::initialize` showed the
same order after inlining. Restoring `m_size(0)` made both functions exact.

## Initializer stores sunk into the first body record

An optimized constructor may put the scalar constant load before the first
body record but sink its store into that record. Check the surrounding member
construction order: scalar stores interleaved between preceding and following
smart-pointer default constructors, in declaration order, support an initializer
list rather than body setter calls. The record boundary alone is insufficient.

`stage_ambient_lighting` initializes effect pointers, then the multiplier and
probe flag, then buffer pointers. Body setters emitted the scalar stores after
all buffer construction; moving the setters before the first effect-creation
call corrected call order but retained the extra load record. Initializing both
scalar members in the initializer list restored the complete store order and
37/37 statement structure, raising the constructor from 99.414% to 100%.
