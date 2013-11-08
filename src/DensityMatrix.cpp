//
// This file is a part of pomerol - a scientific ED code for obtaining 
// properties of a Hubbard model on a finite-size lattice 
//
// Copyright (C) 2010-2011 Andrey Antipov <Andrey.E.Antipov@gmail.com>
// Copyright (C) 2010-2011 Igor Krivenko <Igor.S.Krivenko@gmail.com>
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


/** \file src/DensityMatrix.cpp
** \brief Density matrix of the grand canonical ensemble.
**
** \author Igor Krivenko (Igor.S.Krivenko@gmail.com)
** \author Andrey Antipov (Andrey.E.Antipov@gmail.com)
*/
#include "DensityMatrix.h"

namespace Pomerol{

DensityMatrix::DensityMatrix(const StatesClassification& S, const Hamiltonian& H, RealType beta) : 
    Thermal(beta), ComputableObject(Constructed), S(S), H(H), parts(S.NumberOfBlocks())
{}

DensityMatrix::~DensityMatrix()
{
    for(std::vector<DensityMatrixPart*>::iterator iter = parts.begin(); iter != parts.end(); iter++)
	delete *iter;
}

void DensityMatrix::prepare(void)
{
    if (Status >= Prepared) return;
    BlockNumber NumOfBlocks = parts.size();
    RealType GroundEnergy = H.getGroundEnergy();
    // There is one-to-one correspondence between parts of the Hamiltonian
    // and parts of the density matrix itself. 
    for(BlockNumber n = 0; n < NumOfBlocks; n++)
        parts[n] = new DensityMatrixPart(S, H.getPart(n),beta,GroundEnergy);
    Status = Prepared;
}

void DensityMatrix::compute(void)
{
    if (Status >= Computed) return;
    RealType Z = 0;
    // A total partition function is a sum over partition functions of
    // all non-normalized parts.
    for(std::vector<DensityMatrixPart*>::iterator iter = parts.begin(); iter != parts.end(); iter++)
        Z += (*iter)->computeUnnormalized();
 
    // Divide the density matrix by Z.
    for(std::vector<DensityMatrixPart*>::iterator iter = parts.begin(); iter != parts.end(); iter++)
        (*iter)->normalize(Z);
    Status = Computed;
}

RealType DensityMatrix::getWeight(QuantumState state) const
{
    if ( Status < Computed ) { ERROR("DensityMatrix is not computed yet."); throw (exStatusMismatch()); };
    BlockNumber BlockNumber = S.getBlockNumber(state);
    InnerQuantumState InnerState = S.getInnerState(state);

    return parts[BlockNumber]->getWeight(InnerState);
}
 
const DensityMatrixPart& DensityMatrix::getPart(const QuantumNumbers &in) const
{
    return *parts[S.getBlockNumber(in)];
}
 
const DensityMatrixPart& DensityMatrix::getPart(BlockNumber in) const
{
    return *parts[in];
}

RealType DensityMatrix::getAverageEnergy() const
{
    if ( Status < Computed ) { ERROR("DensityMatrix is not computed yet."); throw (exStatusMismatch()); };
    RealType E = 0;
    for(std::vector<DensityMatrixPart*>::const_iterator iter = parts.begin(); iter != parts.end(); iter++)
    E += (*iter)->getAverageEnergy();
    return E;
};

RealType DensityMatrix::getAverageOccupancy() const
{
    if ( Status < Computed ) { ERROR("DensityMatrix is not computed yet."); throw (exStatusMismatch()); };
    RealType n = 0;
    for(std::vector<DensityMatrixPart*>::const_iterator iter = parts.begin(); iter != parts.end(); iter++)
    n += (*iter)->getAverageOccupancy();
    return n;
};



RealType DensityMatrix::getAverageDoubleOccupancy(ParticleIndex i, ParticleIndex j) const
{
    if ( Status < Computed ) { ERROR("DensityMatrix is not computed yet."); throw (exStatusMismatch()); };
    RealType NN = 0;
    for(std::vector<DensityMatrixPart*>::const_iterator iter = parts.begin(); iter != parts.end(); iter++)
    NN += (*iter)->getAverageDoubleOccupancy(i,j);
    return NN;
};
 
void DensityMatrix::save(H5::CommonFG* RootGroup) const
{
    H5::Group DMRootGroup(RootGroup->createGroup("DensityMatrix"));

    // Save inverse temperature
    HDF5Storage::saveReal(&DMRootGroup,"beta",beta);

    // Save parts
    BlockNumber NumOfBlocks = parts.size();
    H5::Group PartsGroup = DMRootGroup.createGroup("parts");
    for(BlockNumber n = 0; n < NumOfBlocks; n++){
    std::stringstream nStr;
    nStr << n;
    H5::Group PartGroup = PartsGroup.createGroup(nStr.str().c_str());
    parts[n]->save(&PartGroup);
    }
}
 
void DensityMatrix::load(const H5::CommonFG* RootGroup)
{
    H5::Group DMRootGroup(RootGroup->openGroup("DensityMatrix"));  
    RealType newBeta = HDF5Storage::loadReal(&DMRootGroup,"beta");
    if(newBeta != beta)
    throw(H5::DataSetIException("DensityMatrix::load()",
                    "Data in the storage is for another value of the temperature."));

    H5::Group PartsGroup = DMRootGroup.openGroup("parts");
    BlockNumber NumOfBlocks = parts.size();
    if(NumOfBlocks != PartsGroup.getNumObjs())
    throw(H5::GroupIException("DensityMatrix::load()","Inconsistent number of stored parts."));

    for(BlockNumber n = 0; n < NumOfBlocks; n++){
    std::stringstream nStr;
    nStr << n;
    H5::Group PartGroup = PartsGroup.openGroup(nStr.str().c_str());
    parts[n]->load(&PartGroup);
    }
}

} // end of namespace Pomerol
