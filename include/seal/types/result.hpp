#pragma once

#include <optional>
#include <sstream>
#include <stdexcept>
#include <variant>

#include "seal/debug.h"
#include "seal/panic.h"
#include "seal/types/failure.hpp"
#include "seal/types/traits/non_void.hpp"

namespace seal {

	template<typename E>
	class error
	{
	public:
		constexpr error(E error) noexcept;

		/**
		 * Takes the error value inside the error.
		 *
		 * \note: This object is invalidated after
		 */
		constexpr E&& take_error() noexcept;

	private:
		E m_error;
	};

	template<typename T, typename E = failure>
	class result
	{
	public:
		template<typename = std::enable_if_t<std::is_void_v<T>>>
		constexpr result() noexcept;

		/**
		 * Creates a new result with a value value.
		 *
		 * \param value: The error to fail
		 */
		template<typename = std::enable_if_t<!std::is_void_v<T>>>
		constexpr result(_non_void<T> value) noexcept;

		constexpr result(error<E> error) noexcept;

		/**
		 * Checks if the result contains an error or not.
		 *
		 * \return: True if the result doesn't contain an error
		 */
		constexpr bool is_value() const noexcept;

		/**
		 * Checks if the result is an error.
		 *
		 * \return: True if the result is an error
		 */
		constexpr bool is_error() const noexcept;

		template<typename Tx, typename Ex>
		constexpr operator result<Tx, Ex>()
		{
			if(is_value()) {
				return static_cast<Tx>(value());
			} else {
				return seal::error<Ex>(static_cast<Ex>(error()));
			}
		}

		/**
		 * Gets the value inside the result.
		 */
		template<typename = std::enable_if_t<!std::is_void_v<T>>>
		constexpr const _non_void<T>& value() const &;
		template<typename = std::enable_if_t<!std::is_void_v<T>>>
		constexpr _non_void<T>& value() &;
		template<typename = std::enable_if_t<!std::is_void_v<T>>>
		constexpr const _non_void<T>&& value() const &&;
		template<typename = std::enable_if_t<!std::is_void_v<T>>>
		constexpr _non_void<T>&& value() &&;

		/**
		 * Gets the error inside this result.
		 */
		constexpr const E& error() const &;
		constexpr E& error() &;
		constexpr const E&& error() const &&;
		constexpr E&& error() &&;

		/**
		 * Gets the value inside this result.
		 */
		constexpr _non_void<T>&& expect(const std::string& message);

	private:
		template<typename Tx, typename Q>
		struct _storage : public std::variant<Tx, Q>
		{};

		template<typename Q>
		struct _storage<void, Q> : public std::variant<_non_void<T>, Q>
		{};

		/**
		 * Panics if the result has a value.
		 *
		 * \param message: The message to show
		 * \throw result_panic_error: If the result has a value inside
		 */
		constexpr void _panic_at_value(const std::string& message) const;

		/**
		 * Panics if the result has an error.
		 *
		 * \param message: The message to show
		 * \throw result_panic_error: If the result has an error inside
		 */
		constexpr void _panic_at_error(const std::string& message) const;

		_storage<T, E> m_variant;
	};
}

/**
 * Checks if a result has a value, if it doesn't, break early.
 */
#define seal_verify_result(result)                                                                 \
	if(const auto r = (result); r.is_error()) {                                                    \
		return r;                                                                                  \
	}

template<typename E>
constexpr seal::error<E>::error(E error) noexcept
	: m_error(error)
{}

template<typename E>
constexpr E&& seal::error<E>::take_error() noexcept
{
	return std::move(m_error);
}

template<typename T, typename E>
template<typename>
constexpr seal::result<T, E>::result() noexcept
	: m_variant('\0')
{}

template<typename T, typename E>
template<typename>
constexpr seal::result<T, E>::result(_non_void<T> value) noexcept
	: m_variant(std::move(value))
{}

template<typename T, typename E>
constexpr seal::result<T, E>::result(seal::error<E> error) noexcept
	: m_variant(error.take_error())
{}

template<typename T, typename E>
constexpr bool seal::result<T, E>::is_value() const noexcept
{
	return std::holds_alternative<T>(m_variant);
}

template<typename T, typename E>
constexpr bool seal::result<T, E>::is_error() const noexcept
{
	return std::holds_alternative<E>(m_variant);
}

template<typename T, typename E>
constexpr seal::_non_void<T>&& seal::result<T, E>::expect(const std::string& message)
{
	if constexpr(std::is_void_v<T>) {
		_panic_at_error(message);

		// Dummy value
		return static_cast<seal::_non_void<T>&&>(seal::_void_dummy_v<T>);
	} else {
		_panic_at_error(message);

		return std::move(value());
	}
}

template<typename T, typename E>
template<typename>
constexpr const seal::_non_void<T>& seal::result<T, E>::value() const &
{
	seal_debug_only(_panic_at_error("Attempted to get a value from result that contains an error!"));

	return std::get<T>(m_variant);
}

template<typename T, typename E>
template<typename>
constexpr seal::_non_void<T>& seal::result<T, E>::value() &
{
	seal_debug_only(_panic_at_error("Attempted to get a value from result that contains an error!"));

	return std::get<T>(m_variant);
}

template<typename T, typename E>
template<typename>
constexpr const seal::_non_void<T>&& seal::result<T, E>::value() const &&
{
	seal_debug_only(_panic_at_error("Attempted to get a value from result that contains an error!"));

	return std::get<T>(m_variant);
}

template<typename T, typename E>
template<typename>
constexpr seal::_non_void<T>&& seal::result<T, E>::value() &&
{
	seal_debug_only(_panic_at_error("Attempted to get a value from result that contains an error!"));

	return std::get<T>(m_variant);
}

template<typename T, typename E>
constexpr const E& seal::result<T, E>::error() const &
{
	seal_debug_only(_panic_at_value("Attempted to get an error from result that contains a value!"));

	return std::get<E>(m_variant);
}

template<typename T, typename E>
constexpr E& seal::result<T, E>::error() &
{
	seal_debug_only(_panic_at_value("Attempted to get an error from result that contains a value!"));

	return std::get<E>(m_variant);
}

template<typename T, typename E>
constexpr const E&& seal::result<T, E>::error() const &&
{
	seal_debug_only(_panic_at_value("Attempted to get an error from result that contains a value!"));

	return std::get<E>(m_variant);
}

template<typename T, typename E>
constexpr E&& seal::result<T, E>::error() &&
{
	seal_debug_only(_panic_at_value("Attempted to get an error from result that contains a value!"));

	return std::get<E>(m_variant);
}

template<typename T, typename E>
constexpr void seal::result<T, E>::_panic_at_error(const std::string& message) const
{
	if(is_error()) {
		std::stringstream panic_msg;
		panic_msg << message << ": " << std::get<E>(m_variant) << std::endl;

		seal_panic(panic_msg.str());
	}
}

template<typename T, typename E>
constexpr void seal::result<T, E>::_panic_at_value(const std::string& message) const
{
	if(is_value()) {		
		seal_panic(message);
	}
}

template<typename T>
constexpr seal::failure::operator seal::result<T, seal::failure>()
{
	return seal::error(std::move(*this));
}
