#include <array>

#include "nautilus/tag/format_mcnp.hpp"
#include "nautilus/tag/names.hpp"
#include "nautilus/tag/pantag.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

TEST_CASE("format: MCNP partial zaid", "[tag][format][MCNP]")
{
    using nautilus::tag::from_MCNP_partial_zaid;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_MCNP_partial_zaid;

    // Normal nuclides
    // -- all the "main isotopes" I scraped from the Wikipedia sidebars on the pages for elements
    // -- more extensive than perhaps necessary to verify the metastable algorithm
    struct nuc {
        int Z;
        int A;
    };
    constexpr int N = 704;
    std::array<nuc,N> isotopes{
        nuc{1, 1},
        nuc{1, 2},
        nuc{1, 3},
        nuc{2, 3},
        nuc{2, 4},
        nuc{3, 6},
        nuc{3, 7},
        nuc{4, 7},
        nuc{4, 8},
        nuc{4, 9},
        nuc{4, 10},
        nuc{5, 8},
        nuc{5, 10},
        nuc{5, 11},
        nuc{6, 11},
        nuc{6, 12},
        nuc{6, 13},
        nuc{6, 14},
        nuc{7, 13},
        nuc{7, 14},
        nuc{7, 15},
        nuc{7, 16},
        nuc{8, 15},
        nuc{8, 16},
        nuc{8, 17},
        nuc{8, 18},
        nuc{9, 17},
        nuc{9, 18},
        nuc{9, 19},
        nuc{10, 20},
        nuc{10, 21},
        nuc{10, 22},
        nuc{11, 22},
        nuc{11, 23},
        nuc{11, 24},
        nuc{12, 24},
        nuc{12, 25},
        nuc{12, 26},
        nuc{13, 26},
        nuc{13, 27},
        nuc{14, 28},
        nuc{14, 29},
        nuc{14, 30},
        nuc{14, 31},
        nuc{14, 32},
        nuc{15, 31},
        nuc{15, 32},
        nuc{15, 33},
        nuc{16, 32},
        nuc{16, 33},
        nuc{16, 34},
        nuc{16, 35},
        nuc{16, 36},
        nuc{17, 35},
        nuc{17, 36},
        nuc{17, 37},
        nuc{18, 36},
        nuc{18, 37},
        nuc{18, 38},
        nuc{18, 39},
        nuc{18, 40},
        nuc{18, 41},
        nuc{18, 42},
        nuc{19, 39},
        nuc{19, 40},
        nuc{19, 41},
        nuc{20, 40},
        nuc{20, 41},
        nuc{20, 42},
        nuc{20, 43},
        nuc{20, 44},
        nuc{20, 45},
        nuc{20, 46},
        nuc{20, 47},
        nuc{20, 48},
        nuc{21, 43},
        nuc{21, 44},
        nuc{21, 45},
        nuc{21, 46},
        nuc{21, 47},
        nuc{21, 48},
        nuc{22, 44},
        nuc{22, 45},
        nuc{22, 46},
        nuc{22, 47},
        nuc{22, 48},
        nuc{22, 49},
        nuc{22, 50},
        nuc{23, 48},
        nuc{23, 49},
        nuc{23, 50},
        nuc{23, 51},
        nuc{24, 50},
        nuc{24, 51},
        nuc{24, 52},
        nuc{24, 53},
        nuc{24, 54},
        nuc{25, 52},
        nuc{25, 53},
        nuc{25, 54},
        nuc{25, 55},
        nuc{26, 54},
        nuc{26, 55},
        nuc{26, 56},
        nuc{26, 57},
        nuc{26, 58},
        nuc{26, 59},
        nuc{26, 60},
        nuc{27, 56},
        nuc{27, 57},
        nuc{27, 58},
        nuc{27, 59},
        nuc{27, 60},
        nuc{28, 56},
        nuc{28, 57},
        nuc{28, 58},
        nuc{28, 59},
        nuc{28, 60},
        nuc{28, 61},
        nuc{28, 62},
        nuc{28, 63},
        nuc{28, 64},
        nuc{29, 63},
        nuc{29, 64},
        nuc{29, 65},
        nuc{29, 67},
        nuc{30, 64},
        nuc{30, 65},
        nuc{30, 66},
        nuc{30, 67},
        nuc{30, 68},
        nuc{30, 69},
        nuc{30, 70},
        nuc{30, 72},
        nuc{31, 66},
        nuc{31, 67},
        nuc{31, 68},
        nuc{31, 69},
        nuc{31, 70},
        nuc{31, 71},
        nuc{31, 72},
        nuc{31, 73},
        nuc{32, 68},
        nuc{32, 70},
        nuc{32, 71},
        nuc{32, 72},
        nuc{32, 73},
        nuc{32, 74},
        nuc{32, 76},
        nuc{33, 73},
        nuc{33, 74},
        nuc{33, 75},
        nuc{34, 72},
        nuc{34, 74},
        nuc{34, 75},
        nuc{34, 76},
        nuc{34, 77},
        nuc{34, 78},
        nuc{34, 79},
        nuc{34, 80},
        nuc{34, 82},
        nuc{35, 75},
        nuc{35, 76},
        nuc{35, 77},
        nuc{35, 79},
        nuc{35, 80},
        nuc{35, 81},
        nuc{35, 82},
        nuc{36, 78},
        nuc{36, 79},
        nuc{36, 80},
        nuc{36, 81},
        nuc{36, 82},
        nuc{36, 83},
        nuc{36, 84},
        nuc{36, 85},
        nuc{36, 86},
        nuc{37, 82},
        nuc{37, 83},
        nuc{37, 84},
        nuc{37, 85},
        nuc{37, 86},
        nuc{37, 87},
        nuc{38, 82},
        nuc{38, 83},
        nuc{38, 84},
        nuc{38, 85},
        nuc{38, 86},
        nuc{38, 87},
        nuc{38, 88},
        nuc{38, 89},
        nuc{38, 90},
        nuc{39, 87},
        nuc{39, 88},
        nuc{39, 89},
        nuc{39, 90},
        nuc{39, 91},
        nuc{40, 88},
        nuc{40, 89},
        nuc{40, 90},
        nuc{40, 91},
        nuc{40, 92},
        nuc{40, 93},
        nuc{40, 94},
        nuc{40, 95},
        nuc{40, 96},
        nuc{41, 91},
        nuc{41, 92},
        nuc{41, 93},
        nuc{41, 94},
        nuc{41, 95},
        nuc{42, 92},
        nuc{42, 93},
        nuc{42, 94},
        nuc{42, 95},
        nuc{42, 96},
        nuc{42, 97},
        nuc{42, 98},
        nuc{42, 99},
        nuc{42, 100},
        nuc{43, 96},
        nuc{43, 97},
        nuc{43, 98},
        nuc{43, 99},
        nuc{44, 96},
        nuc{44, 97},
        nuc{44, 98},
        nuc{44, 99},
        nuc{44, 100},
        nuc{44, 101},
        nuc{44, 102},
        nuc{44, 103},
        nuc{44, 104},
        nuc{44, 105},
        nuc{45, 99},
        nuc{45, 100},
        nuc{45, 101},
        nuc{45, 102},
        nuc{45, 103},
        nuc{45, 105},
        nuc{46, 100},
        nuc{46, 102},
        nuc{46, 103},
        nuc{46, 104},
        nuc{46, 105},
        nuc{46, 106},
        nuc{46, 107},
        nuc{46, 108},
        nuc{46, 110},
        nuc{47, 105},
        nuc{47, 107},
        nuc{47, 109},
        nuc{47, 111},
        nuc{48, 106},
        nuc{48, 107},
        nuc{48, 108},
        nuc{48, 109},
        nuc{48, 110},
        nuc{48, 111},
        nuc{48, 112},
        nuc{48, 113},
        nuc{48, 114},
        nuc{48, 115},
        nuc{48, 116},
        nuc{49, 111},
        nuc{49, 113},
        nuc{49, 115},
        nuc{50, 112},
        nuc{50, 113},
        nuc{50, 114},
        nuc{50, 115},
        nuc{50, 116},
        nuc{50, 117},
        nuc{50, 118},
        nuc{50, 119},
        nuc{50, 120},
        nuc{50, 122},
        nuc{50, 123},
        nuc{50, 124},
        nuc{50, 126},
        nuc{51, 121},
        nuc{51, 123},
        nuc{51, 125},
        nuc{52, 120},
        nuc{52, 121},
        nuc{52, 122},
        nuc{52, 123},
        nuc{52, 124},
        nuc{52, 125},
        nuc{52, 126},
        nuc{52, 127},
        nuc{52, 128},
        nuc{52, 129},
        nuc{52, 130},
        nuc{53, 123},
        nuc{53, 124},
        nuc{53, 125},
        nuc{53, 127},
        nuc{53, 129},
        nuc{53, 131},
        nuc{53, 135},
        nuc{54, 124},
        nuc{54, 125},
        nuc{54, 126},
        nuc{54, 127},
        nuc{54, 128},
        nuc{54, 129},
        nuc{54, 130},
        nuc{54, 131},
        nuc{54, 132},
        nuc{54, 133},
        nuc{54, 134},
        nuc{54, 135},
        nuc{54, 136},
        nuc{55, 131},
        nuc{55, 133},
        nuc{55, 134},
        nuc{55, 135},
        nuc{55, 137},
        nuc{56, 130},
        nuc{56, 131},
        nuc{56, 132},
        nuc{56, 133},
        nuc{56, 134},
        nuc{56, 135},
        nuc{56, 136},
        nuc{56, 137},
        nuc{56, 138},
        nuc{56, 140},
        nuc{57, 137},
        nuc{57, 138},
        nuc{57, 139},
        nuc{57, 140},
        nuc{58, 134},
        nuc{58, 136},
        nuc{58, 138},
        nuc{58, 139},
        nuc{58, 140},
        nuc{58, 141},
        nuc{58, 142},
        nuc{58, 143},
        nuc{58, 144},
        nuc{59, 141},
        nuc{59, 142},
        nuc{59, 143},
        nuc{60, 140},
        nuc{60, 142},
        nuc{60, 143},
        nuc{60, 144},
        nuc{60, 145},
        nuc{60, 146},
        nuc{60, 147},
        nuc{60, 148},
        nuc{60, 150},
        nuc{61, 143},
        nuc{61, 144},
        nuc{61, 145},
        nuc{61, 146},
        nuc{61, 147},
        nuc{62, 144},
        nuc{62, 145},
        nuc{62, 146},
        nuc{62, 147},
        nuc{62, 148},
        nuc{62, 149},
        nuc{62, 150},
        nuc{62, 151},
        nuc{62, 152},
        nuc{62, 153},
        nuc{62, 154},
        nuc{63, 145},
        nuc{63, 146},
        nuc{63, 147},
        nuc{63, 148},
        nuc{63, 149},
        nuc{63, 150},
        nuc{63, 151},
        nuc{63, 152},
        nuc{63, 153},
        nuc{63, 154},
        nuc{63, 155},
        nuc{63, 156},
        nuc{64, 148},
        nuc{64, 150},
        nuc{64, 151},
        nuc{64, 152},
        nuc{64, 153},
        nuc{64, 154},
        nuc{64, 155},
        nuc{64, 156},
        nuc{64, 157},
        nuc{64, 158},
        nuc{64, 159},
        nuc{64, 160},
        nuc{65, 157},
        nuc{65, 158},
        nuc{65, 159},
        nuc{65, 160},
        nuc{65, 161},
        nuc{66, 154},
        nuc{66, 156},
        nuc{66, 158},
        nuc{66, 159},
        nuc{66, 160},
        nuc{66, 161},
        nuc{66, 162},
        nuc{66, 163},
        nuc{66, 164},
        nuc{66, 165},
        nuc{66, 166},
        nuc{67, 163},
        nuc{67, 164},
        nuc{67, 165},
        nuc{67, 166},
        nuc{67, 167},
        nuc{68, 162},
        nuc{68, 164},
        nuc{68, 165},
        nuc{68, 166},
        nuc{68, 167},
        nuc{68, 168},
        nuc{68, 169},
        nuc{68, 170},
        nuc{69, 167},
        nuc{69, 168},
        nuc{69, 169},
        nuc{69, 170},
        nuc{69, 171},
        nuc{70, 166},
        nuc{70, 168},
        nuc{70, 169},
        nuc{70, 170},
        nuc{70, 171},
        nuc{70, 172},
        nuc{70, 173},
        nuc{70, 174},
        nuc{70, 175},
        nuc{70, 176},
        nuc{71, 173},
        nuc{71, 174},
        nuc{71, 175},
        nuc{71, 176},
        nuc{71, 177},
        nuc{72, 172},
        nuc{72, 174},
        nuc{72, 175},
        nuc{72, 176},
        nuc{72, 177},
        nuc{72, 178},
        nuc{72, 179},
        nuc{72, 180},
        nuc{72, 181},
        nuc{72, 182},
        nuc{73, 177},
        nuc{73, 178},
        nuc{73, 179},
        nuc{73, 180},
        nuc{73, 181},
        nuc{73, 182},
        nuc{73, 183},
        nuc{74, 180},
        nuc{74, 181},
        nuc{74, 182},
        nuc{74, 183},
        nuc{74, 184},
        nuc{74, 185},
        nuc{74, 186},
        nuc{74, 187},
        nuc{74, 188},
        nuc{75, 183},
        nuc{75, 184},
        nuc{75, 185},
        nuc{75, 186},
        nuc{75, 187},
        nuc{76, 184},
        nuc{76, 185},
        nuc{76, 186},
        nuc{76, 187},
        nuc{76, 188},
        nuc{76, 189},
        nuc{76, 190},
        nuc{76, 191},
        nuc{76, 192},
        nuc{76, 193},
        nuc{76, 194},
        nuc{77, 189},
        nuc{77, 190},
        nuc{77, 191},
        nuc{77, 192},
        nuc{77, 193},
        nuc{78, 190},
        nuc{78, 191},
        nuc{78, 192},
        nuc{78, 193},
        nuc{78, 194},
        nuc{78, 195},
        nuc{78, 196},
        nuc{78, 198},
        nuc{79, 195},
        nuc{79, 196},
        nuc{79, 197},
        nuc{79, 198},
        nuc{79, 199},
        nuc{80, 194},
        nuc{80, 195},
        nuc{80, 196},
        nuc{80, 197},
        nuc{80, 198},
        nuc{80, 199},
        nuc{80, 200},
        nuc{80, 201},
        nuc{80, 202},
        nuc{80, 203},
        nuc{80, 204},
        nuc{81, 201},
        nuc{81, 202},
        nuc{81, 203},
        nuc{81, 204},
        nuc{81, 205},
        nuc{82, 202},
        nuc{82, 204},
        nuc{82, 205},
        nuc{82, 206},
        nuc{82, 207},
        nuc{82, 208},
        nuc{82, 209},
        nuc{82, 210},
        nuc{82, 211},
        nuc{82, 212},
        nuc{82, 213},
        nuc{83, 207},
        nuc{83, 208},
        nuc{83, 209},
        nuc{83, 210},
        nuc{84, 206},
        nuc{84, 208},
        nuc{84, 209},
        nuc{84, 210},
        nuc{85, 207},
        nuc{85, 208},
        nuc{85, 209},
        nuc{85, 210},
        nuc{85, 211},
        nuc{86, 210},
        nuc{86, 211},
        nuc{86, 220},
        nuc{86, 222},
        nuc{87, 212},
        nuc{87, 221},
        nuc{87, 222},
        nuc{87, 223},
        nuc{88, 223},
        nuc{88, 224},
        nuc{88, 225},
        nuc{88, 226},
        nuc{88, 228},
        nuc{89, 225},
        nuc{89, 226},
        nuc{89, 227},
        nuc{89, 228},
        nuc{90, 227},
        nuc{90, 228},
        nuc{90, 229},
        nuc{90, 230},
        nuc{90, 231},
        nuc{90, 232},
        nuc{90, 233},
        nuc{90, 234},
        nuc{91, 229},
        nuc{91, 230},
        nuc{91, 231},
        nuc{91, 232},
        nuc{91, 233},
        nuc{91, 234},
        nuc{92, 232},
        nuc{92, 233},
        nuc{92, 234},
        nuc{92, 235},
        nuc{92, 236},
        nuc{92, 238},
        nuc{93, 235},
        nuc{93, 236},
        nuc{93, 237},
        nuc{93, 238},
        nuc{93, 239},
        nuc{94, 236},
        nuc{94, 238},
        nuc{94, 239},
        nuc{94, 240},
        nuc{94, 241},
        nuc{94, 242},
        nuc{94, 244},
        nuc{95, 241},
        //nuc{95, 242}, skip this because it's a special case to be checked later
        nuc{95, 243},
        nuc{96, 242},
        nuc{96, 243},
        nuc{96, 244},
        nuc{96, 245},
        nuc{96, 246},
        nuc{96, 247},
        nuc{96, 248},
        nuc{96, 250},
        nuc{97, 245},
        nuc{97, 246},
        nuc{97, 247},
        nuc{97, 248},
        nuc{97, 249},
        nuc{98, 248},
        nuc{98, 249},
        nuc{98, 250},
        nuc{98, 251},
        nuc{98, 252},
        nuc{98, 253},
        nuc{98, 254},
        nuc{99, 252},
        nuc{99, 253},
        nuc{99, 254},
        nuc{99, 255},
        nuc{100, 252},
        nuc{100, 253},
        nuc{100, 255},
        nuc{100, 257},
        nuc{101, 256},
        nuc{101, 257},
        nuc{101, 258},
        nuc{101, 259},
        nuc{101, 260},
        nuc{102, 253},
        nuc{102, 254},
        nuc{102, 255},
        nuc{102, 257},
        nuc{102, 259},
        nuc{103, 255},
        nuc{103, 256},
        nuc{103, 260},
        nuc{103, 261},
        nuc{103, 262},
        nuc{103, 264},
        nuc{103, 266},
        nuc{104, 263},
        nuc{104, 265},
        nuc{104, 267},
        nuc{105, 262},
        nuc{105, 263},
        nuc{105, 266},
        nuc{105, 267},
        nuc{105, 268},
        nuc{105, 270},
        nuc{106, 265},
        nuc{106, 267},
        nuc{106, 268},
        nuc{106, 269},
        nuc{106, 271},
        nuc{107, 267},
        nuc{107, 270},
        nuc{107, 271},
        nuc{107, 272},
        nuc{107, 274},
        nuc{107, 278},
        nuc{108, 269},
        nuc{108, 270},
        nuc{108, 271},
        nuc{109, 274},
        nuc{109, 276},
        nuc{109, 278},
        nuc{109, 282},
        nuc{110, 279},
        nuc{110, 281},
        nuc{111, 279},
        nuc{111, 280},
        nuc{111, 281},
        nuc{111, 282},
        nuc{111, 283},
        nuc{111, 286},
        nuc{112, 283},
        nuc{112, 285},
        nuc{112, 286},
        nuc{113, 278},
        nuc{113, 282},
        nuc{113, 283},
        nuc{113, 284},
        nuc{113, 285},
        nuc{113, 286},
        nuc{113, 287},
        nuc{113, 290},
        nuc{114, 284},
        nuc{114, 285},
        nuc{114, 286},
        nuc{114, 287},
        nuc{114, 288},
        nuc{114, 289},
        nuc{114, 290},
        nuc{115, 286},
        nuc{115, 287},
        nuc{115, 288},
        nuc{115, 289},
        nuc{115, 290},
        nuc{116, 290},
        nuc{116, 291},
        nuc{116, 292},
        nuc{116, 293},
        nuc{117, 293},
        nuc{117, 294},
        nuc{118, 294}
    };
    for (const auto iso : isotopes) {
        // The MCNP manual notes that excited states can be m = 1, 2, 3, or 4
        for (int m = 0; m < 5; ++m) {
            Pantag tag(iso.Z, iso.A, Pantag::Index::metastable, m);
            int partial_zaid = iso.Z * 1000 + iso.A;
            if (m != 0) {
                partial_zaid += 300 + m * 100;
            }
            CHECK((from_MCNP_partial_zaid(partial_zaid) == tag));
            CHECK(to_MCNP_partial_zaid(tag) == partial_zaid);
        }
    }

    // Special cases

    // Am-242g and Am-242m1 are swapped
    // m == 0
    constexpr Pantag am242g_tag(95, 242);
    constexpr int am242g_zaid = 95642; // should be 95242 except it's swapped with Am-242m1
    CHECK((from_MCNP_partial_zaid(am242g_zaid) == am242g_tag));
    CHECK(to_MCNP_partial_zaid(am242g_tag) == am242g_zaid);
    // m = 1
    constexpr Pantag am242m1_tag(95, 242, Pantag::Index::metastable, 1);
    constexpr int am242m1_zaid = 95242; // should be 95642 except it's swapped with Am-242g
    CHECK((from_MCNP_partial_zaid(am242m1_zaid) == am242m1_tag));
    CHECK(to_MCNP_partial_zaid(am242m1_tag) == am242m1_zaid);
    // m = 2
    constexpr Pantag am242m2_tag(95, 242, Pantag::Index::metastable, 2);
    constexpr int am242m2_zaid = 95742; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m2_zaid) == am242m2_tag));
    CHECK(to_MCNP_partial_zaid(am242m2_tag) == am242m2_zaid);
    // m = 3
    constexpr Pantag am242m3_tag(95, 242, Pantag::Index::metastable, 3);
    constexpr int am242m3_zaid = 95842; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m3_zaid) == am242m3_tag));
    CHECK(to_MCNP_partial_zaid(am242m3_tag) == am242m3_zaid);
    // m = 4
    constexpr Pantag am242m4_tag(95, 242, Pantag::Index::metastable, 4);
    constexpr int am242m4_zaid = 95942; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m4_zaid) == am242m4_tag));
    CHECK(to_MCNP_partial_zaid(am242m4_tag) == am242m4_zaid);

    // m > 4 is not allowed
    // -- 200 <= A < 300 with m = 5 would lead to overflow that changes the Z value, so in general
    //    m > 4 is not permitted
    CHECK(to_MCNP_partial_zaid(6812) == -1);
    CHECK(to_MCNP_partial_zaid(6912) == -1);
    CHECK(to_MCNP_partial_zaid(47907) == -1);

    // proton vs H-1
    // TODO

    // elementals
    CHECK((from_MCNP_partial_zaid(6000) == Pantag(6, Pantag::elemental)));
    CHECK(to_MCNP_partial_zaid(Pantag(13, Pantag::elemental)) == 13000);

    // particles
    CHECK(to_MCNP_partial_zaid(Pantag(nautilus::tag::names::muon)) == -1);

    // user
    CHECK(to_MCNP_partial_zaid(Pantag(Pantag::user, 0)) == -1);

    // unknown
    CHECK(to_MCNP_partial_zaid(Pantag(Pantag::unknown)) == -1);
}

// ================================================================================================

// TODO: MCNP full zaid
// TODO: MCNP particle symbol

/*TEST_CASE("format: NDI zaid", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_zaid;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_NDI_zaid;

    // Particles
    constexpr Pantag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_zaid("1.999nm") == neutron));
    CHECK(to_NDI_zaid(neutron, 0.999) == "1.999nm");
    CHECK(to_NDI_zaid(neutron, 999) == "1.999nm");
    CHECK(to_NDI_zaid(neutron, "999") == "1.999nm");
    CHECK(to_NDI_zaid(neutron, "999nm") == "1.999nm");

    constexpr Pantag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_zaid("0.000nm") == photon));
    CHECK(to_NDI_zaid(photon, 0.456) == "0.456nm");
    CHECK(to_NDI_zaid(photon, 789) == "0.789nm");
    CHECK(to_NDI_zaid(photon, "123") == "0.123nm");
    CHECK(to_NDI_zaid(photon, "369nm") == "0.369nm");

    // Normal nuclides
    constexpr Pantag co59g(27, 59);
    CHECK((from_NDI_zaid("27059.123nm") == co59g));
    CHECK(to_NDI_zaid(co59g, 0.234) == "27059.234nm");
    CHECK(to_NDI_zaid(co59g, 345) == "27059.345nm");
    CHECK(to_NDI_zaid(co59g, "456") == "27059.456nm");
    CHECK(to_NDI_zaid(co59g, "567nm") == "27059.567nm");

    constexpr Pantag ta180m1(73, 180, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("1073180.987nm") == ta180m1));
    CHECK(to_NDI_zaid(ta180m1, 0.876) == "1073180.876nm");
    CHECK(to_NDI_zaid(ta180m1, 765) == "1073180.765nm");
    CHECK(to_NDI_zaid(ta180m1, "654") == "1073180.654nm");
    CHECK(to_NDI_zaid(ta180m1, "543nm") == "1073180.543nm");

    constexpr Pantag k38m2(19, 38, Pantag::Index::metastable, 2);
    CHECK((from_NDI_zaid("2019038.123nm") == k38m2));
    CHECK(to_NDI_zaid(k38m2, 0.000) == "2019038.000nm");
    CHECK(to_NDI_zaid(k38m2, 5) == "2019038.005nm");
    CHECK(to_NDI_zaid(k38m2, "050") == "2019038.050nm");
    CHECK(to_NDI_zaid(k38m2, "500nm") == "2019038.500nm");

    // Special cases

    // Am-242g
    // -- Am-242g and Am-242m1 are swapped in NDI zaid
    // -- Am-242g can take different values
    //    -- 1095242 (most cases)
    //    -- 95042 (specific cases)
    //       -- mendf70x / 701nm
    //       -- mtmg01 / 121nm - 135nm
    //       -- mtmg01ex / 121nm - 135nm
    constexpr Pantag am242g(95, 242);
    CHECK((from_NDI_zaid("1095242.120nm") == am242g)); // suffix that should be 1095242
    CHECK((from_NDI_zaid("1095242.123nm") == am242g)); // suffix that should be 95042 if writing
    CHECK((from_NDI_zaid("95042.120nm") == am242g));   // suffix that should be 1095242 if writing
    CHECK((from_NDI_zaid("95042.123nm") == am242g));   // suffix that should be 95042
    CHECK(to_NDI_zaid(am242g, "083") == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, "083nm") == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, 83) == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, 0.083) == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, "701") == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, "701nm") == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, 701) == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, 0.701) == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, "121") == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, "128") == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, "135") == "95042.135nm");
    CHECK(to_NDI_zaid(am242g, "121nm") == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, "128nm") == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, "135nm") == "95042.135nm");
    CHECK(to_NDI_zaid(am242g, 121) == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, 128) == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, 135) == "95042.135nm");
    CHECK(to_NDI_zaid(am242g, 0.121) == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, 0.128) == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, 0.135) == "95042.135nm");

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped in NDI zaid
    constexpr Pantag am242m1(95, 242, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("95242.133nm") == am242m1));
    CHECK(to_NDI_zaid(am242m1, 0.134) == "95242.134nm");
    CHECK(to_NDI_zaid(am242m1, 135) == "95242.135nm");
    CHECK(to_NDI_zaid(am242m1, "136") == "95242.136nm");
    CHECK(to_NDI_zaid(am242m1, "137nm") == "95242.137nm");

    // Am-242m2
    // -- should be normal
    constexpr Pantag am242m2(95, 242, Pantag::Index::metastable, 2);
    CHECK((from_NDI_zaid("2095242.111nm") == am242m2));
    CHECK(to_NDI_zaid(am242m2, 0.333) == "2095242.333nm");
    CHECK(to_NDI_zaid(am242m2, 555) == "2095242.555nm");
    CHECK(to_NDI_zaid(am242m2, "777") == "2095242.777nm");
    CHECK(to_NDI_zaid(am242m2, "999nm") == "2095242.999nm");

    // Am-243g
    // -- should be normal
    constexpr Pantag am243g(95, 243);
    CHECK((from_NDI_zaid("95243.867nm") == am243g));
    CHECK(to_NDI_zaid(am243g, 0.675) == "95243.675nm");
    CHECK(to_NDI_zaid(am243g, 753) == "95243.753nm");
    CHECK(to_NDI_zaid(am243g, "530") == "95243.530nm");
    CHECK(to_NDI_zaid(am243g, "309nm") == "95243.309nm");

    // Am-243m1
    // -- should be normal
    constexpr Pantag am243m1(95, 243, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("1095243.555nm") == am243m1));
    CHECK(to_NDI_zaid(am243m1, 0.555) == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, 555) == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, "555") == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, "555nm") == "1095243.555nm");

    // Am-243m2
    // -- should be normal
    constexpr Pantag am243m2(95, 243, Pantag::Index::metastable, 2);
    CHECK((from_NDI_zaid("2095243.135nm") == am243m2));
    CHECK(to_NDI_zaid(am243m2, 0.135) == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, 135) == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, "135") == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, "135nm") == "2095243.135nm");

    // Am-244g
    // -- should be normal
    constexpr Pantag am244g(95, 244);
    CHECK((from_NDI_zaid("95244.951nm") == am244g));
    CHECK(to_NDI_zaid(am244g, 0.951) == "95244.951nm");
    CHECK(to_NDI_zaid(am244g, 951) == "95244.951nm");
    CHECK(to_NDI_zaid(am244g, "951") == "95244.951nm");
    CHECK(to_NDI_zaid(am244g, "951nm") == "95244.951nm");

    // Am-244m1
    // -- Am-244m1 can take different values
    //    -- 1095244 (most cases)
    //    -- 95044 (specific cases)
    //       -- endf7act / 660nm
    constexpr Pantag am244m1(95, 244, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("1095244.600nm") == am244m1)); // suffix that should be 1095244
    CHECK((from_NDI_zaid("1095244.700nm") == am244m1)); // suffix that should be 95044 if writing
    CHECK((from_NDI_zaid("95044.600nm") == am244m1));   // suffix that should be 1095244 if writing
    CHECK((from_NDI_zaid("95044.700nm") == am244m1));   // suffix that should be 95044
    CHECK(to_NDI_zaid(am244m1, "060nm") == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, "060") == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, 60) == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, 0.060) == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, "700nm") == "95044.700nm");
    CHECK(to_NDI_zaid(am244m1, "700") == "95044.700nm");
    CHECK(to_NDI_zaid(am244m1, 700) == "95044.700nm");
    CHECK(to_NDI_zaid(am244m1, 0.700) == "95044.700nm");

    // Am-244m2
    // -- should be normal
    constexpr Pantag am244m2(95, 244, Pantag::Index::metastable, 2);
    CHECK((from_NDI_zaid("2095244.700nm") == am244m2));
    CHECK(to_NDI_zaid(am244m2, 0.701) == "2095244.701nm");
    CHECK(to_NDI_zaid(am244m2, 702) == "2095244.702nm");
    CHECK(to_NDI_zaid(am244m2, "703") == "2095244.703nm");
    CHECK(to_NDI_zaid(am244m2, "704nm") == "2095244.704nm");
}

// ================================================================================================

TEST_CASE("format: NDI short string", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_short_string;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_NDI_short_string;

    // Particles
    constexpr Pantag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_short_string("n") == neutron));
    CHECK(to_NDI_short_string(neutron) == "n");

    constexpr Pantag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_short_string("g") == photon));
    CHECK((from_NDI_short_string("g0") == photon));
    CHECK(to_NDI_short_string(photon) == "g");

    // Normal nuclides
    constexpr Pantag co59g(27, 59);
    CHECK((from_NDI_short_string("co59") == co59g));
    CHECK(to_NDI_short_string(co59g) == "co59");

    constexpr Pantag ta180(73, 180);
    CHECK((from_NDI_short_string("ta180") == ta180));
    CHECK(to_NDI_short_string(ta180) == "ta180");

    constexpr Pantag k38(19, 38);
    CHECK((from_NDI_short_string("k38") == k38));
    CHECK(to_NDI_short_string(k38) == "k38");

    // Special cases

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped: "am242" -> Am-242m1; Am-242g not representable
    constexpr Pantag am242m1(95, 242, Pantag::Index::metastable, 1);
    CHECK((from_NDI_short_string("am242") == am242m1));
    CHECK(to_NDI_short_string(am242m1) == "am242");

    // Am-243g
    // -- should be normal
    constexpr Pantag am243g(95, 243);
    CHECK((from_NDI_short_string("am243") == am243g));
    CHECK(to_NDI_short_string(am243g) == "am243");

    // Am-244g
    // -- should be normal
    constexpr Pantag am244g(95, 244);
    CHECK((from_NDI_short_string("am244") == am244g));
    CHECK(to_NDI_short_string(am244g) == "am244");
}*/
