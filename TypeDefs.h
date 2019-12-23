///
/// \file     TypeDefs.h
/// \brief    Type definitions
/// \author   NOVO - Dany Thiffeault, ing. (134853)
///



#ifndef TYPEDEFS_H
#define TYPEDEFS_H


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Definition
////////////////////////////////////////////////////////////////////////////////
#define MAX_VAL_UINT32  4294967295    ///< Maximum value for a unsigned int 32

////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////


typedef uint8_t   UINT8;        ///< Unsigned integer, 8 bits
typedef int8_t    INT8;         ///< Signed integer, 8 bits
typedef uint16_t  UINT16;       ///< Unsigned integer, 16 bits
typedef int16_t   INT16;        ///< Signed integer, 16 bits
typedef uint32_t  UINT32;       ///< Unsigned integer, 32 bits
typedef int32_t   INT32;        ///< Signed integer, 32 bits
typedef float     FLOAT32;      ///< Floating point, 32 bits

typedef void (*CallbackFuncTy) (UINT32);  ///< Default callback function type.



#endif
