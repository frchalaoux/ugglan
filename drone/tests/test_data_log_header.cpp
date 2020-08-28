#include <catch.h>
#include <catch_utils.h>

#include <ctime>
#include <string>

#include <data_log_header.h>

static const std::time_t HEADER_TIME = 0;
static const std::string HEADER_PATH = "./tests/resources/header.json";

TEST_CASE("data_log_header")
{
    std::string exp_header = read_file(HEADER_PATH);
    std::string act_header = generate_header(HEADER_TIME);

    REQUIRE(act_header.size() > 0);
    REQUIRE(exp_header.compare(act_header) == 0);
}
