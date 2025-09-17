Format: NDI
===================================================================================================

Nautilus provides bidirectional conversion between ``EntityTag`` and several formats defined by NDI
(the Nuclear Data Interface library): SZA, FPID, zaid, and short string.

NDI maintains a suite of "libraries" as the data sets they maintain evolve over time.  Each library
consists of a number of "tables".  The mapping from library to table is not unique: some libraries
use multiple tables, and some tables appear in multiple libraries.  Some details of some NDI
formats depend on which library/table is being used, so Nautilus allows the user to specify the
library or table in order to control how the SZA, FPID, and zaid are generated.  The library name
is a string with no standard format.  The table ID is a string of three digits followed by the
letters "nm"; because the letters are always the same in this context, Nautilus can use the full
table ID or simply the three-digit number part of the table ID.  In order to tell Nautilus which
library or table you are using, Nautilus will accept the following inputs:

- Library name as a ``std::string``
- Table ID without the trailing letters as an ``int``
- Table ID without the trailing letters as a ``double``

  - The table ID often appears as a suffix following a period.  For that reason, some users have
    taken to identifying the table as a decimal equal to the integer form of the table ID divided
    by 1,000.
  - Due to floating-point truncation issues, *this method is not recommended*.  It is provided to
    support customers already using such a convention.

- Table ID as a ``std::string``, either with or without the letter(s) at the end of the suffix

  - In the string format, the numerical part of the table ID must be a three-digit value, padded
    with leading zeroes.

If no library or table ID is specified, Nautilus defaults to some preferred variant (usually the
most-common and/or most-modern variant).

NDI SZA
---------------------------------------------------------------------------------------------------

The NDI SZA is an integer format (datatype ``int`` in Nautilus) that primarily represents nuclides,
but have special cases for a few particles.

The conversion routines available for this format are

- ``from_NDI_SZA(sza)`` takes an SZA and returns an ``EntityTag``
- ``to_NDI_SZA(entity_tag, library_or_table_id)`` takes an ``EntityTag`` and optionally a library
  or table ID, then returns an SZA

  - The possible ways to indicate a library or table ID are listed at the top of this page

Nuclides, by default, are encoded as integers according to

.. math:: \text{SZA} = S \times 1{,}000{,}000 + Z \times 1{,}000 + A,

where :math:`S` is the metastable state index, :math:`Z` is the atomic number,
and :math:`A` is the atomic mass number.  This arrangement of :math:`S`, :math:`Z`, and :math:`A`
gives rise to the name "SZA".

As with many formats used at LANL, americium-242 has issues for historical reasons.

- The ground state and first metastable state are reversed, so:

  - 95242 should be the ground state but is actually the first metastable state.
  - 1095242 should be the first metastable state but is actually the ground state.

- Certain libraries / tables used an older notation, where 95042 means the ground state

  - When reading the NDI SZA, Nautilus will accept either 95042 or 1095242 to mean the ground state
    of americium-242.
  - When generating the NDI SZA, Nautilus allows an optional argument to specify the library or
    table ID (see the top of this page for details).  Nautilus maintains a list of which libraries
    and table use the outdated 95042, and will generate the appropriate value (95042 or 1095242)
    depending on the specified library or table.  If no argument is provided, Nautilus will default
    to 1095242.

Additionally, the NDI SZA has issues with americium-244.  The ground and first metastable states
are not reversed for americium-244, but it does have two different values for the first metastable
state: 1095244 and 95044.  Similarly to americium-242, Nautilus maintains a list of the libraries
and tables that use the outdated 95044 in order to translate an ``EntityTag`` into an SZA, but will
default to 1095244 if no library or table identifier is provided.  When translating from an SZA to
an ``EntityTag``, Nautilus will accept either 1095244 or 95044.

The libraries and tables that use the outdated values (95042 and 95044) are listed in the table
below.  Some libraries and tables have both the outdated value and the new value for one or both of
the nuclides in question.  Nautilus only generates the outdated value if that is the *only* value
available in the library or table.  Any libraries or tables not listed here are assumed to use the
values 1095242 and 1095244.

+-------------+---------------+-------------+---------------+--------------+---------------+
| NDI library | table ID      | Am-242g SZA | Am-242g SZA   | Am-244m1 SZA | Am-244m1 SZA  |
|             |               | in NDI      | from Nautilus | in NDI       | from Nautilus |
+=============+===============+=============+===============+==============+===============+
| endf7act    | 700nm         | 95042 and   | 1095242       | 95044        | 95044         |
|             |               | 1095242     |               |              |               |
+-------------+---------------+-------------+---------------+--------------+---------------+
| mendf70x    | 701nm         | 95042       | 95042         | 1095244      | 1095244       |
+-------------+---------------+-------------+---------------+--------------+---------------+
| mtmg01      | 121nm - 135nm | 95042       | 95042         | not present  | 1095244       |
+-------------+---------------+-------------+---------------+--------------+---------------+
| mtmg01ex    | 121nm - 135nm | 95042       | 95042         | not present  | 1095244       |
+-------------+---------------+-------------+---------------+--------------+---------------+

The NDI SZA supports elementals by setting :math:`S` and :math:`A` both to zero.  For example,
26000 is the iron elemental.

The NDI SZA supports two particles:

- photons are encoded as 0
- neutrons are encoded as 1

An ``EntityTag`` for a proton will be automatically translated to H-1 (SZA = 1001).  That makes
this format slightly lossy: ``EntityTag(proton)`` becomes SZA 1001, and then converting back to
``EntityTag`` gives the H-1 nuclide.

User-defined entities are not supported by the NDI SZA format, so when translating from
``EntityTag`` to SZA you get the same result as an unknown tag: ``invalid_ndi_sza``, which has a
value of -1.

NDI FPID
---------------------------------------------------------------------------------------------------

The FPID is the "floating-point identifier" format (datatype ``double`` in Nautilus).  This is not
a "standard" NDI format, but is a variation that is used in some projects.  Due to floating-point
truncation issues, *this format is not recommended*.  It is only provided in order to support
customers who already use this format.

The conversion routines available for this format are

- ``from_NDI_FPID(sza)`` takes an FPID and returns an ``EntityTag``
- ``to_NDI_FPID(entity_tag, table_id)`` takes an ``EntityTag`` and a table ID, then returns an FPID

  - Unlike the SZA, this format explicitly includes a table ID, so the ``table_id`` argument is
    required.
  - Because the mapping between libraries and table IDs is not unique, this format specifically
    requires a table ID and will not accept a library name.
  - The table ID can be provided using the various formats described at the top of this page.

The NDI FPID is simply an extension of the SZA value with the numerical part of the table ID
appended as a suffix.  So the format looks like SZA.###, and then this is translated into a
floating-point value.  Otherwise, all details of the SZA apply to the FPID.

Unknown or unsupported values will be translated to the special value ``invalid_ndi_fpid``, which
has the value of ``std::numeric_limits<double>::max()``.

NDI Zaid
---------------------------------------------------------------------------------------------------

The zaid is the "most complete" identifier for NDI.  The name originally derives from :math:`Z`,
:math:`A`, and table ID.  It is a string-based format (datatype ``std::string`` in Nautilus).  As
with the FPID format, it is an extension of the SZA.  The zaid is "SZA.tableID", where the table ID
is the full five-character format.  Otherwise, all details of the SZA apply to the zaid.

The conversion routines available for this format are

- ``from_NDI_zaid(sza)`` takes an FPID and returns an ``EntityTag``
- ``to_NDI_zaid(entity_tag, table_id)`` takes an ``EntityTag`` and a table ID, then returns a zaid

  - Unlike the SZA, this format explicitly includes a table ID, so the ``table_id`` argument is
    required.
  - Because the mapping between libraries and table IDs is not unique, this format specifically
    requires a table ID and will not accept a library name.
  - The table ID can be provided using the various formats described at the top of this page.
    Nautilus can infer the trailing characters if a numerical format is used.

Unknown or unsupported values will be translated to the special value ``invalid_ndi_zaid``, which
has the value of "unknown".

NDI Short String
---------------------------------------------------------------------------------------------------

The NDI short string is a compact string-based notation used in NDI, especially for things like the
names of reactions.  In Nautilus, it uses the datatype ``std::string``.  It is closely related to
the short standard name and the IC chemsym formats.

The conversion routines available for this format are

- ``from_NDI_short_string(sza)`` takes a short string and returns an ``EntityTag``
- ``to_NDI_short_string(entity_tag)`` takes an ``EntityTag`` and returns a zaid

  - Unlike the SZA, FPID, and zaid formats, the short string does not depend on the library or
    table, so it does not accept a library or table ID argument

Nuclides mostly are the IUPAC atomic symbol (in all lowercase letters) followed by the atomic mass
number.  So, for example: "w184" or "si28".

This format does not permit any metastable state notation, so an ``EntityTag`` with a
non-ground-state nuclide will return the invalid value.  The exception is Am-242.  As with other
NDI formats, Am-242g and Am-242m1 are swapped.  Therefore an NDI short string of "am242" refers to
Am-242m1, and Am-242g cannot be represented as an NDI short string.

A few nuclides have special symbols:

- H-1 is represented as "p"
- H-2 is represented as "d"
- H-3 is represented as "t"
- He-4 is represented as "a"

Nautilus will accept "h1", "h2", "h3", and "he4" on input, but generates the special symbols listed
here.

The NDI short string format does not support elementals, so those will map to the invalid value.

Only two particles are supported by the NDI short string format

- photons are represented as either "g" or "g0"

  - Nautilus will generate "g" but will also accept "g0"

- neutrons are represented as "n"

  - because the NDI short string does not support elementals, there is no ambiguity between a
    neutron and a nitrogen elemental

An ``EntityTag`` for a proton will be automatically translated to H-1 (short string = "p").  That
makes this format slightly lossy: ``EntityTag(proton)`` becomes short string "p", and then
converting back to ``EntityTag`` gives the H-1 nuclide.

User-defined entities are not supported by the NDI short string format, so they will generate the
invalid value.

The invalid short string is given by ``invalid_ndi_short_string``, which has the value "unknown".
