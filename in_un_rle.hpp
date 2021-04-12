void unRLE(std::string &name)
{ 
  format_header file;
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
  out.open(name.erase( (name.length()-4),
           name.length() ), std::ifstream::binary );
  if (!out.is_open())
  {
          std::cerr << "Error of open file\n";
          return;
  }
  file.ReadHeader( in );
  un_rle_alg( in, out );
 // file.ReadFooter( in );//crc
  out.close();
  in.close();
  std::remove(old_name.c_str());
}

void inRLE(std::string &name)
{
  format_header file;
  std::ifstream in(name, std::ifstream::binary);
  if( !in.is_open() )
  {
    std::cerr << "ERROR:(inRLE) " 
              << "unable to open file "
              << name
              << std::endl;
    return;
  }
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
  file.SetFileName("test");
  file.SetComment("comment_test");
  
  file.WriteHeader(out);
  in_rle_alg( in, out ); 
  file.WriteFooter(out);
  out.close();
  in.close();
  std::remove(old_name.c_str()); // будет ли здесь пространство имён filesystem?
}

