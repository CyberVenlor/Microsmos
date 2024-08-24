#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

#include <cstdint>  // For fixed width integer types

// Signed integer types with shorter names
typedef std::int8_t  i8;    // 8-bit signed integer
typedef std::int16_t i16;   // 16-bit signed integer
typedef std::int32_t i32;   // 32-bit signed integer
typedef std::int64_t i64;   // 64-bit signed integer

// Unsigned integer types with shorter names
typedef std::uint8_t  u8;    // 8-bit unsigned integer
typedef std::uint16_t u16;   // 16-bit unsigned integer
typedef std::uint32_t u32;   // 32-bit unsigned integer
typedef std::uint64_t u64;   // 64-bit unsigned integer

// Floating-point types
typedef float   f32;   // 32-bit floating-point
typedef double  f64;   // 64-bit floating-point

// Boolean type
typedef bool    b8;    // Boolean (true/false)

// Character type
typedef char    c8;    // 8-bit character

#endif // TYPEDEFINE_H