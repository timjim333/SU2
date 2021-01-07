/*!
 * \file CNEMOEulerSolver.hpp
 * \brief Headers of the CNEMOEulerSolver class
 * \author S. R. Copeland, F. Palacios, W. Maier.
 * \version 7.0.8 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../variables/CNEMOEulerVariable.hpp"
#include "../fluid/CNEMOGas.hpp"
#include "CFVMFlowSolverBase.hpp"

/*!
 * \class CNEMOEulerSolver
 * \brief Main class for defining the NEMO Euler's flow solver.
 * \ingroup Euler_Equations
 * \author S. R. Copeland, F. Palacios, W. Maier.
 * \version 7.0.8 "Blackbird"
 */
class CNEMOEulerSolver : public CFVMFlowSolverBase<CNEMOEulerVariable, COMPRESSIBLE> {
protected:

  su2double
  Prandtl_Lam = 0.0,              /*!< \brief Laminar Prandtl number. */
  Prandtl_Turb = 0.0;             /*!< \brief Turbulent Prandtl number. */

  unsigned short nSpecies;        /*!< \brief Number of species in the gas mixture. */
                  
  su2double                  
  Energy_ve_Inf,                  /*!< \brief Vib.-el. free stream energy. */
  Temperature_ve_Inf;             /*!< \brief Vib.-el. free stream temperature. */
  const su2double *MassFrac_Inf;  /*!< \brief Free stream species mass fraction. */

  su2double *Source;              /*!< \brief Auxiliary vector to store source terms. */

  unsigned long ErrorCounter = 0; /*!< \brief Counter for number of un-physical states. */

  CNEMOGas  *FluidModel;          /*!< \brief fluid model used in the solver */

  CNEMOEulerVariable* node_infty = nullptr;

public:

  /*!
   * \brief Constructor of the class.
   */
  CNEMOEulerSolver() : CFVMFlowSolverBase<CNEMOEulerVariable, COMPRESSIBLE>() {}

  /*!
     * \overload
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] config - Definition of the particular problem.
     */
  CNEMOEulerSolver(CGeometry *geometry, CConfig *config, unsigned short iMesh, const bool navier_stokes = false);

  /*!
     * \brief Destructor of the class.
     */
  ~CNEMOEulerSolver(void) override;

  /*!
     * \brief Set the maximum value of the eigenvalue.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] config - Definition of the particular problem.
     */
  void SetMax_Eigenvalue(CGeometry *geometry, CConfig *config);

   /*!
  * \brief Compute the time step for solving the Euler equations.
  * \param[in] geometry - Geometrical definition of the problem.
  * \param[in] solver_container - Container vector with all the solutions.
  * \param[in] config - Definition of the particular problem.
  * \param[in] iMesh - Index of the mesh in multigrid computations.
  * \param[in] Iteration - Value of the current iteration.
    */
  void SetTime_Step(CGeometry *geometry,
                    CSolver **solver_container,
                    CConfig *config,
                    unsigned short iMesh,
                    unsigned long Iteration) final;

  /*!
   * \brief Set the initial condition for the Euler Equations.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] ExtIter - External iteration.
   */
  void SetInitialCondition(CGeometry **geometry, CSolver ***solver_container, CConfig *config, unsigned long ExtIter) final;

  /*!
   * \brief Load a solution from a restart file.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver - Container vector with all of the solvers.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_iter - Current external iteration number.
   * \param[in] val_update_geo - Flag for updating coords and grid velocity.
   */
  void LoadRestart(CGeometry **geometry, CSolver ***solver, CConfig *config, int val_iter, bool val_update_geo) final;

  /*!
   * \brief Compute the spatial integration using a centered scheme.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] numerics - Description of the numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iMesh - Index of the mesh in multigrid computations.
   */
  void Centered_Residual(CGeometry *geometry, CSolver **solver_container, CNumerics **numerics,
                         CConfig *config, unsigned short iMesh, unsigned short iRKStep) final;

  /*!
   * \brief Compute the spatial integration using a upwind scheme.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] solver - Description of the numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iMesh - Index of the mesh in multigrid computations.
   */
  void Upwind_Residual(CGeometry *geometry,
                       CSolver **solver_container,
                       CNumerics **numerics_container,
                       CConfig *config,
                       unsigned short iMesh) final;

  /*!
   * \brief Source term integration.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] numerics - Description of the numerical method.
   * \param[in] second_numerics - Description of the second numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iMesh - Index of the mesh in multigrid computations.
   */
  void Source_Residual(CGeometry *geometry,
                       CSolver **solver_container,
                       CNumerics **numerics_container,
                       CConfig *config,
                       unsigned short iMesh) final;

  /*!
   * \brief Preprocessing actions common to the Euler and NS solvers.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iRKStep - Current step of the Runge-Kutta iteration.
   * \param[in] RunTime_EqSystem - System of equations which is going to be solved.
   * \param[in] Output - boolean to determine whether to print output.
   */
  void CommonPreprocessing(CGeometry *geometry, CSolver **solver_container, CConfig *config, unsigned short iMesh,
                           unsigned short iRKStep, unsigned short RunTime_EqSystem, bool Output);

  /*!
   * \brief Compute the velocity^2, SoundSpeed, Pressure, Enthalpy, Viscosity.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] iRKStep - Current step of the Runge-Kutta iteration.
   * \param[in] RunTime_EqSystem - System of equations which is going to be solved.
   */
  void Preprocessing(CGeometry *geometry, CSolver **solver_container, CConfig *config, unsigned short iMesh,
                     unsigned short iRKStep, unsigned short RunTime_EqSystem, bool Output) override;

  /*!
   * \brief Computes primitive variables.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   * \param[in] Output - boolean to determine whether to print output.
   * \return - The number of non-physical points.
   */
  virtual unsigned long SetPrimitive_Variables(CSolver **solver_container,
                                               CConfig *config, bool Output);

  /*!
   * \brief Set the fluid solver nondimensionalization.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] config - Definition of the particular problem.
   */
  void SetNondimensionalization(CConfig *config, unsigned short iMesh) final;

  /*!
   * \brief Impose the far-field boundary condition using characteristics.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] conv_numerics - Description of the numerical method for convective terms.
   * \param[in] visc_numerics - Description of the numerical method for viscous terms.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_marker - Surface marker where the boundary condition is applied.
   */
  void BC_Far_Field(CGeometry *geometry, CSolver **solver_container, CNumerics *conv_numerics,
                    CNumerics *visc_numerics, CConfig *config, unsigned short val_marker) override;

  /*!
     * \brief Impose the symmetry boundary condition using the residual.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] conv_numerics - Description of the numerical method for convective terms.
     * \param[in] visc_numerics - Description of the numerical method for viscous terms.
     * \param[in] config - Definition of the particular problem.
     * \param[in] val_marker - Surface marker where the boundary condition is applied.
     */
  void BC_Sym_Plane(CGeometry *geometry, CSolver **solver_container, CNumerics *conv_numerics,
                    CNumerics *visc_numerics, CConfig *config, unsigned short val_marker) final;

  /*!
     * \brief Impose a subsonic inlet boundary condition.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] conv_numerics - Description of the numerical method for convective terms.
     * \param[in] visc_numerics - Description of the numerical method for viscous terms.
     * \param[in] config - Definition of the particular problem.
     * \param[in] val_marker - Surface marker where the boundary condition is applied.
     */
  void BC_Inlet(CGeometry *geometry, CSolver **solver_container, CNumerics *conv_numerics,
                CNumerics *visc_numerics, CConfig *config, unsigned short val_marker) override;

  /*!
     * \brief Impose a supersonic inlet boundary condition.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] conv_numerics - Description of the numerical method for convective terms.
     * \param[in] visc_numerics - Description of the numerical method for viscous terms.
     * \param[in] config - Definition of the particular problem.
     * \param[in] val_marker - Surface marker where the boundary condition is applied.
     */
  void BC_Supersonic_Inlet(CGeometry *geometry, CSolver **solver_container,
                           CNumerics *conv_numerics, CNumerics *visc_numerics,
                           CConfig *config, unsigned short val_marker) override;
  /*!
   * \brief Impose the supersonic outlet boundary condition.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] solver - Description of the numerical method.
   * \param[in] config - Definition of the particular problem.
   * \param[in] val_marker - Surface marker where the boundary condition is applied.
   */
  void BC_Supersonic_Outlet(CGeometry *geometry, CSolver **solver_container,
                            CNumerics *conv_numerics, CNumerics *visc_numerics,
                            CConfig *config, unsigned short val_marker) override;
  /*!
     * \brief Impose the outlet boundary condition.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] conv_numerics - Description of the numerical method for convective terms.
     * \param[in] visc_numerics - Description of the numerical method for viscous terms.
     * \param[in] config - Definition of the particular problem.
     * \param[in] val_marker - Surface marker where the boundary condition is applied.

     */
  void BC_Outlet(CGeometry *geometry, CSolver **solver_container, CNumerics *conv_numerics,
                 CNumerics *visc_numerics, CConfig *config, unsigned short val_marker) override;

  /*!
     * \brief Update the solution using an explicit Euler scheme.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] config - Definition of the particular problem.
     */
  void ExplicitEuler_Iteration(CGeometry *geometry, CSolver **solver_container, CConfig *config) override;

  /*!
     * \brief Update the solution using an explicit Euler scheme.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] config - Definition of the particular problem.
   * \param[in] iRKStep - Runge-Kutta step.
     */
  void ExplicitRK_Iteration(CGeometry *geometry, CSolver **solver_container,
                            CConfig *config, unsigned short iRKStep) override;

  /*!
     * \brief Update the solution using an implicit Euler scheme.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] config - Definition of the particular problem.
     */
  void ImplicitEuler_Iteration(CGeometry *geometry, CSolver **solver_container, CConfig *config) override;

  /*!
     * \brief Set the total residual adding the term that comes from the Dual Time Strategy.
     * \param[in] geometry - Geometrical definition of the problem.
     * \param[in] solver_container - Container vector with all the solutions.
     * \param[in] config - Definition of the particular problem.
     * \param[in] iRKStep - Current step of the Runge-Kutta iteration.
     * \param[in] iMesh - Index of the mesh in multigrid computations.
     * \param[in] RunTime_EqSystem - System of equations which is going to be solved.
     */
  void SetResidual_DualTime(CGeometry *geometry, CSolver **solver_container, CConfig *config,
                            unsigned short iRKStep, unsigned short iMesh, unsigned short RunTime_EqSystem) override;

  /*!
   * \brief Compute a pressure sensor switch.
   * \param[in] geometry - Geometrical definition of the problem.
   * \param[in] solver_container - Container vector with all the solutions.
   * \param[in] config - Definition of the particular problem.
   */
  inline void SetCentered_Dissipation_Sensor(CGeometry *geometry, CConfig *config) { }

   /*!
   * \brief Set the value of undivided laplacian.
   * \param[in] val_und_lapl_i Undivided laplacian at point i.
   * \param[in] val_und_lapl_j Undivided laplacian at point j.
   */
  inline void SetUndivided_Laplacian(CGeometry *geometry, CConfig *config) { }

    /*!
   * \brief Print verification error to screen.
   * \param[in] config - Definition of the particular problem.
   */
  void PrintVerificationError(const CConfig* config) const final { }

};
