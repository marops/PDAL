.. _json_pipeline_specification:

======================================
Draft PDAL JSON Pipeline Specification
======================================

:Authors:
    Bradley J. Chambers,
    Connor Manning,
    Andrew Bell,
    Howard Butler

:Revision: 0.1
:Date: 21 December 2015

.. sectnum::
.. contents::
   :depth: 4
   :backlinks: none

============
Introduction
============

Examples
--------

A simple PDAL pipeline, inferring the appropriate drivers for the reader and
writer from filenames, and able to be specified as a set of sequential steps:

.. code-block:: json

    {
      "pipeline": [{
        "filename": "input.las"
      }, {
        "type": "crop",
        "bounds": "([0,100],[0,100])"
      }, {
        "filename": "output.bpf"
      }]
    }

A more complex PDAL pipeline, that reprojects the stage tagged ``A1``, merges
the result with ``B``, and writes the merged output with the ``points2grid``
plugin.:

.. code-block:: json

    {
        "pipeline": [{
            "filename": "A.las",
            "spatialreference": "EPSG:26916",
        }, {
            "type": "filters.reprojection",
            "in_srs": "EPSG:26916",
            "out_srs": "EPSG:4326",
            "tag": "A2",
        }, {
            "filename": "B.las",
            "tag": "B"
        }, {
            "type": "filters.merge",
            "tag": "merged",
            "inputs": ["A2", "B"]
        }, {
            "type": "writers.p2g",
            "filename": "output.tif",
        }]
    }

Definitions
-----------

* JavaScript Object Notation (JSON), and the terms object, name, value, array,
  and number, are defined in IETF RTC 4627, at
  http://www.ietf.org/rfc/rfc4627.txt.

* The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD",
  "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this documention are to
  be interpreted as described in IETF RFC 2119, at
  http://www.ietf.org/rfc/rfc2119.txt.

=====================
PDAL Pipeline Objects
=====================

PDAL JSON pipelines always consist of a single object. This object (referred to as the PDAL JSON object below) represents a processing pipeline.

* The PDAL JSON object may have any number of members (name/value pairs).

* The PDAL JSON object must have a ``pipeline`` object.

Pipeline Object
---------------

* The value corresponding to ``pipeline`` is an array. Each element in the array
  is a ``stage`` object (for more on PDAL stages, see
  `stage <http://www.pdal.io/stages/index.html>`_).

Stage Objects
-------------

* A stage object may have a member with the name ``tag`` whose value is a
  string. The purpose of the tag is to cross-reference this stage within other
  stages. Each ``tag`` must be unique.

* A stage object may have a member with the name ``inputs`` whose value is an
  array of strings. Each element in the array is the tag of another stage. If
  ``inputs`` is not specified, the previous stage in the array will be used as
  input. Reader stages will disregard the ``inputs`` member.

* A stage object may have a member with the name ``type`` whose value is a
  string. The ``type`` must specify a valid PDAL stage name, e.g.,
  ``readers.las``. For reader and writer stages, it is often possible to infer
  the ``type`` from the filename extension. For filters, ``type`` is required.

* A stage object may have additional members with names corresponding to
  stage-specific options and their respective values. For example,
  ``"bounds": "([0,100],[0,100])"`` sets the bounds to be used in the
  `crop filter <http://www.pdal.io/stages/filters.crop.html#options>`_.
