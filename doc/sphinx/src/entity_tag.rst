Entity Tag
===================================================================================================

The ``EntityTag`` class provides an identifier for atomic nuclei and particles.  Nautilus provides
a number of translators to bidirectionally convert between ``EntityTag`` and other formats commonly
in use.  The primary advantage that ``EntityTag`` provides beyond what can be found in other
formats is a versioning code attached to the identifier, allowing the ``EntityTag`` format to
evolve to correct mistakes and to expand or change the features available.  Most identifiers that
have been in use for an extended period of time suffer from a number of "quirks" that have
accumulated over time for various reasons and cannot be completely eliminated due to the need to
preserve consistency and compatibility.

.. image:: xkcd927standards.png

The objective for Nautilus's ``EntityTag`` is not to provide a new "super-format" that supports
every possible use-case and replaces all similar formats in all codes.  The primary value of
``EntityTag`` will be in its ability to translate between multiple formats in a consistent way, and
to parse existing formats in a ways that automatically corrects for all of their historical quirks.
If customers are interested in using ``EntityTag`` as their new internal representation, the
Nautilus team will be pleased to continue supporting them.

An ``EntityTag`` can encode three different kinds of entities:

- an unknown entity
- a standard entity
- a user-defined entity

All instances of ``EntityTag`` have the following accessors:

- ``current_version()`` is a static function that returns the most-recent version of ``EntityTag``
  available (currently 0)
- ``get_version()`` returns the version of this instance of ``EntityTag``
- ``is_unknown()`` returns true for unknown entities and false otherwise
- ``is_standard()`` returns true for standard entities and false otherwise
- ``is_user()`` returns true for user-defined entities and false otherwise

All instances of ``EntityTag`` also have a family of ``set()`` methods with the same arguments and
behavior as the various constructors discussed below.

Comparison operators are also available: ``==``, ``!=``, ``<=``, ``>=``, ``<``, and ``>``.  The
ordering implied by the inequality operators is

- particles (ordering between particles unspecified, but will be consistent for a given
  version of ``EntityTag``)
- nuclides

  - elementals before non-elementals
  - otherwise sorted by atomic number (most-significant), then by atomic mass number, then by
    metastable state index (least-significant)

- user-defined entities (ordering depends on user data)
- unknown entities last

Unknown Entities
---------------------------------------------------------------------------------------------------

An "unknown" entity indicates that the ``EntityTag`` was not initialized or that the translation
routine was unable to correctly map from another format into the ``EntityTag`` format.

To build an unknown entity, either call the default constructor (``EntityTag()``) or the
explicit "unknown" constructor (``EntityTag(EntityTag::unknown)``).

Standard Entities
---------------------------------------------------------------------------------------------------

Standard entities include nuclides and particles.  To differentiate, the following accessors are
available:

- ``is_particle()`` will return true for particles, false for nuclides or non-standard instances
- ``is_nuclide()`` will return true for nuclides, false for particles or non-standard instances

Nuclides
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to query the properties of nuclei, the following accessors are available.  All of them
assert that the tag describes a nuclide before returning the result, providing a sanity check when
building Nautilus in debug mode.

- ``is_elemental()`` indicates if the entity is an elemental (true) or a specific nuclide (false)
- ``get_atomic_number()`` and ``get_Z()`` return the atomic number
- ``get_atomic_mass_number()`` and ``get_A()`` return the atomic mass number
- ``get_neutron_number()`` and ``get_N()`` return the neutron number
- ``get_metastable_state()`` returns the index of the excited state, with zero being the ground
  state
- ``is_ground()`` indicates if the entity is in the ground state (true) or an excited state (false)

A few notes regarding the metastable / excited state:

- The metastable state index does not have a recognized shorthand like :math:`Z`, :math:`N`, or
  :math:`A`.  For example, NDI formats use the symbol :math:`S` while MCNP formats use the symbol
  :math:`m`.  For this reason, no shorthand accessor is provided in Nautilus.
- The metastable state doesn't make sense for elementals, so Nautilus will default elementals to
  the ground state.
- Because the metastable state is not well-defined, Nautilus cannot convert between different
  definitions of "metastable" or between metastable and excited state indices.  If the user sets a
  metastable state index, Nautilus will simply report back that same index.  Therefore, if the user
  wants to use an excited state index it is up to the user to recall that they have defined the
  list of metastable states to be "all excited states".

To construct a nuclide tag there are two possible constructors:

- ``EntityTag(atomic_number, EntityTag::elemental)`` will construct an elemental with the specified
  atomic number.
- ``EntityTag(atomic_number, atomic_mass_number, metastable_index)`` will construct a nuclide with
  the specified values.  The metastable index can be left off, in which case it will default to the
  ground state (metastable index of zero).

Particles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The only additional accessor available for particles is ``get_particle_index()``, which will assert
that the entity is a particle and then return an index for the particle.  The particle index is not
intended to encode a specific meaning.  Instead, Nautilus defines a number of compile-time
constants in ``nautilus/entity_tag/names.hpp`` for the various particles known to Nautilus.  For
example, to see if a particle is a muon would be ``entity.get_particle_index() == names::muon``.
Be aware that the specific values for these compile-time constants is not meaningful and could
change.

To construct a particle tag use the constructor ``EntityTag(particle_index)``, where the particle
index should be one of the compile-time constants from ``nautilus/entity_tag/names.hpp``.

User-Defined Entities
---------------------------------------------------------------------------------------------------

A user-defined entity is simply space for users to add their own custom entities, defined according
to their own needs.  Examples of this could be

- Some astrophysics codes define simplified thermonuclear reaction networks with "pseudo-materials"
  that are some useful distribution of elements and isotopes that provide a "good enough"
  approximation intermediate to a calculation.
- Some transport codes may need particles that Nautilus has not yet defined, including known
  particles that didn't make the list or theoretical particles that have not yet been observed
  experimentally.
- The user may wish to duplicate one of the standard formats but also pack some "extra" information
  into a Nautilus ``EntityTag`` for bookkeeping purposes (such as "this is a proton, and we use the
  data from such-and-such source").

The only accessor for user-defined entities is ``get_user_data()``.

To construct a user-defined entity use the constructor ``EntityTag(EntityTag::user, user_data)``,
which builds a user-defined entity with the specified user data.  ``EntityTag`` encodes user data
into 24 bits, so data outside the valid range will generate an error or silently do the wrong
thing.  The value where all 24 bits are set to one is a special value that is reserved for
Nautilus, so users should not attempt to set that value or the tag will default to an "unknown"
entity.
