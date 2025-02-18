.. _sphinx-doc:

How to Use Sphinx for Writing Docs
==================================

* `Sphinx Installation`_

.. _Sphinx Installation: https://www.sphinx-doc.org/en/master/usage/installation.html

* `Sphinx reStructuredText Documentation`_

.. _Sphinx reStructuredText Documentation: https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html

* `Sphinx Multiversion Documentation`_

.. _Sphinx Multiversion Documentation: https://holzhaus.github.io/sphinx-multiversion/master/index.html

How to Get the Dependencies
---------------------------

Using Docker
^^^^^^^^^^^^

If you are using `Docker`_, then simply pull the docker image specified below:

.. _Docker: https://www.docker.com

.. code-block::

  image: sphinxdoc/sphinx-latexpdf

Then, after running :code:`docker run -it <docker-image-name> /bin/bash`, install the theme we are using :code:`pip install sphinx_rtd_theme`

Using Spack
^^^^^^^^^^^

If you are using `Spack`_ to provision dependencies, then follow the steps as such:

.. _Spack: https://spack.io

.. code-block::

   - mkdir spack_env
   - spack env create -d spack_env
   - spack env activate -d spack_env
   - spack repo add --scope env:spack_env spack-repo
   - spack repo list
   - spack mirror list
   - spack add nautilus@main+doc%gcc@12.2.0
   - spack install -j $(nproc) --show-log-on-error --no-checksum --yes-to-all --include-build-deps --fresh -u cmake

from :code:`.gitlab-ci.yml`

.. warning::
   If you do not have either Docker or Spack locally, you would need to install one of them first.

   For Docker, refer to their `Get Docker Guide`_.

   For Spack, refer to their `Getting Started Guide`_.

.. _Get Docker Guide: https://docs.docker.com/get-docker

.. _Getting Started Guide: https://spack.readthedocs.io/en/latest/getting_started.html#installation

How to Build .rst into .html
----------------------------

After you have the dependencies in your environment, then simply build your documentation as the following:

.. code-block::

   source .gitlab/build_docs.sh <cluster-name>

.. note:: 
   You can view the documentation webpage locally on your web browser by passing in the URL as :code:`file:///path/to/nautilus/doc/sphinx/_build/html/index.html`

How to Deploy
-------------

#. Submit a MR with your .rst changes for documentation on `Gitlab Nautilus`_
#. Get your MR reviewed and merged into `main`
#. Make sure the :code:`pages` CI job passes in the Gitlab CI pipeline

.. _Gitlab Nautilus: https://re-git.lanl.gov/xcap/oss/nautilus

As soon as the MR is merged into `main`, this will trigger the Gitlab Pages deployment automatically if the :code:`pages` CI job passes on the repo.

As long as you have access to the `re-git Nautilus`_, then you can go and behold the beautiful, updated `Nautilus Documentation`_!

.. _re-git Nautilus: https://re-git.lanl.gov/xcap/oss/nautilus

.. _Nautilus Documentation: http://xcap.re-pages.lanl.gov/oss/nautilus

More Info.
----------

* `Sphinx CheatSheet`_

.. _Sphinx CheatSheet: https://thomas-cokelaer.info/tutorials/sphinx/rest_syntax.html

* `SphinxRTDTheme Configurations`_

.. _SphinxRTDTheme Configurations: https://sphinx-rtd-theme.readthedocs.io/en/stable/configuring.html
