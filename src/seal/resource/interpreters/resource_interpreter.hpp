#pragma once

namespace seal {

	// clang-format off
	template<typename InterpreterT>
	concept resource_interpreter = requires(InterpreterT interpreter)
	{
		{typename InterpreterT::owning_type};
		{typename InterpreterT::value_type};
		{typename InterpreterT::view_type};
		{interpreter.load(std::declval<u32>())} -> std::same_as<typename InterpreterT::view_type>;
	};
	// clang-format on
}
