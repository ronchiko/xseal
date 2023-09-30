#pragma once

#include "seal/types/traits/enum.hpp"

namespace seal {
	template<seal::enum_type EnumT>
	class flags
	{
	public:
		using enum_t = EnumT;
		using underlying_t = std::underlying_type_t<enum_t>;
		
		constexpr flags();

		/**
		   Creates a new object with a specified flag set.
		 */
		template<std::same_as<enum_t>... Flags>
		constexpr flags(Flags... flags) 
			: m_Value(static_cast<underlying_t>(seal::join_enums(flags...)))
		{}

		/**
		   Activates a flag in this object.
		  
		   \param flag: The flag to activate.
		 */
		constexpr void activate(enum_t flag);

		/**
		   Deactivates a flag in this object.
		  
		   \param flag: The flag to object to deactivate.
		 */
		constexpr void deactivate(enum_t flag);
		
		/**
		   Gets the state this flag is holding.
		 */
		constexpr enum_t value() const;

		/**
		   Checks if a flag is active.
		  
		   \param flag: The flag to check if active.
		   \return: True if the flag is active.
		 */
		constexpr bool is_active(enum_t flag) const;

		template<std::same_as<enum_t>... VariadicT>
		constexpr bool is_active(enum_t flag, VariadicT... rest) const
		{
			return is_active(flag) && is_active(rest...);
		}

		constexpr enum_t operator|(enum_t flag) const;
		constexpr flags& operator|=(enum_t flag);

		constexpr enum_t operator&(enum_t flag) const;

		/**
		   Converts this object to its base enum type.
		 */
		constexpr operator enum_t() const;

	private:
		underlying_t m_Value;
	};
}

template<seal::enum_type EnumT>
constexpr seal::flags<EnumT>::flags()
	: m_Value(static_cast<underlying_t>(0))
{}

template<seal::enum_type EnumT>
constexpr void seal::flags<EnumT>::activate(EnumT flag)
{
	m_Value |= static_cast<underlying_t>(flag);
}

template<seal::enum_type EnumT>
constexpr void seal::flags<EnumT>::deactivate(EnumT flag)
{
	m_Value &= ~static_cast<underlying_t>(flag);
}

template<seal::enum_type EnumT>
constexpr seal::flags<EnumT>::enum_t seal::flags<EnumT>::value() const
{
	return static_cast<enum_t>(m_Value);
}

template<seal::enum_type EnumT>
constexpr bool seal::flags<EnumT>::is_active(enum_t flag) const
{
	return m_Value & static_cast<underlying_t>(flag);
}

template<seal::enum_type EnumT>
constexpr seal::flags<EnumT>::enum_t seal::flags<EnumT>::operator|(enum_t flag) const
{
	return static_cast<enum_t>(m_Value | static_cast<underlying_t>(flag));
}

template<seal::enum_type EnumT>
constexpr seal::flags<EnumT>& seal::flags<EnumT>::operator|=(enum_t flag)
{
	m_Value |= static_cast<underlying_t>(flag);

	return *this;
}

template<seal::enum_type EnumT>
constexpr seal::flags<EnumT>::enum_t seal::flags<EnumT>::operator&(enum_t flag) const
{
	return static_cast<enum_t>(m_Value & static_cast<underlying_t>(flag));
}

template<seal::enum_type EnumT>
constexpr seal::flags<EnumT>::operator enum_t() const
{
	return static_cast<enum_t>(m_Value);
}