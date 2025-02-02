#include <catch2/catch_test_macros.hpp>




TEST_CASE( "test", "[test]" ) {

    SECTION( "test" ) {

        REQUIRE( 1 == 1 );
    }
}