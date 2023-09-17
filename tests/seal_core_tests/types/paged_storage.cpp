#include <unordered_map>

#include <gtest/gtest.h>

#include "seal/types/paged_storage.hpp"

#include "destroy_t.h"

using seal::ut::data_t;
using seal::ut::destory_t;

TEST(test_paged_storage, test_paged_storage_store_sanity)
{
	seal::paged_storage<512, int> storage;

	auto& a = storage.store(5);
	a = 4;

	ASSERT_EQ(a, 4);
}

TEST(test_paged_storage, test_paged_storage_store_reuse)
{
	seal::paged_storage<4096, int> storage;

	std::reference_wrapper<int> first = storage.store(6);
	int& second = storage.store(7);
	storage.erase(first.get());
	storage.erase(second);
	int& third = storage.store(8);

	/*std::vector<std::reference_wrapper<int>> values_to_remove;
	for(int i = 0; i < 720; ++i) {
		auto& a = storage.store(5);
		if(33 == i % 100) {
			values_to_remove.push_back(std::ref(a));
		}
	}*/

	// Erase selected items
	/*for(auto& ref : values_to_remove) {
		storage.erase(ref.get());
	}*/

	for(int i = 0; i < 7200; ++i) {
		storage.store(6);
	}
}

TEST(test_paged_storage, test_paged_storage_on_complex_type)
{
	seal::ut::destory_t::reset();
	using type_t = std::unordered_map<std::string, int>;
	seal::paged_storage<512, type_t> storage;

	// Add three items to the storage
	auto& to_be_erased = storage.store(type_t{});
}

TEST(test_paged_storage, test_paged_storage_destruction)
{
	seal::ut::destory_t::reset();

	{
		seal::paged_storage<512, data_t> storage;

		// Add three items to the storage
		auto& to_be_erased = storage.store(seal::ut::destory_t::alloc(5));
		storage.store(seal::ut::destory_t::alloc(6));
		storage.store(seal::ut::destory_t::alloc(7));

		// Erase one element explicitly
		storage.erase(to_be_erased);

		// Ensure that element was deleted.
		ASSERT_EQ(1, seal::ut::destory_t::destroy_count);
	} // Implicitly delete all elements

	// Ensure all the delete calls happend
	ASSERT_EQ(seal::ut::destory_t::alloc_count, seal::ut::destory_t::destroy_count);
}
