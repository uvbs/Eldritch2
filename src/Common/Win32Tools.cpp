/*==================================================================*\
  Win32Tools.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Win32Tools.hpp>
#include <Common/Memory.hpp>
//------------------------------------------------------------------//
#include <wchar.h>
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
#include <shlwapi.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "Shlwapi.lib" )
//------------------------------------------------------------------//

#if defined GetCommandLine
#	undef GetCommandLine
#endif

namespace Eldritch2 {
namespace {

	static int	cmdShow = 0;

}	// anonymous namespace

	void StoreMainArguments( HINSTANCE /*hInstance*/, int nCmdShow ) {
		cmdShow = nCmdShow;
	}

// ---------------------------------------------------

	const char* GetCommandLine() {
		return GetCommandLineA();
	}

// ---------------------------------------------------

	const wchar_t* GetWideCommandLine() {
		return GetCommandLineW();
	}

// ---------------------------------------------------

	bool ETNeverThrowsHint CommandLineContains( char* const option ) {
		return FindFirstInstance( GetCommandLine(), option ) != nullptr;
	}

// ---------------------------------------------------

	bool ETNeverThrowsHint CommandLineContains( wchar_t* const option ) {
		return FindFirstInstance( GetWideCommandLine(), option ) != nullptr;
	}

// ---------------------------------------------------

	HINSTANCE GetModuleByAddress( void* addressOfItemInModule ) {
		HINSTANCE	instance( nullptr );

		if (!addressOfItemInModule) {
			addressOfItemInModule = reinterpret_cast<void*>( &GetModuleByAddress );
		}

		GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPWSTR>(addressOfItemInModule), &instance );

		return instance;
	}

// ---------------------------------------------------

	int ETPureFunctionHint ETNeverThrowsHint GetCmdShow() {
		return cmdShow;
	}

}	// namespace Eldritch2