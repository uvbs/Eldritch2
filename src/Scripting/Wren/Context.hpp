/*==================================================================*\
  Context.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetDatabase;
	}
}

struct	WrenHandle;
struct	WrenVM;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class Context {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Context instance.
		Context( const Assets::AssetDatabase& assets, Logging::Log& log );
	//!	Disable copy construction.
		Context( const Context& ) = delete;

		~Context();

	// ---------------------------------------------------

	public:
	/*!	Returns a callable handle (i.e. may be used as an argument to `wrenCall()`) to a stub function that sends `call(_)`
		to the object bound as the receiver. */
		WrenHandle*	GetUnaryCallStub() const;

		WrenVM*		GetVm();

	// ---------------------------------------------------

	public:
		bool	Interpret( const char* source );

	// ---------------------------------------------------

	public:
		template <typename Type>
		WrenHandle*	FindForeignClass() const;

		WrenHandle*	CreateForeignClass( const char* module, const char* name, Type type );

		void*		CreateVariable( const char* module, const char* name, WrenHandle* klass, size_t size );
		void		CreateVariable( const char* module, const char* name, double value );

	// ---------------------------------------------------

	public:
		template <size_t formatSize, typename... Arguments>
		void	WriteLog( Logging::MessageType type, const Utf8Char (&format)[formatSize], Arguments&&... arguments ) const;

	// ---------------------------------------------------

	public:
		void	BindResources( size_t heapSizeInBytes );

		void	FreeResources();

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Log used to track errors and print debug messages.
		mutable Logging::ChildLog			_log;
	//!	@ref AssetDatabase used to service asset/script import requests. Never null.
		const Assets::AssetDatabase*		_assets;
	//!	Database of Wren classes indexed by C++ type.
		CachingHashMap<Type, WrenHandle*>	_classesByType;
	//!	Virtual machine that will run all script code for the game world.
		WrenVM*								_vm;
	//!	Wren call handle for objects that respond to `call(_)`.
		WrenHandle*							_unaryCallStub;
	};

// ---------------------------------------------------

	Context&	AsContext( WrenVM* vm );

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Context.inl>
//------------------------------------------------------------------//