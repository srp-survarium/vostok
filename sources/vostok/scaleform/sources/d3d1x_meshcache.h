////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef D3D1X_MESHCACHE_H_INCLUDED
#define D3D1X_MESHCACHE_H_INCLUDED

// Reference-only PDB mirror; translation units include Render/D3D1x SDK headers.
// Function ownership: docs/binary_matching/scaleform/inline_reference_ownership.md.

#include "Render/Render_MeshCache.h"
#include "d3d1x_sync.h"

#include <d3d11.h>

namespace Scaleform {
namespace Render {
namespace D3D1x {

class MeshBufferSet;
class MeshCache;
class ShaderManager;

class MeshBuffer : public Render::MeshBuffer {
public:
	struct LockList {
		inline			LockList	( ) { /* no source */ }

		inline	void	Add			( MeshBuffer* arg_0 ) { /* no source */ }

		inline	void	UnlockAll	( ID3D11DeviceContext* arg_0 ) { /* no source */ }

	public:
		/* 0x0000 */	MeshBuffer*		pFirst;
	}; // struct LockList

	inline				MeshBuffer		( u32 arg_0, AllocType arg_1, u32 arg_2 ) { /* no source */ }
	virtual				~MeshBuffer		( ) { /* no source */ }

	inline	u32			GetIndex		( ) const { /* no source */ return Index; }

	virtual	bool		allocBuffer		( ID3D11Device* arg_0 ) = 0;

	virtual	bool		DoLock			( ID3D11DeviceContext* arg_0 ) = 0;

	virtual	void		Unlock			( ID3D11DeviceContext* arg_0 ) = 0;

	virtual	BufferType	GetBufferType	( ) const = 0;

	inline	pbyte		Lock			( LockList& arg_0, ID3D11DeviceContext* arg_1 ) { /* no source */ return 0; }

private:
	/* 0x0000 */	/* Scaleform::Render::MeshBuffer */
	/* 0x001c */	u32				Index;
	/* 0x0020 */	MeshBuffer*		pNextLock;
}; // class MeshBuffer

class MeshBufferSet {
public:
	inline					MeshBufferSet	( MemoryHeap* arg_0, u32 arg_1 ) { /* no source */ }
	virtual					~MeshBufferSet	( ) { /* no source */ }

	inline	void			SetGranularity	( u32 arg_0 ) { /* no source */ }

	inline	AllocAddr&		GetAllocator	( ) { /* no source */ return Allocator; }

	inline	u32				GetGranularity	( ) const { /* no source */ return Granularity; }

	inline	u32				GetTotalSize	( ) const { /* no source */ return TotalSize; }

	virtual	MeshBuffer*		CreateBuffer	(
								u32							arg_0,
								MeshBuffer::AllocType		arg_1,
								u32							arg_2,
								MemoryHeap*					arg_3,
								ID3D11Device*				arg_4
							) = 0;

	// STATE[STUB]
	inline	void			DestroyBuffers	( MeshBuffer::AllocType arg_0 )
	{
		// FUNCTION BODY[0x0b4930]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
	}

	inline	void			DestroyBuffer	( MeshBuffer* arg_0, bool arg_1 ) { /* no source */ }

	inline	bool			Alloc			( u32 arg_0, MeshBuffer** arg_1, u32* arg_2 ) { /* no source */ return false; }

	inline	u32				Free			( u32 arg_0, MeshBuffer* arg_1, u32 arg_2 ) { /* no source */ return 0; }

	static	inline	u32		SizeToAllocatorUnit( u32 arg_0 ) { /* no source */ return 0; }

private:
	/* 0x0004 */	ArrayLH< MeshBuffer*, 2, ArrayDefaultPolicy >	Buffers;
	/* 0x0010 */	AllocAddr		Allocator;
	/* 0x001c */	u32				Granularity;
	/* 0x0020 */	u32				TotalSize;
}; // class MeshBufferSet

template < class Buffer >
class MeshBufferSetImpl : public MeshBufferSet {
public:
	inline					MeshBufferSetImpl( MemoryHeap* arg_0, u32 arg_1 ) { /* no source */ }

	// STATE[STUB]
	// FUNCTION BODY[0x0b4b60] (VertexBuffer) / FUNCTION BODY[0x0b4b90] (IndexBuffer)
	virtual	MeshBuffer*		CreateBuffer	(
								u32							arg_0,
								MeshBuffer::AllocType		arg_1,
								u32							arg_2,
								MemoryHeap*					arg_3,
								ID3D11Device*				arg_4
							) override { /* no source */ return 0; }

	virtual					~MeshBufferSetImpl( ) { /* no source */ }
}; // class MeshBufferSetImpl

template < class BType, class Derived >
class MeshBufferImpl : public MeshBuffer {
public:
	inline					MeshBufferImpl	( u32 arg_0, AllocType arg_1, u32 arg_2 ) { /* no source */ }
	virtual					~MeshBufferImpl	( ) { /* no source */ }

	inline	BType*			GetHWBuffer		( ) const { /* no source */ return pBuffer; }

	// STATE[STUB]
	// FUNCTION BODY[0x0b4b20] (both instantiations ICF-folded)
	virtual	bool			DoLock			( ID3D11DeviceContext* arg_0 ) override { /* no source */ return false; }

	// STATE[STUB]
	// FUNCTION BODY[0x0b4b00] (both instantiations ICF-folded)
	virtual	void			Unlock			( ID3D11DeviceContext* arg_0 ) override { /* no source */ }

	// STATE[STUB]
	// FUNCTION BODY[0x0b4bc0] (VertexBuffer) / FUNCTION BODY[0x0b4cc0] (IndexBuffer)
	static	Derived*		Create			(
								u32							arg_0,
								AllocType					arg_1,
								u32							arg_2,
								MemoryHeap*					arg_3,
								ID3D11Device*				arg_4,
								MeshBufferSet&				arg_5
							) { /* no source */ return 0; }

private:
	/* 0x0024 */	Ptr< BType >				pBuffer;
	/* 0x0028 */	D3D11_MAPPED_SUBRESOURCE	MappedBuffer;
}; // class MeshBufferImpl

class VertexBuffer : public MeshBufferImpl< ID3D11Buffer, VertexBuffer > {
public:
	inline				VertexBuffer	( u32 arg_0, AllocType arg_1, u32 arg_2 ) { /* no source */ }

	// STATE[STUB]
	virtual	bool		allocBuffer		( ID3D11Device* arg_0 ) override
	{
		// FUNCTION BODY[0x0b49e0]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
		return false;
	}

	// STATE[STUB]
	virtual	BufferType	GetBufferType	( ) const override
	{
		// FUNCTION BODY[0x0427c0]
		return Buffer_Vertex;
	}

	virtual				~VertexBuffer	( ) { /* no source */ }
}; // class VertexBuffer

class IndexBuffer : public MeshBufferImpl< ID3D11Buffer, IndexBuffer > {
public:
	inline				IndexBuffer		( u32 arg_0, AllocType arg_1, u32 arg_2 ) { /* no source */ }

	// STATE[STUB]
	virtual	bool		allocBuffer		( ID3D11Device* arg_0 ) override
	{
		// FUNCTION BODY[0x0b4a80]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
		return false;
	}

	// STATE[STUB]
	virtual	BufferType	GetBufferType	( ) const override
	{
		// FUNCTION BODY[0x0b53b0]
		return Buffer_Index;
	}

	virtual				~IndexBuffer	( ) { /* no source */ }
}; // class IndexBuffer

class MeshCacheItem : public Render::MeshCacheItem {
public:
	inline		MeshCacheItem	( ) { /* no source */ }
	inline		~MeshCacheItem	( ) { /* no source */ }

	// STATE[STUB]
	static	MeshCacheItem*	Create	(
					MeshType				arg_0,
					MeshCacheListSet*		arg_1,
					MeshBaseContent&		arg_2,
					VertexBuffer*			arg_3,
					IndexBuffer*			arg_4,
					u32						arg_5,
					u32						arg_6,
					u32						arg_7,
					u32						arg_8,
					u32						arg_9,
					u32						arg_10
				)
	{
		// FUNCTION BODY[0x0b48e0]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8, arg_9, arg_10 );
		return 0;
	}

private:
	/* 0x0000 */	/* Scaleform::Render::MeshCacheItem */
	/* 0x0038 */	VertexBuffer*	pVertexBuffer;
	/* 0x003c */	IndexBuffer*	pIndexBuffer;
	/* 0x0040 */	u32				VBAllocOffset;
	/* 0x0044 */	u32				VBAllocSize;
	/* 0x0048 */	u32				IBAllocOffset;
	/* 0x004c */	u32				IBAllocSize;
}; // class MeshCacheItem

class MeshCache : public Render::MeshCache {
public:
	// everything unmarked is out-of-line in d3d1x_meshcache.cpp per the rich index
	inline	MeshCache*	getThis							( ) { /* no source */ return this; }

	inline	u32			getTotalSize					( ) const { /* no source */ return 0; }

			bool		createStaticVertexBuffers		( ID3D11Device* arg_0 );

			bool		createMaskEraseBatchVertexBuffer( ID3D11Device* arg_0 );

			bool		allocCacheBuffers				( u32 arg_0, MeshBuffer::AllocType arg_1, u32 arg_2 );

			bool		evictMeshesInBuffer				(
							MeshCacheListSet::ListSlot*	arg_0,
							u32							arg_1,
							MeshBuffer*					arg_2
						);

			bool		allocBuffer						(
							u32*			arg_0,
							MeshBuffer**	arg_1,
							MeshBufferSet&	arg_2,
							u32				arg_3,
							bool			arg_4
						);

			void		destroyBuffers					( MeshBuffer::AllocType arg_0 );

			void		adjustMeshCacheParams			( MeshCacheParams* arg_0 );

			void		destroyPendingBuffers			( );

						MeshCache						( MemoryHeap* arg_0, MeshCacheParams const& arg_1 );
	virtual				~MeshCache						( );

			bool		Initialize						(
							ID3D11Device*			arg_0,
							ID3D11DeviceContext*	arg_1,
							ShaderManager*			arg_2
						);

			void		Reset							( );

	// STATE[STUB]
	virtual	QueueMode	GetQueueMode					( ) const override
	{
		// FUNCTION BODY[0x0b53b0] (ICF-folded with IndexBuffer::GetBufferType)
		return QM_ExtendLocks;
	}

	virtual	void		ClearCache						( ) override;

	virtual	bool		SetParams						( MeshCacheParams const& arg_0 ) override;

	virtual	void		BeginFrame						( ) override;

	virtual	void		EndFrame						( ) override;

	virtual	bool		LockBuffers						( ) override;

	virtual	void		UnlockBuffers					( ) override;

	// STATE[STUB]
	virtual	bool		AreBuffersLocked				( ) const override
	{
		// FUNCTION BODY[0x0b4ad0]
		return Locked;
	}

	virtual	u32			Evict							(
							Render::MeshCacheItem*	arg_0,
							AllocAddr*				arg_1,
							MeshBase*				arg_2
						) override;

	virtual	bool		PreparePrimitive				(
							PrimitiveBatch*						arg_0,
							Render::MeshCacheItem::MeshContent&	arg_1,
							bool								arg_2
						) override;

	virtual	AllocResult	AllocCacheItem					(
							Render::MeshCacheItem**				arg_0,
							pbyte*								arg_1,
							u16**								arg_2,
							Render::MeshCacheItem::MeshType		arg_3,
							Render::MeshCacheItem::MeshBaseContent&	arg_4,
							u32									arg_5,
							u32									arg_6,
							u32									arg_7,
							bool								arg_8,
							VertexFormat const*					arg_9
						) override;

	virtual	void		GetStats						( Stats* arg_0 ) override;

	inline	RenderSync*	GetRenderSync					( ) { /* no source */ return &RSync; }

private:
	/* 0x0000 */	/* Scaleform::Render::MeshCache */
	/* 0x0058 */	Ptr< ID3D11Device >			pDevice;
	/* 0x005c */	Ptr< ID3D11DeviceContext >	pDeviceContext;
	/* 0x0060 */	ShaderManager*				pShaderManager;
	/* 0x0064 */	MeshCacheListSet			CacheList;
	/* 0x00b0 */	RenderSync					RSync;
	/* 0x0120 */	MeshBufferSetImpl< VertexBuffer >	VertexBuffers;
	/* 0x0144 */	MeshBufferSetImpl< IndexBuffer >	IndexBuffers;
	/* 0x0168 */	bool						Locked;
	/* 0x016c */	u32							VBSizeEvictedInLock;
	/* 0x0170 */	MeshBuffer::LockList		LockedBuffers;
	/* 0x0174 */	List< Render::MeshBuffer, Render::MeshBuffer >	ChunkBuffers;
	/* 0x017c */	List< Render::MeshBuffer, Render::MeshBuffer >	PendingDestructionBuffers;
	/* 0x0184 */	Ptr< ID3D11Buffer >			pMaskEraseBatchVertexBuffer;
}; // class MeshCache

typedef MeshBufferSetImpl< VertexBuffer >	VertexBufferSet;
typedef MeshBufferSetImpl< IndexBuffer >	IndexBufferSet;

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform

STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::MeshBuffer, 0x24);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::MeshBufferSet, 0x24);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::VertexBuffer, 0x34);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::IndexBuffer, 0x34);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::MeshCacheItem, 0x50);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::MeshCache, 0x188);

#endif // #ifndef D3D1X_MESHCACHE_H_INCLUDED
