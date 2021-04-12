#include <string>
#include <fstream>
#include <list>
#include <stdexcept>
#include <iostream>

#include <cstdint>

using std::string;
using std::runtime_error;

struct extra_record
{
  uint16_t type;
  uint16_t size;
  string data;
};

class format_header
{
  private:
    uint8_t  ID1, ID2, CM, FLG, XFL, OS;
    uint64_t MTIME;
    ;
    uint16_t XLEN;
    std::list<extra_record> EXTRA;
    ;
    string   FNAME;
    string   FCOMM;
    uint32_t CRC32;
    
    uint64_t CRC64;
    uint64_t ISIZE;
  
  public:
    format_header()
    : ID1(0x1F), ID2(0x20), CM(0), FLG(0), 
      MTIME(0), XFL(0), OS(0x03),
      XLEN(0), CRC32(0), CRC64(0), ISIZE(0)
    {}

    void SetFileName( const string & name )
    {
      if( name.length() == 0 )
      {
        FNAME = "";
        FLG &= 0xF7 ;
      }
      else
      {
        FNAME = name;
        FLG |= 0x08 ;
      }
    }
    const string & GetFileName() const
    {
      return FNAME;
    }
    
    void SetComment( const string & comm )
    {
      if( comm.length() == 0 )
      {
        FCOMM = "";
        FLG &= 0xEF ;
      }
      else
      {
        FCOMM = comm;
        FLG |= 0x10 ;
      }
    }
    const string & GetComment() const
    {
      return FCOMM;
    }
    
    void ReadHeader( std::ifstream & in )
    {
      ID1 = in.get();
      if(in.eof()) throw runtime_error("Error reading ID1"); 
      ID2 = in.get();
      if(in.eof()) throw runtime_error("Error reading ID2"); 

      if( (ID1 != 0x1F) || (ID2 != 0x20) )
        throw runtime_error("Error: not rle format"); 

      CM = in.get();
      if(in.eof()) throw runtime_error("Error reading CM"); 
      
      FLG = in.get();
      if(in.eof()) throw runtime_error("Error reading FLG"); 

      if(!in.read(reinterpret_cast<char*>(&MTIME),8))
        throw runtime_error("Error reading MTIME"); 
      
      XFL = in.get();
      if(in.eof()) throw runtime_error("Error reading XFL"); 
      
      OS = in.get();
      if(in.eof()) throw runtime_error("Error reading OS"); 
      
      //
      
      if( FLG & 0x04 ) // FEXTRA
      {
        if(!in.read(reinterpret_cast<char*>(&XLEN),2))
          throw runtime_error("Error reading XLEN"); 
        else {
          if( !in.seekg(XLEN,std::ifstream::cur) ) 
              throw runtime_error("Error positioning"); 
        }
      }
      if( FLG & 0x08 ) // FNAME
      {
        while(true)
        {
          char c = in.get();
          if( in.eof() ) 
            throw runtime_error("Error reading FNAME"); 
          if( c == 0 ) break;
          FNAME += c;
        }
      }
      if( FLG & 0x10 ) // FCOMMENT
      {
        while(true)
        {
          char c = in.get();
          if( in.eof() ) 
            throw runtime_error("Error reading FCOMM"); 
          if( c == 0 ) break;
          FCOMM += c;
        }
      }
      if( FLG & 0x02 ) // FHCRC
      {
        if(!in.read(reinterpret_cast<char*>(&CRC32),4))
          throw runtime_error("Error reading CRC32"); 
      }
    }
    void ReadFooter( std::ifstream & in )
    {
      if(!in.read(reinterpret_cast<char*>(&CRC64),8))
        throw runtime_error("Error reading CRC64"); 
      
      if(!in.read(reinterpret_cast<char*>(&ISIZE),8))
        throw runtime_error("Error reading ISIZE"); 
    }
    
    void WriteHeader( std::ofstream & out )
    {
      if( !out.put(ID1) ) throw runtime_error("Error writing ID1"); 
      if( !out.put(ID2) ) throw runtime_error("Error writing ID2"); 

      if( !out.put(CM) )  throw runtime_error("Error writing CM"); 
      
      if( !out.put(FLG) ) throw runtime_error("Error writing FLG"); 

      if( !out.write(reinterpret_cast<char*>(&MTIME),8) )
        throw runtime_error("Error writing MTIME"); 
      
      if( !out.put(XFL) ) throw runtime_error("Error writing XFL"); 
      
      if( !out.put(OS) )  throw runtime_error("Error reading OS"); 
      
      //
      
      if( FLG & 0x04 ) // FEXTRA
      {
        if( !out.write(reinterpret_cast<char*>(&XLEN),2) )
          throw runtime_error("Error writing XLEN"); 
        else {
          if( !out.seekp(XLEN,std::ifstream::cur) ) 
              throw runtime_error("Error positioning"); 
        }
      }
      if( FLG & 0x08 ) // FNAME
      {
        if( !out.write(FNAME.c_str(),FNAME.length()) )
          throw runtime_error("Error writing FNAME"); 
        if( !out.put(0) ) throw runtime_error("Error writing FNAME"); 
      }
      if( FLG & 0x10 ) // FCOMMENT
      {
        if( !out.write(FCOMM.c_str(),FCOMM.length()) )
          throw runtime_error("Error writing FCOMM"); 
        if( !out.put(0) ) throw runtime_error("Error writing FCOMM"); 
      }
      if( FLG & 0x02 ) // FHCRC
      {
        if( !out.write(reinterpret_cast<char*>(&CRC32),4) )
          throw runtime_error("Error writing CRC32"); 
      }
    }
    
    void WriteFooter( std::ofstream & out )
    {
      if( !out.write(reinterpret_cast<char*>(&CRC64),8) )
        throw runtime_error("Error writing CRC64"); 
      
      if( !out.write(reinterpret_cast<char*>(&ISIZE),8) )
        throw runtime_error("Error writing ISIZE"); 
    }
};
/*
int main(int argc, char **argv)
{
  format_header h;
  
  h.SetFileName("tets");
  h.SetComment("OMG WTF ICQ");
  
  //---
  std::ofstream out("tets.rle", std::fstream::binary);
  h.WriteHeader(out);
  h.WriteFooter(out);
  out.close();

  std::ifstream in("tets.rle", std::fstream::binary);
  h.ReadHeader(in);
  h.ReadFooter(in);
  in.close();
  //---
  
  return 0;
}
*/
/*
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
  //
/*  
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
*/
