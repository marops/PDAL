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

#include <cassert>
#include <cmath>
#include <cstdint>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

#ifndef PDAL_DLL
#if defined(_WIN32)
#   define PDAL_DLL   __declspec(dllexport)
#else
#  if defined(USE_GCC_VISIBILITY_FLAG)
#    define PDAL_DLL     __attribute__ ((visibility("default")))
#  else
#    define PDAL_DLL
#  endif
#endif
#endif

namespace pdal
{

namespace FileUtils
{
    // open existing file for reading
    std::istream* openFile(std::string const& filename, bool asBinary=true);

    // open new file for writing
    std::ostream* createFile(std::string const& filename, bool asBinary=true);

    bool directoryExists(std::string const& dirname);
    bool createDirectory(std::string const& dirname);
    void deleteDirectory(std::string const& dirname);

    void closeFile(std::ostream* ofs);
    void closeFile(std::istream* ifs);

    bool deleteFile(const std::string& filename);
    void renameFile(const std::string& dest, const std::string& src);
    bool fileExists(const std::string& filename);
    uintmax_t fileSize(const std::string& filename);

    // reads a file into a text string for you
    std::string readFileIntoString(const std::string& filename);

    // return current working dir
    // the result will always have a trailing '/'
    std::string getcwd();

    // return the file component of the given path,
    // e.g. "d:/foo/bar/a.c" -> "a.c"
    std::string getFilename(const std::string& path);

    // return the directory component of the given path,
    // e.g. "d:/foo/bar/a.c" -> "d:/foo/bar"
    // the result will always have a trailing '/'
    std::string getDirectory(const std::string& path);

    // returns true iff the path is not relative
    bool isAbsolutePath(const std::string& path);

    // if the filename is an absolute path, just return it
    // otherwise, make it absolute (relative to current working dir)
    // and return that
    std::string toAbsolutePath(const std::string& filename);

    // if the filename is an absolute path, just return it
    // otherwise, make it absolute (relative to base dir) and return that
    //
    // note: if base dir is not absolute, first make it absolute via
    // toAbsolutePath(base)
    std::string toAbsolutePath(const std::string& filename,
        const std::string base);
    
    std::string readFileAsString(std::string const& filename);
    void fileTimes(const std::string& filename, struct tm *createTime,
        struct tm *modTime);

    std::string extension(const std::string& filename);
}

} // namespace pdal
