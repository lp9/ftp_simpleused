/***************************************************************************
 *  author:  lp9
 *Sciaganie i wysyłanie za pomoca libcurl. wziete z przykladow i zmodyfikowane lekko
 *Jak działa dokładnie nie wiem  -_-
 *
 *Downloading and sending via libcurl. and examples taken from a slightly modified
  *  How exactly do not know -_-
 ***************************************************************************/

#include "libf.hpp"

int main()
{
 ftp ftp(1); //1 on verbose mode
  //Pobieranie. Download from FTP
  ftp.get("URL","NAME_SAVE_TO_DISK"); //podajemy np. ("ftp://ftp.aster.pl/pub/speed/1m","1m")
  //Upload file to FTP.
  ftp.upl("LOCAL_FILE_NAME","ftp://USER:PASSWD@URL");  //podajemy np. ("READ","URL")
  ftp.cdir("URL","NEWDIR");

  return 0;
}

