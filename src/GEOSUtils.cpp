/******************************************************************************
* Copyright (c) 2015, Howard Butler (howard@hobu.co)
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

#include <pdal/GEOSUtils.hpp>
#include <pdal/util/Utils.hpp>

#include <functional>
#include <map>

#ifdef PDAL_COMPILER_MSVC
#  pragma warning(disable: 4127)  // conditional expression is constant
#endif

namespace pdal
{
namespace geos
{

ErrorHandler::ErrorHandler(bool isDebug, pdal::LogPtr log)
    : m_isDebug(isDebug)
    , m_log(log)
{
    if (m_isDebug)
        m_geos_callback = std::bind(&ErrorHandler::log, this, std::placeholders::_1 );
    else
        m_geos_callback = std::bind(&ErrorHandler::error, this, std::placeholders::_1 );

    m_context = initGEOS_r(GEOSWarningHandler, GEOSErrorHandler);

    GEOSContextHandle_t* ctx = static_cast<GEOSContextHandle_t*>(m_context);
//     GEOSContext_setErrorHandler_r(*ctx, &ErrorHandler::trampoline);
    GEOSContext_setErrorMessageHandler_r(*ctx, &ErrorHandler::trampoline);
}

void ErrorHandler::log(char const* msg)
{
    std::ostringstream oss;

//     if (code == CE_Failure || code == CE_Fatal)
//         error(code, num, msg);
//     else if (code == CE_Debug)
//     {
//         oss << "GDAL debug: " << msg;
//         if (m_log)
//             m_log->get(LogLevel::Debug) << oss.str() << std::endl;
//     }
}


void ErrorHandler::error(char const* msg)
{
    std::ostringstream oss;
//     if (code == CE_Failure || code == CE_Fatal)
//     {
//         oss << "GDAL Failure number = " << num << ": " << msg;
//         throw pdal_error(oss.str());
//     }
}


ErrorHandler::~ErrorHandler()
{
//     CPLPopErrorHandler();
}
} // namespace geos
} // namespace pdal

