Shared Components
===================================================================================================

Nautilus provides a set of shared components, essentially pieces that were originally written for
`Singe`_ but were identified as being potentially useful to `SuNRe`_.  These components provide a
place to share code instead of reimplementing features, but they may be superseded as newer tools
are developed within Nautilus.

.. _Singe: https://xcap.re-pages.lanl.gov/ec/singe
.. _SuNRe: https://xcap.re-pages.lanl.gov/ec/sunre

Nautilus makes use of common terms and symbols for atomic nuclei, which are summarized `here`_.

.. _`here`: concepts.html

Be aware that the components described on this page were developed before certain terminology was
clarified, and so it may not match the terminology as used in the discussion of the ``EntityTag``
class, which was designed to address the confusion of terminology and standards more carefully.

SZA
---------------------------------------------------------------------------------------------------

An SZA is a way to denote a particular nuclide.  Due to its use in various data libraries, such as
the Nuclear Data Interface (NDI), the SZA is common in many codes.

Depending on your specific use-cases, you may want to consider the newer ``EntityTag`` instead of
an ``SZA``, as it addresses some corner cases and better supports conversions between different
formats.  However, the ``SZA`` class is still supported as the more minimal design may be more
appropriate for some applications.

An SZA is an integer value, defined as

.. math:: \text{SZA} = S * 1{,}000{,}000 + Z * 1{,}000 + A

Nautilus provides the ``SZA`` class, which stores the SZA value and provides the following
accessors:

- ``A()`` -- the atomic mass number
- ``N()`` -- the neutron number
- ``S()`` -- the metastable index
- ``Z()`` -- the atomic number

It also provides the following additional operations:

- explicit cast to an integer value
- all comparison operators: ``<``, ``<=``, ``==``, ``!=``, ``>=``, ``>``; comparison is ordered
  first by :math:`Z`, then by :math:`A` for nuclides with the same :math:`Z` value, then by
  :math:`S` for nuclides with the same :math:`Z` and :math:`A` values

Nuclide
---------------------------------------------------------------------------------------------------

The ``Nuclide`` class is based on the ``Isotope`` class from Singe.  For compatibility there is an
alias called ``Isotope`` in Nautilus, but it is deprecated.

The ``Nuclide`` class consists of an SZA and a mass, providing a slightly more detailed description
of the nuclide in question.  It provides the following accessors:

- ``A()`` -- the atomic mass number
- ``N()`` -- the neutron number
- ``S()`` -- the metastable index
- ``Z()`` -- the atomic number
- ``sza()`` -- the ``SZA`` instance identifying the nuclide
- ``mass()`` -- the mass of the nuclide

The ``Nuclide`` class also provides all comparison operators, ordered the same as the ``SZA``
class.

Nautilus also provides the streaming operator for a ``Nuclide``, providing a simple way to print
values.

IndexedNuclide
---------------------------------------------------------------------------------------------------

The ``IndexedNuclide`` class is based on the ``ReactionIsotope`` class from Singe.  For
compatibility there is an alias called ``ReactionIsotope``, but it is deprecated.

The ``IndexedNuclide`` class extends the ``Nuclide`` class, additionally adding an index so that
information related to this nuclide can be accessed from arrays, so long as those arrays are
ordered consistently.  In addition to the accessors and operators provided by ``Nuclide``, the
``IndexedNuclide`` class adds the ``index()`` accessor, which returns the index.

