
#include "gtest.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <ranges>
#include <unique_resource.h>
#include <vector>

namespace test_unique_resource {

    auto pass_unique_resource( std::ostream& out )
    {
        return std::make_unique_resource( std::make_unique< int >( 42 ),
                                          [ &out ]( auto& i ) { out << "cleaned " << *i; } );
    }

    class test_unique_resource : public testing::Test
    {
    protected:
        test_unique_resource() {};
    };

    TEST_F( test_unique_resource, test_semantics )
    {
        std::ostringstream out {};
        {
            auto res = std::make_unique_resource( 1, [ &out ]( int i ) { out << "cleaned " << i; } );
        }

        EXPECT_EQ( out.str(), "cleaned 1" );
    }

    TEST_F( test_unique_resource, test_semantics_reset )
    {
        std::ostringstream out {};
        {
            auto cleanup = std::make_unique_resource( 1, [ &out ]( int i ) { out << "cleaned " << i; } );
            cleanup.reset( 2 );
        }

        EXPECT_EQ( out.str(), "cleaned 1cleaned 2" );
    }

    TEST_F( test_unique_resource, test_semantics_reset_move )
    {
        std::ostringstream out {};
        {
            auto cleanup = std::make_unique_resource( std::make_unique< int >( 42 ),
                                                      [ &out ]( auto const& ) { out << "cleaned "; } );
            cleanup.reset( nullptr );
        }
        EXPECT_EQ( out.str(), "cleaned cleaned " );
    }

    TEST_F( test_unique_resource, test_semantics_release )
    {
        std::ostringstream out {};
        {
            auto cleanup = std::make_unique_resource( 5, [ &out ]( int i ) { out << "cleaned " << i; } );
            EXPECT_EQ( cleanup.release(), 5 );
        }
        EXPECT_EQ( out.str(), "" );
    }

    TEST_F( test_unique_resource, test_with_pointer )
    {
        std::ostringstream out {};
        {
            auto cleanup = std::make_unique_resource( static_cast< char const* const >( "hello" ),
                                                      [ &out ]( const char* s ) { out << "cleaned " << s; } );
            EXPECT_EQ( *cleanup, 'h' );
        }
        EXPECT_EQ( out.str(), "cleaned hello" );
    }

    TEST_F( test_unique_resource, test_address_of )
    {
        std::ostringstream out {};
        {
            int j = 4;
            auto cleanup = std::make_unique_resource( std::move( j ), [ &out ]( int i ) { out << "cleaned " << i; } );
            EXPECT_EQ( *&cleanup, j );
        }
        EXPECT_EQ( out.str(), "cleaned 4" );
    }

    TEST_F( test_unique_resource, test_with_failure_value )
    {
        std::ostringstream out {};
        {
            auto cleanup = std::make_unique_resource_checked( -1, -1, [ &out ]( int i ) { out << "cleaned " << i; } );
            EXPECT_EQ( cleanup.release(), -1 );
        }
        EXPECT_EQ( out.str(), "" );
    }

    TEST_F( test_unique_resource, test_move_enable )
    {
        std::ostringstream out {};
        {
            auto cleanup = std::make_unique_resource( -1, [ &out ]( int i ) { out << "cleaned " << i; } );
            auto cleanup2 = std::move( cleanup );
            cleanup2.release();
            cleanup2.reset( 42 );
        }
        EXPECT_EQ( out.str(), "cleaned 42" );
    }

    TEST_F( test_unique_resource, test_unique_resource_can_be_moved )
    {
        std::ostringstream out {};
        {
            auto cleanup = pass_unique_resource( out );
            EXPECT_EQ( *cleanup.get(), 42 );
        }
        EXPECT_EQ( out.str(), "cleaned 42" );
    }
}
