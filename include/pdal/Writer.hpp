/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#pragma once

#include <pdal/Options.hpp>
#include <pdal/PointView.hpp>
#include <pdal/Stage.hpp>

namespace pdal
{

class Writer;
class UserCallback;

/**
  A Writer is a terminal stage for a PDAL pipeline.  It usually writes output
  to a file, but this isn't a requirement.  The class provides support for
  some operations common for producing point output.
*/
class PDAL_DLL Writer : public Stage
{
    friend class WriterWrapper;
    friend class FlexWriter;

public:
    /**
      Construct a writer.
    */
    Writer() : m_hashPos(std::string::npos)
        {}

protected:
    std::string m_filename;  ///< Output filename
    XForm m_xXform;          ///< X-dimension transform (scale/offset)
    XForm m_yXform;          ///< Y-dimension transform (scale/offset)
    XForm m_zXform;          ///< Z-dimension transform (scale/offset)
    StringList m_outputDims; ///< List of dimensions to write
    std::string::size_type m_hashPos;

    /**
      Compute an automatic scale/offset for points in the PointView.

      \param view  PointView on which scale should be computed.
    */
    virtual void setAutoXForm(const PointViewPtr view);

    /**
      Locate template placeholder ('#') and validate filename with respect
      to placeholder.
    */
    void handleFilenameTemplate();

private:
    virtual PointViewSet run(PointViewPtr view)
    {
        PointViewSet viewSet;
        write(view);
        viewSet.insert(view);
        return viewSet;
    }
    virtual void writerProcessOptions(const Options& options);

    /**
      Write the point in a PointView.  This is a simplification of the
      \ref run() interface for convenience.  Impelment in subclass if desired.
    */
    virtual void write(const PointViewPtr /*view*/)
        { std::cerr << "Can't write with stage = " << getName() << "!\n"; }

    Writer& operator=(const Writer&); // not implemented
    Writer(const Writer&); // not implemented
};

} // namespace pdal

