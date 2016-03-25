/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Networking/Steamworks/EngineService.hpp>
#include <Networking/Steamworks/WorldView.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/ThreadScheduler.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Utility/CountedResult.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/Result.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#if( ET_COMPILER_IS_MSVC )
//	Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 6340 )
#endif
#include <steam_api.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_PLATFORM_WINDOWS )
	ET_LINK_LIBRARY( "steam_api64.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
															   _allocator( GetEngineAllocator(), UTF8L("Steamworks Networking Service Root Allocator") ),
															   _steamPort( 6690u ),
															   _gamePort( 6691u ),
															   _queryPort( 6692u ),
															   _versionString( UTF8_VERSION_STRING, _allocator ),
															   _lobbyWorldName( UTF8_PROJECT_NAME UTF8L("Lobby"), _allocator ),
															   _lobbyRulesName( UTF8_PROJECT_NAME UTF8L("LobbyRules@") UTF8_PROJECT_NAME, _allocator ),
															   _useVACAuthentication( true ),
															   _useSteamworks( false ),
															   _replicationUploadBandwidthLimitInKilobytesPerSecond( 1024u ),
															   _replicationDownloadBandwidthLimitInKilobytesPerSecond( 3u * 1024u ),
															   _contentDownloadBandwidthLimitInKilobytesPerSecond( 3u * 1024u ),
															   _contentUploadBandwidthLimitInKilobytesPerSecond( 1024u ),
															   _banList( { _allocator, UTF8L("Steamworks Ban List Allocator") } ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {
		::SteamAPI_Shutdown();
	}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const { 
		return UTF8L("Steamworks Networking Service");
	}

// ---------------------------------------------------

	ErrorCode EngineService::AllocateWorldView( Allocator& allocator, World& world ) {
		return new(allocator, Allocator::AllocationDuration::Normal) WorldView( *this, world ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------

	void EngineService::OnEngineConfigurationBroadcast( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Steamworks Networking Service", "Initiate Steam connection", 0xBBBBBB );

		GetLogger()( UTF8L("Connecting to Steam instance.") ET_UTF8_NEWLINE_LITERAL );

		const ::EServerMode	serverMode( _useVACAuthentication ? ::eServerModeAuthenticationAndSecure : ::eServerModeAuthentication );
		const uint32		serverAddress( 0u ); // INADDR_ANY

		// Attempt to establish a connection with the Steam master servers.
		if( ::SteamAPI_Init() ) {
			GetLogger()( UTF8L("Initial Steam connection established.") ET_UTF8_NEWLINE_LITERAL );
		} else {
			GetLogger( LogMessageType::Error )( UTF8L("Unable to initialize Steam API!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Steamworks") );

		visitor.Register( UTF8L("SteamPort"), _steamPort ).Register( UTF8L("GamePort"), _gamePort ).Register( UTF8L("QueryPort"), _queryPort ).Register( UTF8L("VersionString"), _versionString );
		visitor.Register( UTF8L("UseValveAntiCheatAuthentication"), _useVACAuthentication ).Register( UTF8L("UseSteamworks"), _useSteamworks );
		visitor.Register( UTF8L("ReplicationUploadBandwidthLimitInKilobytesPerSecond"), _replicationUploadBandwidthLimitInKilobytesPerSecond );
		visitor.Register( UTF8L("ReplicationDownloadBandwidthLimitInKilobytesPerSecond"), _replicationDownloadBandwidthLimitInKilobytesPerSecond );
		visitor.Register( UTF8L("ContentDownloadBandwidthLimitInKilobytesPerSecond"), _contentDownloadBandwidthLimitInKilobytesPerSecond );
		visitor.Register( UTF8L("ContentUploadBandwidthLimitInKilobytesPerSecond"), _contentUploadBandwidthLimitInKilobytesPerSecond );

		visitor.PushSection( UTF8_PROJECT_NAME );
		visitor.Register( UTF8L("LobbyWorldName"), _lobbyWorldName ).Register( UTF8L("LobbyRulesName"), _lobbyRulesName );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptApiRegistrationInitializationVisitor& visitor ) {
		WorldView::ExposeScriptApi( visitor );
	}

// ---------------------------------------------------

	void EngineService::OnEngineInitializationCompleted( WorkerContext& /*executingContext*/ ) {
		auto	createLobbyWorldResult( CreateWorld( _lobbyWorldName.AsCString(), _lobbyRulesName.AsCString() ) );

		if( !createLobbyWorldResult ) {
			GetLogger( LogMessageType::Error )( UTF8L("Unable to create lobby world!") ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		_lobbyWorld = ::std::move( createLobbyWorldResult.object );

		// AcknowledgePlayerConnection( { ::SteamUser()->GetSteamID(), 0u } );
	}

// ---------------------------------------------------

	void EngineService::OnServiceTickStarted( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Steamworks Networking Service", "Process and dispatch callbacks", 0xAAAAAA );

		::SteamAPI_RunCallbacks();
		::SteamGameServer_RunCallbacks();

		if( _lobbyWorld && !( _lobbyWorld->CanExecute() ) ) {
			_lobbyWorld = nullptr;
		}
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2