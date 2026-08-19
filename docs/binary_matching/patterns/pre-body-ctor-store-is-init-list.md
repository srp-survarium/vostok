# A store before the first ctor body record belongs in the member initializer list
tags: cpp:ctor cpp:init-list cpp:member | asm:mov | topic:structure-shape topic:line-records
symptoms: unaddressed mov [this+member],0 before the first addressed body statement; the same store precedes that statement when the ctor is inlined
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
