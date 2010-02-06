/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS writer class 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
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
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
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

#include <liblas/lasversion.hpp>
#include <liblas/laswriter.hpp>
#include <liblas/detail/writer/writer10.hpp>

// std
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstring> // std::memset
#include <cassert>

namespace liblas
{

LASWriter::LASWriter(std::ostream& ofs, LASHeader const& header) :
    m_pimpl(detail::WriterFactory::Create(ofs, header)), m_header(header)
{
    m_pimpl->WriteHeader(m_header);
}

LASWriter::~LASWriter()
{
    assert(0 != m_pimpl.get());

    m_pimpl->UpdateHeader(m_header);
}

LASHeader const& LASWriter::GetHeader() const
{
    return m_header;
}

bool LASWriter::WritePoint(LASPoint const& point)
{
    if (!point.IsValid())
    {
        return false;
    }

    m_pimpl->WritePointRecord(point, m_header);

    return true;
}

std::ostream& LASWriter::GetStream() const
{
    return m_pimpl->GetStream();
}

void LASWriter::WriteHeader(LASHeader& header)
{
    m_pimpl->WriteHeader(header);
    m_header = header;
}

bool LASWriter::SetSRS(const LASSpatialReference& srs)
{
    m_pimpl->SetOutputSRS(srs, m_header);
    return true;
}

bool LASWriter::SetInputSRS(const LASSpatialReference& srs)
{
    m_pimpl->SetInputSRS(srs);
    return true;
}

bool LASWriter::SetOutputSRS(const LASSpatialReference& srs)
{
    m_pimpl->SetOutputSRS(srs, m_header);
    return true;
}
} // namespace liblas

