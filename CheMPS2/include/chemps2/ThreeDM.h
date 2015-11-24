/*
   CheMPS2: a spin-adapted implementation of DMRG for ab initio quantum chemistry
   Copyright (C) 2013-2015 Sebastian Wouters

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef THREEDM_CHEMPS2_H
#define THREEDM_CHEMPS2_H

#include "TensorT.h"
#include "TensorL.h"
#include "TensorF0.h"
#include "TensorF1.h"
#include "TensorS0.h"
#include "TensorS1.h"
#include "Tensor3RDM.h"
#include "SyBookkeeper.h"

namespace CheMPS2{
/** ThreeDM class.
    \author Sebastian Wouters <sebastianwouters@gmail.com>
    \date November 16, 2015
    
    The ThreeDM class stores the spin-summed three-particle reduced density matrix (3-RDM) of a converged DMRG calculation: \n
    \f$ \Gamma_{ijk;lmn} = \sum_{\sigma \tau s} \braket{ a^{\dagger}_{i \sigma} a^{\dagger}_{j \tau} a^{\dagger}_{k s} a_{n s} a_{m \tau} a_{l \sigma}} \f$\n
    Because the wave-function belongs to a certain Abelian irrep, \f$ I_{i} \otimes I_{j} \otimes I_{k} = I_{l} \otimes I_{m} \otimes I_{n} \f$ must be valid before the corresponding element \f$ \Gamma_{ijk;lmn} \f$ is non-zero.
*/
   class ThreeDM{

      public:
      
         //! Constructor
         /** \param denBKIn Symmetry sector bookkeeper
             \param ProbIn The problem to be solved */
         ThreeDM(const SyBookkeeper * book_in, const Problem * prob_in);
         
         //! Destructor
         virtual ~ThreeDM();
         
         //! Get a 3-RDM term, using the DMRG indices
         /** \param cnt1 the first index
             \param cnt2 the second index
             \param cnt3 the third index
             \param cnt4 the fourth index
             \param cnt5 the fifth index
             \param cnt6 the sixth index
             \return the desired value */
         double get_dmrg_index(const int cnt1, const int cnt2, const int cnt3, const int cnt4, const int cnt5, const int cnt6) const;
         
         //! Get a 3-RDM, using the HAM indices
         /** \param cnt1 the first index
             \param cnt2 the second index
             \param cnt3 the third index
             \param cnt4 the fourth index
             \param cnt5 the fifth index
             \param cnt6 the sixth index
             \return the desired value */
         double get_ham_index(const int cnt1, const int cnt2, const int cnt3, const int cnt4, const int cnt5, const int cnt6) const;
         
         //! Fill the 3-RDM terms corresponding to site denT->gIndex()
         /** \param denT DMRG site-matrices
             \param Ltens Ltensors
             \param F0tens F0tensors
             \param F1tens F1tensors
             \param S0tens S0tensors
             \param S1tens S1tensors
             \param dm3_a_J0_doublet Renormalized operator of three second quantized operators: annihilator, annihilator, annihilator
             \param dm3_a_J1_doublet Renormalized operator of three second quantized operators: annihilator, annihilator, annihilator
             \param dm3_a_J1_quartet Renormalized operator of three second quantized operators: annihilator, annihilator, annihilator
             \param dm3_b_J0_doublet Renormalized operator of three second quantized operators: annihilator, annihilator, creator
             \param dm3_b_J1_doublet Renormalized operator of three second quantized operators: annihilator, annihilator, creator
             \param dm3_b_J1_quartet Renormalized operator of three second quantized operators: annihilator, annihilator, creator
             \param dm3_c_J0_doublet Renormalized operator of three second quantized operators: annihilator, creator, annihilator
             \param dm3_c_J1_doublet Renormalized operator of three second quantized operators: annihilator, creator, annihilator
             \param dm3_c_J1_quartet Renormalized operator of three second quantized operators: annihilator, creator, annihilator
             \param dm3_d_J0_doublet Renormalized operator of three second quantized operators: creator, annihilator, annihilator
             \param dm3_d_J1_doublet Renormalized operator of three second quantized operators: creator, annihilator, annihilator
             \param dm3_d_J1_quartet Renormalized operator of three second quantized operators: creator, annihilator, annihilator */
         void fill_site( TensorT * denT, TensorL *** Ltens, TensorF0 **** F0tens, TensorF1 **** F1tens, TensorS0 **** S0tens, TensorS1 **** S1tens,
                         Tensor3RDM **** dm3_a_J0_doublet, Tensor3RDM **** dm3_a_J1_doublet, Tensor3RDM **** dm3_a_J1_quartet,
                         Tensor3RDM **** dm3_b_J0_doublet, Tensor3RDM **** dm3_b_J1_doublet, Tensor3RDM **** dm3_b_J1_quartet,
                         Tensor3RDM **** dm3_c_J0_doublet, Tensor3RDM **** dm3_c_J1_doublet, Tensor3RDM **** dm3_c_J1_quartet,
                         Tensor3RDM **** dm3_d_J0_doublet, Tensor3RDM **** dm3_d_J1_doublet, Tensor3RDM **** dm3_d_J1_quartet );
         
         //! After the whole 3-RDM is filled, a prefactor for higher multiplicities should be applied
         void correct_higher_multiplicities();
             
         //! Return the trace (should be N(N-1)(N-2))
         /** \return Trace of the 3-RDM */
         double trace() const;
         
         //! Save the 3-RDM to disk
         void save() const;
         
         //! Load the 3-RDM from disk
         void read();
         
         //! Add the 3-RDM elements of all MPI processes
         void mpi_allreduce();
         
      private:
      
         //The BK containing all the irrep information
         const SyBookkeeper * book;
         
         //The problem containing orbital reshuffling and symmetry information
         const Problem * prob;
         
         //The DMRG chain length
         int L;
         
         //The 3-RDM elements (L*L*L*L*L*L array of doubles)
         double * elements;
         
         //Set all twelve-fold permutation symmetric 3-RDM elements, using the DMRG indices.
         void set_dmrg_index(const int cnt1, const int cnt2, const int cnt3, const int cnt4, const int cnt5, const int cnt6, const double value);
         
         //Helper functions
         static int trianglefunction(const int k, const int glob);
         static void tripletrianglefunction(const int global, int * jkl);
         static int phase(const int two_times_power){ return ((((two_times_power/2)%2)!=0)?-1:1); }
         
         //Partitioning 2-4-0
         double diagram1(TensorT * denT, TensorF0 * denF0, double * workmem) const;
         
         //Partitioning 0-4-2
         double diagram3(TensorT * denT, TensorF0 * denF0, double * workmem) const;
         
         //Partitioning 3-3-0
         double diagram4_5_6_7_8_9(TensorT * denT, Tensor3RDM * d3tens, double * workmem, const char type) const;
         
         //Partitioning 2-3-1
         double diagram10(TensorT * denT, TensorS0 * denS0, TensorL * denL, double * workmem, double * workmem2) const;
         double diagram11(TensorT * denT, TensorS1 * denS1, TensorL * denL, double * workmem, double * workmem2) const;
         double diagram12(TensorT * denT, TensorF0 * denF0, TensorL * denL, double * workmem, double * workmem2) const;
         double diagram13(TensorT * denT, TensorF1 * denF1, TensorL * denL, double * workmem, double * workmem2) const;
         double diagram14(TensorT * denT, TensorF0 * denF0, TensorL * denL, double * workmem, double * workmem2) const;
         double diagram15(TensorT * denT, TensorF1 * denF1, TensorL * denL, double * workmem, double * workmem2) const;
         
         //Partitioning 1-3-2
         double diagram16(TensorT * denT, TensorL * denL, TensorS0 * denS0, double * workmem, double * workmem2) const;
         double diagram17(TensorT * denT, TensorL * denL, TensorS1 * denS1, double * workmem, double * workmem2) const;
         double diagram18(TensorT * denT, TensorL * denL, TensorF0 * denF0, double * workmem, double * workmem2) const;
         double diagram19(TensorT * denT, TensorL * denL, TensorF1 * denF1, double * workmem, double * workmem2) const;
         double diagram20(TensorT * denT, TensorL * denL, TensorF0 * denF0, double * workmem, double * workmem2) const;
         double diagram21(TensorT * denT, TensorL * denL, TensorF1 * denF1, double * workmem, double * workmem2) const;
         
         //Partitioning 2-2-2
         void       fill_tens_29_33(TensorT * denT, TensorF0 * tofill, TensorF0 * denF0, double * workmem) const;
         void       fill_tens_30_32(TensorT * denT, TensorF1 * tofill, TensorF1 * denF1, double * workmem) const;
         void       fill_tens_36_42(TensorT * denT, TensorF1 * tofill, TensorF0 * denF0, double * workmem) const;
         void fill_tens_34_35_37_38(TensorT * denT, TensorF1 * fill34, TensorF0 * fill35, TensorF1 * fill37, TensorF1 * fill38, TensorF1 * denF1, double * workmem, double * workmem2) const;
         void       fill_tens_49_51(TensorT * denT, TensorF0 * tofill, TensorS0 * denS0, double * workmem) const;
         void       fill_tens_50_52(TensorT * denT, TensorF1 * tofill, TensorS1 * denS1, double * workmem) const;
         void       fill_tens_22_24(TensorT * denT, TensorS0 * tofill, TensorS0 * denS0, double * workmem) const;
         void          fill_tens_28(TensorT * denT, TensorS1 * tofill, TensorS0 * denS0, double * workmem) const;
         void          fill_tens_23(TensorT * denT, TensorS1 * tofill, TensorS1 * denS1, double * workmem) const;
         void    fill_tens_25_26_27(TensorT * denT, TensorS1 * fill25, TensorS1 * fill26, TensorS0 * fill27, TensorS1 * denS1, double * workmem, double * workmem2) const;
         void       fill_tens_45_47(TensorT * denT, TensorS0 * tofill, TensorF0 * denF0, double * workmem, const bool first) const;
         void       fill_tens_46_48(TensorT * denT, TensorS1 * tofill, TensorF1 * denF1, double * workmem, const bool first) const;
         
         //Partitioning 3-2-1 and 1-4-1
         void    fill_53_54(TensorT * denT, Tensor3RDM * tofill, TensorL * denL, double * workmem) const;
         void fill_55_to_60(TensorT * denT, Tensor3RDM * tofill, TensorL * denL, double * workmem) const;
         void       fill_61(TensorT * denT, Tensor3RDM * tofill, TensorL * denL, double * workmem) const;
         void    fill_63_65(TensorT * denT, Tensor3RDM * fill63, Tensor3RDM * fill65, Tensor3RDM * fill67, Tensor3RDM * fill68,
                                            Tensor3RDM * fill76, Tensor3RDM * fill77, TensorL * denL, double * workmem, double * workmem2) const;
         void fill_69_78_79(TensorT * denT, Tensor3RDM * fill69, Tensor3RDM * fill78, Tensor3RDM * fill79, TensorL * denL, double * workmem, double * workmem2) const;
         
         //Partitioning 3-1-2 (d90 --> d189)
         void           fill_a_S0(TensorT * denT, Tensor3RDM * tofill,                        TensorS0 * denS0, double * workmem) const;
         void         fill_bcd_S0(TensorT * denT, Tensor3RDM * tofill,                        TensorS0 * denS0, double * workmem) const;
         void             fill_F0(TensorT * denT, Tensor3RDM * tofill,                        TensorF0 * denF0, double * workmem) const;
         void           fill_F0_T(TensorT * denT, Tensor3RDM * tofill,                        TensorF0 * denF0, double * workmem) const;
         void           fill_a_S1(TensorT * denT, Tensor3RDM * doublet, Tensor3RDM * quartet, TensorS1 * denS1, double * workmem, double * workmem2) const;
         void         fill_bcd_S1(TensorT * denT, Tensor3RDM * doublet, Tensor3RDM * quartet, TensorS1 * denS1, double * workmem, double * workmem2) const;
         void             fill_F1(TensorT * denT, Tensor3RDM * doublet, Tensor3RDM * quartet, TensorF1 * denF1, double * workmem, double * workmem2) const;
         void           fill_F1_T(TensorT * denT, Tensor3RDM * doublet, Tensor3RDM * quartet, TensorF1 * denF1, double * workmem, double * workmem2) const;
         
   };
}

#endif
