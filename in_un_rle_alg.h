void in_rle_alg( std::ifstream& in, std::ofstream& out )
{
    int val = in.get();
    int old = val;
    int cnt = 0;

    while( !in.eof() )                                         
        {                                                         
          if (val != old)                                         
          {                                                       
            out << cnt;                                         
            out.put(old);                                         
            old = val;                                            
            cnt = 1;                                              
          }                                                       
          else if ( cnt < 255 ) cnt += 1;                         
          else
          {
            out << cnt;
            out.put(old);
            cnt = 0;
          }
          val = in.get();
        }
        if ( cnt != 0 )
        {
            out << cnt;
            out.put(old);
        }
        out.put(0);
}

void un_rle_alg ( std::ifstream& in, std::ofstream& out )
{
    int cnt;
    in >> cnt;
    int val = in.get();
    int qwa = val;
    while( cnt != 0 ) //изменить
    {                                                       
        for (size_t i = 0;i < cnt; ++i){
            out.put(qwa);
        }                                                     
        in >> cnt;
        val = in.get();
        qwa = val;                                            
    }
}

void in_rle_crypt_alg( std::ifstream& in, std::ofstream& out)
{
    std::string key;
    std::cout << "Wright KEY: ";
    std::cin >> key;
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
    int last = 0;
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
          out.put( val ^ key[++crc] );
          cnt = 0;
        }
        val = in.get();
      }
      crc = 0;
    }
    //(crc!=len) ? out.put(0^key[crc+1]) : out.put(0^key[0]);
}

void un_rle_crypt_alg( std::ifstream& in, std::ofstream& out )
{
    std::string key;                                         
    std::cout << "Wright KEY: ";                             
    std::cin >> key; 
    int len = key.length();
    if ( len % 2 != 0 )                                        
    {
      key.erase( len -1, len );
      --len;
    }
    int crc = 0;
    int cnt = (in.get()) ^ key[crc];
    int val = (in.get()) ^ key[++crc];
    while (cnt && val && !in.eof() )
    {
        for (int i = 0;i < cnt; ++i){
          out.put(val);
        }
        if ( (crc+1) == len )
        {
          crc = 0;
          cnt = (in.get()) ^ key[crc];
        }
        else { cnt = (in.get()) ^ key[++crc]; }
        val = (in.get()) ^ key[++crc];
        std::cout << "cnt = " << cnt << "\nval = " << val << std::endl;
    }
    out.put(10);
}
