#include "string_processing.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("tokenize nuclide", "[string_processing]")
{
    using nautilus::entity_tag::tokenize_nuclide;
    auto tokens = tokenize_nuclide("abc987DEF654");
    CHECK(tokens[0] == "abc");
    CHECK(tokens[1] == "987");
    CHECK(tokens[2] == "DEF654");
    tokens = tokenize_nuclide("a0-a");
    CHECK(tokens[0] == "a");
    CHECK(tokens[1] == "0");
    CHECK(tokens[2] == "-a");
}

// ================================================================================================

TEST_CASE("to lowercase", "[string_processing]")
{
    using nautilus::entity_tag::to_lower;
    // letters
    CHECK(to_lower('a') == 'a');
    CHECK(to_lower('b') == 'b');
    CHECK(to_lower('c') == 'c');
    CHECK(to_lower('d') == 'd');
    CHECK(to_lower('e') == 'e');
    CHECK(to_lower('f') == 'f');
    CHECK(to_lower('g') == 'g');
    CHECK(to_lower('h') == 'h');
    CHECK(to_lower('i') == 'i');
    CHECK(to_lower('j') == 'j');
    CHECK(to_lower('k') == 'k');
    CHECK(to_lower('l') == 'l');
    CHECK(to_lower('m') == 'm');
    CHECK(to_lower('n') == 'n');
    CHECK(to_lower('o') == 'o');
    CHECK(to_lower('p') == 'p');
    CHECK(to_lower('q') == 'q');
    CHECK(to_lower('r') == 'r');
    CHECK(to_lower('s') == 's');
    CHECK(to_lower('t') == 't');
    CHECK(to_lower('u') == 'u');
    CHECK(to_lower('v') == 'v');
    CHECK(to_lower('w') == 'w');
    CHECK(to_lower('x') == 'x');
    CHECK(to_lower('y') == 'y');
    CHECK(to_lower('z') == 'z');
    CHECK(to_lower('A') == 'a');
    CHECK(to_lower('B') == 'b');
    CHECK(to_lower('C') == 'c');
    CHECK(to_lower('D') == 'd');
    CHECK(to_lower('E') == 'e');
    CHECK(to_lower('F') == 'f');
    CHECK(to_lower('G') == 'g');
    CHECK(to_lower('H') == 'h');
    CHECK(to_lower('I') == 'i');
    CHECK(to_lower('J') == 'j');
    CHECK(to_lower('K') == 'k');
    CHECK(to_lower('L') == 'l');
    CHECK(to_lower('M') == 'm');
    CHECK(to_lower('N') == 'n');
    CHECK(to_lower('O') == 'o');
    CHECK(to_lower('P') == 'p');
    CHECK(to_lower('Q') == 'q');
    CHECK(to_lower('R') == 'r');
    CHECK(to_lower('S') == 's');
    CHECK(to_lower('T') == 't');
    CHECK(to_lower('U') == 'u');
    CHECK(to_lower('V') == 'v');
    CHECK(to_lower('W') == 'w');
    CHECK(to_lower('X') == 'x');
    CHECK(to_lower('Y') == 'y');
    CHECK(to_lower('Z') == 'z');
    // numbers (no-op)
    CHECK(to_lower('0') == '0');
    CHECK(to_lower('1') == '1');
    CHECK(to_lower('2') == '2');
    CHECK(to_lower('3') == '3');
    CHECK(to_lower('4') == '4');
    CHECK(to_lower('5') == '5');
    CHECK(to_lower('6') == '6');
    CHECK(to_lower('7') == '7');
    CHECK(to_lower('8') == '8');
    CHECK(to_lower('9') == '9');
    // symbols (no-op)
    CHECK(to_lower('~') == '~');
    CHECK(to_lower('`') == '`');
    CHECK(to_lower('!') == '!');
    CHECK(to_lower('@') == '@');
    CHECK(to_lower('#') == '#');
    CHECK(to_lower('$') == '$');
    CHECK(to_lower('%') == '%');
    CHECK(to_lower('^') == '^');
    CHECK(to_lower('&') == '&');
    CHECK(to_lower('*') == '*');
    CHECK(to_lower('(') == '(');
    CHECK(to_lower(')') == ')');
    CHECK(to_lower('-') == '-');
    CHECK(to_lower('_') == '_');
    CHECK(to_lower('=') == '=');
    CHECK(to_lower('+') == '+');
    CHECK(to_lower('[') == '[');
    CHECK(to_lower('{') == '{');
    CHECK(to_lower(']') == ']');
    CHECK(to_lower('}') == '}');
    CHECK(to_lower('\\') == '\\');
    CHECK(to_lower('|') == '|');
    CHECK(to_lower(';') == ';');
    CHECK(to_lower(':') == ':');
    CHECK(to_lower('\'') == '\'');
    CHECK(to_lower('"') == '"');
    CHECK(to_lower(',') == ',');
    CHECK(to_lower('<') == '<');
    CHECK(to_lower('.') == '.');
    CHECK(to_lower('>') == '>');
    CHECK(to_lower('/') == '/');
    CHECK(to_lower('?') == '?');
}

// ================================================================================================

TEST_CASE("to uppercase", "[string_processing]")
{
    using nautilus::entity_tag::to_upper;
    // letters
    CHECK(to_upper('a') == 'A');
    CHECK(to_upper('b') == 'B');
    CHECK(to_upper('c') == 'C');
    CHECK(to_upper('d') == 'D');
    CHECK(to_upper('e') == 'E');
    CHECK(to_upper('f') == 'F');
    CHECK(to_upper('g') == 'G');
    CHECK(to_upper('h') == 'H');
    CHECK(to_upper('i') == 'I');
    CHECK(to_upper('j') == 'J');
    CHECK(to_upper('k') == 'K');
    CHECK(to_upper('l') == 'L');
    CHECK(to_upper('m') == 'M');
    CHECK(to_upper('n') == 'N');
    CHECK(to_upper('o') == 'O');
    CHECK(to_upper('p') == 'P');
    CHECK(to_upper('q') == 'Q');
    CHECK(to_upper('r') == 'R');
    CHECK(to_upper('s') == 'S');
    CHECK(to_upper('t') == 'T');
    CHECK(to_upper('u') == 'U');
    CHECK(to_upper('v') == 'V');
    CHECK(to_upper('w') == 'W');
    CHECK(to_upper('x') == 'X');
    CHECK(to_upper('y') == 'Y');
    CHECK(to_upper('z') == 'Z');
    CHECK(to_upper('A') == 'A');
    CHECK(to_upper('B') == 'B');
    CHECK(to_upper('C') == 'C');
    CHECK(to_upper('D') == 'D');
    CHECK(to_upper('E') == 'E');
    CHECK(to_upper('F') == 'F');
    CHECK(to_upper('G') == 'G');
    CHECK(to_upper('H') == 'H');
    CHECK(to_upper('I') == 'I');
    CHECK(to_upper('J') == 'J');
    CHECK(to_upper('K') == 'K');
    CHECK(to_upper('L') == 'L');
    CHECK(to_upper('M') == 'M');
    CHECK(to_upper('N') == 'N');
    CHECK(to_upper('O') == 'O');
    CHECK(to_upper('P') == 'P');
    CHECK(to_upper('Q') == 'Q');
    CHECK(to_upper('R') == 'R');
    CHECK(to_upper('S') == 'S');
    CHECK(to_upper('T') == 'T');
    CHECK(to_upper('U') == 'U');
    CHECK(to_upper('V') == 'V');
    CHECK(to_upper('W') == 'W');
    CHECK(to_upper('X') == 'X');
    CHECK(to_upper('Y') == 'Y');
    CHECK(to_upper('Z') == 'Z');
    // numbers (no-oP)
    CHECK(to_upper('0') == '0');
    CHECK(to_upper('1') == '1');
    CHECK(to_upper('2') == '2');
    CHECK(to_upper('3') == '3');
    CHECK(to_upper('4') == '4');
    CHECK(to_upper('5') == '5');
    CHECK(to_upper('6') == '6');
    CHECK(to_upper('7') == '7');
    CHECK(to_upper('8') == '8');
    CHECK(to_upper('9') == '9');
    // symbols (no-op)
    CHECK(to_upper('~') == '~');
    CHECK(to_upper('`') == '`');
    CHECK(to_upper('!') == '!');
    CHECK(to_upper('@') == '@');
    CHECK(to_upper('#') == '#');
    CHECK(to_upper('$') == '$');
    CHECK(to_upper('%') == '%');
    CHECK(to_upper('^') == '^');
    CHECK(to_upper('&') == '&');
    CHECK(to_upper('*') == '*');
    CHECK(to_upper('(') == '(');
    CHECK(to_upper(')') == ')');
    CHECK(to_upper('-') == '-');
    CHECK(to_upper('_') == '_');
    CHECK(to_upper('=') == '=');
    CHECK(to_upper('+') == '+');
    CHECK(to_upper('[') == '[');
    CHECK(to_upper('{') == '{');
    CHECK(to_upper(']') == ']');
    CHECK(to_upper('}') == '}');
    CHECK(to_upper('\\') == '\\');
    CHECK(to_upper('|') == '|');
    CHECK(to_upper(';') == ';');
    CHECK(to_upper(':') == ':');
    CHECK(to_upper('\'') == '\'');
    CHECK(to_upper('"') == '"');
    CHECK(to_upper(',') == ',');
    CHECK(to_upper('<') == '<');
    CHECK(to_upper('.') == '.');
    CHECK(to_upper('>') == '>');
    CHECK(to_upper('/') == '/');
    CHECK(to_upper('?') == '?');
}

// ================================================================================================

TEST_CASE("my_strcmp", "[string_processing]")
{
    using nautilus::entity_tag::my_strcmp;
    CHECK(my_strcmp("yes", "yes"));
    CHECK(!my_strcmp("yes", "yes!"));
    std::string s = "string";
    std::string_view sv(s);
    CHECK(my_strcmp(sv, "string"));
    CHECK(!my_strcmp(sv, "STRING"));
}
