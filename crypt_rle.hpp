/*
* #include <iostream>
* #include <fstream>
*/
void inRLE_crypt( std::string& name )
{
    std::ifstream in( name, std::ifstream::binary );
    if( !in.is_open() ) //проверка открытия
    {
            std::cerr << "ERROR:(unRLE) " 
                << "unable to open file "
                << name
                << std::endl;
      return;
    }
    std::string rle = ".rle";   
    std::string old_name = name;
    name = name + rle;
    std::ofstream out( name, std::ifstream::binary );
    if ( !out.is_open() )
    {
      std::cerr << "Error of open file\n";
      return;
    }
    
    
    in_rle_crypt_alg(in, out );
    
    //out.put('\n');
    out.close();
    in.close();
    std::remove(old_name.c_str());
}

void unRLE_crypt(std::string& name)
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
    std::string old_name = name;
    std::ofstream out(name.erase( (name.length()-4), name.length() ), std::ifstream::binary );
    if (!out.is_open())
    {
            std::cerr << "Error of open file\n";
            return;
    }
    
    un_rle_crypt_alg( in, out );
    
    //out.put('\n');
    out.close();
    in.close();
    std::remove(old_name.c_str());
}

