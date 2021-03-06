/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::DRGEP

Description
    The Dynamic Adaptive Chemistry (DAC) method [1] simplify the chemistry
    using the matrix rAB defined by (DRGEP algorithm [2])

            |sum_i=1->Nr vAi wi dBi|
	rAB = --------------------------- ,
                max(PA, CA)

    PA = sum_i=1->Nr (max (0, vAi wi))  -> production of species A
    
    CA = sum_i=1->Nr (max (0, -vAi wi)) -> consumption of species A
    
	where i is the reaction index, Nr the number of reactions, vAi is the net 
	stoechiometric coefficient of species A in the ith reaction (vAi = v''-v')
    , wi is the progress variable of reaction i and dBi equals 1 if reaction i
    involves B and O otherwise.
	rAB show the error introduced to the production rates of A when B and all
	the reactions including it are removed. It is computed as in [3] so that 
	the algorithm is O(Nr).

	DAC uses a initial set of species that represents the major parts of the
	combustion mechanism, i.e. H2/O2, fuel decomposition and CO2 production.
	Usualy, it includes the fuel, HO2 and CO. Then it computes the dependance 
	of these set to the other species. This is done by introducing R-value
	defined by

	R_V0 (V) = max_SP(product(rij)) , 

	where SP is the set of all possible paths leading from V0 to V and 
	product(rij) is the chain product of the weights of the edges along the 
	given path. The R-value for the initial set species is 1. 

	When the R-value of a species is larger than a user-defined tolerance
	then the species is included in the simplified mechanism. Otherwise, 
	the species is removed along with all the reactions including it.

	During this process, instead of looking over all species like described
	in [1], the algorithm implemented here creates dynamic list to retain
	the initialized edges only (see [3]).

    To avoid using the target species when they are not contributing yet or 
    anymore to the system, a coefficient based on the exchange of element is 
    introduced:
    
                NTa |PT - CT|
    alphaTa = ----------------
                    Pa
    
    Pa = sum_speciesS NSa max(0, PS-CS)
    
    where a refers to different elements present in the system (namely C, H, O and N for 
    conventionail hydrocarbon combustion), NTa is the number of element a in species T.
    When this coefficient alpha is below the specified threshold, the species is removed
    from the search initiating set. In the original paper from Pepiot-Desjardins et al.[2],
    this coefficient is further transformed to compute a global normalized scaling coefficient
    but here as it is dynamicly computed, alpha is not introduced in the calculation of R.
    

	[1] L. Liang, J. G. Stevens, and J. T. Farrell. A dynamic adaptive chemistry 
	scheme for reactive flow computations. Proceedings of the Combustion 
	Institute, 32(1):527–534, 2009

    [2] P. Pepiot-Desjardins and H. Pitsch. An efficient error-propagation-based
    reduction method for large chemical kinetic mechanisms. Combustion and Flame,
    154(1-2):67–81, 7 2008

	[3] T. Lu and C. K. Law. Linear time reduction of large kinetic mechanisms 
	with directed relation graph: n-heptane and iso-octane. Combustion and 
	Flame, 144(1-2):24–36, 1 2006

SourceFiles
    DRGEP.C

\*---------------------------------------------------------------------------*/

#ifndef DRGEP_H
#define DRGEP_H

#include "mechanismReduction.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class DRGEP
:
    public mechanismReduction<CompType, ThermoType>
{
    // Private data

    List<label> sC_,sH_,sO_,sN_;
    label NGroupBased_;
    

public:

    //- Runtime type information
    TypeName("DRGEP");


    // Constructors

        //- Construct from components
        DRGEP
        (
            const dictionary& dict,
            TDACChemistryModel<CompType,ThermoType>& chemistry
        );


    // Destructor

        ~DRGEP();


    // Member Functions

        //- Reduce the mechanism
        void reduceMechanism
        (
            const scalarField &c,
            const scalar T,
            const scalar p
        ) ;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "DRGEP.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
