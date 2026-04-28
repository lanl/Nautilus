Format: MCNP
===================================================================================================

Nautilus provides bidirectional conversion between ``EntityTag`` and several formats [1] defined by
MCNP (the Monte Carlo N-Particle code): partial zaid, full zaid, and particle symbol.

MCNP Partial Zaid
---------------------------------------------------------------------------------------------------

The MCNP partial zaid is an integer format (datatype ``int`` in Nautilus) that primarily represents
nuclides.  The conversion routines available for this format are

- ``from_MCNP_partial_zaid`` that takes a partial zaid and returns an ``EntityTag``
- ``to_MCNP_partial_zaid`` that takes an ``EntityTag`` and returns a partial zaid

Nuclides, by default, are encoded as :math:`Z \times 1{,}000 + A`.  If the nuclide is not in the
ground state, add to that :math:`300 + m \times 100`, where :math:`m` is the metastable state
index.  Because of this choice for how to encode the metastable state index, the partial zaid
format does not support :math:`m > 4`.

Similarly to other long-lived LANL formats, the partial zaid has issues with americium-242.  The
ground state (Am-242g) and the first metastable state (Am-242m1) are swapped, so:

- 95242 should be Am-242g but is actually Am-242m1
- 95642 should be Am-242m1 but is actually Am-242g

Elementals are encoded as :math:`Z \times 1{,}000` (implicitly setting :math:`A` and :math:`m` to
zero).

Particles and user-defined entities are not supported by this format, so they will generate the
invalid value ``invalid_mcnp_partial_zaid`` (equal to -1).

MCNP Full Zaid
---------------------------------------------------------------------------------------------------

The MCNP full zaid is a string format (datatype ``std::string`` in Nautilus), and is an extension of
the partial zaid format.  It consists of a partial zaid, a period, and a suffix.  The conversion
routines available for this format are

- ``from_MCNP_full_zaid`` that takes a full zaid and returns an ``EntityTag``
- ``to_MCNP_full_zaid`` that takes an ``EntityTag`` and a suffix, and returns a full zaid

  - The suffix is a string that is appended to the end of the full zaid with no additional
    processing.

Because the full zaid is an extension of the partial zaid, it follows the same rules as the partial
zaid.

Invalid full zaids are given the valud ``invalid_mcnp_full_zaid``, which is equal to "unknown".

MCNP Particle Symbol
---------------------------------------------------------------------------------------------------

The MCNP particle symbol is a character format (datatype ``char`` in Nautilus) that primarily
represents particles.  The conversion routines available for this format are

- ``from_MCNP_particle_symbol`` that takes a particle symbol and returns an ``EntityTag``
- ``to_MCNP_particle_symbol`` that takes an ``EntityTag`` and returns a particle symbol

Only a few nuclides are supported by this format, shown in the table below:

==========  ===============
nuclide     particle symbol
==========  ===============
H-1         H
H-2         D
H-3         T
He-3        S
He-4        A
all others  #
==========  ===============

Because "#" is ambiguous, it will generate an unknown ``EntityTag``.

This format does not support elementals or excited states.

The particles supported by this format are shown in the table below:

=========================   ===============
particle                    particle symbol
=========================   ===============
neutron                     N
photon                      P
electron                    E
muon                        \|
antineutron                 Q
electron neutrino           U
muon neutrino               V
positron                    F
proton                      H
neutral lambda baryon       L
positive sigma baryon       \+
negative sigma baryon       \-
neutral xi baryon           X
negative xi baryon          Y
negative omega baryon       O
antimuon                    !
electron antineutrino       <
muon antineutrino           >
antiproton                  G
positive pion               /
neutral pion                Z
positive kaon               K
short kaon                  %
long kaon                   ^
neutral lambda antibaryon   B
negative sigma antibaryon   _
positive sigma antibaryon   ~
neutral xi antibaryon       C
positive xi antibaryon      W
positive omega antibaryon   @
negative pion               \*
negative kaon               ?
=========================   ===============

This format is slightly lossy, because H-1 and a proton are both represented by "P".  Because this
format is mostly particle-focused, Nautilus will translate "P" to a proton, meaning that
``EntityTag(H-1)`` becomes particle symbol "P", which converts back to an ``EntityTag`` as a proton
instead of as H-1.

This format does not support user-defined entities.

The "invalid" value for particle symbols is ``invalid_mcnp_particle_symbol``, which has the value
" " (a space).

Notes and References
---------------------------------------------------------------------------------------------------

[1]: The MCNP manual (version 6.2,
https://mcnp.lanl.gov/pdf_files/TechReport_2017_LANL_LA-UR-17-29981_WernerArmstrongEtAl.pdf) is the
primary reference for the MCNP notations discussed here.
