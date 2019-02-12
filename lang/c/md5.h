/* taken from RFC-1321/Appendices A.1/A.2 */

#include <stdint.h>

/* PROTOTYPES should be set to one if and only if the compiler supports
function argument prototyping.
The following makes PROTOTYPES default to 0 if it has not already
been defined with C compiler flags.
*/
#ifndef _SYS_MD5_H_
#define _SYS_MD5_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */
   
    /* UINT4 defines a four byte word */
    // typedef unsigned long int UINT4;
    //typedef unsigned int UINT4;
    typedef uint32_t UINT4;

    typedef unsigned char UCHAR;

    /* UINT2 defines a two byte word */
    //typedef unsigned short int UINT2;
    typedef uint16_t UINT2;
    
    typedef unsigned char *POINTER; // POINTER defines a generic pointer type


#ifndef PROTOTYPES
#define PROTOTYPES 1  /* jeikul modified this from 0 to 1 */
#endif


    /* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
    If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
    returns an empty list.
    */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

    /* MD5.H - header file for MD5C.C
    */

    /* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
    rights reserved.

    License to copy and use this software is granted provided that it
    is identified as the "RSA Data Security, Inc. MD5 Message-Digest
    Algorithm" in all material mentioning or referencing this software
    or this function.

    License is also granted to make and use derivative works provided
    that such works are identified as "derived from the RSA Data
    Security, Inc. MD5 Message-Digest Algorithm" in all material
    mentioning or referencing the derived work.

    RSA Data Security, Inc. makes no representations concerning either
    the merchantability of this software or the suitability of this
    software for any particular purpose. It is provided "as is"
    without express or implied warranty of any kind.

    These notices must be retained in any copies of any part of this
    documentation and/or software.
    */

    /* MD5 context. */
    typedef struct {
        UINT4 state[4];                                   /* state (ABCD) */
        UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
        unsigned char buffer[64];                         /* input buffer */
    } MD5_CTX;

    void MD5Init PROTO_LIST ((MD5_CTX *));
    void MD5Update PROTO_LIST
        ((MD5_CTX *, unsigned char *, unsigned int));
    void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));
    // The function to calculate the message digest string
    // of a given string based on the MD5 algrithm.
    void MD5Calc(const unsigned char *input, unsigned int inlen, unsigned char *output);

#ifdef __cplusplus
}
#endif /* __cplusplus  */
#endif /* _SYS_MD5_H_ */