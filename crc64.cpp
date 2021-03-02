/**
 * CRC64 ECMA-182
 * 
 * width   = 64 
 * poly    = 0x42f0e1eba9ea3693 
 * init    = 0x0000000000000000 
 * refin   = false 
 * refout  = false 
 * xorout  = 0x0000000000000000 
 * check   = 0x6c40df5f0b497347 
 * residue = 0x0000000000000000
 *
 */

#define CRC64_POLY 0x42F0E1EBA9EA3693ULL

uint64_t crc64_tab[256] = {0};

void gen_crc64_tab()
{
  for( uint64_t i=0; i<256; ++i )
  {
    uint64_t crc = 0 ;
    uint64_t c   = i << 56 ;
  
    for( uint64_t j=0; j<8; ++j )
    {
      if( (crc^c)&0x8000000000000000ULL )
        crc = (crc<<1)^CRC64_POLY;
      else
        crc <<= 1;
      c <<= 1;
    }
    
    crc64_tab[i] = crc;
  }
}

uint64_t crc64(uint64_t crc, const uint8_t *d, size_t l)
{
  for( size_t i=0; i<l; ++i)
  {
    size_t t = ( (crc>>56)^(*d++) ) &0xFF;
    crc = crc64_tab[t] ^ (crc<<8);
  }
  
  return crc;
}

void print_crc64_tab()
{
  gen_crc64_tab();
  
  std::cout << "uint64_t crc64_tab[256] = {" 
            << std::endl << '\t';
  
  std::cout << std::hex ;
  for(size_t i=0; i<255; )                            
  {
    std::cout << "0x" << std::setw(16) << std::setfill('0')
              << crc64_tab[i] << "ULL, ";
    if( ++i % 2 == 0 ) std::cout << std::endl << '\t';
  }
  std::cout   << "0x" << std::setw(16) << std::setfill('0')
              << crc64_tab[255] << "ULL "
              << std::endl << '}' << std::endl;
}

/*
int main()
{
  uint8_t data[] = {'1','2','3','4','5','6','7','8','9'};
  
  gen_crc64_tab();
  //print_crc64_tab();
  
  uint64_t crc = 0;
  crc = crc64(crc, data, 9);
  crc ^= 0;
  
  std::cout << "0x" 
            << crc 
            << std::endl ;
  
  return 0;
}
*/
