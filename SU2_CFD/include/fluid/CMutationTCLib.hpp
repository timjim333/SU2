/*!
 * \file CMutationTCLib.hpp
 * \brief Defines the class for the link to Mutation++ ThermoChemistry library.
 * \author C. Garbacz
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

#include "CNEMOGas.hpp"

/*!
 * \derived class CMutationTCLib
 * \brief Child class for Mutation++ nonequilibrium gas model.
 * \author:  C. Garbacz
 */
class CMutationTCLib : public CNEMOGas {

private:

  vector<su2double> Cv_ks,                /*!< \brief Species specific heats at constant volume. */
  es,                                     /*!< \brief Species energies. */
  omega_vec,                              /*!< \brief Dummy vector for vibrational energy source term. */
  h_RT;                                   /*!< \brief Enthalpy divided by R*T. */

  su2double Tref;                         /*!< \brief Reference temperature. */

public:

  /*!
   * \brief Constructor of the class.
   */
  CMutationTCLib(const CConfig* config, unsigned short val_nDim);

  /*!
   * \brief Destructor of the class.
   */
  virtual ~CMutationTCLib(void);

  /*!
   * \brief Set mixture therodynamic state.
   * \param[in] rhos - Species partial densities.
   * \param[in] T    - Translational/Rotational temperature.
   * \param[in] Tve  - Vibrational/Electronic temperature.
   */
  void SetTDStateRhosTTv(vector<su2double>& val_rhos, su2double val_temperature, su2double val_temperature_ve) final;

  /*!
   * \brief Get species T-R specific heats at constant volume.
   */
  vector<su2double>& GetSpeciesCvTraRot() final;

  /*!
   * \brief Get species V-E specific heats at constant volume.
   */
  vector<su2double>& GetSpeciesCvVibEle() final;
    
  /*!
   * \brief Get mixture energies (total internal energy and vibrational energy).
   */
  vector<su2double>& GetMixtureEnergies() final;

  /*!
   * \brief Get vector of species V-E energy.
   */
  vector<su2double>& GetSpeciesEve(su2double val_T) final;
  
  /*!
   * \brief Get species net production rates.
   */
  vector<su2double>& GetNetProductionRates() final;

  /*!
   * \brief Get vibrational energy source term.
   */
  su2double GetEveSourceTerm() final;
  
  /*!
   * \brief Get species enthalpies.
   */
  vector<su2double>& GetSpeciesEnthalpy(su2double val_T, su2double val_Tve, su2double *val_eves) final;

  /*!
   * \brief Get species diffusion coefficients.
   */
  vector<su2double>& GetDiffusionCoeff() final;

  /*!
   * \brief Get viscosity.
   */
  su2double GetViscosity() final;

  /*!
   * \brief Get T-R and V-E thermal conductivities vector.
   */
  vector<su2double>& GetThermalConductivities() final;
  
  /*!
   * \brief Get translational and vibrational temperatures vector.
   */
  vector<su2double>& GetTemperatures(vector<su2double>& val_rhos, su2double rhoE, su2double rhoEve, su2double rhoEvel) final;
   
  /*!
   * \brief Get species molar mass.
   */
  vector<su2double>& GetSpeciesMolarMass() final;

  /*!
   * \brief Get reference temperature.
   */
  vector<su2double>& GetRefTemperature() final;

  /*!
   * \brief Get species formation enthalpy.
   */
  vector<su2double>& GetSpeciesFormationEnthalpy() final;

};
