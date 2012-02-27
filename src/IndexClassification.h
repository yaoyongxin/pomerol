//
// This file is a part of pomerol - a scientific ED code for obtaining 
// properties of a Hubbard model on a finite-size lattice 
//
// Copyright (C) 2010-2011 Andrey Antipov <antipov@ct-qmc.org>
// Copyright (C) 2010-2011 Igor Krivenko <igor@shg.ru>
//
// pomerol is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// pomerol is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pomerol.  If not, see <http://www.gnu.org/licenses/>.


/** \file IndexClassification.h
**  \brief Declaration of IndexClassification class.
** 
**  \author    Andrey Antipov (antipov@ct-qmc.org)
*/

#ifndef __INCLUDE_INDEXCLASSIFICATION_H
#define __INCLUDE_INDEXCLASSIFICATION_H

#include"Misc.h"
#include"Logger.h"
#include"Lattice.h"
#include"Index.h"

namespace Pomerol{

/** This class handles all the indices classification, it allocates the indices to particular Site+Spin+Orbital configuration.
 *  It also returns the information about current ParticleIndex on request. */
class IndexClassification {
private:
    /** A structure, which holds the site label, orbital and spin of a ParticleIndex. */
    struct IndexInfo;
    /** A link to a Lattice object. */
    Lattice *L;
    /** Total number of indices. */
    ParticleIndex IndexSize;
    /** A map of each ParticleIndex to the information about it. */
    std::map<IndexInfo, ParticleIndex> InfoToIndices;
    /** A vector of IndexInfo - each element corresponds to its number. */
    std::vector<IndexInfo> IndicesToInfo;
public:
    /** Returns total number of ParticleIndices. */
    const ParticleIndex getIndexSize();

    /** Returns a list of indices, which belong to a current site. 
     * \param[in] SiteLabel Label of the Site. 
     */
    std::list<ParticleIndex>& findIndices(const std::string &SiteLabel);

    /** Returns a list of indices, which belong to a current site. 
     * \param[in] A Lattice::Site to match.
     */
    std::list<ParticleIndex>& findIndices(const Lattice::Site &Site);

    /** Returns a ParticleIndex, which corresponds to a given site, orbital and spin */ 
    ParticleIndex findIndex(const std::string &Site, const unsigned short &Orbital, const unsigned short &Spin); 

    /** Constructor 
     * \param[in] L A pointer to a Lattice Object. 
     */
    IndexClassification (Lattice *L);
};

/** This structure holds the site label, the orbital and spin of a ParticleIndex */
struct IndexClassification::IndexInfo 
{
private:
    /** Site label hash */
    std::size_t SiteLabelHash;
public:
    /** Site label. */
    const std::string SiteLabel;
    /** Orbital. */
    const unsigned short Orbital;
    /** Spin. */
    const unsigned short Spin;
    /** Operator < to make the object available for maps. */
    bool operator<(const IndexClassification::IndexInfo& rhs) const ;
    /** Constructor */
    IndexInfo( const std::string &SiteLabel, const unsigned short Orbital, const unsigned short Spin);
};

} // end of namespace Pomerol
#endif // endif :: #ifndef #__INCLUDE_INDEXCLASSIFICATION_H

