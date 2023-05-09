
#include "gtest/gtest.h"

#include "seal/types/rotating_queue.hpp"

#include "destroy_t.h"

TEST(test_rotating_queue, test_rotating_queue_santiy)
{
	seal::rotating_queue<int> queue;

	for(int i = 0; i < 15; ++i) {
		queue.enqueue(i);
	}

	ASSERT_EQ(queue.size(), 15);

	for(int i = 0; i < 15; ++i) {
		ASSERT_EQ(queue.peek(), i);
		queue.dequeue();
	}

	ASSERT_EQ(queue.size(), 0);
	ASSERT_TRUE(queue.empty());
}

TEST(test_rotating_queue, test_rotating_queue_imprefect_dispatch)
{
	constexpr auto FIRST_ENQUEUE_COUNT = 5;
	constexpr auto FIRST_DEQUEUE_COUNT = 2;

	seal::rotating_queue<int> queue;

	for(int i = 0; i < FIRST_ENQUEUE_COUNT; ++i) {
		queue.enqueue(0x33);
	}

	ASSERT_EQ(queue.size(), FIRST_ENQUEUE_COUNT);

	for(int i = 0; i < FIRST_DEQUEUE_COUNT; ++i) {
		ASSERT_EQ(queue.peek(), 0x33);
		queue.dequeue();
	}

	ASSERT_EQ(queue.size(), FIRST_ENQUEUE_COUNT - FIRST_DEQUEUE_COUNT);

	// Insert some items to make sure we regrow
	for(int i = 0; i < 10; ++i) {
		queue.enqueue(0x33);
	}
}

TEST(test_rotating_queue, test_rotating_queue_destruction)
{
	seal::ut::destory_t::reset();

	{
		seal::rotating_queue<seal::ut::data_t> queue;

		// Insert items to the queue.
		queue.enqueue(seal::ut::destory_t::alloc(10));
		queue.enqueue(seal::ut::destory_t::alloc(11));
		queue.enqueue(seal::ut::destory_t::alloc(12));

		// The queue doesn't gurantee an immidiate destruction of a dequeued element.

		// Move the element out and delete it.
		{
			auto element = std::move(queue.peek());
			queue.dequeue();
		}
		ASSERT_EQ(1, seal::ut::destory_t::destroy_count);
	}

	// Ensure all elements were deleted.
	ASSERT_EQ(seal::ut::destory_t::alloc_count, seal::ut::destory_t::destroy_count);
}
