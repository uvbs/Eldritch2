/*==================================================================*\
  AssetDatabase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetDatabase;
		class	Asset;
	}
}

namespace Eldritch2 {
namespace Assets {

	class AssetApiBuilder {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using AssetFactory	= Function<UniquePointer<Asset>( Allocator&, const Utf8Char* )>;
		template <typename Value>
		using ExtensionMap	= CachingHashMap<Utf8Literal, Value>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder( const AssetApiBuilder& ) = default;
	//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder();

		~AssetApiBuilder() = default;

	// ---------------------------------------------------

	public:
	//!	Registers an object creation handler for a specified class of asset object.
	/*!	@param[in] extension @ref Utf8Literal containing the extension of the asset to listen for. This string should include a
			leading dot character.
		@returns A reference to *this for method chaining. */
		AssetApiBuilder&	DefineType( Utf8Literal extension, AssetFactory factory );
	//!	Registers an object creation handler for a specified class of asset object.
	/*!	@param[in] extension @ref Utf8Literal containing the extension of the asset to listen for. This string should include a
			leading dot character.
		@returns A reference to *this for method chaining. */
		template <typename PublicAsset>
		AssetApiBuilder&	DefineType( Utf8Literal extension );
		
	// ---------------------------------------------------

	public:
		ExtensionMap<AssetFactory>&	GetFactories();

	// ---------------------------------------------------

	public:
		AssetApiBuilder&	operator=( const AssetApiBuilder& ) = default;

	// - DATA MEMBERS ------------------------------------

	private:
		ExtensionMap<AssetFactory>	_factoriesByExtension;
	};

// ---

	class AssetDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct AssetEqual {
			ETPureFunctionHint bool	operator()( const Asset*, Pair<Type, const Utf8Char*> ) const;
			ETPureFunctionHint bool	operator()( const Asset*, const Asset* ) const;
		};

	// ---

	public:
		struct AssetHash {
			ETPureFunctionHint size_t	operator()( Pair<Type, const Utf8Char*>, size_t seed = 0u ) const;
			ETPureFunctionHint size_t	operator()( const Asset*, size_t seed = 0u ) const;
		};

	// ---

	public:
		using AssetFactory	= AssetApiBuilder::AssetFactory;
		template <typename Value>
		using ExtensionMap	= AssetApiBuilder::ExtensionMap<Value>;
		template <typename Value>
		using ResidentSet	= CachingHashSet<Value*, AssetHash, AssetEqual>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Disable copy construction.
		AssetDatabase( const AssetDatabase& ) = delete;
	//! Constructs this @ref AssetDatabase instance.
		AssetDatabase();

		~AssetDatabase();

	// - ASSET MANAGEMENT --------------------------------

	public:
	//!	Locates an existing asset in the database.
	/*!	@param[in] path A null-terminated UTF-8-encoded character sequence containing the name of the resource to search for.
		@returns A pointer to the asset, if successful, or null in the event no matching asset was found.
		@remarks Thread-safe. */
		template <typename PublicAsset>
		const PublicAsset*	Require( const Utf8Char* const path ) const;

	// ---------------------------------------------------

	public:
		UniquePointer<Asset>	CreateAsset( const Utf8Char* path );

		bool					RegisterAsset( Asset& asset );

		void					UnregisterAsset( Asset& asset );

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( ExtensionMap<AssetFactory> factories );

		void		FreeResources();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		AssetDatabase&	operator=( const AssetDatabase& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable UsageMixin<MallocAllocator>	_allocator;
		mutable Mutex						_assetsMutex;
		ResidentSet<Asset>					_assets;
	/*!	Collection of handlers that will allocate concrete @ref Asset instances given metadata and
		initialization byte streams. */
		ExtensionMap<AssetFactory>			_factoriesByExtension;
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetDatabase.inl>
//------------------------------------------------------------------//