/*
 *      MSACM library
 *
 *      Copyright 1998  Patrik Stridvall
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "mmsystem.h"
#define NOBITMAP
#include "mmreg.h"
#include "msacm.h"
#ifdef _MSCVER
#include "msacmdrv.h"
#else
#include "windows/msacmdrv.h"
#endif
#include "wineacm16.h"
#include "wine/debug.h"
#include "wine/winbase16.h"
#include "wownt32.h"

WINE_DEFAULT_DEBUG_CHANNEL(msacm);
void WINAPI K32WOWHandle16DestroyHint(HANDLE handle, WOW_HANDLE_TYPE type);
#define WOW_TYPE_HANDLE (WOW_TYPE_FULLHWND + 1)
#define WOW_TYPE_HAS WOW_TYPE_HANDLE
#define HACMSTREAM_16(h32)		WOWHandle16(h32, WOW_TYPE_HAS)
#define HACMSTREAM_32(h16)		((HACMSTREAM)WOWHandle32(h16, WOW_TYPE_HAS))
/**************************************************************************
 *		DllEntryPoint (MSACM.3)
 *
 * MSACM DLL entry point
 *
 */
BOOL WINAPI DllEntryPoint16(DWORD fdwReason, HINSTANCE16 hinstDLL, WORD ds,
                            WORD wHeapSize, DWORD dwReserved1, WORD wReserved2)
{
    return TRUE;
}

/***********************************************************************
 *		acmGetVersion (MSACM.7)
 */
DWORD WINAPI acmGetVersion16(void)
{
  return acmGetVersion();
}

/***********************************************************************
 *		acmMetrics (MSACM.8)
 */

MMRESULT16 WINAPI acmMetrics16(
  HACMOBJ16 hao, UINT16 uMetric, LPVOID pMetric)
{
  FIXME("(0x%04x, %d, %p): semi-stub\n", hao, uMetric, pMetric);

  if(!hao) return acmMetrics(0, uMetric, pMetric);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 *		acmDriverEnum (MSACM.10)
 */
MMRESULT16 WINAPI acmDriverEnum16(
  ACMDRIVERENUMCB16 fnCallback, DWORD dwInstance, DWORD fdwEnum)
{
  FIXME("(%p, %d, %d): stub\n",
    fnCallback, dwInstance, fdwEnum
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverDetails (MSACM.11)
 */

MMRESULT16 WINAPI acmDriverDetails16(
  HACMDRIVERID16 hadid, LPACMDRIVERDETAILS16 padd, DWORD fdwDetails)
{
  FIXME("(0x%04x, %p, %d): stub\n", hadid, padd, fdwDetails);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverAdd (MSACM.12)
 */
MMRESULT16 WINAPI acmDriverAdd16(
  LPHACMDRIVERID16 phadid, HINSTANCE16 hinstModule,
  LPARAM lParam, DWORD dwPriority, DWORD fdwAdd)
{
  FIXME("(%p, 0x%04x, %ld, %d, %d): stub\n",
    phadid, hinstModule, lParam, dwPriority, fdwAdd
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverRemove (MSACM.13)
 */
MMRESULT16 WINAPI acmDriverRemove16(
  HACMDRIVERID16 hadid, DWORD fdwRemove)
{
  FIXME("(0x%04x, %d): stub\n", hadid, fdwRemove);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverOpen (MSACM.14)
 */
MMRESULT16 WINAPI acmDriverOpen16(
  LPHACMDRIVER16 phad, HACMDRIVERID16 hadid, DWORD fdwOpen)
{
  FIXME("(%p, 0x%04x, %d): stub\n", phad, hadid, fdwOpen);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverClose (MSACM.15)
 */
MMRESULT16 WINAPI acmDriverClose16(
  HACMDRIVER16 had, DWORD fdwClose)
{
  FIXME("(0x%04x, %d): stub\n", had, fdwClose);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverMessage (MSACM.16)
 */
LRESULT WINAPI acmDriverMessage16(
  HACMDRIVER16 had, UINT16 uMsg, LPARAM lParam1, LPARAM lParam2)
{
  FIXME("(0x%04x, %d, %ld, %ld): stub\n",
    had, uMsg, lParam1, lParam2
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return 0;
}

/***********************************************************************
 * 		acmDriverID (MSACM.17)
 */
MMRESULT16 WINAPI acmDriverID16(
  HACMOBJ16 hao, LPHACMDRIVERID16 phadid, DWORD fdwDriverID)
{
  FIXME("(0x%04x, %p, %d): stub\n", hao, phadid, fdwDriverID);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmDriverPriority (MSACM.18)
 */
MMRESULT16 WINAPI acmDriverPriority16(
 HACMDRIVERID16 hadid, DWORD dwPriority, DWORD fdwPriority)
{
  FIXME("(0x%04x, %d, %d): stub\n",
    hadid, dwPriority, fdwPriority
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFormatTagDetails (MSACM.30)
 */
MMRESULT16 WINAPI acmFormatTagDetails16(
  HACMDRIVER16 had, LPACMFORMATTAGDETAILS16 paftd, DWORD fdwDetails)
{
  return acmFormatTagDetailsA(had, paftd, fdwDetails);
}

/***********************************************************************
 * 		acmFormatTagEnum (MSACM.31)
 */
MMRESULT16 WINAPI acmFormatTagEnum16(
  HACMDRIVER16 had, LPACMFORMATTAGDETAILS16 paftd,
  ACMFORMATTAGENUMCB16 fnCallback, DWORD dwInstance, DWORD fdwEnum)
{
  FIXME("(0x%04x, %p, %p, %d, %d): stub\n",
    had, paftd, fnCallback, dwInstance, fdwEnum
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFormatChoose (MSACM.40)
 */
MMRESULT16 WINAPI acmFormatChoose16(
  LPACMFORMATCHOOSE16 pafmtc)
{
  FIXME("(%p): stub\n", pafmtc);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFormatDetails (MSACM.41)
 */
MMRESULT16 WINAPI acmFormatDetails16(
  HACMDRIVER16 had, LPACMFORMATDETAILS16 pafd, DWORD fdwDetails)
{
  MMRESULT ret;
  ACMFORMATDETAILSA afd;
  afd.cbStruct = pafd->cbStruct;
  afd.dwFormatIndex = pafd->dwFormatIndex;
  afd.dwFormatTag = pafd->dwFormatTag;
  afd.fdwSupport = pafd->fdwSupport;
  afd.pwfx = MapSL(pafd->pwfx);
  afd.cbwfx = pafd->cbwfx;
  memcpy(&afd.szFormat, &pafd->szFormat, pafd->cbStruct - (6*4));
  ret = acmFormatDetailsA(had, &afd, fdwDetails);
  pafd->dwFormatIndex = afd.dwFormatIndex;
  pafd->dwFormatTag = afd.dwFormatTag;
  pafd->fdwSupport = afd.fdwSupport;
  pafd->cbwfx = afd.cbwfx;
  memcpy(&pafd->szFormat, &afd.szFormat, pafd->cbStruct - (6*4));
  return ret; 
}

/***********************************************************************
 * 		acmFormatEnum (MSACM.42)
 */
MMRESULT16 WINAPI acmFormatEnum16(
  HACMDRIVER16 had, LPACMFORMATDETAILS16 pafd,
  ACMFORMATENUMCB16 fnCallback, DWORD dwInstance, DWORD fdwEnum)
{
  FIXME("(0x%04x, %p, %p, %d, %d): stub\n",
    had, pafd, fnCallback, dwInstance, fdwEnum
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFormatSuggest (MSACM.45)
 */
MMRESULT16 WINAPI acmFormatSuggest16(
  HACMDRIVER16 had, LPWAVEFORMATEX pwfxSrc,
  LPWAVEFORMATEX pwfxDst, DWORD cbwfxDst, DWORD fdwSuggest)
{
  return acmFormatSuggest(had, pwfxSrc, pwfxDst, cbwfxDst, fdwSuggest);
}

/***********************************************************************
 * 		acmFilterTagDetails (MSACM.50)
 */
MMRESULT16 WINAPI acmFilterTagDetails16(
  HACMDRIVER16 had, LPACMFILTERTAGDETAILS16 paftd, DWORD fdwDetails)
{
  FIXME("(0x%04x, %p, %d): stub\n", had, paftd, fdwDetails);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFilterTagEnum (MSACM.51)
 */
MMRESULT16 WINAPI acmFilterTagEnum16(
  HACMDRIVER16 had, LPACMFILTERTAGDETAILS16 paftd,
  ACMFILTERTAGENUMCB16 fnCallback, DWORD dwInstance, DWORD fdwEnum)
{
  FIXME("(0x%04x, %p, %p, %d, %d): stub\n",
    had, paftd, fnCallback, dwInstance, fdwEnum
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFilterChoose (MSACM.60)
 */
MMRESULT16 WINAPI acmFilterChoose16(
  LPACMFILTERCHOOSE16 pafltrc)
{
  FIXME("(%p): stub\n", pafltrc);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFilterDetails (MSACM.61)
 */
MMRESULT16 WINAPI acmFilterDetails16(
  HACMDRIVER16 had, LPACMFILTERDETAILS16 pafd, DWORD fdwDetails)
{
  FIXME("(0x%04x, %p, %d): stub\n", had, pafd, fdwDetails);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmFilterEnum (MSACM.62)
 */
MMRESULT16 WINAPI acmFilterEnum16(
  HACMDRIVER16 had, LPACMFILTERDETAILS16 pafd,
  ACMFILTERENUMCB16 fnCallback, DWORD dwInstance, DWORD fdwEnum)
{
  FIXME("(0x%04x, %p, %p, %d, %d): stub\n",
    had, pafd, fnCallback, dwInstance, fdwEnum
  );
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmStreamOpen (MSACM.70)
 */
MMRESULT16 WINAPI acmStreamOpen16(
  LPHACMSTREAM16 phas, HACMDRIVER16 had,
  LPWAVEFORMATEX pwfxSrc, LPWAVEFORMATEX pwfxDst,
  LPWAVEFILTER pwfltr, DWORD dwCallback,
  DWORD dwInstance, DWORD fdwOpen)
{
  HACMSTREAM strm;
  MMRESULT ret;
  if (fdwOpen & ACM_STREAMOPENF_ASYNC)
  {
    FIXME("ASYNC not supported\n");
    return MMSYSERR_ERROR;
  }
  ret = acmStreamOpen(&strm, had, pwfxSrc, pwfxDst, pwfltr, dwCallback, dwInstance, fdwOpen);
  *phas = HACMSTREAM_16(strm);
  return ret;
}

/***********************************************************************
 * 		acmStreamClose (MSACM.71)
 */
MMRESULT16 WINAPI acmStreamClose16(
  HACMSTREAM16 has, DWORD fdwClose)
{
  MMRESULT ret;
  HACMSTREAM has32 = HACMSTREAM_32(has);
  ret = acmStreamClose(has32, fdwClose);
  if (!ret)
    K32WOWHandle16DestroyHint(has32, WOW_TYPE_HAS);
  return ret;
}

/***********************************************************************
 * 		acmStreamSize (MSACM.72)
 */
MMRESULT16 WINAPI acmStreamSize16(
  HACMSTREAM16 has, DWORD cbInput,
  LPDWORD pdwOutputBytes, DWORD fdwSize)
{
    return acmStreamSize(HACMSTREAM_32(has), cbInput, pdwOutputBytes, fdwSize);
}

/***********************************************************************
 * 		acmStreamConvert (MSACM.75)
 */
MMRESULT16 WINAPI acmStreamConvert16(
  HACMSTREAM16 has, LPACMSTREAMHEADER16 pash, DWORD fdwConvert)
{
  MMRESULT ret;
  LPACMSTREAMHEADER pash32 = pash->dwReservedDriver[0];
  pash32->fdwStatus = pash->fdwStatus;
  pash32->dwUser = pash->dwUser;
  pash32->pbSrc = MapSL(pash->pbSrc);
  pash32->cbSrcLength = pash->cbSrcLength;
  pash32->dwSrcUser = pash->dwSrcUser;
  pash32->pbDst = MapSL(pash->pbDst);
  pash32->cbDstLength = pash->cbDstLength;
  pash32->dwDstUser = pash->dwDstUser;
  ret = acmStreamConvert(HACMSTREAM_32(has), pash32, fdwConvert);
  pash->fdwStatus = pash32->fdwStatus;
  pash->cbSrcLengthUsed = pash32->cbSrcLengthUsed;
  pash->cbDstLengthUsed = pash32->cbDstLengthUsed;
  return ret;
}

/***********************************************************************
 * 		acmStreamReset (MSACM.76)
 */
MMRESULT16 WINAPI acmStreamReset16(
  HACMSTREAM16 has, DWORD fdwReset)
{
  FIXME("(0x%04x, %d): stub\n", has, fdwReset);
  SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
  return MMSYSERR_ERROR;
}

/***********************************************************************
 * 		acmStreamPrepareHeader (MSACM.77)
 */
MMRESULT16 WINAPI acmStreamPrepareHeader16(
  HACMSTREAM16 has, LPACMSTREAMHEADER16 pash, DWORD fdwPrepare)
{
  MMRESULT ret;
  LPACMSTREAMHEADER pash32 = HeapAlloc(GetProcessHeap(), 0, sizeof(ACMSTREAMHEADER));
  pash32->cbStruct = sizeof(ACMSTREAMHEADER);
  pash32->fdwStatus = pash->fdwStatus;
  pash32->dwUser = pash->dwUser;
  pash32->pbSrc = MapSL(pash->pbSrc);
  pash32->cbSrcLength = pash->cbSrcLength;
  pash32->dwSrcUser = pash->dwSrcUser;
  pash32->pbDst = MapSL(pash->pbDst);
  pash32->cbDstLength = pash->cbDstLength;
  pash32->dwDstUser = pash->dwDstUser;
  ret = acmStreamPrepareHeader(HACMSTREAM_32(has), pash32, fdwPrepare);
  if (!ret)
  {
    pash->dwReservedDriver[0] = pash32;
    pash->fdwStatus = pash32->fdwStatus;
  }
  else
    HeapFree(GetProcessHeap(), 0, pash32);
  return ret;
}

/***********************************************************************
 * 		acmStreamUnprepareHeader (MSACM.78)
 */
MMRESULT16 WINAPI acmStreamUnprepareHeader16(
  HACMSTREAM16 has, LPACMSTREAMHEADER16 pash, DWORD fdwUnprepare)
{
  MMRESULT ret;
  LPACMSTREAMHEADER pash32 = pash->dwReservedDriver[0];
  ret = acmStreamUnprepareHeader(HACMSTREAM_32(has), pash32, fdwUnprepare);
  if (!ret)
    HeapFree(GetProcessHeap(), 0, pash32);
  return ret;
}

/***********************************************************************
 *		ACMAPPLICATIONEXIT (MSACM.150)
 * FIXME
 *   No documentation found.
 */

/***********************************************************************
 *		ACMHUGEPAGELOCK (MSACM.175)
 *FIXME
 *   No documentation found.
 */

/***********************************************************************
 *		ACMHUGEPAGEUNLOCK (MSACM.176)
 * FIXME
 *   No documentation found.
 */

/***********************************************************************
 *		ACMOPENCONVERSION (MSACM.200)
 * FIXME
 *   No documentation found.
 */

/***********************************************************************
 *		ACMCLOSECONVERSION (MSACM.201)
 * FIXME
 *   No documentation found.
 */

/***********************************************************************
 *		ACMCONVERT (MSACM.202)
 * FIXME
 *   No documentation found.
 */

/***********************************************************************
 *		ACMCHOOSEFORMAT (MSACM.203)
 * FIXME
 *   No documentation found.
 */
