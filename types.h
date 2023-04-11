#pragma once

// Not the way I'd like it but it stops stdint-gcc from fighting me
typedef signed char			int8_t;
typedef unsigned char		uint8_t;
typedef short int			int16_t;
typedef short unsigned int	uint16_t;
typedef int					int32_t;
typedef unsigned int		uint32_t;
typedef long int			int64_t;
typedef long unsigned int	uint64_t;

#define nullptr ((void*)0)

typedef uint8_t bool;
#define true 1
#define false 0