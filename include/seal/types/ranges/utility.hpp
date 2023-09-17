#pragma once

#include <ranges>

namespace seal::ranges {

	namespace detail {

		// clang-format off
		template<typename Iterator>
		concept has_iterator_category_type = requires {
			{ Iterator::iterator_category };
		};

		// clang-format on

		// This struct is a hack to get around the fact that `std::ranges::transform_view` only has
		// the iterator_category field if the iterator is a forward iterator. It also defines the
		// `pointer` and `reference` because does are missing and required by std::iterator_traits.
		template<typename Base>
		struct infered_input_iterator : public Base
		{
			infered_input_iterator(Base&& b)
				: Base(std::move(b))
			{}

			using iterator_category = std::input_iterator_tag;

			// Keep these void to not because the constructors dont need them.
			using pointer = void;
			using reference = void;
		};

		template<typename Iterator>
		struct infered_range
		{
			Iterator m_begin, m_end;

			constexpr auto begin() const
			{
				return m_begin;
			}

			constexpr auto end() const
			{
				return m_end;
			}
		};

		// An helper object to help us lazily construct a container from a range.
		template<template<typename...> typename Container>
		class lazy_constructor
		{
		public:
			template<std::ranges::range Iterable>
			auto construct(Iterable&& iterable)
			{
				using iterator = decltype(iterable.begin());
				using value_type = decltype(*iterable.begin());

				if constexpr(!has_iterator_category_type<iterator>) {
					// If we dont have the iterator_category type, infer the category of our
					// iterator is input_iterator because its the weakest form of an iterator.

					const auto begin = infered_input_iterator(iterable.begin());
					const auto end = infered_input_iterator(iterable.end());

					return construct_range(begin, end);
				} else {
					return construct_range(iterable.begin(), iterable.end());
				}
			}

			template<typename Iterator>
			static auto construct_range(Iterator&& begin, Iterator&& end)
			{
				return Container(begin, end);
			}
		};

		template<template<typename...> typename Container>
		class lazy_move_constructor : public lazy_constructor<Container>
		{
		public:
			template<std::ranges::range Iterable>
			auto construct(Iterable&& iterable)
			{
				const auto begin = std::make_move_iterator(iterable.begin());
				const auto end = std::make_move_iterator(iterable.end());


				return lazy_constructor<Container>::construct(infered_range{begin, end});
			}
		};
	}

	/**
		Converts a range into a container.
	 */
	template<template<typename...> typename Container>
	constexpr auto to()
	{
		return detail::lazy_constructor<Container>();
	}

	/**
		Converts a range into a container.
	*/
	template<template<typename...> typename Container>
	constexpr auto move_to()
	{
		return detail::lazy_move_constructor<Container>();
	}
}

template<std::ranges::range Iterable, template<typename...> typename Container>
constexpr auto operator|(Iterable&& iterable,
						 seal::ranges::detail::lazy_constructor<Container> builder)
{
	return builder.construct(iterable);
}

template<std::ranges::range Iterable, template<typename...> typename Container>
constexpr auto operator|(Iterable&& iterable,
						 seal::ranges::detail::lazy_move_constructor<Container> builder)
{
	return builder.construct(iterable);
}
