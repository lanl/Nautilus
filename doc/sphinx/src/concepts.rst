Basic Concepts
===================================================================================================

Nautilus provides support for data identifiers that represent a variety of related concepts that
are useful in nuclear and atomic computational tools.  This page discusses some of these concepts
and the terminology that Nautilus uses, to help provide clarity in understanding the capabilities
of Nautilus.

Atomic Nuclei
---------------------------------------------------------------------------------------------------

An atomic nucleus consists of protons and neutrons, which are generically referred to as nucleons.

- The number of protons (:math:`Z`) is called the *atomic number*.
- The number of neutrons (:math:`N`) is called the *neutron number*.
- The number of nucleons (:math:`A = Z + N`) is called the *atomic mass number*.

Nuclei with the same atomic number are the same *element*.  Nuclei with the same atomic number but
different neutron numbers are different *isotopes* of the same element.

However, two nuclei that are the same isotope can still differ in their excitation state.  The
*ground state* is the lowest-energy state of a given nuclide, while all other states are called
*excited states*.  The ground state is often, but not always, the longest-lived state of a given
isotope.

There are special excited states called *metastable states*.  Unfortunately, there is not a
standard definition of metastable states, but the various definitions typically focus on the
"long-lived" states.  For example, `Wikipedia`_ presents two different definitions of metastable
states:

- Metastable states have a half-life of 1ns or longer, "100 to 1000 times longer than the
  half-lives of the excited nuclear states that decay with a 'prompt' half-live (ordinarily on the
  order of 1e-12 seconds).
- Metastable states have a half-life of 5ns or longer, "to distinguish the metastable half-life
  from the normal 'prompt' gamma-emission half-life."

.. _Wikipedia: https://en.wikipedia.org/wiki/Nuclear_isomer

Nuclei of the same isotope but different excited states are referred to as *isomers* (although
sometimes this term refers only to the metastable states and not to "short-lived" excited states).

The term *nuclide* refers to a specific nucleus: the same atomic number, the same atomic mass
number, and the same excited state.  If any of these differs, then you are considering different
nuclides.

To summarize some terminology, when comparing two nuclei:

- If the atomic number differs, they are different elements.
- If the atomic number is the same but the neutron number differs, then they are different isotopes
  of the same element.
- If the atomic number and neutron number are the same but the excitation state differs, then they
  are different isomers of the same isotope of the same element.
- If the atomic number, neutron number, and excitation state are all the same, then they are the
  same nuclide.

Nautilus is focused primarily on nuclear applications, and thus does not provide any indication of
the ionization state of an atomic nucleus.

Elementals vs. Nuclides
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For a given element, we can speak of specific nuclides or we can speak of distributions with a
mixture of different isotopes.  Following a convention used in various data libraries, Nautilus
also can indicate *elementals*, which are distributions of different isotopes of a single element.
The distribution is defined by the data library, but is typically the natural abundance
distribution of that element.

Element Names and Symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The official names and symbols for elements are defined by the `International Union of Pure and
Applied Chemistry`_ (IUPAC).  However, because many elements have been known since ancient times,
there are regional and linguistic variations in the names of some elements.  Nautilus focuses only
on the English names, but that still leaves four distinct variants:

- IUPAC standard
- American English
- British English
- Canadian English

Some references use "Commonwealth English" instead of "British English", to indicate the wide
geographic usage of that particular variety of English.  Nautilus has opted for "British English"
because the spelling variations discussed here differ between England and Canada, despite Canada
being part of the Commonwealth of Nations.  For nations in the Commonwealth but outside of North
America, either "British English" or the IUPAC standard is likely to be the correct regional
spelling.

.. _`International Union of Pure and Applied Chemistry`: https://iupac.org

Fortunately, the distinctions between these regional variations is only in the spelling of a few
elements.  By default, Nautilus will use the IUPAC standard spelling.  Some features of Nautilus
provide the option to specifically request the spelling according to one of the regional standards.

IUPAC also defines the "systematic name" for elements that do not yet have an official IUPAC name.
As of the time of writing, it appears that all elements that have been observed in nature or
confirmed in experiment have official names.  Therefore Nautilus does not currently support the
systematic name.

The atomic symbol for each element is, thankfully, fully standardized according to the rules set
out by IUPAC with no regional variations.

Particles
---------------------------------------------------------------------------------------------------

In addition to atomic nuclei, Nautilus can also indicate particles.  Before presenting the list of
available particles, we first present some basic particle physics terminology.

Classes of Particles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will be a very brief, incomplete, and approximate sketch of the standard model of particle
physics, with an eye towards defining some terms useful in further discussion of particles.

The standard model identifies four classes of *elementary particles*:

- *quarks*: up, down, charm, strange, top, bottom
- *leptons*: electron, muon, tau; each with its own corresponding neutrino
- *gauge bosons*: gluons, photon, Z boson, W bosons
- *scalar bosons*: Higgs boson

Each of these particles also has an *antiparticle* with the same mass and opposite charge, with the
exception of the neutral bosons, which are their own antiparticle.

The quarks and leptons are organized into *generations* (first, second, and third) and *subtypes*
(down-type or up-type for quarks, charged or neutral for leptons).  Nautilus does not include any
quarks, but they are useful for classifying composite particles.

Composite particles are called *hadrons* and are described as a combination of two or more
"valence" quarks.  If the particle has two valence quarks, it is called a *meson*.  If the particle
has three valence quarks, it is called a *baryon*.  Further combinations (*tetraquarks*,
*pentaquarks*, and so on) are sometimes referred to as "exotic" hadrons, and are currently not
included in Nautilus.

Particle Names and Symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The standard name for particles is defined by the `Particle Data Group`_ (PDG).  Unlike with
elements, there are no regional variations of the particle names.  However, there is an alternate
standard used by some authors (see, for example, Claude Amsler's 2015 book "Nuclear and Particle
Physics"), which can cause some confusion due to the sometimes subtle differences between the two
standards.

.. _`Particle Data Group`: https://pdg.lbl.gov

The difference between the two standards is the naming of antibaryons.  In the PDG standard, the
name of a particle includes its own charge, regardless of whether that particle is ordinary matter
or antimatter.  However, in the alternate standard, antibaryons are named for the charge of their
corresponding ordinary matter particle.  Consider, for example, the positive sigma baryon (valence
quarks: :math:`uus`), which is paired with an antiparticle of the same mass but opposite charge
(valence quarks: :math:`\overline{u}\overline{u}\overline{s}`).  In the PDG standard, that
antiparticle is called the negative sigma antibaryon because it has a negative charge.  However, in
the alternate standard, that antiparticle is called the antiparticle of the positive sigma baryon
or the positive sigma antibaryon.  The confusion gets worse if you consider that there is a
negative sigma baryon (valence quarks: :math:`dds`) and its corresponding antiparticle, which is
called either the positive sigma antibaryon (PDG standard) or the antiparticle of the negative
sigma baryon or the negative sigma antibaryon (alternate standard).

The symbols, if drawn carefully, can help to disambiguate.  Using our example above with the sigma
(anti)baryons, the PDG standard gives :math:`\Sigma^+` and its antiparticle
:math:`\overline{\Sigma}^-`, along with :math:`\Sigma^-` and its antiparticle
:math:`\overline{\Sigma}^+`.  The alternate standard gives :math:`\Sigma^+` and its antiparticle
:math:`\overline{\Sigma^+}` along with :math:`\Sigma^-` and its antiparticle
:math:`\overline{\Sigma^-}`.  However, the distinction is subtle, being simply whether or not the
overline continues over the sign.

Nautilus defaults to the PDG standard, but some parts of Nautilus provide the option to work with
the alternate standard.

Particles Available in Nautilus
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The particles that Nautilus currently supports are listed below, along with some annotations
regarding the classification and standard symbol for each particle.

+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| particle                         | class       | generation / subtype or valence quarks               | PDG symbol                   |
+==================================+=============+======================================================+==============================+
| photon                           | gauge boson                                                        | :math:`\gamma`               |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| electron (a.k.a., negative beta) | lepton      | first generation / charged                           | :math:`e^-`                  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positron (a.k.a., positive beta) | lepton      | first generation / charged                           | :math:`e^+`                  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| electron neutrino                | lepton      | first generation / neutral                           | :math:`\nu_e`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| electron antineutrino            | lepton      | first generation / neutral                           | :math:`\overline{\nu}_e`     |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| muon                             | lepton      | second generation / charged                          | :math:`\mu^-`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| antimuon                         | lepton      | second generation / charged                          | :math:`\mu^+`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| muon neutrino                    | lepton      | second generation / neutral                          | :math:`\nu_\mu`              |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| muon antineutrino                | lepton      | second generation / neutral                          | :math:`\overline{\nu}_\mu`   |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| neutral pion                     | meson       | :math:`\frac{u\overline{u}-d\overline{d}}{\sqrt{2}}` | :math:`\pi^0`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positive pion                    | meson       | :math:`u\overline{d}`                                | :math:`\pi^+`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| negative pion                    | meson       | :math:`d\overline{u}`                                | :math:`\pi^-`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| short kaon                       | meson       | :math:`\frac{d\overline{s}-s\overline{d}}{\sqrt{2}}` | :math:`K^0_S`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| long kaon                        | meson       | :math:`\frac{d\overline{s}+s\overline{d}}{\sqrt{2}}` | :math:`K^0_L`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positive kaon                    | meson       | :math:`u\overline{s}`                                | :math:`K^+`                  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| negative kaon                    | meson       | :math:`s\overline{u}`                                | :math:`K^-`                  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| neutron                          | baryon      | :math:`udd`                                          | :math:`n`                    |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| antineutron                      | baryon      | :math:`\overline{u}\overline{d}\overline{d}`         | :math:`\overline{n}`         |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| proton                           | baryon      | :math:`uud`                                          | :math:`p`                    |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| antiproton                       | baryon      | :math:`\overline{u}\overline{u}\overline{d}`         | :math:`\overline{p}`         |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| neutral lambda baryon            | baryon      | :math:`uds`                                          | :math:`\Lambda^0`            |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| neutral lambda antibaryon        | baryon      | :math:`\overline{u}\overline{d}\overline{s}`         | :math:`\overline{\Lambda}^0` |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positive sigma baryon            | baryon      | :math:`uus`                                          | :math:`\Sigma^+`             |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| negative sigma antibaryon        | baryon      | :math:`\overline{u}\overline{u}\overline{s}`         | :math:`\overline{\Sigma}^-`  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| negative sigma baryon            | baryon      | :math:`dds`                                          | :math:`\Sigma^-`             |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positive sigma antibaryon        | baryon      | :math:`\overline{d}\overline{d}\overline{s}`         | :math:`\overline{\Sigma}^+`  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| neutral xi baryon                | baryon      | :math:`uss`                                          | :math:`\Xi^0`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| neutral xi antibaryon            | baryon      | :math:`\overline{u}\overline{s}\overline{s}`         | :math:`\overline{\Xi}^0`     |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| negative xi baryon               | baryon      | :math:`dss`                                          | :math:`\Xi^-`                |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positive xi antibaryon           | baryon      | :math:`\overline{d}\overline{s}\overline{s}`         | :math:`\overline{\Xi}^+`     |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| negative omega baryon            | baryon      | :math:`sss`                                          | :math:`\Omega^-`             |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+
| positive omega antibaryon        | baryon      | :math:`\overline{s}\overline{s}\overline{s}`         | :math:`\overline{\Omega}^+`  |
+----------------------------------+-------------+------------------------------------------------------+------------------------------+

An ionized hydrogen-1 nucleus is just a bare proton, so there is some ambiguity between a proton
(particle) and hydrogen-1 (isotope) in Nautilus.  Nautilus addresses this in different ways
depending on the context, so this will be discussed in more detail in each context.

In some contexts, a few light atomic nuclei are treated as particles and given special names for
this usage.  While Nautilus does not encode them as particles (other than the hydrogen-1 vs proton
question as noted above), it is worth being aware of the "particle" names that are sometimes used
for these light nuclei:

- hydrogen-1 is also known as protium, and as a particle is called a proton
- hydrogen-2 is also known as deuterium, and as a particle is called a deuteron
- hydrogen-3 is also known as tritium, and as a particle is called a triton
- helium-3 as a particle is called a helion
- helium-4 as a particle is called an alpha
