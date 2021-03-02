using std::filesystem::path ;
using std::filesystem::exists ;
using std::filesystem::file_status ;
using std::filesystem::status ;
using std::filesystem::is_regular_file ;
using std::filesystem::file_size ;
using std::filesystem::file_time_type ;
using std::filesystem::last_write_time ;

inline void info_in_rle(std::string name)
{
  path p = (name);
  uint8_t ID1;
  ID1 = 0;
  out.put(ID1);
  
  uint8_t ID2;
  ID2 = 0;
  out.put(ID2);

  uint8_t CM;
  CM = 0;
  out.put(CM);

  uint8_t FLG;
  FLG = 0;
  out.put(FLG);

  uint64_t MTIME;
  MTIME = last_write_time(name);
  out.put(MTIME);

  uint8_t XFL;
  XFL = 0;
  out.put(XFL);

  uint8_t OS;
  OS = 3;
  /*if(in.eof()){ std::cerr << "Error reading XFL" << std::endl; return 1; }
  else {
          std::cout << "\toperation system: " ;
    switch(OS) {
      case   0: std::cout << "FAT filesystem" ;  break;
      case   1: std::cout << "Amiga" ;           break;
      case   2: std::cout << "VMS" ;             break;
      case   3: std::cout << "Unix" ;            break;
      case   4: std::cout << "VM/CMS" ;          break;
      case   5: std::cout << "Atari TOS" ;       break;
      case   6: std::cout << "HPFS filesystem" ; break;
      case   7: std::cout << "Macintosh" ;       break;
      case   8: std::cout << "Z-System" ;        break;
      case   9: std::cout << "CP/M" ;            break;
      case  10: std::cout << "TOPS-20" ;         break;
      case  11: std::cout << "NTFS filesystem" ; break;
      case  12: std::cout << "QDOS" ;            break;
      case  13: std::cout << "Acorn RISCOS" ;    break;
      case 255: std::cout << "unknown" ;         break;
      default : std::cout << "wrong value" ;
    }
    */

  out.put(OS);
}

inline void info_in_end_rle(std::string name)
{
  gen_crc64_tab();
  uint64_t CRC64 = 0;
  CRC64 = crc64(crc, data, 9);
  CRC64 ^= 0;
  out.put(CRC64);
  uint64_t ISIZE;
  path n = (name); 
  ISIZE = file_size(n);
  out.put(ISIZE);	// вроде как можно сразу запихнуть ISIZE

}
/*
void info_un_rle(std::string name)
{

}

void info_un_end_rle(std::string name)
{

}
*/

void info_output_rle(std::string name)
{
  std::ifstream in(name, std::ifstream::binary);
  
  if( !in.is_open() )
  {
    std::cerr << "ERROR: "
              << "unable to open file "
              << name     //!
              << std::endl;
    
    return EXIT_FAILURE;
  }
  /*
  if( exists(p) )
  {
    file_status fs = status(p);
    
    if( is_regular_file(fs) )
      std::cout << "size: "  <<            file_size(p) <<std::endl;

    file_time_type mt = last_write_time(p);
    
    // file_time_type -> std::chrono::duration -> öåëîå ÷èñ
    std::cout << "time: " 
              << "?" //ðåøèòå ýòó çàäà÷êó ñàìîñòîÿòåëüíî ;)
              << std::endl;
    
    //äåìîíñòðâöèÿ stat(2)
    struct stat st;
    if( stat(p.c_str(),&st) == 0 )
    {
      //std::cout << "time: " << st.st_mtime << std::endl;
      //std::cout << "size: " << st.st_size  <<std::endl;
    }
  }
  */
  //---
  uint8_t ID1 = in.get();
  if(in.eof()){ std::cerr << "Error reading ID1" << std::endl; return 1; }
  uint8_t ID2 = in.get();
  if(in.eof()){ std::cerr << "Error reading ID2" << std::endl; return 1; }

  if( (ID1 != 0x1F) || (ID2 != 0x8B) )
  { std::cerr << "Error: not rle format" << std::endl; return 1;}
  else
  { std::cout << "Reading rle archive: " << name << std::endl; }

  uint8_t CM = in.get();
  if(in.eof()){ std::cerr << "Error reading CM" << std::endl; return 1; }
  else {
    std::cout << "\tcompression method: " 
         << ( CM == 8 ? "deflate" : "unknown" ) << std::endl; 
  }
  
  uint8_t FLG = in.get();
  if(in.eof()){ std::cerr << "Error reading FLG" << std::endl; return 1; }
  else {
    cout << "\tflags: " ;
    if( FLG & 0x01 ) std::cout << "FTEXT " ;
    if( FLG & 0x02 ) std::cout << "FHCRC " ;
    if( FLG & 0x04 ) std::cout << "FEXTRA " ;
    if( FLG & 0x08 ) std::cout << "FNAME " ;
    if( FLG & 0x10 ) std::cout << "FCOMMENT " ;
    std::cout << std::endl;
  }

  uint64_t MTIME ;
  if(!in.read(reinterpret_cast<char*>(&MTIME),4))
  { std::cerr << "Error reading MTIME" << std::endl; return 1; }
  else {
    time_t mt = MTIME;
    std::cout << "\ttime: " ;
    if( mt != 0 ) std::cout << std::ctime( &mt ) ;
    else          std::cout << "not awailable" << std::endl;
  }
  
  uint8_t XFL = in.get();
  if(in.eof()){ std::cerr << "Error reading XFL" << std::endl; return 1; }
  else {
	  std::cout << "\textra flags: " ;
    if(CM==8) {
      if(XFL==2) std::cout << "maximum compression" ;
      else if(XFL==4) std::cout << "maximum speed" ;
      else std::cout << "unknown" ;
    }
    std::cout << std::endl;
  }
  
  uint8_t OS = in.get();
  if(in.eof()){ std::cerr << "Error reading XFL" << std::endl; return 1; }
  else {
	  std::cout << "\toperation system: " ;
    switch(OS) {
	    case   0: std::cout << "FAT filesystem" ;  break;
      case   1: std::cout << "Amiga" ;           break;
      case   2: std::cout << "VMS" ;             break;
      case   3: std::cout << "Unix" ;            break;
      case   4: std::cout << "VM/CMS" ;          break;
      case   5: std::cout << "Atari TOS" ;       break;
      case   6: std::cout << "HPFS filesystem" ; break;
      case   7: std::cout << "Macintosh" ;       break;
      case   8: std::cout << "Z-System" ;        break;
      case   9: std::cout << "CP/M" ;            break;
      case  10: std::cout << "TOPS-20" ;         break;
      case  11: std::cout << "NTFS filesystem" ; break;
      case  12: std::cout << "QDOS" ;            break;
      case  13: std::cout << "Acorn RISCOS" ;    break;
      case 255: std::cout << "unknown" ;         break;
      default : std::cout << "wrong value" ;
    }
    std::cout << std::endl ;
  }
  
  //
  
  if( FLG & 0x04 ) // FEXTRA
  {
    uint16_t XLEN ;
    if(!in.read(reinterpret_cast<char*>(&XLEN),2))
    { std::cerr << "Error reading XLEN" << std::endl; return 1; }
    else {
      std::cout << "\textra filds: " << XLEN << " bytes length" << std::endl;
    }
  }
  if( FLG & 0x08 ) // FNAME
  {
    std::cout << "\toriginal name: " ;
    while(true)
    {
      int c = in.get();
      if( c == 0 ) break; // eof???// почему не в условии while?
      std::cout << static_cast<char>(c);
    }
    std::cout << std::endl;
  }
  if( FLG & 0x10 ) // FCOMMENT
  {
    std::cout << "\tcomment: " ;
    while(true)
    {
      int c = in.get();
      if( c == 0 ) break; // eof??? // почему не в условии while?
      std::cout << static_cast<char>(c);
    }
    std::cout << std::endl;
  }
 
  //
  
  if( !in.seekg(-8,std::ifstream::end) )
  { std::cerr << "Error positioning stream" << std::endl; return 1; }
  else std::cout << "\t...skip data to file end..." << std::endl;
  
  uint64_t CRC64 ;
  if(!in.read(reinterpret_cast<char*>(&CRC64),8))
  { std::cerr << "Error reading CRC32" << std::endl; return 1; }
  else std::cout << "\tcrc32: " << std::hex << CRC64 << std::endl;
  
  uint64_t ISIZE ;
  if(!in.read(reinterpret_cast<char*>(&ISIZE),8))
  { std::cerr << "Error reading ISIZE" << std::endl; return 1; }
  else std::cout << "\toriginal size: " << std::dec << ISIZE << std::endl;
  //---
  
  in.close();
  
}




