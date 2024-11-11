/*
 *	OLE2DISP library
 *
 *	Copyright 1995	Martin von Loewis
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

#include "config.h"

#include <stdarg.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "ole2.h"
#include "oleauto.h"
#include "winerror.h"

#include "ole2disp.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

#define E_OUTOFMEMORY16 MAKE_SCODE(SEVERITY_ERROR, FACILITY_NULL, 2)
#define E_INVALIDARG16  MAKE_SCODE(SEVERITY_ERROR, FACILITY_NULL, 3)

#include <pshpack1.h>
/* BSTR structure */
typedef struct
{
    ULONG clSize;
    byte abData[1];
} BYTE_BLOB16;
#include <poppack.h>
static SEGPTR safearray_alloc(ULONG size)
{
    HANDLE16 h;
    return MAKESEGPTR(WOWGlobalAlloc16(GPTR, size), 0);
}

static void safearray_free(SEGPTR ptr)
{
    WOWGlobalUnlockFree16(ptr);
}

static SEGPTR safearray_realloc(SEGPTR ptr, ULONG size)
{
    return MAKESEGPTR(GlobalReAlloc16(HIWORD(ptr), size, GMEM_ZEROINIT), 0);
}

static ULONG safearray_getcellcount(const SAFEARRAY16 *sa)
{
    const SAFEARRAYBOUND16 *sab = sa->rgsabound;
    USHORT count = sa->cDims;
    ULONG cells = 1;

    while (count--)
    {
        if (!sab->cElements)
            return 0;
        cells *= sab->cElements;
        sab++;
    }

    return cells;
}

static HRESULT safearray_lock(SAFEARRAY16 *sa)
{
    if (sa->cLocks == 0xffff)
        return E_UNEXPECTED;

    sa->cLocks++;
    return S_OK;
}

/* pad safearray so no element spans a 64K boundry */
static ULONG safearray_size(ULONG cells, WORD cbcell)
{
    DWORD size = cells * cbcell;
    int pad = 0x10000 % cbcell;
    return (pad * (size / (0x10000 - pad))) + size;
}

static SEGPTR safearray_ptrofindex(SEGPTR array, ULONG index, WORD cbcell)

{
    ULONG pos = safearray_size(index, cbcell) + LOWORD(array);
    return MAKELONG(pos & 0xffff, ((pos >> 16) << 3) + HIWORD(array));
}

/* Free data items in an array */
static HRESULT SAFEARRAY_DestroyData(SAFEARRAY16 *psa, ULONG ulStartCell)
{
  if (psa->pvData)
  {
    ULONG ulCellCount = safearray_getcellcount(psa);

    if (ulStartCell > ulCellCount) {
      FIXME("unexpted ulcellcount %d, start %d\n",ulCellCount,ulStartCell);
      return E_UNEXPECTED;
    }

    ulCellCount -= ulStartCell;

    if (psa->fFeatures & (FADF_UNKNOWN|FADF_DISPATCH))
    {
      while(ulCellCount--)
      {
        SEGPTR *lpUnknown = (SEGPTR *)MapSL(safearray_ptrofindex(psa->pvData, ulStartCell, psa->cbElements));
        if (*lpUnknown)
          IUnknown16_Release(*lpUnknown);
        ulStartCell++;
      }
    }
    else if (psa->fFeatures & FADF_BSTR)
    {
      while(ulCellCount--)
      {
        SEGPTR lpBstr = safearray_ptrofindex(psa->pvData, ulStartCell, psa->cbElements);
        SysFreeString16(*(SEGPTR *)MapSL(lpBstr));
        ulStartCell++;
      }
    }
    else if (psa->fFeatures & FADF_VARIANT)
    {

      while(ulCellCount--)
      {
        SEGPTR lpVariant = safearray_ptrofindex(psa->pvData, ulStartCell, psa->cbElements);
        HRESULT hRet = VariantClear16((VARIANT16 *)MapSL(lpVariant));

        if (FAILED(hRet)) FIXME("VariantClear of element failed!\n");
        ulStartCell++;
      }
    }
  }
  return S_OK;
}

/******************************************************************************
 *    SafeArrayGetDim [OLE2DISP.17]
 */
USHORT WINAPI SafeArrayGetDim16(SAFEARRAY16 *sa)
{
    TRACE("(%p)\n", sa);
    return sa->cDims;
}

/******************************************************************************
 *    SafeArrayGetElemsize [OLE2DISP.18]
 */
USHORT WINAPI SafeArrayGetElemsize16(SAFEARRAY16 *sa)
{
    TRACE("(%p)\n", sa);
    return sa->cbElements;
}

/******************************************************************************
 *    SafeArrayGetUBound [OLE2DISP.19]
 */
HRESULT WINAPI SafeArrayGetUBound16(SAFEARRAY16 *sa, UINT16 dim, LONG *ubound)
{
    TRACE("(%p, %u, %p)\n", sa, dim, ubound);

    if (!sa)
        return E_INVALIDARG16;

    if (!dim || dim > sa->cDims)
        return DISP_E_BADINDEX;

    *ubound = sa->rgsabound[sa->cDims - dim].lLbound + sa->rgsabound[sa->cDims - dim].cElements - 1;

    return S_OK;
}

/******************************************************************************
 *    SafeArrayGetLBound [OLE2DISP.20]
 */
HRESULT WINAPI SafeArrayGetLBound16(SAFEARRAY16 *sa, UINT16 dim, LONG *lbound)
{
    TRACE("(%p, %u, %p)\n", sa, dim, lbound);

    if (!sa)
        return E_INVALIDARG16;

    if (!dim || dim > sa->cDims)
        return DISP_E_BADINDEX;

    *lbound = sa->rgsabound[sa->cDims - dim].lLbound;

    return S_OK;
}

/******************************************************************************
 *    SafeArrayLock [OLE2DISP.21]
 */
HRESULT WINAPI SafeArrayLock16(SAFEARRAY16 *sa)
{
    TRACE("(%p)\n", sa);

    if (!sa)
        return E_INVALIDARG16;

    return safearray_lock(sa);
}

/******************************************************************************
 *    SafeArrayUnlock [OLE2DISP.22]
 */
HRESULT WINAPI SafeArrayUnlock16(SAFEARRAY16 *sa)
{
    TRACE("(%p)\n", sa);

    if (!sa)
        return E_INVALIDARG16;

    if (sa->cLocks == 0)
        return E_UNEXPECTED;

    sa->cLocks--;
    return S_OK;
}

/******************************************************************************
 *    SafeArrayAccessData [OLE2DISP.23]
 */
HRESULT WINAPI SafeArrayAccessData16(SAFEARRAY16 *sa, SEGPTR *data)
{
    HRESULT hr;

    TRACE("(%p, %p)\n", sa, data);

    /* arguments are not tested, it crashes if any of them is NULL */

    hr = safearray_lock(sa);
    if (FAILED(hr))
        return hr;

    *data = sa->pvData;
    return S_OK;
}

/******************************************************************************
 *    SafeArrayUnaccessData [OLE2DISP.24]
 */
HRESULT WINAPI SafeArrayUnaccessData16(SAFEARRAY16 *sa)
{
    TRACE("(%p)\n", sa);
    return SafeArrayUnlock16(sa);
}

/******************************************************************************
 *    SafeArrayAllocDescriptor [OLE2DISP.38]
 */
HRESULT WINAPI SafeArrayAllocDescriptor16(UINT16 dims, SEGPTR *ret)
{
    SAFEARRAY16 *sa;
    ULONG size;

    TRACE("%u, %p\n", dims, ret);

    if (!dims)
        return E_INVALIDARG16;

    size = sizeof(SAFEARRAY16) + sizeof(SAFEARRAYBOUND16) * (dims - 1);
    *ret = safearray_alloc(size);
    if (!*ret)
        return E_OUTOFMEMORY16;

    sa = MapSL(*ret);
    sa->cDims = dims;
    return S_OK;
}

/******************************************************************************
 *    SafeArrayAllocData [OLE2DISP.39]
 */
HRESULT WINAPI SafeArrayAllocData16(SAFEARRAY16 *sa)
{
    ULONG size;

    TRACE("%p\n", sa);

    if (!sa)
        return E_INVALIDARG16;

    size = safearray_getcellcount(sa);
    sa->pvData = safearray_alloc(safearray_size(size, sa->cbElements));
    return sa->pvData ? S_OK : E_OUTOFMEMORY16;
}

/******************************************************************************
 *    SafeArrayDestroyDescriptor [OLE2DISP.40]
 */
HRESULT WINAPI SafeArrayDestroyDescriptor16(SEGPTR s)
{
    TRACE("0x%08x\n", s);

    if (s)
    {
        SAFEARRAY16 *sa = MapSL(s);

        if (sa->cLocks)
            return DISP_E_ARRAYISLOCKED;

        safearray_free(s);
    }

    return S_OK;
}

/******************************************************************************
 *    SafeArrayDestroyData [OLE2DISP.41]
 */
HRESULT WINAPI SafeArrayDestroyData16(SAFEARRAY16 *sa)
{
    TRACE("%p\n", sa);

    if (!sa)
        return S_OK;

    if (sa->cLocks)
        return DISP_E_ARRAYISLOCKED;

    HRESULT hr = SAFEARRAY_DestroyData(sa, 0);
    if (!hr)
        return hr;

    if (!(sa->fFeatures & FADF_STATIC))
        safearray_free(sa->pvData);

    return S_OK;
}

HRESULT WINAPI SafeArrayCopy16(SAFEARRAY16 *sa, SAFEARRAY16 **ppsaout)
{
    FIXME("(%p,%p) stub!\n", sa, ppsaout);
    return E_INVALIDARG;
}

HRESULT WINAPI SafeArrayDestroy16(SEGPTR spsa)
{
  SAFEARRAY16 *psa = (SAFEARRAY16*)MapSL(spsa);
  TRACE("(%08x)\n", spsa);

  if(!psa)
    return S_OK;

  if(psa->cLocks > 0)
    return DISP_E_ARRAYISLOCKED;

  /* Native doesn't check to see if the free succeeds */
  SafeArrayDestroyData16(psa);
  SafeArrayDestroyDescriptor16(spsa);
  return S_OK;
}

/* Get the size of a supported VT type (0 means unsupported) */
static DWORD SAFEARRAY_GetVTSize(VARTYPE vt)
{
  switch (vt)
  {
    case VT_I1:
    case VT_UI1:      return sizeof(BYTE);
    case VT_BOOL:
    case VT_I2:
    case VT_UI2:      return sizeof(SHORT);
    case VT_I4:
    case VT_UI4:
    case VT_R4:
    case VT_ERROR:    return sizeof(LONG);
    case VT_R8:
    case VT_I8:
    case VT_UI8:      return sizeof(LONG64);
    case VT_INT:
    case VT_UINT:     return sizeof(INT);
    case VT_INT_PTR:
    case VT_UINT_PTR: return sizeof(UINT_PTR);
    case VT_CY:       return sizeof(CY);
    case VT_DATE:     return sizeof(DATE);
    case VT_BSTR:     return sizeof(BSTR);
    case VT_DISPATCH: return sizeof(LPDISPATCH);
    case VT_VARIANT:  return sizeof(VARIANT);
    case VT_UNKNOWN:  return sizeof(LPUNKNOWN);
    case VT_DECIMAL:  return sizeof(DECIMAL);
    /* Note: Return a non-zero size to indicate vt is valid. The actual size
     * of a UDT is taken from the result of IRecordInfo_GetSize().
     */
    case VT_RECORD:   return 32;
  }
  return 0;
}
/*************************************************************************
 *             SafeArrayAllocDescriptorEx (OLEAUT32.41)
 *
 * Allocate and initialise a descriptor for a SafeArray of a given type.
 *
 * PARAMS
 *  vt      [I] The type of items to store in the array
 *  cDims   [I] Number of dimensions of the array
 *  ppsaOut [O] Destination for new descriptor
 *
 * RETURNS
 *  Success: S_OK. ppsaOut is filled with a newly allocated descriptor.
 *  Failure: An HRESULT error code indicating the error.
 *
 * NOTES
 *  - This function does not check that vt is an allowed VARTYPE.
 *  - Unlike SafeArrayAllocDescriptor(), vt is associated with the array.
 *  See SafeArray.
 */
HRESULT WINAPI SafeArrayAllocDescriptorEx16(VARTYPE vt, UINT cDims, SEGPTR *ppsaOut)
{
  ULONG cbElements;
  HRESULT hRet;

  TRACE("(%d->%s,%d,%p)\n", vt, debugstr_vt(vt), cDims, ppsaOut);

  cbElements = SAFEARRAY_GetVTSize(vt);
  if (!cbElements)
    WARN("Creating a descriptor with an invalid VARTYPE!\n");

  hRet = SafeArrayAllocDescriptor16(cDims, ppsaOut);

  if (SUCCEEDED(hRet))
  {
    ((SAFEARRAY16*)MapSL(*ppsaOut))->cbElements = cbElements;
  }
  return hRet;
}
/* Create an array */
static SEGPTR SAFEARRAY_Create(VARTYPE vt, UINT cDims, const SAFEARRAYBOUND16 *rgsabound, ULONG ulSize)
{
    SEGPTR psa = NULL;
  unsigned int i;

  if (!rgsabound)
    return NULL;

  if (SUCCEEDED(SafeArrayAllocDescriptorEx16(vt, cDims, &psa)))
  {
    switch (vt)
    {
      case VT_BSTR:     ((SAFEARRAY16*)MapSL(psa))->fFeatures |= FADF_BSTR; break;
      case VT_UNKNOWN:  ((SAFEARRAY16*)MapSL(psa))->fFeatures |= FADF_UNKNOWN; break;
      case VT_DISPATCH: ((SAFEARRAY16*)MapSL(psa))->fFeatures |= FADF_DISPATCH; break;
      case VT_VARIANT:  ((SAFEARRAY16*)MapSL(psa))->fFeatures |= FADF_VARIANT; break;
    }

    for (i = 0; i < cDims; i++)
      memcpy(((SAFEARRAY16*)MapSL(psa))->rgsabound + i, rgsabound + cDims - 1 - i, sizeof(SAFEARRAYBOUND16));

    if (ulSize)
        ((SAFEARRAY16*)MapSL(psa))->cbElements = ulSize;

    if (!((SAFEARRAY16*)MapSL(psa))->cbElements || FAILED(SafeArrayAllocData16(((SAFEARRAY16*)MapSL(psa)))))
    {
      SafeArrayDestroyDescriptor16(psa);
      psa = NULL;
    }
  }
  return psa;
}

SEGPTR WINAPI SafeArrayCreate16(VARTYPE vt, UINT16 cDims, SAFEARRAYBOUND16 *rgsabound)
{
    TRACE("(%d->%s,%d,%p)\n", vt, debugstr_vt(vt), cDims, rgsabound);

    if (vt == VT_RECORD)
        return 0;

    return SAFEARRAY_Create(vt, cDims, rgsabound, 0);
}

/************************************************************************
 *		SafeArrayRedim (OLEAUT32.40)
 *
 * Changes the characteristics of the last dimension of a SafeArray
 *
 * PARAMS
 *  psa      [I] Array to change
 *  psabound [I] New bound details for the last dimension
 *
 * RETURNS
 *  Success: S_OK. psa is updated to reflect the new bounds.
 *  Failure: An HRESULT error code indicating the error.
 *
 * NOTES
 * See SafeArray.
 */
HRESULT WINAPI SafeArrayRedim16(SAFEARRAY16 *psa, SAFEARRAYBOUND16 *psabound)
{
  SAFEARRAYBOUND16 *oldBounds;
  HRESULT hr;

  TRACE("(%p,%p)\n", psa, psabound);
  
  if (!psa || psa->fFeatures & FADF_FIXEDSIZE || !psabound)
    return E_INVALIDARG;

  if (psa->cLocks > 0)
    return DISP_E_ARRAYISLOCKED;

  hr = SafeArrayLock16(psa);
  if (FAILED(hr))
    return hr;

  oldBounds = psa->rgsabound;
  oldBounds->lLbound = psabound->lLbound;

  if (psabound->cElements != oldBounds->cElements)
  {
    if (psabound->cElements < oldBounds->cElements)
    {
      /* Shorten the final dimension. */
      ULONG ulStartCell = psabound->cElements *
                          (safearray_getcellcount(psa) / oldBounds->cElements);
      SAFEARRAY_DestroyData(psa, ulStartCell);
    }
    else
    {
      /* Lengthen the final dimension */
      ULONG ulOldSize, ulNewSize;
      PVOID pvNewData;

      ulOldSize = safearray_getcellcount(psa);
      if (ulOldSize)
        ulNewSize = (ulOldSize / oldBounds->cElements) * psabound->cElements;
      else {
	int oldelems = oldBounds->cElements;
	oldBounds->cElements = psabound->cElements;
        ulNewSize = safearray_getcellcount(psa);
	oldBounds->cElements = oldelems;
      }

      if (!(pvNewData = safearray_realloc(psa->pvData, safearray_size(ulNewSize, psa->cbElements))))
      {
        SafeArrayUnlock16(psa);
        return E_OUTOFMEMORY;
      }
      psa->pvData = pvNewData;
    }
    oldBounds->cElements = psabound->cElements;
  }

  SafeArrayUnlock16(psa);
  return S_OK;
}

/************************************************************************
 *		VectorFromBstr (OLEAUT32.@)
 *
 * Create a SafeArray Vector from the bytes of a BSTR.
 *
 * PARAMS
 *  bstr [I] String to get bytes from
 *  ppsa [O] Destination for the array
 *
 * RETURNS
 *  Success: S_OK. ppsa contains the strings bytes as a VT_UI1 array.
 *  Failure: An HRESULT error code indicating the error.
 *
 * NOTES
 * See SafeArray.
 */
HRESULT WINAPI VectorFromBstr16(SEGBSTR16 bstr, /* SAFEARRAY16 * */SEGPTR *ppsa)
{
  SAFEARRAYBOUND16 sab;

  TRACE("(%p,%p)\n", bstr, ppsa);
  
  if (!ppsa)
    return E_INVALIDARG;

  sab.lLbound = 0;
  sab.cElements = SysStringByteLen16(bstr);

  *ppsa = SAFEARRAY_Create(VT_UI1, 1, &sab, 0);

  if (*ppsa)
  {
    memcpy(((SAFEARRAY16*)MapSL(*ppsa))->pvData, bstr, sab.cElements);
    return S_OK;
  }
  return E_OUTOFMEMORY;
}
/* This implementation of the BSTR API is 16-bit only. It
   represents BSTR as a 16:16 far pointer, and the strings
   as ISO-8859 */

SEGPTR get_blob16_from_bstr16(SEGPTR bstr)
{
    return bstr - sizeof(BYTE_BLOB16) + sizeof(OLECHAR16);
}
SEGPTR get_bstr16_from_blob16(SEGPTR blob)
{
    return blob + sizeof(BYTE_BLOB16) - sizeof(OLECHAR16);
}
HRESULT WINAPI get_task_imalloc16(SEGPTR *lpMalloc);
/*
NOTE: win16 caches BSTR.
typedef struct
{
    DWORD unk1;
    DWORD imalloc; CoGetMalloc(MEMCTX_TASK)
    DWORD unk2;
    DWORD cache_len; default: 8?
    DWORD cache; far pointer to BYTE_BLOB16
    char unk3[10];
} OLE2DISP_UNK_DATA;

All resources are released when COM is being uninitialized.
(Is ETASK used?)
 */
/******************************************************************************
 *		BSTR_AllocBytes	[Internal]
 */
static SEGPTR BSTR_AllocBytes(int n)
{
    SEGPTR mal;
    SEGPTR p;
    BYTE_BLOB16 *ptr;
    if (FAILED(get_task_imalloc16(&mal)))
        return 0;
    p = IMalloc16_Alloc(mal, n + sizeof(BYTE_BLOB16) - sizeof(OLECHAR16));
    ptr = (BYTE_BLOB16*)MapSL(p);
    ptr->clSize = n - 1;
    return get_bstr16_from_blob16(p);
}

/******************************************************************************
 * BSTR_Free [INTERNAL]
 */
static void BSTR_Free(SEGPTR in)
{
    SEGPTR mal;
    void *ptr;
    if (!in)
        return;
    if (FAILED(get_task_imalloc16(&mal)))
        return;
   ptr = MapSL( get_blob16_from_bstr16(in) );
   UnMapLS( get_blob16_from_bstr16(in) );
   IMalloc16_Free(mal, get_blob16_from_bstr16(in));
}

/******************************************************************************
 * BSTR_GetAddr [INTERNAL]
 */
static void* BSTR_GetAddr(SEGPTR in)
{
    return in ? MapSL(in) : 0;
}

HRESULT WINAPI BstrFromVector16(SAFEARRAY16 *psa, /*BSTR*/SEGPTR *pbstr)
{
  TRACE("(%p,%p)\n", psa, pbstr);

  if (!pbstr)
    return E_INVALIDARG;

  *pbstr = NULL;

  if (!psa || psa->cbElements != 1 || psa->cDims != 1)
    return E_INVALIDARG;

  *pbstr = SysAllocStringByteLen16(psa->pvData, psa->rgsabound[0].cElements);
  if (!*pbstr)
    return E_OUTOFMEMORY;
  return S_OK;
}

/******************************************************************************
 *		SysAllocString	[OLE2DISP.2]
 *
 * Create a BSTR16 from an OLESTR16 (16 Bit).
 *
 * PARAMS
 *  oleStr [I] Source to create BSTR16 from
 *
 * RETURNS
 *  Success: A BSTR16 allocated with SysAllocStringLen16().
 *  Failure: NULL, if oleStr is NULL.
 */
SEGPTR WINAPI SysAllocString16(LPCOLESTR16 oleStr)
{
	SEGPTR out;

	if (!oleStr) return 0;

	out = BSTR_AllocBytes(strlen(oleStr)+1);
	if (!out) return 0;
	strcpy((char*)BSTR_GetAddr(out),oleStr);
	return out;
}

/******************************************************************************
 *		SysReallocString	[OLE2DISP.3]
 *
 * Change the length of a previously created BSTR16 (16 Bit).
 *
 * PARAMS
 *  pbstr  [I] BSTR16 to change the length of
 *  oleStr [I] New source for pbstr
 *
 * RETURNS
 *  Success: 1
 *  Failure: 0.
 *
 * NOTES
 *  SysAllocStringStringLen16().
 */
INT16 WINAPI SysReAllocString16(SEGPTR *pbstr,LPCOLESTR16 oleStr)
{
	SEGPTR new=SysAllocString16(oleStr);
	BSTR_Free(*pbstr);
	*pbstr=new;
	return 1;
}

/******************************************************************************
 *		SysAllocStringLen	[OLE2DISP.4]
 *
 * Create a BSTR16 from an OLESTR16 of a given character length (16 Bit).
 *
 * PARAMS
 *  oleStr [I] Source to create BSTR16 from
 *  len    [I] Length of oleStr in wide characters
 *
 * RETURNS
 *  Success: A newly allocated BSTR16 from SysAllocStringByteLen16()
 *  Failure: NULL, if len is >= 0x80000000, or memory allocation fails.
 *
 * NOTES
 *  See SysAllocStringByteLen16().
 */
SEGPTR WINAPI SysAllocStringByteLen16(const char *oleStr, int len)
{
    SEGPTR out=BSTR_AllocBytes(len+1);

    if (!out)
        return 0;

    /*
    * Copy the information in the buffer.
    * Since it is valid to pass a NULL pointer here, we'll initialize the
    * buffer to nul if it is the case.
    */
    if (oleStr != 0)
    {
        memcpy(BSTR_GetAddr(out), oleStr, len);
        ((char*)BSTR_GetAddr(out))[len] = '\0';
    }
    else
    {
        memset(BSTR_GetAddr(out), 0, len+1);
    }

    return out;
}
SEGPTR WINAPI SysAllocStringLen16(SEGPTR bstr, int len)
{
    SEGPTR out=BSTR_AllocBytes(len+1);
    char *oleStr=MapSL(bstr); // BSTR_AllocBytes might cause the segment bstr is in to move

    if (!out)
        return 0;

    /*
    * Copy the information in the buffer.
    * Since it is valid to pass a NULL pointer here, we'll initialize the
    * buffer to nul if it is the case.
    */
    if (oleStr != 0)
    {
        memcpy(BSTR_GetAddr(out), oleStr, len);
        ((char*)BSTR_GetAddr(out))[len] = '\0';
    }
    else
    {
        memset(BSTR_GetAddr(out), 0, len+1);
    }

    return out;
}

/******************************************************************************
 *		SysReAllocStringLen	[OLE2DISP.5]
 *
 * Change the length of a previously created BSTR16 (16 Bit).
 *
 * PARAMS
 *  pbstr  [I] BSTR16 to change the length of
 *  oleStr [I] New source for pbstr
 *  len    [I] Length of oleStr in characters
 *
 * RETURNS
 *  Success: 1. The size of pbstr is updated.
 *  Failure: 0, if len >= 0x8000 or memory allocation fails.
 *
 * NOTES
 *  See SysAllocStringByteLen16().
 *  *pbstr may be changed by this function.
 */
int WINAPI SysReAllocStringLen16(SEGPTR *old,SEGPTR in,int len)
{
	/* FIXME: Check input length */
	SEGPTR new=SysAllocStringLen16(in,len);
	TRACE("free %08x alloc %08x\n", *old, new);
	if (!in)
	{
		memcpy(BSTR_GetAddr(new), BSTR_GetAddr(*old), min(len, SysStringLen16(*old)));
	}
	BSTR_Free(*old);
	*old=new;
	return 1;
}

/******************************************************************************
 *		SysFreeString	[OLE2DISP.6]
 *
 * Free a BSTR16 (16 Bit).
 *
 * PARAMS
 *  str [I] String to free.
 *
 * RETURNS
 *  Nothing.
 */
void WINAPI SysFreeString16(SEGPTR str)
{
	BSTR_Free(str);
}

/******************************************************************************
 *		SysStringLen	[OLE2DISP.7]
 *
 * Get the allocated length of a BSTR16 in characters (16 Bit).
 *
 * PARAMS
 *  str [I] BSTR16 to find the length of
 *
 * RETURNS
 *  The allocated length of str, or 0 if str is NULL.
 */
int WINAPI SysStringByteLen16(SEGPTR str)
{
    return SysStringLen16(str);
}
int WINAPI SysStringLen16(SEGPTR str)
{
    if (!str)
        return 0;
    BYTE_BLOB16 *bb16 = (BYTE_BLOB16*)MapSL(get_blob16_from_bstr16(str));
    return bb16->clSize;
}

#include <pshpack1.h>

typedef struct
{
    /* TCHAR* */SEGPTR szName;
    VARTYPE vt;
} PARAMDATA16, *LPPARAMDATA16;

typedef struct
{
    /* TCHAR* */SEGPTR szName;
    /* PARAMDATA* */SEGPTR ppdata;
    DISPID dispid;
    UINT16 iMeth;
    /* CALLCONV */WORD cc;
    UINT16 cArgs;
    unsigned short wFlags;
    VARTYPE vtReturn;
} METHODDATA16, *LPMETHODDATA16;

typedef struct
{
    /* METHODDATA * */ SEGPTR pmethdata;
    UINT16 cMembers;
} INTERFACEDATA16, *LPINTERFACEDATA16;
#include <poppack.h>

HRESULT WINAPI CreateDispTypeInfo16Impl(
    INTERFACEDATA *pidata, /* [I] Description of the interface to build type info for */
    LCID lcid, /* [I] Locale Id */
    ITypeInfo **pptinfo); /* [O] Destination for created ITypeInfo object */
/******************************************************************************
 * CreateDispTypeInfo [OLE2DISP.31]
 */
HRESULT WINAPI CreateDispTypeInfo16(
	INTERFACEDATA16 *pidata,
	LCID lcid,
	SEGPTR *pptinfo)
{
    int i;
    INTERFACEDATA *pidata32 = HeapAlloc(GetProcessHeap(), 0, sizeof(INTERFACEDATA));
    METHODDATA16 *pimethdata16 = (METHODDATA16*)MapSL(pidata->pmethdata);
    ITypeInfo *ptinfo = NULL;
    HRESULT result;
    TRACE("(%p,%d,%p)\n", pidata, lcid, pptinfo);
    pidata32->cMembers = pidata->cMembers;
    pidata32->pmethdata = (METHODDATA*)HeapAlloc(GetProcessHeap(), 0, sizeof(METHODDATA) * pidata32->cMembers);
    for (i = 0; i < pidata32->cMembers; i++)
    {
        PARAMDATA16 *param16 = (PARAMDATA16*)MapSL(pimethdata16[i].ppdata);
        pidata32->pmethdata[i].szName = strdupAtoW((LPCSTR)MapSL(pimethdata16[i].szName));
        pidata32->pmethdata[i].dispid = pimethdata16[i].dispid;
        pidata32->pmethdata[i].iMeth = pimethdata16[i].iMeth;
        pidata32->pmethdata[i].cc = pimethdata16[i].cc;
        pidata32->pmethdata[i].cArgs = pimethdata16[i].cArgs;
        pidata32->pmethdata[i].wFlags = pimethdata16[i].wFlags;
        pidata32->pmethdata[i].vtReturn = pimethdata16[i].vtReturn;
        if (param16)
        {
            PARAMDATA *param32 = (PARAMDATA*)HeapAlloc(GetProcessHeap(), 0, sizeof(PARAMDATA) * pimethdata16[i].cArgs);
            int j;
            pidata32->pmethdata[i].ppdata = param32;
            for (j = 0; j < pimethdata16[i].cArgs; j++)
            {
                param32[j].szName = strdupAtoW((LPCSTR)MapSL(param16[j].szName));
                param32[j].vt = param16[j].vt;
            }
        }
        else
        {
            pidata32->pmethdata[i].ppdata = NULL;
        }
    }
    result = hresult32_16(CreateDispTypeInfo16Impl(pidata32, lcid, &ptinfo));
    if (1)
    {
        for (i = 0; i < pidata32->cMembers; i++)
        {
            if (pidata32->pmethdata[i].ppdata)
            {
                int j;
                for (j = 0; j < pidata32->pmethdata[i].cArgs; j++)
                {
                    HeapFree(GetProcessHeap(), 0, pidata32->pmethdata[i].ppdata[j].szName);
                }
            }
            HeapFree(GetProcessHeap(), 0, pidata32->pmethdata[i].szName);
        }
        HeapFree(GetProcessHeap(), 0, pidata32->pmethdata);
        HeapFree(GetProcessHeap(), 0, pidata32);
    }
    *pptinfo = iface32_16(&IID_ITypeInfo, ptinfo);
	return result;
}

HRESULT WINAPI DispGetIDsOfNames16(SEGPTR ptinfo, SEGPTR rgszNames, UINT16 cNames, SEGPTR pMemId)
{
    return ITypeInfo16_GetIDsOfNames(ptinfo, rgszNames, cNames, pMemId);
}

HRESULT WINAPI DispInvoke16(SEGPTR _this, SEGPTR ptinfo, LONG dispidMember, UINT16 wFlags, SEGPTR pparams, SEGPTR pvarResult, SEGPTR pexcepinfo, SEGPTR puArgErr)
{
    return ITypeInfo16_Invoke(ptinfo, _this, dispidMember, wFlags,
        pparams, pvarResult, pexcepinfo, puArgErr);
}

/******************************************************************************
 * CreateStdDispatch [OLE2DISP.32]
 */
HRESULT WINAPI CreateStdDispatch16(
    SEGPTR punkOuter,
    SEGPTR pvThis,
    SEGPTR ptinfo,
    SEGPTR* ppunkStdDisp)
{
    HRESULT result;
    IUnknown *punkStdDisp = NULL;
    TRACE("(%p,%p,%p,%p)\n",punkOuter, pvThis, ptinfo, ppunkStdDisp);
    result = CreateStdDispatch((IUnknown*)iface16_32(&IID_IUnknown, punkOuter), pvThis, (ITypeInfo*)iface16_32(&IID_ITypeInfo, ptinfo), &punkStdDisp);
    *ppunkStdDisp = iface32_16(&IID_IUnknown, punkStdDisp);
    return hresult32_16(result);
}

/******************************************************************************
 * RegisterActiveObject [OLE2DISP.35]
 */
HRESULT WINAPI RegisterActiveObject16(
	SEGPTR punk, REFCLSID rclsid, DWORD dwFlags, unsigned long *pdwRegister
) {
	TRACE("(%08x,%s,0x%08x,%p)\n",punk,debugstr_guid(rclsid),dwFlags,pdwRegister);
	return hresult32_16(RegisterActiveObject((IUnknown*)iface16_32(&IID_IUnknown, punk), rclsid, dwFlags, pdwRegister));
}

HRESULT WINAPI RevokeActiveObject16(unsigned long dwRegister, SEGPTR pvreserved)
{
    TRACE("(%08x, %08x)\n", dwRegister, pvreserved);
    if (pvreserved)
    {
        ERR("pvreserved must be NULL.\n");
    }
    return hresult32_16(RevokeActiveObject(dwRegister, NULL));
}

HRESULT WINAPI GetActiveObject16(REFCLSID rclsid, SEGPTR pvreserved, SEGPTR *ppunk)
{
    HRESULT result;
    IUnknown *punk = NULL;
    TRACE("(%s,%08x,%p)\n", debugstr_guid(rclsid), pvreserved, ppunk);
    if (pvreserved)
    {
        ERR("pvreserved must be NULL.\n");
    }
    result = GetActiveObject(rclsid, NULL, &punk);
    *ppunk = iface32_16(&IID_IUnknown, punk);
    return hresult32_16(result);
}

/******************************************************************************
 * SetErrorInfo [OLE2DISP.110]
 */
HRESULT WINAPI SetErrorInfo16(ULONG dwReserved, SEGPTR perrinfo)
{
    TRACE("(%08x,%08x)\n", dwReserved, perrinfo);
    return hresult32_16(SetErrorInfo(dwReserved, (IErrorInfo*)iface16_32(&IID_IErrorInfo, perrinfo)));
}

HRESULT WINAPI GetErrorInfo16(ULONG dwReserved, SEGPTR *pperrinfo)
{
    HRESULT result;
    IErrorInfo *perrinfo = NULL;
    TRACE("(%08x,%08x)\n", dwReserved, pperrinfo);
    result = GetErrorInfo(dwReserved, &perrinfo);
    *pperrinfo = iface32_16(&IID_IErrorInfo, perrinfo);
    return hresult32_16(result);
}

HRESULT WINAPI CreateErrorInfo16(SEGPTR *pperrinfo)
{
    HRESULT result;
    ICreateErrorInfo *perrinfo = NULL;
    TRACE("(%p)\n", perrinfo);
    result = CreateErrorInfo(&perrinfo);
    *pperrinfo = iface32_16(&IID_ICreateErrorInfo, perrinfo);
    return hresult32_16(result);
}


HRESULT WINAPI DispGetParam16(
	DISPPARAMS16 *pdispparams, /* [in] Parameter list */
	UINT16       position,    /* [in] Position of parameter to coerce in pdispparams */
	VARTYPE      vtTarg,      /* [in] Type of value to coerce to */
	VARIANT16    *pvarResult,  /* [out] Destination for resulting variant */
	UINT16       *puArgErr)    /* [out] Destination for error code */
{
    /* position is counted backwards */
    UINT pos;
    HRESULT hr;

    TRACE("position=%d, cArgs=%d, cNamedArgs=%d\n",
          position, pdispparams->cArgs, pdispparams->cNamedArgs);

    if (position < pdispparams->cArgs)
    {
      /* positional arg? */
      pos = pdispparams->cArgs - position - 1;
    }
    else
    {
      /* FIXME: is this how to handle named args? */
      for (pos=0; pos<pdispparams->cNamedArgs; pos++)
        if (((DISPID*)MapSL(pdispparams->rgdispidNamedArgs))[pos] == position) break;

      if (pos==pdispparams->cNamedArgs)
        return DISP_E_PARAMNOTFOUND;
    }

    if (pdispparams->cArgs > 0 && !pdispparams->rgvarg)
    {
        hr = E_INVALIDARG;
        goto done;
    }

    if (!pvarResult)
    {
        hr = E_INVALIDARG;
        goto done;
    }

    hr = VariantChangeType16(pvarResult,
                           &((VARIANT16*)MapSL(pdispparams->rgvarg))[pos],
                           0, vtTarg);

done:
    if (FAILED(hr))
        *puArgErr = pos;

    return hr;
}

#if 0
/******************************************************************************
 * VariantChangeType [OLE2DISP.12]
 */
HRESULT WINAPI VariantChangeType16(VARIANTARG16 *vargDest, VARIANTARG16 *varSrc, unsigned short flags, VARTYPE vt)
{
    FIXME("stub: (%p, %p, %d, %d)\n", vargDest, varSrc, flags, vt);
    return E_NOTIMPL;
}
/******************************************************************************
 * VariantChangeTypeEx [OLE2DISP.108]
 */
HRESULT WINAPI VariantChangeTypeEx16(VARIANTARG16 *dest, const VARIANTARG16 *src, LCID lcid, USHORT flags, VARTYPE vt)
{
        FIXME("stub: %p %p %d %d %d\n", dest, src, lcid, flags, vt);
        return E_INVALIDARG;
}

/******************************************************************************
 * VariantInit [OLE2DISP.8]
 */
void WINAPI VariantInit16(VARIANTARG16 *v)
{
    TRACE("(%p)\n", v);
    v->vt = VT_EMPTY;
}

HRESULT WINAPI VariantClear16(VARIANTARG16 *v)
{
    TRACE("(%p)\n", v);
    HRESULT hres = S_OK;
    if (!V_ISBYREF(v))
    {
        if (V_ISARRAY(v) || V_VT(v) == VT_SAFEARRAY)
        {
            FIXME("V_ISARRAY\n");
            hres = S_OK;
        }
        if (V_VT(v) == VT_BSTR)
        {
            /* free string */
            SysFreeString16(V_BSTR(v));
        }
        if (V_VT(v) == VT_DISPATCH)
        {
            /* free object */
            IUnknown16_Release(V_UNKNOWN(v));
        }
    }
    v->vt = VT_EMPTY;
    v->wReserved1 = 0;
    v->wReserved2 = 0;
    v->wReserved3 = 0;
    return S_OK;
}
/* based on wine */


/* Get just the type from a variant pointer */
#define V_TYPE(v)  (V_VT((v)) & VT_TYPEMASK)

/* Flags set in V_VT, other than the actual type value */
#define VT_EXTRA_TYPE (VT_VECTOR|VT_ARRAY|VT_BYREF|VT_RESERVED)

/* Get the extra flags from a variant pointer */
#define V_EXTRA_TYPE(v) (V_VT((v)) & VT_EXTRA_TYPE)

/******************************************************************************
 * Check if a variants type is valid.
 */
static inline HRESULT VARIANT_ValidateType(VARTYPE vt)
{
  VARTYPE vtExtra = vt & VT_EXTRA_TYPE;

  vt &= VT_TYPEMASK;

  if (!(vtExtra & (VT_VECTOR|VT_RESERVED)))
  {
    if (vt < VT_VOID || vt == VT_RECORD || vt == VT_CLSID)
    {
      if ((vtExtra & (VT_BYREF|VT_ARRAY)) && vt <= VT_NULL)
        return DISP_E_BADVARTYPE;
      if (vt != (VARTYPE)15)
        return S_OK;
    }
  }
  return DISP_E_BADVARTYPE;
}


HRESULT WINAPI VariantCopy16(VARIANTARG16* pvargDest, VARIANTARG16* pvargSrc)
{
    HRESULT hres = S_OK;
    if (V_TYPE(pvargSrc) == VT_CLSID || /* VT_CLSID is a special case */
        FAILED(VARIANT_ValidateType(V_VT(pvargSrc))))
        return DISP_E_BADVARTYPE;

    if (pvargSrc != pvargDest &&
        SUCCEEDED(hres = VariantClear16(pvargDest)))
    {
        *pvargDest = *pvargSrc; /* Shallow copy the value */

        if (!V_ISBYREF(pvargSrc))
        {
            switch (V_VT(pvargSrc))
            {
            case VT_BSTR:
                V_BSTR(pvargDest) = SysAllocStringLen16((char*)MapSL(V_BSTR(pvargSrc)), SysStringLen16(MapSL(V_BSTR(pvargSrc))));
                if (!V_BSTR(pvargDest))
                    hres = E_OUTOFMEMORY;
                break;
            case VT_RECORD:
            {
                FIXME("VT_RECORD, stub\n");
            }
                break;
            case VT_DISPATCH:
            case VT_UNKNOWN:
                V_UNKNOWN(pvargDest) = V_UNKNOWN(pvargSrc);
                if (V_UNKNOWN(pvargSrc))
                    IUnknown16_AddRef(V_UNKNOWN(pvargSrc));
                break;
            default:
                if (V_ISARRAY(pvargSrc))
                    hres = SafeArrayCopy16(MapSL(V_ARRAY(pvargSrc)), &V_ARRAY(pvargDest));
            }
        }
    }
    return hres;
}

HRESULT WINAPI VariantCopyInd16(VARIANT16 *v, VARIANTARG16 *src)
{
    if (!V_ISBYREF(v))
        return VariantCopy16(v, src);
    FIXME("(%p, %p) stub.\n", v, src);
    *v = *src;
    return E_INVALIDARG;
}
#endif
