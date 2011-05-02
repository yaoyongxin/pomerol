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

/** \file Misc.h
**    \brief Declares very common type names and macros.
** 
** \author    Igor Krivenko (igor@shg.ru)
** \author    Alexey Rubtsov (alex@shg.ru)
** \author    Andrey Antipov (antipov@shg.ru)
*/

#ifndef __INCLUDE_MISC_H
#define __INCLUDE_MISC_H

/** \cond */
//#define NDEBUG
/** \endcond */

#include<iostream>
#include<complex>
#include<string>
#include<vector>
#include<list>
#include<map>

#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
#include<Eigen/Core>
#include<Eigen/Sparse>

#ifdef pomerolOpenMP
#include <omp.h>
#endif
/** Real floating point type. */
typedef double RealType;
/** Complex type. */
typedef std::complex<RealType> ComplexType;

/** Dense complex matrix. */
typedef Eigen::Matrix<ComplexType,Eigen::Dynamic,Eigen::Dynamic,Eigen::AutoAlign|Eigen::RowMajor> MatrixType;
/** Dense real matrix. */
typedef Eigen::Matrix<RealType,Eigen::Dynamic,Eigen::Dynamic,Eigen::AutoAlign|Eigen::RowMajor> RealMatrixType;

typedef Eigen::Matrix<RealType,Eigen::Dynamic,Eigen::Dynamic,Eigen::AutoAlign|Eigen::RowMajor> LowerTriangularRealMatrixType;

/** Dense complex vector. */
typedef Eigen::Matrix<ComplexType,Eigen::Dynamic,1,Eigen::AutoAlign> VectorType;
/** Dense real vector. */
typedef Eigen::Matrix<RealType,Eigen::Dynamic,1,Eigen::AutoAlign> RealVectorType;
/** Dense vector of integers. */
typedef Eigen::Matrix<int,Eigen::Dynamic,1,Eigen::AutoAlign> IntVectorType;

/** Sparse complex matrix */
typedef Eigen::SparseMatrix<RealType,Eigen::ColMajor> ColMajorMatrixType;
typedef Eigen::SparseMatrix<RealType,Eigen::RowMajor> RowMajorMatrixType;
typedef Eigen::DynamicSparseMatrix<RealType,Eigen::ColMajor> DynamicSparseMatrixType;

/** Index represents a combination of spin, orbital, and lattice indices **/
typedef unsigned short ParticleIndex;

/** Possible spin projections are \b down and \b up */
enum spin {down, up};

/** This represent the progress of calculation of any complex object in the code */
enum ObjectStatus {Constructed, Prepared, Computed};

/** A short name for imaginary unit. */
static const ComplexType I = ComplexType(0.0,1.0);    // 'static' to prevent linking problems


/** Generalized 'square' function. */
template<typename T> inline T sqr(T x) { return x*x; }

/** Easy enumeration for orbital names */
enum OrbitalValue {s=0, p=1, d=2, f=3};             //!< The enum for s,p,d,f - orbitals

/** Permutation of 3 elements */
struct Permutation3 {
    const size_t perm[3];
    const int sign;
    bool operator==(const Permutation3& rhs) const {return (sign==rhs.sign && perm[0] == rhs.perm[0] && perm[1]==rhs.perm[1]);};
    bool operator!=(const Permutation3& rhs) const {return !(*this==rhs);};
    friend std::ostream& operator<<(std::ostream& out, const Permutation3 &rhs)
        { out << (rhs.sign==-1?"-":" ") << rhs.perm[0]+1 << rhs.perm[1]+1 << rhs.perm[2]+1 << std::flush; return out;}; 
};

/** Permutation of 4 elements */
struct Permutation4 {
    const size_t perm[4];
    const int sign;
    bool operator==(const Permutation4& rhs) const {return (sign==rhs.sign && perm[0] == rhs.perm[0] && perm[1]==rhs.perm[1] && perm[2] == rhs.perm[2]);};
    bool operator!=(const Permutation4& rhs) const {return !(*this==rhs);};
    friend std::ostream& operator<<(std::ostream& out, const Permutation4 &rhs)
        { out << (rhs.sign==-1?"-":" ") << rhs.perm[0]+1 << rhs.perm[1]+1 << rhs.perm[2]+1 << rhs.perm[3]+1 << std::flush; return out;}; 
};

static const Permutation4 permutations4[24] = {
    {{0,1,2,3}, 1},  {{0,1,3,2},-1},  {{0,2,1,3},-1},  {{0,2,3,1}, 1},  {{0,3,1,2}, 1},  {{0,3,2,1},-1},
    {{1,0,2,3},-1},  {{1,0,3,2}, 1},  {{1,2,0,3}, 1},  {{1,2,3,0},-1},  {{1,3,0,2},-1},  {{1,3,2,0}, 1},
    {{2,0,1,3}, 1},  {{2,0,3,1},-1},  {{2,1,0,3},-1},  {{2,1,3,0}, 1},  {{2,3,0,1}, 1},  {{2,3,1,0},-1},
    {{3,0,1,2},-1},  {{3,0,2,1}, 1},  {{3,1,0,2}, 1},  {{3,1,2,0},-1},  {{3,2,0,1},-1},  {{3,2,1,0}, 1}
};
/** */
#define DUMP_FLOATING_POINT_NUMBERS    10

//@{
/** Do-It-Once environment from A. Rubtsov
**
** When you want a piece of code to run exactly once, just write:
** \verbatim
do_once
    ... your code goes here...
end_do_once
\endverbatim
**/
#define do_once { static bool done_once=false; if (!done_once) {done_once=true;
#define end_do_once }; };
//@}

#define INFO(MSG)                 std::cout << MSG << std::endl;
#define INFO_NONEWLINE(MSG)       std::cout << MSG << std::flush;
#define ERROR(MSG)                std::cerr << MSG << std::endl;

#ifdef NDEBUG
  #define DEBUG(x)
#else
  #define DEBUG(x)  INFO(x)
#endif

#define num_cout std::cout << std::setprecision(8) << std::setw(9) << std::left
#define iomanip_prefs std::setprecision(8) << std::setw(9) << std::left

/** \mainpage 
 * \section   ref_intro Introduction
 * pomerol is an Exact Diagonalization code, mostly oriented on computing single and two-particle Greens Functions and corresponding Vertex functions.
 * It supports calculations of Hubbard model on finite size lattices. The lattices are provided with a *.json 
 * files with a flag -l <LatticeFile> when executing pomerolDiag.
 * \section   ref_API API
 * The general sequence of a calculation is:
 * -    Open a Lattice .json file ( by LatticeAnalysis )
 * -    Define all indices of each mode of the system, i.e. site + spin indices ( by IndexClassification ). 
 *      This is also a moment to discover symmetries against permutations of indices
 * -    Define the Hilbert space in Fock basis - create QuantumStates and sort them into the blocks by their Quantum Numbers. ( by StatesClassification )
 * -    Enter blocks of the hamiltonian ( by Hamiltonian and HamiltonianPart) and diagonalize them
 * -    Find creation and annihilation operators in the space of EigenFunctions ( by FieldOperator and FieldOperatorPart )
 * -    Calculate thermal quantities :
 *      -   The density matrix ( by DensityMatrix and DensityMatrixPart )
 *      -   The Greens Function ( by GreensFunction, GreensFunctionPart and GFContainer to store the values of GF for various index combinations )
 *      -   The TwoParticle Greens Function ( by TwoParticleGF, TwoParticleGFPart and TwoParticleGFContainer )
 * -    Calculcate the Vertex Function out of GF- and TwoParticleGF- containers - no work with Hilbert Space is done
 * 
 * This structure can be seen Class Hierarchy tab under Classes
 *
 * \section ref_conventions Conventions
 *
 * Greens Function:
 * \f[
 *      G(\omega_n) = -\int_0^\beta \langle\mathbf{T}c_i(\tau)c^+_j(0)\rangle e^{i\omega_n\tau} d\tau
 * \f]
 *
 * Two-Particle Greens Function:
 * \f[ \chi_{ijkl}(\omega_{n_1},\omega_{n_2};\omega_{n_3},\omega_{n_1}+\omega_{n_2}-\omega_{n_3}) =
 *   -\int_0^\beta
 *      \langle\mathbf{T} c_i(\tau_1)c_j(\tau_2)c^+_k(\tau_3)c^+_l(0) \rangle
 *      \exp(i\omega_{n_1}\tau_1+i\omega_{n_2}\tau_2-i\omega_{n_3}\tau_3)
 *    d\tau_1 d\tau_2 d\tau_3
 * \f]
 *
 * An irreducible vertex part:
 * \f[ \Gamma_{1234}(\omega_1,\omega_2;\omega_3,\omega_4) \equiv 
 *     \chi_{1234}(\omega_1,\omega_2;\omega_3,\omega_4) -
 *     \chi^{0}_{1234}(\omega_1,\omega_2;\omega_3,\omega_4)
 * \f]
 *
 * An amputated irreducible vertex part:
 * \f[ \gamma_{1234}(\omega_1,\omega_2;\omega_3,\omega_4) \equiv 
 *     \sum_{1'2'3'4'}
 *     (g^{-1}(\omega_1))_{11'} (g^{-1}(\omega_2))_{22'}
 *     \Gamma_{1'2'3'4'}(\omega_1,\omega_2;\omega_3,\omega_4)
 *     (g^{-1}(\omega_3))_{3'3} (g^{-1}(\omega_4))_{4'4}
 * \f]
 *
 *
 * \section ref_authors Authors
 *      Andrey Antipov <antipov[at]ct-qmc.org>
 *      Igor Krivenko <igor[at]shg.ru>
 *
 * with kind help of Mikhail Alejnikov, Alexey Rubtsov, Christoph Jung and Aljoscha Wilhelm
 */

#endif // #ifndef __INCLUDE_MISC_H

