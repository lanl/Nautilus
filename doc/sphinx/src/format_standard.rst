Format: Standard
===================================================================================================

The standard format provides bidirectional conversion between ``EntityTag`` and the accepted
standard names and symbols.

Short Standard Name
---------------------------------------------------------------------------------------------------

The "short standard" name is what is normally recognized as the standard symbol for each entity, in
the form of a ``std::string``.  The conversion functions are

- ``from_standard_name(string)`` accepts a string in either short-standard format or long-standard
  format and returns an ``EntityTag``

  - For consistency, Nautilus also provides the alias ``from_short_standard_name``.

- ``to_short_standard_format(entity)`` accepts an ``EntityTag`` and returns the short-standard
  format string

Nuclides follow the standard IUPAC notation and take the form "Xx-AAAmM", where

- "Xx" is the IUPAC symbol (first letter capitalized, second letter lowercase)
- "AAA" is the atomic mass number
- "M" is the metastable state index ("mM" is left off for the ground state)

The ``from_standard_name`` function also accepts variant suffixes:

- no suffix implies the ground state
- a suffix of "m" without any value for "<M>" implies the first metastable state, equivalent to
  "m1"
- a suffix of "g" explicitly indicates the ground state

Elementals are just the atomic symbol by itself.

Examples:

- "Ni-56g" (ground state of nickel-56)
- "C-12" (ground state of carbon-12)
- "Ta-180m1" (first metastable state of tantalum-180)
- "N" (elemental nitrogen)

Particles return a symbol using Unicode to represent the particle symbol.  These symbols use glyphs
not normally represented by ``std::string``, so there are a few important caveats

- The "alternate" particle format symbols cannot all be represented using Unicode, so the symbol is
  always the PDG format symbol.
- Because Unicode combining characters are necessary to represent some symbols, the length returned
  by ``std::string`` may not be the same as the number of characters printed by the string
- Not all subscripts are available in Unicode, so there are some substitutions.  For example, the
  symbol for a muon neutrino is a nu with a subscript mu, but no subscript mu is available.
  Instead the symbol returned is nu with a subscript m.  Similarly the long and short kaon normally
  use uppercase L and S subscripts respectively, but because of Unicode restrictions Nautilus uses
  lowercase l and s.
- The standard symbols stack superscripts above subscripts, but this is not possible in Unicode.
  Nautilus places the subscript first and then the superscript.

User-defined entities return the string "U:XXXXXX", where "XXXXXX" is the user data in
hexadecimal.

Unknown entities return ``invalid_short_standard_name``, which has the value "unknown".

Long Standard Name
---------------------------------------------------------------------------------------------------

The "long standard" name is what is normally recognized as the standard name for each entity, in
the form of a ``std::string``.  The conversion functions are

- ``from_standard_name(string)`` accepts a string in either short-standard format or long-standard
  format and returns an ``EntityTag``

  - For consistency, Nautilus also provides the alias ``from_long_standard_name``.

- ``to_long_standard_format(entity)`` accepts an ``EntityTag`` and returns the long-standard format
  string

Unlike with the short standard name, the long standard name can handle a variety of formats.
Therefore the ``to_long_standard_format`` accepts optional argments of the nuclide and/or particle
format to use

- allowed nuclide formats are "IUPAC", "American", "British", and "Canadian"
- allowed particle formats are "PDG" and "alternate"

The rules for nuclides is the same as for the short standard name, except

- the full name (all lowercase) is used instead of the atomic symbol, spelled according to the
  specified format (default: IUPAC)
- elementals are written out as "elemental <name>"

Examples:

- "nickel-56g" (ground state of nickel-56)
- "carbon-12" (ground state of carbon-12)
- "tantalum-180m1" (first metastable state of tantalum-180)
- "elemental nitrogen" (elemental nitrogen)

Particles return a name based on the format specified (default: PDG).

User-defined entities return the string "user entity 0xXXXXXX", where "XXXXXX" is the user data in
hexadecimal.

Unknown entities return ``invalid_long_standard_name``, which has the value "unknown".
