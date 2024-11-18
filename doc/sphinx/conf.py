# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html


# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.

import os
import sys
from datetime import date
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'Nautilus'
copyright = '2021-' + str(date.today().year) + ', Triad National Security, LLC. All Rights Reserved.'
author = 'The Nautilus team'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosectionlabel',
    'sphinx_rtd_theme',
    'sphinx_multiversion',
    'sphinx_rtd_dark_mode'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ["_templates"]

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ["_static"]

html_logo = "_static/Logo_Color_512x512_white.png"
html_theme_options = {
    'logo_only': False,
    'display_version': True,
}


# -- Options for sphinx_multiversion -----------------------------------------

# Use "sphinx-multiversion --dump-metadata" to:
# - check which tags/branches are matched
# - see a list of available placeholder names and their values for each version

# Branches or tags that don’t contain both the sphinx source
# directory and the "conf.py" file will be skipped automatically.

# To list values to match, you can use "git branch", "git tag" and "git remote"

# Whitelist pattern for tags (set to None to ignore all tags)
smv_tag_whitelist = r'^.*$'

# Whitelist pattern for branches (set to None to ignore all branches)
smv_branch_whitelist = r'^(main|master)$'
# smv_branch_whitelist = r'^.*$'

# Whitelist pattern for remotes (set to None to use local branches only)
smv_remote_whitelist = r'^(origin)$'
# smv_branch_whitelist = None

# To list all refnames , you can use:
#   "git for-each-ref --format "%(refname)" | sed 's/^refs\///g' "

# Pattern for released versions
smv_released_pattern = r'^tags/.*$'

# Format for versioned output directories inside the build directory
smv_outputdir_format = '{ref.name}'

# Determines whether remote or local git branches/tags are preferred if their output dirs conflict
smv_prefer_remote_refs = False


# -- Options for sphinx_rtd_dark_mode -----------------------------------------

# user starts in dark mode
default_dark_mode = True

