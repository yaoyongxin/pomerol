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


/** \file Operator.cpp
**  \brief Implementation of the OperatorPresets class.
** 
**  \author    Andrey Antipov (Andrey.E.Antipov@gmail.com)
*/

#include "OperatorPresets.h"

namespace Pomerol {
namespace OperatorPresets {

//
// Operator N
//

N::N(ParticleIndex Nmodes):Operator(),Nmodes(Nmodes)
{
    std::vector<ElemOp> ops;
    ops.resize(2);
    for (ParticleIndex index=0; index<Nmodes; ++index) {
        ops[0]=boost::make_tuple(true,index);
        ops[1]=boost::make_tuple(false,index);
        Terms.push_back(boost::make_tuple(1.0, ops));
    };
};
    
std::map<FockState,MelemType> N::actRight(const FockState &ket) const
{
    std::map<FockState,MelemType> output;
    output[ket]=this->getMatrixElement(ket);
    return output;
}

MelemType N::getMatrixElement(const FockState &bra, const FockState &ket) const
{
    return (bra!=ket)?0:getMatrixElement(ket);
}

MelemType N::getMatrixElement(const FockState &ket) const
{
    return ket.count();
}

//
// Operator Sz
//

Sz::Sz(ParticleIndex Nmodes):Operator(),Nmodes(Nmodes)
{
    if (Nmodes%2 == 1 ) { throw ( Pomerol::Operator::exWrongLabel() ); ERROR("Sz operator requires even number of indices"); }; 
    SpinUpIndices=std::vector<ParticleIndex>(0);
    for (ParticleIndex i=Nmodes/2; i<Nmodes; i++) SpinUpIndices.push_back(i);

    SpinDownIndices=std::vector<ParticleIndex>(0);
    for (ParticleIndex i=0; i<Nmodes/2; i++) SpinDownIndices.push_back(i);
    generateTerms();
}

Sz::Sz(const std::vector<ParticleIndex> & SpinUpIndices, const std::vector<ParticleIndex> & SpinDownIndices) 
    : Operator(),Nmodes(SpinUpIndices.size() + SpinDownIndices.size()),SpinUpIndices(SpinUpIndices), SpinDownIndices(SpinDownIndices)
{
    if (SpinUpIndices.size() != SpinDownIndices.size() ) { throw ( Pomerol::Operator::exWrongLabel() ); ERROR("Sz operator requires even number of indices"); }; 
    generateTerms();
}

void Sz::generateTerms()
{
    std::vector<ElemOp> ops;
    ops.resize(2);

    for (ParticleIndex i=0; i<SpinUpIndices.size(); ++i) {
        ops[0]=boost::make_tuple(1,SpinUpIndices[i]);
        ops[1]=boost::make_tuple(0,SpinUpIndices[i]);
        Terms.push_back(boost::make_tuple(0.5, ops));

        ops[0]=boost::make_tuple(1,SpinDownIndices[i]);
        ops[1]=boost::make_tuple(0,SpinDownIndices[i]);
        Terms.push_back(boost::make_tuple(-0.5, ops));
    }
}

MelemType Sz::getMatrixElement(const FockState &ket) const
{
    int up_value=0, down_value=0;
    std::vector<ParticleIndex>::const_iterator it_up=SpinUpIndices.begin(), it_down=SpinDownIndices.begin();
    for (;it_up!=SpinUpIndices.end();it_up++) up_value+=ket.test(*it_up);
    for (;it_down!=SpinDownIndices.end();it_down++) down_value+=ket.test(*it_down);
    return 0.5*(up_value-down_value);
}

MelemType Sz::getMatrixElement(const FockState &bra, const FockState &ket) const
{
    return (bra!=ket)?0:getMatrixElement(ket);
}

std::map<FockState,MelemType> Sz::actRight(const FockState &ket) const
{
    std::map<FockState,MelemType> output;
    output[ket]=this->getMatrixElement(ket);
    return output;
}

//
// Operator Cdag ( c^\dagger )
//

Cdag::Cdag(ParticleIndex index):Operator(),index(index)
{
    std::vector<ElemOp> ops;
    ops.push_back(boost::make_tuple(1,index));
    Terms.push_back(boost::make_tuple(1,ops));
}

//
// Operator C
//

C::C(ParticleIndex index):Operator(),index(index)
{
    std::vector<ElemOp> ops;
    ops.push_back(boost::make_tuple(0,index));
    Terms.push_back(boost::make_tuple(1,ops));
}

} // end of namespace OperatorPresets
} // end of namespace Pomerol