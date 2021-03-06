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
    Foam::ode

Description
    An ODE solver for chemistry

SourceFiles
    odeTDAC.C

\*---------------------------------------------------------------------------*/

#ifndef odeTDAC_H
#define odeTDAC_H

#include "chemistrySolverTDAC.H"
#include "ODESolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
template<class CompType, class ThermoType>
class odeTDAC;

/*---------------------------------------------------------------------------*\
                            Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class odeTDAC
:
    public chemistrySolverTDAC<CompType, ThermoType>
{
    // Private data

        dictionary coeffsDict_;
        const word solverName_;
        autoPtr<ODESolver> odeSolver_;

        // Model constants

            scalar eps_;
            scalar scale_;


public:

    //- Runtime type information
    TypeName("ODETDAC");


    // Constructors

        //- Construct from components
        odeTDAC
        (
            TDACChemistryModel<CompType, ThermoType>& model,
            const word& modelName
        );


    //- Destructor
    virtual ~odeTDAC();


    // Member Functions

        scalar solve
        (
            scalarField& c,
            const scalar T,
            const scalar p,
            const scalar t0,
            const scalar dt
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "odeTDAC.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
