#pragma once

#include <ostream>
#include <optional>

#include <fmt/format.h>

#include "seal/types.hpp"

namespace seal {
	template<typename T, typename E>
	class result;

	/**
	 * Represents a failure.
	 */
	class failure : public std::runtime_error
	{
	private:
		static constexpr u32 NON_FATAL_ERROR = 0;

		static constexpr u32 FATAL_ERROR = 5000;

	public:
		enum fail_type : u32
		{
			CustomNonFatal = NON_FATAL_ERROR,
			EndReached, // The end of something was reached (file or stream)
			ExternalFailure,	// An external library/program has caused the failure.

			Custom = FATAL_ERROR,
			NotSupported,	// The requested operation is not supported
			NotImplemented,  // The requested operation is not implemented
		};

		inline explicit failure(std::string message, fail_type type = Custom);

		template<typename... Fmt>
		explicit failure(std::string message, Fmt&&...);


		/// Implemented in result.hpp
		template<typename T>
		constexpr operator seal::result<T, seal::failure>();

		/*
			Checks if the failure is a fatal failure
		*/
		constexpr bool is_fatal() const;

		/*
			Gets the type of the failure
		 */
		constexpr fail_type type() const;

	private:
		fail_type m_Type;
		std::string m_Message;
	};

	/**
		Creates an error with a custom type.

		\tparam Type: The type of failure to make.
		\param message [optional]: The message to add with the failure.
	*/
	template<failure::fail_type Type>
	constexpr seal::failure fail(std::optional<std::string> message = std::nullopt)
	{
		return seal::failure(message.has_value() ? std::move(message.value()) : "", Type);
	}
}

inline seal::failure::failure(std::string message, fail_type type)
	: std::runtime_error(std::move(message))
	, m_Type(type)
{}

template<typename... Fmt>
inline seal::failure::failure(std::string message, Fmt&&...rest)
	: std::runtime_error(fmt::vformat(message.c_str(), fmt::make_format_args(rest...)))
	, m_Type(Custom)
{}

constexpr bool seal::failure::is_fatal() const
{
	return static_cast<u32>(m_Type) <= FATAL_ERROR;
}

constexpr seal::failure::fail_type seal::failure::type() const
{
	return m_Type;
}

inline std::ostream& operator<<(std::ostream& stream, const seal::failure& failure)
{
	return stream << failure.what();
}

template<>
struct fmt::formatter<seal::failure>
{
	/// Fixed presentation
	char presentation = 'f';

	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
	{
		if(ctx.begin() != ctx.end()) {
			throw format_error("Invalid format!");
		}

		return ctx.end();
	}

	/**
	   Writes the failure to the an iterator.

	   \param failure: The failure to write
	   \param ctx: The iterator to write to
	 */
	template<typename FormatContext>
	auto format(const seal::failure& failure, FormatContext& ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "{}", failure.what());
	}
};