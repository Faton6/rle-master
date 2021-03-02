#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdint>
#include <iomanip>

#if defined(__cplusplus) && (__cplusplus >=201703L)
#include <filesystem>
#else
#  error "C++17 required!"
#endif

#ifdef __linux //нужно ли?
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#elif _WIN32
//#include ...
#endif

//#include "info_rle.hpp"
#include "crc64.cpp"
#include "getopt_rle.hpp" //hpp или cpp?
#include "in_un_rle.hpp"  //hpp - class
#include "crypt_rle.hpp"  //cpp - func?
#include "cat_put_rle.hpp"

//#include "crc64.cpp"

/*
 * RLE - run length encodind, кодирование длинн серий
 *
 * Пример:  вход - AAAAABBBBBCCCCDDDD  A BB C B A     BDBAD
 *
 *                 5A  5B  4C  4D      1A 2B 1C 1B 1A 1B1D...
 *
 * Может:  rle    - зажимает
 * 	   unrle  - раcпаковывает
 *         putRLE - записать методом put в уже существующий .rle файл:
 *         ./<prog_name> -p <file_name>.rle
 *         rlecat - распаковывает в стандартный вывод:
 *         ./<prog_name> -t <file_name>.rle
 *         Проверка > 255
 *         шифрование кодовой фразой - xor-методом
 * Проверить:
 * 	   нужна ли  проверка в xor-шифровании, если результат ^ будет
 * 	   больше допустимого значения кода ASCII
 * 	   откуда был взят код read_format.cpp?
 * 	   constexpr - const // вроде разобрался
 * 	   inline функция
 * 	   
 */
using std::filesystem::path ;
using std::filesystem::exists ;
using std::filesystem::file_status ;
using std::filesystem::status ;
using std::filesystem::is_regular_file ;
using std::filesystem::file_size ;
using std::filesystem::file_time_type ;
using std::filesystem::last_write_time ;



void unRLE(std::string& name);		// оставлять?
void inRLE(std::string& name);
void putRLE(std::string name);
void catRLE(std::string name);
void inRLE_crypt(std::string& name, std::string key);
void unRLE_crypt(std::string& name, std::string key);
//void info_in_rle(std::string& name);
//void info_un_rle(std::string name);


uint8_t temp = 0;		
uint64_t my_file_time = 0;	 
uint64_t old_file_size = 0;     
int main(int argc, char **argv)
{
  int opt;
  std::string key; // key for crypt
  void (*process_func)(std::string& name) = inRLE;
  std::string name = argv[1]; 
  std::string rle = ".rle";
  path o("");
  path n("");
  if ( argc == 3 )
  {
  while ((opt = getopt(argc, argv, "iucdpt:")) != -1) {
    switch (opt) {
      case 'i':
        name = argv[2]; 
	inRLE(name);
	temp = 1;
        break;
      case 'u':
        name = argv[2];
	temp = 2;
	unRLE(name);
	break;
      case 'c':
	std::cout << "Wright KEY: ";
	std::cin >> key;
        name = argv[2];
	temp = 3;
	inRLE_crypt(name, key);
	break;
      case 'd':
	std::cout << "Wright KEY: ";
	std::cin >> key;
        name = argv[2]; 
	temp = 4;
	unRLE_crypt(name, key);
	break;
      case 'p':
        name = argv[2]; 
	temp = 5;
	putRLE(name);
	break;
     case 't':
	name = argv[2];
	temp = 6;
	catRLE(name);
	break;
      default: /* '?' */
        std::cerr << "Usage: " << argv[0] << " [-t val] [-n] arg"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
  }
  }
  
  if (optind >= argc+1) 
  {
    std::cerr << "Expected argument after options" << std::endl;
    exit(EXIT_FAILURE);
  }
  if ( (name.find(rle) == -1) && ( temp == 0 ) )
  {
    //o = (argv[1]); // path 
    //old_file_size = file_size(o);
    process_func(name);
    //n = (name.c_str());
    //std::cout << "Файл уменьшился на: " << old_file_size - file_size(n) << "б\n";
    //old_file_size = file_size(n);
  }
  else if ( (name.rfind(rle) > 0) && ( temp == 0 ) )
  {
    //o = (argv[1]);
    //old_file_size = file_size(o);
    process_func = unRLE;
    process_func(name);
    //n = (name.c_str());
    //std::cout << "Файл увеличелся на: " << file_size(n) - old_file_size << "б\n";
  }


  exit(EXIT_SUCCESS);
}

/*
void catRLE(std::string name)
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
  if ( temp == 6 )
  {
   while( !in.eof())
    {
      for (size_t i = 0;i < cnt; ++i){
	std::cout.put(val);
      }
      cnt = in.get();
      val = in.get();
    }
  }
  std::cout.put('\n');
  in.close();
}

void putRLE(std::string name)
{
  std::cout << "Enter the line: ";
  std::string input;
  std::getline(std::cin, input);
  std::ifstream in(name, std::ifstream::binary);
  if( !in.is_open() )
  {
    std::cerr << "ERROR: " 
              << "unable to open file "
              << name
              << std::endl;
    return;
  }
  char val = input[0];
  char old = val;
  int cnt = 0;
  int counter = 0;
  std::ofstream out;
  out.open(name, std::ofstream::app);
  if (!out.is_open())
  {
          std::cerr << "Error of open file\n";
  }
  else
  {
    while( input.length() > counter )
    {
      if (val != old)
      {
        out << cnt << old;
        old = val;
        cnt = 1;
      }
      else {cnt += 1;}
      val = input[1 +counter];
      ++counter;
    }
  }
  if(cnt != 0) out << cnt <<old;
  out.close();
  in.close();
}
*/
/*
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
   while( !in.eof())
    {   
      for (size_t i = 0;i < cnt; ++i){
	  out.put(qwa);
      }
      cnt = in.get();
      val = in.get();
      qwa = val;
    }
  out.put('\n');
  out.close();
  in.close();
  std::remove(old_name.c_str());
}

void inRLE(std::string& name)
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
  out.put('\n'); // необходим
  out.close();
  in.close();
  std::remove(old_name.c_str());
}
*/
/*
void inRLE_crypt( std::string& name, std::string key )
{ 
  std::string rle = ".rle";   
  std::string old_name = name;
  std::ofstream out;
  name = name + rle;
  out.open( name, std::ifstream::binary );
  if ( !out.is_open() )
  {
    std::cerr << "Error of open file\n";
    return;
  }
  std::ifstream in( old_name, std::ifstream::binary );
  int len = key.length();
  if ( len % 2 != 0 )
  {
    key.erase( len -1, len );
    --len;
  }
  int val = in.get();
  int old = val;
  int cnt = 0;
  int crc = 0;
  while ( !in.eof() )
  {
    while ( crc < len && !in.eof() )
    {
      if ( val != old )
      {                               
        out.put( cnt ^ key[crc] );      
        out.put( old ^ key[(++crc)] );
        old = val;
        cnt = 1;
        ++crc;
      }
      else if (cnt < 255) cnt += 1;
      else
      {
        out.put( cnt ^ key[crc] );  
        out.put( old ^ key[++crc] );
        cnt = 0;
      }
      val = in.get();
    }
    crc = 0;
  }
  out.put('\n');
  out.close();
  in.close();
  std::remove(old_name.c_str());
}

void unRLE_crypt(std::string& name, std::string key)
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
  std::ofstream out;
  std::string old_name = name;
  out.open(name.erase( (name.length()-4), name.length() ), std::ifstream::binary );
  if (!out.is_open())
  {
          std::cerr << "Error of open file\n";
          return;
  }
  int len = key.length();
  if ( len % 2 != 0 )
  {
    key.erase( len -1, len );
    --len;
  }
  int crc = 0;
  int cnt = (in.get()) ^ key[crc];
  int val = (in.get()) ^ key[++crc];
  int qwa = val;
  while ( !in.eof() )
  {
      for (int i = 0;i < cnt; ++i){
        out.put(qwa);
      }
      if ( (crc+1) == len )
      {
        crc = 0;
        cnt = (in.get()) ^ key[crc];
      }
      else cnt = (in.get()) ^ key[++crc];
      val = (in.get()) ^ key[++crc];
      qwa = val;
    }
  out.put('\n');
  out.close();
  in.close();
  std::remove(old_name.c_str());
}

*/
