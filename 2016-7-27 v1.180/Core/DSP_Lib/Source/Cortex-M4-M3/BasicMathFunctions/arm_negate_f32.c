/* ----------------------------------------------------------------------  
* Copyright (C) 2010 ARM Limited. All rights reserved.  
*  
* $Date:        29. November 2010  
* $Revision: 	V1.0.3  
*  
* Project: 	    CMSIS DSP Library  
* Title:		arm_negate_f32.c  
*  
* Description:	Negates floating-point vectors.  
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
* Version 0.0.7  2010/06/10   
*    Misra-C changes done  
* ---------------------------------------------------------------------------- */ 
 
#include "arm_math.h" 
 
/**  
 * @ingroup groupMath  
 */ 
 
/**  
 * @defgroup negate Vector Negate  
 *  
 * Negates the elements of a vector.  
 *  
 * <pre>  
 *     pDst[n] = -pSrc[n],   0 <= n < blockSize.  
 * </pre>  
 */ 
 
/**  
 * @addtogroup negate  
 * @{  
 */ 
 
/**  
 * @brief  Negates the elements of a floating-point vector.  
 * @param  *pSrc points to the input vector  
 * @param  *pDst points to the output vector  
 * @param  blockSize number of samples in the vector  
 * @return none.  
 */ 
 
void arm_negate_f32( 
  float32_t * pSrc, 
  float32_t * pDst, 
  uint32_t blockSize) 
{ 
  uint32_t blkCnt;                               /* loop counter */ 
 
  /*loop Unrolling */ 
  blkCnt = blockSize >> 2u; 
 
  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.  
   ** a second loop below computes the remaining 1 to 3 samples. */ 
  while(blkCnt > 0u) 
  { 
    /* C = ~A */ 
    /* Negate and then store the results in the destination buffer. */ 
    *pDst++ = -*pSrc++; 
    *pDst++ = -*pSrc++; 
    *pDst++ = -*pSrc++; 
    *pDst++ = -*pSrc++; 
 
    /* Decrement the loop counter */ 
    blkCnt--; 
  } 
 
  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.  
   ** No loop unrolling is used. */ 
  blkCnt = blockSize % 0x4u; 
 
  while(blkCnt > 0u) 
  { 
    /* C = ~A */ 
    /* Negate and then store the results in the destination buffer. */ 
    *pDst++ = -*pSrc++; 
 
    /* Decrement the loop counter */ 
    blkCnt--; 
  } 
} 
 
/**  
 * @} end of negate group  
 */ 
