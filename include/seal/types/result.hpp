#pragma once

#include <optional>
#include <sstream>
#include <stdexcept>
#include <variant>

#include "seal/panic.h"
#include "seal/log/log.hpp"
#include "seal/types/error_value.hpp"
#include "seal/types/failure.hpp"
#include "seal/types/traits/pointer.hpp"

namespace seal {

	class unexpected_result_type : public std::runtime_error
	{
	public:
		explicit unexpected_result_type(const std::string& message)
			: std::runtime_error(message)
		{}
	};

	template<typename TError>
	class error
	{
	public:
		constexpr error(TError error)
			: m_error(std::move(error))
		{}

		constexpr TError&& take()
		{
			return std::move(m_error);
		}

	private:
		TError m_error;
	};

	template<typename TValue, typename TError>
	class result
	{
	public:
		using ValueT = TValue;
		using ErrorT = TError;

		
		using ptr_type = std::conditional_t<is_shared_pointer_v<ValueT> ||
													is_unique_pointer_v<ValueT>,
												raw_ptr_type<ValueT>,
												ValueT *>;

		/**
			Creates a new result with a value in it.

			\param value: The value to put inside the result
		 */
		constexpr result(TValue value)
			: m_value(std::move(value))
		{}

		/**
		   Creates a new result with an error in it.

		   \param error: The error to put inside the result
		 */
		constexpr result(error<TError> error)
			: m_value(std::move(error.take()))
		{}

		/**
		   Checks if the result contains an error.

		   \return: True if the result contains an error
		 */
		constexpr bool is_error() const
		{
			return std::holds_alternative<TError>(m_value);
		}

		/**
		   Gets a const reference to the value inside the result.
		 */
		constexpr const TValue& value() const
		{
			return std::get<TValue>(m_value);
		}

		constexpr TValue& value()
		{
			return std::get<TValue>(m_value);
		}

		/**
		   Gets a const reference to the error inside the result.
		 */
		constexpr const TError& error() const
		{
			return std::get<TError>(m_value);
		}

		constexpr TError& error()
		{
			return std::get<TError>(m_value);
		}

		/**
		   Checks if the result is ok.
		 */
		constexpr operator bool() const
		{
			return !is_error();
		}

		/**
		   Checks if the result is an error.
		 */
		constexpr bool operator!() const
		{
			return is_error();
		}

		/**
		   Reference to the value inside the result
		 */
		constexpr const TValue& operator*() const
		{
			return value();
		}

		constexpr TValue& operator*()
		{
			return std::get<TValue>(m_value);
		}

		constexpr ptr_type operator->() const
		{
			return get_raw_pointer(std::get<TValue>(m_value));
		}

		constexpr ptr_type operator->()
		{
			return get_raw_pointer(std::get<TValue>(m_value));
		}

		/**
		   Forcibly gets the value out of this result, pancing if it contains an error.

		   \param message: The message to print on error.
		 */
		inline TValue except(const std::string& message);

		template<typename Ty>
		constexpr operator result<Ty, TError>() const
		{
			return seal::error(std::move(error()));
		}

	private:
		std::variant<TValue, TError> m_value;
	};

	template<typename TError>
	class result<void, TError>
	{
	public:
		using ValueT = void;
		using ErrorT = TError;

		constexpr result() noexcept
			: m_error(std::nullopt)
		{}

		constexpr result(error<TError> error)
			: m_error(std::move(error.take()))
		{}

		constexpr bool is_error() const
		{
			return m_error.has_value();
		}

		constexpr const TError& error() const
		{
			return m_error.value();
		}

		constexpr TError& error()
		{
			return m_error.value();
		}

		constexpr operator bool() const
		{
			return !is_error();
		}

		constexpr bool operator!() const
		{
			return is_error();
		}

		inline void expect(const std::string& message);

		template<typename Ty>
		constexpr operator result<Ty, TError>() const
		{
			return seal::error(std::move(error()));
		}

	private:
		std::optional<TError> m_error;
	};

	/// A result that represents any error (same as checking is_error() on a result)
	struct generic_result_error
	{
		constexpr generic_result_error() = default;
	};
}

#define seal_verify_result(...)                                                                    \
	if(const auto& r = (__VA_ARGS__); ::seal::error_value_v<decltype(__VA_ARGS__)> == r) {         \
		return r;                                                                                  \
	}

template<typename TValue, typename TError>
TValue seal::result<TValue, TError>::except(const std::string& message)
{
	if(is_error()) {
		seal::log::error("{}: {}", message, error());

		seal_panic(message.c_str());
	}

	return std::move(value());
}

template<typename TError>
void seal::result<void, TError>::expect(const std::string& message)
{
	if(is_error()) {
		seal::log::error("{}: {}", message, error());

		seal_panic(message.c_str());
	}
}

/// ----------- seal::failure::operator seal::result<T, seal::failure> ----

template<typename T>
constexpr seal::failure::operator seal::result<T, seal::failure>()
{
	return ::seal::error(std::move(*this));
}

/// ----------- seal::error_value<seal::result<T, E>> -----------------------

template<typename TValue, typename TError>
inline bool operator==(const seal::result<TValue, TError>& result, seal::generic_result_error gerr)
{
	return result.is_error();
}

template<typename TValue, typename TError>
inline bool operator==(seal::generic_result_error gerr, const seal::result<TValue, TError>& result)
{
	return result.is_error();
}

template<typename TValue, typename TError>
struct seal::error_value<seal::result<TValue, TError>>
{
	constexpr static generic_result_error VALUE = {};
};