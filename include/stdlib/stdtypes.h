/*
  This file contain's defination's for standard datatype's
  Made By : Ankit Sharma : maglash64@gmail.com
*/

typedef char                    int8_t;
typedef short                   int16_t;
typedef int                     int32_t;
typedef long long int           int64_t;

typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;


void zeromem(uint8_t * data, uint32_t len)
{
  for(uint32_t i = 0;i < len;i++,data++)
    *data = 0;
}
