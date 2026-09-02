// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LINK_RESOLVER_H_INCLUDED
#define LINK_RESOLVER_H_INCLUDED

namespace survarium {

class base_project;

class link_resolver { // sushi@TODO: NO_VTABLE?
public:
	virtual	void		resolve_links	( base_project* p, vostok::configs::binary_config_value cfg ) = 0;
}; // class link_resolver

STATIC_SIZE_ASSERT(link_resolver, 0x4);

} // namespace survarium

#endif // #ifndef LINK_RESOLVER_H_INCLUDED
