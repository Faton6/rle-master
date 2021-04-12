#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <cstdint>


/** 
 *  USE_POSIX - использование POSIX-API (конец строки LF)
 *  USE_WIN32 - использование WIN32-API (конец строки CR LF)
 *  USE_MACOS = USE_POSIX + конец строки CR
 *  USE_CXX17 - использование платформонезависимого кода на языке Си++,
 *              в том числе стандарта Си++17 (совместно с POSIX/WIN32/MACOS)
 *
 *  Матрица совместимости:
 *  -----
 *       POSIX WIN32 MACOS CXX17
 *  POSIX  +     -     +     +  
 *  WIN32  -     +     -     +  
 *  MACOS  +     -     +     +  
 *  CXX17  +     +     +     +  
 *  
 */

#ifdef   USE_MACOS
#ifndef  USE_POSIX
#define  USE_POSIX
#endif //USE_POSIX
#endif //USE_MACOS
/*
#if (!defined USE_POSIX) && (!defined USE_WIN32)
#warning WARNING: target platform not specified, choosing POSIX-API by default
#define  USE_POSIX
#endif
*/ //разобраться почему не работает

#ifdef   USE_POSIX
#ifdef   USE_WIN32
#error ERROR: can not use POSIX-API and WIN32-API together
#else  //USE_WIN32
#ifdef   USE_CXX17
#include "tools/getopt.cpp"
#else  //USE_CXX17
#include <unistd.h>
#endif //USE_CXX17
#endif //USE_WIN32
#endif //USE_POSIX

//#include "Algorithm.cpp"
//#include "RLE.h"
//#include "UNRLE.h"

//порядок подключения важен
#include "info_rle.h"
#include "in_un_rle_alg.h"
//#include "crc64.cpp"
#include "getopt_rle.hpp" 
#include "in_un_rle.hpp" 
#include "crypt_rle.hpp" 
#include "cat_put_rle.hpp"

/*
 * RLE - run length encodind, кодирование длинн серий
 *
 * Пример:  вход - AAAAABBBBBCCCCDDDD  A BB C B A     BDBAD
 *
 *                 5A  5B  4C  4D      1A 2B 1C 1B 1A 1B1D...
 *
 * Может:  rle    - зажимает
 * 	      unrle  - раcпаковывает
 *         putRLE - записать методом put в уже существующий .rle файл:
 *         ./<prog_name> -p <file_name>.rle
 *         rlecat - распаковывает в стандартный вывод:
 *         ./<prog_name> -t <file_name>.rle
 *         Проверка > 255
 *         шифрование кодовой фразой - xor-методом

 * 	   
 */


void unRLE(std::string& name);		
void inRLE(std::string& name);
void putRLE(std::string& name);
void catRLE(std::string& name);
void inRLE_crypt(std::string& name);
void unRLE_crypt(std::string& name);
//void info_in_rle(std::string& name);
//void info_un_rle(std::string& name);
bool keep_file = false;


int main(int argc, char **argv)
{
  int opt;
  void (*process_func)(std::string& name) = inRLE;
  std::string name = argv[1]; 
  std::string rle = ".rle";
  if ( argc > 2 )
  {
      name = argv[2];

  while ((opt = getopt(argc, argv, "kucdpt:")) != -1) 
  {
    switch (opt) 
    {
      case 'k':     //keep file
        keep_file = true;
        break;

      case 'd':     //decompress
        process_func = unRLE;
	    break;

      case 'e':     //encryption
	    process_func = inRLE_crypt;
    	break;

      case 'u':     //decryption
	    process_func = unRLE_crypt;
    	break;

      case 'p':     //new data from std input in <file>.rle
	    process_func = putRLE;
	    break;

     case 'c':      //stdout
	    process_func = catRLE;
        break;

      default:
        std::cerr << "Usaged: " << argv[0] << " invalid arg"
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
  if ( (name.length() > 3) && name.rfind(rle) 
          && process_func == inRLE  ) process_func = unRLE;

    process_func(name);
    exit(EXIT_SUCCESS);
}

