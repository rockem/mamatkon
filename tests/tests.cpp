#include <gmock/gmock.h>

int main(int argc, char** argv) {
	// ::MaTest::GTEST_FLAG(throw_on_failure) = true;
	// ::MaTest::InitGoogleMock(&argc, argv);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
    // return UnitTest::RunAllTests();
}
