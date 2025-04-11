.. _`Atomic Nuclei`:

Atomic Nuclei
===================================================================================================

This is a reference page regarding basic concepts of atomic nuclei and terminology used in
Nautilus.

Atomic nuclei, also called *nuclides*, are composed of protons and neutrons (collectively called
nucleons).  Those nucleons can be in their lowest-energy state or can be excited into higher-energy
states.  Terminology and symbols used throughout Nautilus (based on common usage) are:

- The number of protons is known as the *atomic number* and is represented by the symbol :math:`Z`.
- The number of nucleons is known as the *atomic mass number* and is represented by the symbol
  :math:`A`.
- The number of neutrons is represented by the symbol :math:`N`.  This is less commonly used than
  the atomic mass number, :math:`A`, but is sometimes called the *neutron number*.
- The excited state terminology is more complicated and is discussed below.

The term *element* refers to a nuclide with a particular proton count.  Two nuclides that are the
same element may have different neutron counts and/or excited states.

The term *isotope* refers to a nuclide with a particular proton count and neutron count.  Two
nuclides that are the same isotope may have different excited states.

The term *isomer* refers to a nuclide with a particular proton count, neutron count, and excited
state.  Since these are the three quantities used to indicate a specific nuclide, the term "isomer"
is not as common as "isotope" or "element", so we sometimes simply use the term nuclide.

In many codes, there is also the concept of an *elemental* (for example, "elemental carbon").  This
is a specific mixture of different nuclides of the same element.  The exact mixtures used for
elementals are typically the natural abundances; specifics can be found in various data libraries,
and are beyond the scope of Nautilus.  In terms of representation in software, elemental mixtures
are often stored as a "nuclide" with some special value to indicate an elemental mixture instead of
a specific nuclide.  Typically the special value used to indicate an elemental mixture is setting
:math:`A = 0`, but that can depend on how nuclide data is encoded in memory.

Excited States and Metastable States
---------------------------------------------------------------------------------------------------

The lowest-energy state of a nuclide is called the *ground state*.  All other states have higher
energy and are called *excited states*.  For most isotopes, the ground state is the longest-lived
isomer, but there are exceptions.

There is also the concept of a *metastable state*, which is a long-lived excited state.  What
"long-lived" means varies, but a common definition is having a half-life of 1 ns or longer.
Because the definition varies, which excited states are considered metastable states also varies
depending on the data set.  The states identified as metastable states are then given a metastable
index :math:`S`, with higher-energy metastable states have higher indices.  When working with
metastable state indices, Nautilus will simply respect the index convention provided without
attempting to map to another convention.  Many codes at LANL have an accepted metastable state
convention, and tend to prefer metastable states over excited states, so this is typically
sufficient to identify different isomers of the same isotope.  Future work on Nautilus will provide
better support for excited states other than metastable states.

