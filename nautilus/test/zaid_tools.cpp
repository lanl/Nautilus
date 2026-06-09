#include "zaid_tools.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <string>

TEST_CASE("nuclide zaids from reaction zaid", "[zaid_tools]")
{
    using nautilus::get_nuclide_zaids;
    using nautilus::get_product_zaids;
    using nautilus::get_reactant_zaids;

    // special cases
    std::string const dtna{"d+t->n+a"};
    std::cout << dtna << std::endl;
    auto dtna_reactants = get_reactant_zaids(dtna);
    CHECK(dtna_reactants.size() == 2);
    CHECK(dtna_reactants[0] == 1002);
    CHECK(dtna_reactants[1] == 1003);
    auto dtna_products = get_product_zaids(dtna);
    CHECK(dtna_products.size() == 2);
    CHECK(dtna_products[0] == 1);
    CHECK(dtna_products[1] == 2004);
    auto dtna_nuclides = get_nuclide_zaids(dtna);
    std::sort(dtna_nuclides.begin(), dtna_nuclides.end());
    CHECK(dtna_nuclides.size() == 4);
    CHECK(dtna_nuclides[0] == 1);
    CHECK(dtna_nuclides[1] == 1002);
    CHECK(dtna_nuclides[2] == 1003);
    CHECK(dtna_nuclides[3] == 2004);

    // three reactants
    std::string const nb10{"n+b10->a+li7+g"};
    std::cout << nb10 << std::endl;
    auto nb10_reactants = get_reactant_zaids(nb10);
    CHECK(nb10_reactants.size() == 2);
    CHECK(nb10_reactants[0] == 1);
    CHECK(nb10_reactants[1] == 5010);
    auto nb10_products = get_product_zaids(nb10);
    CHECK(nb10_products.size() == 3);
    CHECK(nb10_products[0] == 2004);
    CHECK(nb10_products[1] == 3007);
    CHECK(nb10_products[2] == 0);
    auto nb10_nuclides = get_nuclide_zaids(nb10);
    std::sort(nb10_nuclides.begin(), nb10_nuclides.end());
    CHECK(nb10_nuclides.size() == 5);
    CHECK(nb10_nuclides[0] == 0);
    CHECK(nb10_nuclides[1] == 1);
    CHECK(nb10_nuclides[2] == 2004);
    CHECK(nb10_nuclides[3] == 3007);
    CHECK(nb10_nuclides[4] == 5010);

    // multiplicity > 1 in both reactants and products
    std::string const tt2na{"t+t->2n+a"};
    std::cout << tt2na << std::endl;
    auto tt2na_reactants = get_reactant_zaids(tt2na);
    CHECK(tt2na_reactants.size() == 1);
    CHECK(tt2na_reactants[0] == 1003);
    auto tt2na_products = get_product_zaids(tt2na);
    CHECK(tt2na_products.size() == 2);
    CHECK(tt2na_products[0] == 1);
    CHECK(tt2na_products[1] == 2004);
    auto tt2na_nuclides = get_nuclide_zaids(tt2na);
    std::sort(tt2na_nuclides.begin(), tt2na_nuclides.end());
    CHECK(tt2na_nuclides.size() == 3);
    CHECK(tt2na_nuclides[0] == 1);
    CHECK(tt2na_nuclides[1] == 1003);
    CHECK(tt2na_nuclides[2] == 2004);
}
