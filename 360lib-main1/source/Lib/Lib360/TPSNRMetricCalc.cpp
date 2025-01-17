/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2018, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     TPSNRMetricCalc.cpp
    \brief    PSNRMetric class
*/

#include "TPSNRMetricCalc.h"

#if SVIDEO_SPSNR_NN
TPSNRMetric::TPSNRMetric()
{
  m_dPSNR[0] = m_dPSNR[1] = m_dPSNR[2] = 0;
}

TPSNRMetric::~TPSNRMetric()
{
}

Void TPSNRMetric::setOutputBitDepth(const BitDepths& iOutputBitDepth)
{
  for (ChannelType ch = ChannelType::LUMA; ch < ChannelType::NUM; ch++)
  {
    m_outputBitDepth[ch] = iOutputBitDepth[ch];
  }
}

Void TPSNRMetric::setReferenceBitDepth(const BitDepths& iReferenceBitDepth)
{
  for (ChannelType ch = ChannelType::LUMA; ch < ChannelType::NUM; ch++)
  {
    m_referenceBitDepth[ch] = iReferenceBitDepth[ch];
  }
}

Void TPSNRMetric::xCalculatePSNR( PelUnitBuf* pcOrgPicYuv, PelUnitBuf* pcPicD )
{
  Int iBitDepthForPSNRCalc[MAX_NUM_CHANNEL_TYPE];
  Int iReferenceBitShift[MAX_NUM_CHANNEL_TYPE];
  Int iOutputBitShift[MAX_NUM_CHANNEL_TYPE];
  iBitDepthForPSNRCalc[Int(ChannelType::LUMA)] = std::max(m_outputBitDepth[ChannelType::LUMA], m_referenceBitDepth[ChannelType::LUMA]);
  iBitDepthForPSNRCalc[Int(ChannelType::CHROMA)] = std::max(m_outputBitDepth[ChannelType::CHROMA], m_referenceBitDepth[ChannelType::CHROMA]);
  iReferenceBitShift[Int(ChannelType::LUMA)] = iBitDepthForPSNRCalc[Int(ChannelType::LUMA)] - m_referenceBitDepth[ChannelType::LUMA];
  iReferenceBitShift[Int(ChannelType::CHROMA)] = iBitDepthForPSNRCalc[Int(ChannelType::CHROMA)] - m_referenceBitDepth[ChannelType::CHROMA];
  iOutputBitShift[Int(ChannelType::LUMA)] = iBitDepthForPSNRCalc[Int(ChannelType::LUMA)] - m_outputBitDepth[ChannelType::LUMA];
  iOutputBitShift[Int(ChannelType::CHROMA)] = iBitDepthForPSNRCalc[Int(ChannelType::CHROMA)] - m_outputBitDepth[ChannelType::CHROMA];

  memset(m_dPSNR, 0, sizeof(Double)*3);

  PelUnitBuf &picd=*pcPicD;
  //===== calculate PSNR =====

    for(Int chan=0; chan<getNumberValidComponents(pcPicD->chromaFormat); chan++)
    {
      const ComponentID ch=ComponentID(chan);
      const Pel*  pOrg       = pcOrgPicYuv->get(ch).bufAt(0, 0);
      const Int   iOrgStride = (Int)pcOrgPicYuv->get(ch).stride;
      const Pel*  pRec       = picd.get(ch).bufAt(0, 0);
      const Int   iRecStride = (Int)picd.get(ch).stride;
      const Int   iWidth  = pcPicD->get(ch).width ;
      const Int   iHeight = pcPicD->get(ch).height;
      Int   iSize   = iWidth*iHeight;

      Double SSDpsnr=0;
      for(Int y = 0; y < iHeight; y++ )
      {
        for(Int x = 0; x < iWidth; x++ )
        {
          Intermediate_Int iDiff = (Intermediate_Int)( (pOrg[x]<<iReferenceBitShift[Int(toChannelType(ch))]) - (pRec[x]<<iOutputBitShift[Int(toChannelType(ch))]) );
          SSDpsnr += iDiff * iDiff;
        }
        pOrg += iOrgStride;
        pRec += iRecStride;
      }
      const Int maxval = 255<<(iBitDepthForPSNRCalc[Int(toChannelType(ch))]-8) ;
      const Double fRefValue = (Double) maxval * maxval * iSize;
      m_dPSNR[ch] = ( SSDpsnr ? 10.0 * log10( fRefValue / (Double)SSDpsnr ) : 999.99 );
    }
}

#endif
