.. _`Versioning`:

Versioning
===================================================================================================

Following LANL policy, the leading part of the version number relates to the approved scope of the
Nautilus library, and a change in this number implies a change in the scope of Nautilus.
Therefore, Nautilus uses a four-part versioning scheme, with a leading number for compliance with
LANL policy, and the remaining three parts approximately follow the rules for `semantic
versioning`_.

.. _`semantic versioning`: https://semver.org

For a Nautilus version A.B.C.D, the meaning of each part is:

- A: the scope number, which will rarely or never increase
- B: the compatibility number, which will increase when backwards compatibility is broken (compare
  SemVer's "major" version number)
- C: the feature number, which will increase when new features are added without breaking
  compatibility (compare SemVer's "minor" version number)
- D: the bug fix number, which will increase when fixing bugs or making other trivial changes
  (compare SemVer's "patch" version number)
