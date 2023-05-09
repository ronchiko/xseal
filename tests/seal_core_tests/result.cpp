
#include <gtest/gtest.h>

#include "seal/types/result.hpp"

TEST(test_result, test_reference_result) {
	struct my_struct
	{
		int a;
		int b;
		int c;

		int get_value() const {
			return a + b + c;
		}
	};
	
	seal::result<my_struct&> value = *new my_struct{};
	ASSERT_FALSE(value.is_error());

	value->a = 5;
	(*value).b = 3;
	value->c = 11;

	ASSERT_EQ(value->get_value(), 5 + 3 + 11);

	seal::result<my_struct&> other = seal::failure("AAAA");
	ASSERT_TRUE(other.is_error());
}
