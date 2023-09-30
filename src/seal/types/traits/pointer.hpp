#pragma once

#include <memory>
#include <type_traits>

namespace seal {
	// ------------------ Unique pointer --------------------------
	template<typename>
	struct is_unique_pointer : std::false_type
	{};

	template<typename T>
	struct is_unique_pointer<std::unique_ptr<T>> : std::true_type
	{};

	template<typename>
	struct unique_pointer_traits
	{
		using type = nullptr_t;
		using pointer_type = nullptr_t;
	};

	template<typename T>
	struct unique_pointer_traits<std::unique_ptr<T>>
	{
		using type = T;
		using pointer_type = T *;
	};

	template<typename T>
	constexpr bool is_unique_pointer_v = is_unique_pointer<T>::value;

	template<typename T>
	concept unique_pointer = is_unique_pointer_v<T>;

	// ------------------ Shared pointer ---------------------------
	template<typename>
	struct is_shared_pointer : std::false_type
	{};

	template<typename T>
	struct is_shared_pointer<std::shared_ptr<T>> : std::true_type
	{};

	template<typename>
	struct shared_pointer_traits
	{
		using type = nullptr_t;
		using pointer_type = nullptr_t;
	};

	template<typename T>
	struct shared_pointer_traits<std::shared_ptr<T>>
	{
		using type = T;
		using pointer_type = T *;
	};

	template<typename T>
	constexpr bool is_shared_pointer_v = is_shared_pointer<T>::value;

	template<typename T>
	concept shared_pointer = is_shared_pointer_v<T>;

	// -------------------------- Other -----------------------------
	template<typename T>
	constexpr bool is_smart_pointer_v = is_unique_pointer_v<T> or is_shared_pointer_v<T>;

	/**
	   The raw pointer type of T (for example if T = std::unique_ptr<int> then this will return
	   int*). Works for normal type, raw pointers, unique_ptr & shared_ptr.
	 */
	template<typename T>
	using raw_ptr_type = std::conditional_t<
		is_unique_pointer_v<std::remove_reference_t<T>>,
		typename unique_pointer_traits<T>::pointer_type,
		std::conditional_t<is_shared_pointer_v<std::remove_reference_t<T>>,
						   typename shared_pointer_traits<std::remove_reference_t<T>>::pointer_type,
						   std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>,
											  T,
											  std::remove_reference_t<T> *>>>;

	/**
	   Given a type T returns a raw pointer to T's base type.
	   \see raw_pointer_type.

	   \param obj: The object to get the pointer of.
	 */
	template<typename T>
	constexpr raw_ptr_type<T> get_raw_pointer(T& obj)
	{
		if constexpr(is_smart_pointer_v<T>) {
			return obj.get();
		} else if constexpr(std::is_pointer_v<T>) {
			return obj;
		} else {
			return &obj;
		}
	}

	template<typename T>
	constexpr raw_ptr_type<const T> get_raw_pointer(const T& obj)
	{
		if constexpr(is_smart_pointer_v<T>) {
			return obj.get();
		} else if constexpr(std::is_pointer_v<T>) {
			return obj;
		} else {
			return &obj;
		}
	}
}
