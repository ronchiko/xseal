#pragma once

namespace seal {
	/**
	   A pointer to an arena allocated object.
	 */
	template<typename T>
	class arena_object
	{
	public:
		constexpr arena_object(T *value)
			: m_Value(value)
		{}

		constexpr T *operator->() noexcept
		{
			return m_Value;
		}

		constexpr const T *operator->() noexcept const
		{
			return m_Value;
		}

	private:
		T *m_Value;
	};
}
