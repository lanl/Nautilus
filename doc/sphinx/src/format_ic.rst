Format: IC
===================================================================================================

Nautilus provides bidirectional conversion between ``EntityTag`` and the IC chemsym format.  This
format comes from several codes in LANL's ASC-IC (Advanced Simulation and Computing - Integrated
Codes) program, and is related to IUPAC's chemical symbol (see the short standard name).  Nautilus
uses ``std::string`` as the type for the IC chemsym format.

To convert between ``EntityTag`` and IC chemsym, Nautilus provides the following functions

- ``to_IC_chemsym`` accepts an ``EntityTag`` and returns a string with the IC chemsym
- ``from_IC_chemsym`` accepts a string with the IC chemsym and returns an ``EntityTag``

Nuclides largely follow a variation of the standard IUPAC notation.  The default case is to encode
nuclides with the atomic symbol in all lowercase, no hyphen (which is used in the short standard
name format), the atomic number, and then a metastable state suffix.  The suffix can be

- no suffix: implies the ground state
- "g": explicit indicates the ground state
- "mS": indicates metastable state S
- "m" (without any number): indicates the first metastable state

When generating the IC chemsym format, Nautilus will use no suffix for the ground state and will
always include a number for the metastable state otherwise.

For historical reasons, the IC chemsym format (like many formats used at LANL) has an issue with
the americium-242 isotope:

- The ground state and first metastable state are reversed.

  - "am242" would normally be the ground state, but is actually the first metastable state
  - "am242m1" would normally be the first metastable state, but is actually the ground state

- Due to the confusion between the two, each state has multiple notations

  - The ground state can be indicated by "am242m1", "am042", and "am42".  When generating the IC
    chemsym format, Nautilus will use "am242m1".  When reading the IC chemsym format, Nautilus
    accepts all three.
  - The first metastable state can be indicated by "am242" or "am242g".  When generating the IC
    chemsym format, Nautilus will use "am242".  When reading the IC chemsym format, Nautilus
    accepts either.

Elementals are encoded as just the atomic symbol in all lowercase.

The IC chemsym format is primarily for nuclides, but does include a few particles:

- Photons (a.k.a., gamma particles) are encoded as either "g" or "g0".  Nautilus accepts both, but
  will generate "g0".
- Neutrons are encoded as "nt1" to avoid possible ambiguity with elemental nitrogen ("n").

The IC chemsym format does not have provisions for user-defined entities, so it will generate the
same value as unknown entities: "unknown" (available as the variable ``invalid_ic_chemsym``).
