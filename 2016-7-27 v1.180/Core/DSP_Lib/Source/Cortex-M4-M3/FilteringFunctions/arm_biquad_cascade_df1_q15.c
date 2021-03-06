/* ----------------------------------------------------------------------  
* Copyright (C) 2010 ARM Limited. All rights reserved.  
*  
* $Date:        29. November 2010  
* $Revision: 	V1.0.3  
*  
* Project: 	    CMSIS DSP Library  
* Title:	    arm_biquad_cascade_df1_q15.c  
*  
* Description:	Processing function for the  
*				Q15 Biquad cascade DirectFormI(DF1) filter.  
*  
* Target Processor: Cortex-M4/Cortex-M3
*  
* Version 1.0.3 2010/11/29 
*    Re-organized the CMSIS folders and updated documentation.  
*   
* Version 1.0.2 2010/11/11  
*    Documentation updated.   
*  
* Version 1.0.1 2010/10/05   
*    Production release and review comments incorporated.  
*  
* Version 1.0.0 2010/09/20   
*    Production release and review comments incorporated.  
*  
* Version 0.0.5  2010/04/26   
* 	 incorporated review comments and updated with latest CMSIS layer  
*  
* Version 0.0.3  2010/03/10   
*    Initial version  
* -------------------------------------------------------------------- */ 
 
#include "arm_math.h" 
 
/**  
 * @ingroup groupFilters  
 */ 
 
/**  
 * @addtogroup BiquadCascadeDF1  
 * @{  
 */ 
 
/**  
 * @brief Processing function for the Q15 Biquad cascade filter.  
 * @param[in]  *S points to an instance of the Q15 Biquad cascade structure.  
 * @param[in]  *pSrc points to the block of input data.  
 * @param[out] *pDst points to the location where the output result is written.  
 * @param[in]  blockSize number of samples to process per call.  
 * @return none.  
 *  
 *  
 * <b>Scaling and Overflow Behavior:</b>  
 * \par  
 * The function is implemented using a 64-bit internal accumulator.  
 * Both coefficients and state variables are represented in 1.15 format and multiplications yield a 2.30 result.  
 * The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.  
 * There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.  
 * The accumulator is then shifted by <code>postShift</code> bits to truncate the result to 1.15 format by discarding the low 16 bits.  
 * Finally, the result is saturated to 1.15 format.  
 *  
 * \par  
 * Refer to the function <code>arm_biquad_cascade_df1_fast_q15()</code> for a faster but less precise implementation of this filter.  
 */ 
 
void arm_biquad_cascade_df1_q15( 
  const arm_biquad_casd_df1_inst_q15 * S, 
  q15_t * pSrc, 
  q15_t * pDst, 
  uint32_t blockSize) 
{ 
  q15_t *pIn = pSrc;                             /*  Source pointer                               */ 
  q15_t *pOut = pDst;                            /*  Destination pointer                          */ 
  q31_t in;                                      /*  Temporary variable to hold input value       */ 
  q31_t out;                                     /*  Temporary variable to hold output value      */ 
  q15_t b0;                                      /*  Temporary variable to hold bo value          */ 
  q31_t b1, a1;                                  /*  Filter coefficients                          */ 
  q31_t state_in, state_out;                     /*  Filter state variables                       */ 
  q63_t acc;                                     /*  Accumulator                                  */ 
  int32_t shift = (15 - (int32_t) S->postShift); /*  Post shift                                   */ 
  q15_t *pState = S->pState;                     /*  State pointer                                */ 
  q15_t *pCoeffs = S->pCoeffs;                   /*  Coefficient pointer                          */ 
  q31_t *pState_q31;                             /*  32-bit state pointer for SIMD implementation */ 
  uint32_t sample, stage = (uint32_t) S->numStages;     /*  Stage loop counter                           */ 
 
 
 
  do 
  { 
    /* Initialize state pointer of type q31 */ 
    pState_q31 = (q31_t *) (pState); 
 
    /* Read the b0 and 0 coefficients using SIMD  */ 
    b0 = *__SIMD32(pCoeffs)++; 
 
    /* Read the b1 and b2 coefficients using SIMD */ 
    b1 = *__SIMD32(pCoeffs)++; 
 
    /* Read the a1 and a2 coefficients using SIMD */ 
    a1 = *__SIMD32(pCoeffs)++; 
 
    /* Read the input state values from the state buffer:  x[n-1], x[n-2] */ 
    state_in = (q31_t) (*pState_q31++); 
 
    /* Read the output state values from the state buffer:  y[n-1], y[n-2] */ 
    state_out = (q31_t) (*pState_q31); 
 
    /* Apply loop unrolling and compute 2 output values simultaneously. */ 
    /*      The variable acc hold output values that are being computed:  
     *  
     *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]  
     *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]  
     */ 
    sample = blockSize >> 1u; 
 
    /* First part of the processing with loop unrolling.  Compute 2 outputs at a time.  
     ** a second loop below computes the remaining 1 sample. */ 
    while(sample > 0u) 
    { 
 
      /* Read the input */ 
      in = *__SIMD32(pIn)++; 
 
      /* out =  b0 * x[n] + 0 * 0 */ 
      out = (q31_t) b0 * ((q15_t) in); 
      /* acc +=  b1 * x[n-1] +  b2 * x[n-2] + out */ 
      acc = __SMLALD(b1, state_in, out); 
      /* acc +=  a1 * y[n-1] +  a2 * y[n-2] */ 
      acc = __SMLALD(a1, state_out, acc); 
 
      /* The result is converted from 3.29 to 1.31 if postShift = 1, and then saturation is applied */ 
      out = __SSAT((acc >> shift), 16); 
 
      /* Every time after the output is computed state should be updated. */ 
      /* The states should be updated as:  */ 
      /* Xn2 = Xn1    */ 
      /* Xn1 = Xn     */ 
      /* Yn2 = Yn1    */ 
      /* Yn1 = acc   */ 
      /* x[n-N], x[n-N-1] are packed together to make state_in of type q31 */ 
      /* y[n-N], y[n-N-1] are packed together to make state_out of type q31 */ 
      state_in = __PKHBT(in, state_in, 16); 
      state_out = __PKHBT(out, state_out, 16); 
 
      /* out =  b0 * x[n] + 0 * 0 */ 
      out = (q31_t) b0 * ((q15_t) (in >> 16)); 
      /* acc +=  b1 * x[n-1] +  b2 * x[n-2] + out */ 
      acc = __SMLALD(b1, state_in, out); 
      /* acc +=  a1 * y[n-1] + a2 * y[n-2] */ 
      acc = __SMLALD(a1, state_out, acc); 
 
      /* The result is converted from 3.29 to 1.31 if postShift = 1, and then saturation is applied */ 
      out = __SSAT((acc >> shift), 16); 
 
      /* Store the output in the destination buffer. */ 
      *__SIMD32(pOut)++ = __PKHBT(state_out, out, 16); 
 
      /* Every time after the output is computed state should be updated. */ 
      /* The states should be updated as:  */ 
      /* Xn2 = Xn1    */ 
      /* Xn1 = Xn     */ 
      /* Yn2 = Yn1    */ 
      /* Yn1 = acc   */ 
      /* x[n-N], x[n-N-1] are packed together to make state_in of type q31 */ 
      /* y[n-N], y[n-N-1] are packed together to make state_out of type q31 */ 
      state_in = __PKHBT(in >> 16, state_in, 16); 
      state_out = __PKHBT(out, state_out, 16); 
 
      /* Decrement the loop counter */ 
      sample--; 
 
    } 
 
    /* If the blockSize is not a multiple of 2, compute any remaining output samples here.  
     ** No loop unrolling is used. */ 
 
    if((blockSize & 0x1u) != 0u) 
    { 
      /* Read the input */ 
      in = *pIn++; 
 
      /* out =  b0 * x[n] + 0 * 0 */ 
      out = (q31_t) in *b0; 
      /* acc =  b1 * x[n-1] + b2 * x[n-2] + out */ 
      acc = __SMLALD(b1, state_in, out); 
      /* acc +=  a1 * y[n-1] + a2 * y[n-2] */ 
      acc = __SMLALD(a1, state_out, acc); 
 
      /* The result is converted from 3.29 to 1.31 if postShift = 1, and then saturation is applied */ 
      out = __SSAT((acc >> shift), 16); 
 
      /* Store the output in the destination buffer. */ 
      *pOut++ = (q15_t) out; 
 
      /* Every time after the output is computed state should be updated. */ 
      /* The states should be updated as:  */ 
      /* Xn2 = Xn1    */ 
      /* Xn1 = Xn     */ 
      /* Yn2 = Yn1    */ 
      /* Yn1 = acc   */ 
      /* x[n-N], x[n-N-1] are packed together to make state_in of type q31 */ 
      /* y[n-N], y[n-N-1] are packed together to make state_out of type q31 */ 
      state_in = __PKHBT(in, state_in, 16); 
      state_out = __PKHBT(out, state_out, 16); 
 
    } 
 
    /*  The first stage goes from the input wire to the output wire.  */ 
    /*  Subsequent numStages occur in-place in the output wire  */ 
    pIn = pDst; 
 
    /* Reset the output pointer */ 
    pOut = pDst; 
 
    /*  Store the updated state variables back into the state array */ 
    *__SIMD32(pState)++ = __PKHBT(state_in, (state_in >> 16), 16); 
    *__SIMD32(pState)++ = __PKHBT(state_out, (state_out >> 16), 16); 
 
    /* Decrement the loop counter */ 
    stage--; 
 
  } while(stage > 0u); 
} 
 
 
/**  
 * @} end of BiquadCascadeDF1 group  
 */ 
