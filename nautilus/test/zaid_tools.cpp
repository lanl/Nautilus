#include "zaid_tools.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>

TEST_CASE("chemsym to zaid", "[zaid_tools]")
{
    using nautilus::chemsym_to_zaid;
    int const zaid_g = chemsym_to_zaid("g");
    CHECK(zaid_g == 0);
    int const zaid_n = chemsym_to_zaid("n");
    CHECK(zaid_n == 1);
    int const zaid_p = chemsym_to_zaid("p");
    CHECK(zaid_p == 1001);
    int const zaid_h1 = chemsym_to_zaid("h1");
    CHECK(zaid_h1 == 1001);
    int const zaid_d = chemsym_to_zaid("d");
    CHECK(zaid_d == 1002);
    int const zaid_h2 = chemsym_to_zaid("h2");
    CHECK(zaid_h2 == 1002);
    int const zaid_t = chemsym_to_zaid("t");
    CHECK(zaid_t == 1003);
    int const zaid_h3 = chemsym_to_zaid("h3");
    CHECK(zaid_h3 == 1003);
    int const zaid_he3 = chemsym_to_zaid("he3");
    CHECK(zaid_he3 == 2003);
    int const zaid_a = chemsym_to_zaid("a");
    CHECK(zaid_a == 2004);
    int const zaid_he4 = chemsym_to_zaid("he4");
    CHECK(zaid_he4 == 2004);
    int const zaid_li6 = chemsym_to_zaid("li6");
    CHECK(zaid_li6 == 3006);
    int const zaid_li7 = chemsym_to_zaid("li7");
    CHECK(zaid_li7 == 3007);
    int const zaid_c12 = chemsym_to_zaid("c12");
    CHECK(zaid_c12 == 6012);
    int const zaid_ni56 = chemsym_to_zaid("ni56");
    CHECK(zaid_ni56 == 28056);
    int const zaid_es253 = chemsym_to_zaid("es253");
    CHECK(zaid_es253 == 99253);
    int const zaid_og294 = chemsym_to_zaid("og294");
    CHECK(zaid_og294 == 118294);
}

TEST_CASE("isotope zaids from reaction zaid", "[zaid_tools]")
{
    using nautilus::get_isotope_zaids;
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
    auto dtna_isotopes = get_isotope_zaids(dtna);
    std::sort(dtna_isotopes.begin(), dtna_isotopes.end());
    CHECK(dtna_isotopes.size() == 4);
    CHECK(dtna_isotopes[0] == 1);
    CHECK(dtna_isotopes[1] == 1002);
    CHECK(dtna_isotopes[2] == 1003);
    CHECK(dtna_isotopes[3] == 2004);

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
    auto nb10_isotopes = get_isotope_zaids(nb10);
    std::sort(nb10_isotopes.begin(), nb10_isotopes.end());
    CHECK(nb10_isotopes.size() == 5);
    CHECK(nb10_isotopes[0] == 0);
    CHECK(nb10_isotopes[1] == 1);
    CHECK(nb10_isotopes[2] == 2004);
    CHECK(nb10_isotopes[3] == 3007);
    CHECK(nb10_isotopes[4] == 5010);

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
    auto tt2na_isotopes = get_isotope_zaids(tt2na);
    std::sort(tt2na_isotopes.begin(), tt2na_isotopes.end());
    CHECK(tt2na_isotopes.size() == 3);
    CHECK(tt2na_isotopes[0] == 1);
    CHECK(tt2na_isotopes[1] == 1003);
    CHECK(tt2na_isotopes[2] == 2004);
}
