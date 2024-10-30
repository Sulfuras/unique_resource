#include "gtest.h"

using ::testing::Return;

int main( int argc, char **argv )
{
    std::locale::global( std::locale( ".UTF8" ) );

    ::testing::InitGoogleTest( &argc, argv );

    return RUN_ALL_TESTS();
}
