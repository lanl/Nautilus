Format: Standard
===================================================================================================

The standard format provides bidirectional conversion between ``EntityTag`` and the accepted
standard names and symbols.

Standard Symbol
---------------------------------------------------------------------------------------------------

The standard symbol is represented by a ``std::string``.  The conversion functions are

- ``from_standard_symbol`` accepts a string and returns an ``EntityTag``
- ``to_standard_symbol`` accepts an ``EntityTag`` and returns the standard symbol as a string

Nuclides follow the standard IUPAC notation[1] and take the form "Xx-AAAmM", where

- "Xx" is the IUPAC symbol (first letter capitalized, second letter lowercase)
- "AAA" is the atomic mass number
- "M" is the metastable state index ("mM" is left off for the ground state)

The ``from_standard_symbol`` function also accepts variant suffixes:

- no suffix implies the ground state
- a suffix of "g" explicitly indicates the ground state
- a suffix of "m" without any value for "<M>" implies the first metastable state, equivalent to
  "m1"

Examples:

- "Ni-56g" (ground state of nickel-56)
- "C-12" (ground state of carbon-12)
- "Ta-180m1" (first metastable state of tantalum-180)

Elementals are just the atomic symbol by itself, for example "C" for elemental carbon.

Particles return a symbol using Unicode to represent the particle symbol.  These symbols use glyphs
not normally represented by ``std::string``, so there are a few important caveats

- The "alternate" particle format [2] symbols cannot all be represented using Unicode, so the
  symbol is always the PDG format [3] symbol.
- Because Unicode combining characters are necessary to represent some symbols, the length returned
  by ``std::string`` may not be the same as the number of characters printed by the string
- Not all subscripts are available in Unicode, so there are some substitutions.  For example, the
  symbol for a muon neutrino is a nu with a subscript mu, but no subscript mu is available.
  Instead the symbol returned is nu with a subscript m.  Similarly the long and short kaon normally
  use uppercase L and S subscripts respectively, but because of Unicode restrictions Nautilus uses
  lowercase l and s.
- The standard symbols stack superscripts above subscripts, but this is not possible in Unicode.
  Nautilus places the subscript first and then the superscript.

User-defined entities return the string "U:XXXXXXX", where "XXXXXXX" is the user data in
hexadecimal.

Unknown entities return ``invalid_standard_symbol``, which has the value "unknown".

Standard Name
---------------------------------------------------------------------------------------------------

The standard name is what is represented by a ``std::string``.  The conversion functions are

- ``from_standard_name`` accepts a string and returns an ``EntityTag``
- ``to_standard_name`` accepts an ``EntityTag`` and returns the standard name as a string

Unlike with the standard symbol, the standard name has several variants[1][4].  Therefore the
``to_standard_name`` accepts optional argments to specify the nuclide and/or particle variants.

- allowed nuclide formats are "IUPAC", "American", "British", and "Canadian"; default is IUPAC
- allowed particle formats are "PDG" and "alternate"; default is PDG

The rules for nuclides is the same as for the standard symbol, except

- the full name (all lowercase) is used instead of the atomic symbol, spelled according to the
  specified format

Examples:

- "nickel-56g" (ground state of nickel-56)
- "carbon-12" (ground state of carbon-12)
- "tantalum-180m1" (first metastable state of tantalum-180)

Elementals are written out as "elemental <name>", for example "elemental nitrogen".

Particles return a name based on the format specified.

User-defined entities return the string "user entity 0xXXXXXXX", where "XXXXXXX" is the user data
in hexadecimal.

Unknown entities return ``invalid_standard_name``, which has the value "unknown".

Notes and References
---------------------------------------------------------------------------------------------------

[1]: The official IUPAC names and symbols are recorded in various IUPAC documents, but a convenient
summary table can be found at https://en.wikipedia.org/wiki/List_of_chemical_elements.

[2]: The "alternate" format is used in a variety of sources.  One source that explains the notation
is Claude Amsler's "Nuclear and Particle Physics" (2015); see especially page 10-9 and note the
quark structures of the various particles and antiparticles.  Compare this to the Particle Data
Group format [3], especially item 7 in section 8.4 of "Naming Scheme for Hadrons".

[3]: The Particle Data Group defines the official standard names and symbols for  particles.  More
information can be found at https://pdg.lbl.gov/2023/listings/contents_listings.html.  A document
specifically on naming of particles can be found at
https://pdg.lbl.gov/2023/reviews/rpp2023-rev-naming-scheme-hadrons.pdf.

[4]: Alternate spellings can be found in the articles on Wikipedia for individual elements.
The three with regional spellings in English are aluminium/aluminum
(https://en.wikipedia.org/wiki/Aluminium), sulfur/sulphur (https://en.wikipedia.org/wiki/Sulfur),
and caesium/cesium (https://en.wikipedia.org/wiki/Caesium).
