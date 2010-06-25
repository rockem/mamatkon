#include <gmock/gmock.h>
#include <core/fraction.h>

using namespace Mamatkon;

TEST(FractionTest, TestShouldGetDecimalValueOfFraction) {
	EXPECT_EQ(0.75, Fraction("3/4").toFloat());
	EXPECT_EQ(1.75, Fraction("1 3/4").toFloat());
	EXPECT_EQ(2, Fraction("2").toFloat());
}