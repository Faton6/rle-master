/* 
 * #include <iostream>
 * #include <fstream>
*/
/*
using std::filesystem::path ;
using std::filesystem::exists ;
using std::filesystem::file_status ;
using std::filesystem::status ;
using std::filesystem::is_regular_file ;
using std::filesystem::file_size ;
using std::filesystem::file_time_type ;
using std::filesystem::last_write_time ;
*/
void unRLE(std::string &name)
{   
  std::ifstream in(name, std::ifstream::binary);
  if( !in.is_open() ) //проверка открытия
  {
    std::cerr << "ERROR:(unRLE) " 
              << "unable to open file "
              << name
              << std::endl;
    return;
  }
  int cnt = in.get();
  int val = in.get();
  int qwa = val;
  std::ofstream out;
  std::string old_name = name;
  out.open(name.erase( (name.length()-4), name.length() ), std::ifstream::binary );
  if (!out.is_open())
  {
          std::cerr << "Error of open file\n";
          return;
  }
  //if( !in.seekg(14, std::ifstream::beg) )
  //{ std::cerr << "Error positioning stream" << std::endl; return ; }
   while( !in.eof() ) //изменить
    {   
      for (size_t i = 0;i < cnt; ++i){
          out.put(qwa);
      }
      cnt = in.get();
      val = in.get();
      qwa = val;
    }
  //in.seekg(-16, std::ifstream::beg)
  
  out.put('\n');
  out.close();
  in.close();
  std::remove(old_name.c_str());
}

void inRLE(std::string &name)
{
  std::ifstream in(name, std::ifstream::binary);
  if( !in.is_open() )
  {
    std::cerr << "ERROR:(inRLE) " 
              << "unable to open file "
              << name
              << std::endl;
    return;
  }
  int val = in.get();
  int old = val;
  int cnt = 0;
  std::string rle = ".rle";
  std::string old_name = name;
  std::ofstream out;
  name = name + rle;
  out.open( name, std::ifstream::binary );
  //inline info_put_rle(std::string& name);
  if (!out.is_open())
  {
          std::cerr << "Error of open file\n";
          return;
  }
  while( !in.eof())
  {
    if (val != old)
    {
      out.put(cnt);
      // некорректно отображается
      //out << cnt;
      // некорректно записывается
      out.put(old);
      old = val;
      cnt = 1;
    }
    else if ( cnt < 255 ) cnt += 1;
    else
    {
      std::cout << cnt << '\t' << old << '\n';
      out.put(cnt);
      out.put(old);
      cnt = 0;
    }
    val = in.get();
  }
  out.put('\n');
/*
  uint8_t data[] = {'1','2','3','4','5','6','7','8','9'};
  gen_crc64_tab();
  uint64_t CRC64 = 0;
  CRC64 = crc64(CRC64, data, 9);
  CRC64 ^= 0;
  out.put(CRC64);
  uint64_t ISIZE;
  path n = (name); 
  ISIZE = file_size(n);
  out.put(ISIZE);       // вроде как можно сразу запихнуть ISIZE

  //info_in_end_rle(name);
  out.put('\n');
  */
  out.close();
  in.close();
  std::filesystem::remove(old_name); // будет ли здесь пространство имён filesystem?
}

