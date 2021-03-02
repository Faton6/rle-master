/*
* #include <iostream>
* #include <fstream>
*/
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

