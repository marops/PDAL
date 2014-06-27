/******************************************************************************
* Copyright (c) 2014, Peter J. Gadomski (pete.gadomski@gmail.com)
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

#include <pdal/drivers/sbet/Reader.hpp>

namespace pdal
{
namespace drivers
{
namespace sbet
{

Options SbetReader::getDefaultOptions()
{
    Options options;
    return options;
}


void SbetReader::processOptions(const Options& options)
{
    m_filename = options.getOption("filename").getValue<std::string>();
}


void SbetReader::buildSchema(Schema *s)
{
    std::vector<Dimension> dims = getDefaultDimensions();
    for (auto di = dims.begin(); di != dims.end(); ++di)
        m_dims.push_back(s->appendDimension(*di));
}


void SbetReader::ready(PointContext ctx)
{
    size_t pointSize = 0;
    for (auto di = m_dims.begin(); di != m_dims.end(); ++di)
    {
        Dimension *dim = *di;
        pointSize += dim->getByteSize();
    }

    boost::uintmax_t fileSize = FileUtils::fileSize(m_filename);
    if (fileSize % pointSize != 0)
        throw pdal_error("invalid sbet file size");
    m_numPts = fileSize / pointSize;
    m_stream.reset(new ILeStream(m_filename));
}


StageSequentialIterator* SbetReader::createSequentialIterator() const
{
    return new drivers::sbet::iterators::sequential::SbetSeqIterator(m_dims,
        m_numPts, *m_stream);
}


namespace iterators
{
namespace sequential
{

point_count_t SbetSeqIterator::readImpl(PointBuffer& buf, point_count_t numPts)
{
    PointId nextId = buf.size();
    PointId idx = m_index;
    point_count_t numRead = 0;
    seek(idx);
    while (numRead < numPts && idx < m_numPts)
    {
        for (auto di = m_dims.begin(); di != m_dims.end(); ++di)
        {
            double d;
            m_stream >> d;
            Dimension *dim = *di;
            buf.setField(*dim, nextId, d);
        }
        idx++;
        nextId++;
        numRead++;
    }
    m_index = idx;
    return numRead;
}


uint64_t SbetSeqIterator::skipImpl(uint64_t pointsToSkip)
{
    uint32_t lastIndex = (uint32_t)m_index;
    uint64_t newIndex = m_index + pointsToSkip;
    m_index = (uint32_t)std::min((uint64_t)m_numPts, newIndex);
    return std::min(pointsToSkip, m_index - lastIndex);
}


bool SbetSeqIterator::atEndImpl() const
{
    return m_index >= m_numPts;
}


void SbetSeqIterator::seek(PointId idx)
{
    m_stream.seek(idx * sizeof(double) * m_dims.size());
}

} // namespace sequential
} // namespace iterators

} // namespace sbet
} // namespace drivers
} // namespace pdal

