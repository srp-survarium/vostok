# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.diff - target-vs-base SOURCE SHAPE, above the byte level.

objdiff answers "do these bytes match". These answer the questions that come
before it, where a wrong answer caps every consumer downstream:

    layout        class layouts: total size (a wrong STATIC_SIZE_ASSERT caps
                  every function that walks the type), missing fields, missing
                  methods, missing types
    order         class DECLARATION order - member and method order is part of
                  the structure, and getting it wrong drives inline/codegen
                  divergence
    tu_order      function DEFINITION order within one TU, which COMDAT order
                  follows
    enums         every enum the target has, against ours - and the persistent
                  work queue it maintains
    enums_compare the older, narrower view: enum bodies as the two structure
                  dumps render them, with no queue upkeep

All of them read `binaries/structure/{base,target}` - regenerate it with
`vostok.build.generate_structure` before trusting a diff.
"""
