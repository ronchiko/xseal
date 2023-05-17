
#include <gtest/gtest.h>

#include "seal/opt/math.hpp"
#include "seal/types/flags.hpp"

enum class dummy_enum : int
{
	f1 = seal::bit(0),
	f2 = seal::bit(1),
	f3 = seal::bit(3)
};

TEST(test_flag_object, test_flag_syntax) {
	seal::flags<dummy_enum> flags;

	flags.activate(dummy_enum::f1);
	// Classic comparison
	ASSERT_TRUE(static_cast<int>(flags.value()) & static_cast<int>(dummy_enum::f1));

	// Specific comparison
	ASSERT_TRUE(flags.is_active(dummy_enum::f1));
	ASSERT_FALSE(flags.is_active(dummy_enum::f2));
	ASSERT_FALSE(flags.is_active(dummy_enum::f3));

	flags |= dummy_enum::f2;
	ASSERT_TRUE(flags.is_active(dummy_enum::f1, dummy_enum::f2));
	ASSERT_FALSE(flags.is_active(dummy_enum::f3));

	flags.deactivate(dummy_enum::f1);
	ASSERT_FALSE(flags.is_active(dummy_enum::f1));
	ASSERT_TRUE(flags.is_active(dummy_enum::f2));
	ASSERT_FALSE(flags.is_active(dummy_enum::f3));

	// Check overloads works.
	ASSERT_TRUE((flags | dummy_enum::f2) == dummy_enum::f2);
	ASSERT_TRUE(static_cast<bool>(flags & dummy_enum::f2));

	seal::flags<dummy_enum> flags2 = dummy_enum::f1;
	ASSERT_TRUE(flags2.is_active(dummy_enum::f1));
	
	flags2 = flags | dummy_enum::f1;
	ASSERT_TRUE(flags2.is_active(dummy_enum::f1));

	flags2 = flags & dummy_enum::f1;
	ASSERT_FALSE(flags2.is_active(dummy_enum::f2));

	flags2 = seal::flags<dummy_enum>{ dummy_enum::f1, dummy_enum::f3 };
	ASSERT_TRUE(flags2.is_active(dummy_enum::f1, dummy_enum::f3));
	ASSERT_FALSE(flags2.is_active(dummy_enum::f2));
}
