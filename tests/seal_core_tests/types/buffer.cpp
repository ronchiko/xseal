
#include <gtest/gtest.h>

#include "seal/types/buffer.hpp"

#include "destroy_t.h"

template<typename BufferT>
constexpr size_t count_buffer_elements(const seal::buffer<BufferT>& buf)
{
	size_t count = 0;
	for(const auto& in : buf) {
		count++;
	}

	return count;
}

TEST(test_buffer, test_buffer_sanity) {
	seal::buffer<int> buffer(512);
	
	// Check the buffers begin and end work
	ASSERT_EQ(count_buffer_elements(buffer), 512);

	// Check we can write to the buffer
	int i = 0;
	for (auto& element : buffer) {
		element = i++;
	}

	ASSERT_EQ(count_buffer_elements(buffer), 512);

	// Assert the write succeeded
	i = 0;
	for(const auto& element : buffer) {
		ASSERT_EQ(element, i++);
	}
}

TEST(test_buffer, test_buffer_constructors)
{
	seal::buffer<int> buffer(512);

	// Check the buffers begin and end work
	ASSERT_EQ(count_buffer_elements(buffer), 512);

	// Check a copy
	seal::buffer<int> buffer2 = buffer;
	ASSERT_NE(buffer2.data(), buffer.data());
	int i = 0;
	for(auto& element : buffer) {
		ASSERT_EQ(element, buffer2[i]);
	}

	seal::buffer<int> buffer3 = std::move(buffer);
	ASSERT_NE(buffer3.data(), buffer.data());
	ASSERT_EQ(count_buffer_elements(buffer), 0);
	ASSERT_EQ(count_buffer_elements(buffer3), 512);

	i = 0;
	for(auto& element : buffer3) {
		ASSERT_EQ(element, buffer2[i]);
	}
}

TEST(test_buffer, test_buffer_resize)
{
	seal::buffer<int> buffer(512);

	// Check the buffers begin and end work
	ASSERT_EQ(count_buffer_elements(buffer), 512);

	// Check buffer growth
	buffer.resize(1024);
	ASSERT_EQ(count_buffer_elements(buffer), 1024);

	// Check buffer shrink
	buffer.resize(256);
	ASSERT_EQ(count_buffer_elements(buffer), 256);
}

TEST(test_buffer, test_buffer_destruction)
{
	seal::ut::destory_t::reset();

	{
		seal::buffer<seal::ut::data_t> buffer(512);

		// Since the buffer doesn't have any actual items nothing should be destroyed.
		buffer.resize(1024);
		ASSERT_EQ(0, seal::ut::destory_t::destroy_count);

		// Fill the buffer.
		for (auto& item : buffer) {
			item = seal::ut::destory_t::alloc(12);
		}

		buffer.resize(256);
		// Still nothing should be destroyed, because shrinking doesn't involve reallocation.
		ASSERT_EQ(0, seal::ut::destory_t::destroy_count);

		// Now check no destructions happen on resize.
		buffer.resize(2048);
		// We expect 768 destructions because all the element that were shrinked out should now be destoryed.
		ASSERT_EQ(768, seal::ut::destory_t::destroy_count);
	}

	ASSERT_EQ(seal::ut::destory_t::alloc_count, seal::ut::destory_t::destroy_count);
}