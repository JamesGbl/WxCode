//////////////////////////////////////////////////////////////////////////////
// Name:        wxadvtabledefs.h
// Purpose:     wxAdvTable basic definitions
// Author:      Moskvichev Andrey V.
// Created:     29/10/2008
// RCS-ID:      $Id: wxAdvTable.cpp,v 1.4 2008/10/29 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef WXADVTABLEDEFS_H_
#define WXADVTABLEDEFS_H_

#ifndef NULL
#define NULL 0
#endif /* NULL */

#ifndef ABS
#define ABS(a) ((a) < 0) ? (-1*(a)) : (a)
#endif

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef N
#define N(arr) (sizeof(arr) / sizeof(*arr))
#endif /* N */

#define SAFE_DELETE(ptr) if (ptr != NULL) { delete ptr; ptr = NULL; }

#define SAFE_DELETE_WITH_ELEMENTS(arrPtr, arrSize) do {	\
	if (arrPtr != NULL) {								\
		for (int n = 0; n < arrSize; n++) {				\
			SAFE_DELETE(arrPtr[n]);						\
		}												\
		delete arrPtr;									\
		arrPtr = NULL;									\
	}													\
} while (0)

#define SAFE_REPLACE(dst, src) do {						\
	SAFE_DELETE(dst);									\
	dst = src;											\
} while (0)

#endif /* WXADVTABLEDEFS_H_ */
