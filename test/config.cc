// Main Catch config file which generates the main function
#define CATCH_CONFIG_MAIN
// Force Catch to use cpp11's 'std::to_string'
// instead of 'std::stringstream' workaround.
#define CATCH_CONFIG_CPP11_TO_STRING

#include "catch2/catch.hpp"

TEST_CASE("Setup and compile catch for use in other test files", "[compilation-only]") {
    SUCCEED("Nothing to test.");
}

