#include "catch.hpp"

namespace
{
    constexpr char id1[] = "id1";
    constexpr char id2[] = "id2";

    // Provide two overloads of test(std::vector<const char*>&).  The overloads
    // will be unambiguously distinguisable by the number of template
    // parameters they each accept.

    // This overload accepts exactly one template parameter.
    template<const char *Key>
    void test_template(std::vector<const char*>& out)
    {
        out.push_back(Key);
    }

    // This overload accepts two or more template parameters.
    template<const char *Key1, const char *Key2, const char*...Keys>
    void test_template(std::vector<const char*>& out)
    {
        // Call the single parameter version of the function to keep the logic
        // in once place.
        test_template<Key1>(out);
        test_template<Key2, Keys...>(out);
    }
}

SCENARIO("template pattern example") {
    std::vector<const char*> ids;
    // Execute the function 'test_template' on each item in the parameter pack,
    // going left to right.
    test_template<id1, id2>(ids);

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == id1);
    REQUIRE(ids[1] == id2);
}

