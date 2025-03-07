.. _`Overview`:

Overview
===================================================================================================

Currently Nautilus provides only a very minimal set of shared utilities, essentially pieces that
were originally written for Singe but were identified as being potentially useful to SuNRe.  The
goal is to further develop the capabilities of Nautilus and provide new features that will be more
widely useful, while also supporting its shared code goal for atomic and nuclear applications.

Nautilus makes use of common terms and symbols for atomic nuclei, which are summarized `here`_.

.. _`here`: `Atomic Nuclei`_

SZA
---------------------------------------------------------------------------------------------------

An SZA is a way to denote a particular nuclide.  Due to its use in various data libraries, such as
the Nuclear Data Interface (NDI), the SZA is common in many codes.

An SZA is an integer value, defined as

.. math:: \text{SZA} = S * 1,000,000 + Z * 1,000 + A

Nautilus provides the ``SZA`` class, which stores the SZA value and provides the following
accessors:

- ``A()`` -- the atomic mass number
- ``S()`` -- the metastable index
- ``Z()`` -- the atomic number
- ``N()`` -- the neutron number

It also provides the following additional operations:

- explicit cast to an integer value
- all comparison operators: ``<``, ``<=``, ``==``, ``!=``, ``>=``, ``>``; comparison is ordered
  first by :math:`Z`, then by :math:`A` for nuclides with the same :math:`Z` value, then by
  :math:`S` for nuclides with the same :math:`Z` and :math:`A` values

Isotope
---------------------------------------------------------------------------------------------------

(TODO: Right now it's called ``Isotope``, but would it make sense to change this to ``Nuclide`` in
the future?  Similar for related types like ``ReactionIsotope``.  The argument for this would be to
clarify terminology: an "element" has the same Z but may differ in N and S, and "isotope" has the
same Z and N but may differ in S, and a "nuclide" has the same Z, N, and S.  Is that the correct
terminology to apply?)

The ``Isotope`` class consists of an SZA and a mass, providing a slightly more detailed description
of the nuclide in question.  It provides the following accessors:

- ``A()`` -- the atomic mass number
- ``N()`` -- the neutron number
- ``S()`` -- the metastable index
- ``Z()`` -- the atomic number
- ``sza()`` -- the ``SZA`` instance identifying the nuclide
- ``mass()`` -- the mass of the nuclide

The ``Isotope`` class also provides all comparison operators, ordered the same as the ``SZA``
class.

Nautilus also provides the streaming operator for an ``Isotope``, providing a simple way to print
values.

ReactionIsotope
---------------------------------------------------------------------------------------------------

The ``ReactionIsotope`` class extends the ``Isotope`` class, additionally adding an index so that
information related to this nuclide can be accessed from arrays, so long as those arrays are
ordered consistently.  In addition to the accessors and operators provided by ``Isotope``, the
``ReactionIsotope`` class adds the ``index()`` accessor, which returns the index.

(TODO: ``ReactionIsotope`` vs ``ReactionNuclide`` vs ``IndexedNuclide`` vs ``IndexedIsotope`` vs
...?)

Zaid Tools
---------------------------------------------------------------------------------------------------

The ``zaid_tools.hpp`` header file provides some useful tools for parsing zaids, which are
identifiers used by NDI.  This header is likely to be superceded by a more extensive family of
tools in future versions of Nautilus.  Additionally, the names in this header are not always
consistent and correct, as the names were in some cases chosen without a detailed understanding of
the names and definitions for some formats.  (TODO: I need to put together a documentation page
about the different formats discussed here: isotope/nuclide zaid, SZA, chemsym, reaction zaid,
other?)

The ``chemsym_to_zaid`` function accepts a string in a format known as the "chemsym" format,
because it is related to (but not the same as) the IUPAC chemical symbol.  It currently only
handles a subset of the full chemsym format.  Given a chemsym, it will return an integer with the
corresponding SZA value.  The names "zaid" and "SZA" were incorrectly used interchangeably when
this function was initially written.

The ``half_reaction_zaid_to_chemsym_list`` function accepts a string consisting of either the
products or reactants of a reaction, written in the format expected for NDI reaction zaids.  It
will parse the string and return a ``std::vector`` of strings in the chemsym format.  In practice,
this is not fully consistent with the chemsym format, because there was some early confusion about
the distinction between two related formats.

The ``append_zaids`` function combines the ``half_reaction_zaid_to_chemsym_list`` and
``chemsym_to_zaid`` functions, taking a string listing the products or reactants of a reaction and
returning a ``std::vector`` of SZA values, with repeated entries deduplicated.

The ``get_reactant_zaids`` function accepts an NDI reaction zaid as a string, identifies the
substring with the reactants, then calls ``append_zaids`` and returns the resulting list of integer
SZA values.  The ``get_product_zaids`` function is the same, but it will extract and parse the list
of reaction products instead of the reactants.  The ``get_isotope_zaids`` function is similar, but
it returns the list of all reactants and reaction products.

