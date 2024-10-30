
#include "gtest.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <ranges>
#include <unique_resource.h>
#include <vector>

namespace test_unique_resource {

    class test_unique_resource : public testing::Test
    {
    protected:
        test_unique_resource() {};
    };

    TEST_F( test_unique_resource, test1 )
    {
        EXPECT_TRUE( false );
    }
}
