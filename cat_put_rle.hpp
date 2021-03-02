/*
* #include <iostream>
* #include <fstream>
*/
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
  while( !in.eof())
  {
    for (size_t i = 0;i < cnt; ++i){
      std::cout.put(val);
    }
    cnt = in.get();
    val = in.get();
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
