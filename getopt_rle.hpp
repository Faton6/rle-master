/*
* #include <iostream>
* #include <string>
*/
#define ERR(s,c)                      \
  if(opterr)                          \
    std::cerr << argv[0]              \
              << (s)                  \
              << static_cast<char>(c) \
              << std::endl; 

int opterr = 1;
int optind = 1;
int optopt;
char *optarg;

int getopt(int argc, char **argv, const char *opts)
{
  static int sp = 1;
  int c;
  size_t cp; //!

  std::string opts_str(opts); //!
  std::string c_str(" ");     //!

  if(sp == 1)
      if(optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
        return -1;
      else if( std::string(argv[optind]) == "--") {
          optind++;
          return -1;
      }
  optopt = c = argv[optind][sp];
  c_str[0] = c; //!
  if( c == ':' || (cp=opts_str.find(c_str)) == std::string::npos) //!
  {
    ERR(": illegal option -- ", c);
    if(argv[optind][++sp] == '\0') {
      optind++;
      sp=1;
    }
    return '?';
  }
  if( opts_str[cp+1] == ':') //!
  {
    if(argv[optind][sp+1] != '\0')
      optarg = &argv[optind++][sp+1];
    else if(++optind >= argc) {
      ERR(": option requires an argument -- ", c);
      sp = 1;
      return '?';
    }
    else
      optarg = argv[optind++];
    sp = 1;
  }
  else
  {
    if(argv[optind][++sp] == '\0') {
      sp = 1;
      optind++;
    }
    optarg = NULL;
  }

  return c;
}
