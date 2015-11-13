/*==================================================================*\
  ScriptAPIRegistrationInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/UserDefinedTypeRegistrar.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class ScriptAPIRegistrationInitializationVisitor : public Scripting::AngelScript::UserDefinedTypeRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScriptAPIRegistrationInitializationVisitor instance.
		/*!	@param[in] arguments Template parameter pack containing the arguments to be forwarded to the real type registrar constructor.
			*/
		template <typename... Arguments>
		ETInlineHint ScriptAPIRegistrationInitializationVisitor( Arguments&&... arguments );

		~ScriptAPIRegistrationInitializationVisitor() = default;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.inl>
//------------------------------------------------------------------//
