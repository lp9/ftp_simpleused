/***************************************************************************
 *  author:  lp9
 *Sciaganie i wysyłanie za pomoca libcurl. wziete z przykladow i zmodyfikowane lekko
 *Jak działa dokładnie nie wiem  -_-
 *
 *Downloading and sending via libcurl. and examples taken from a slightly modified
  *  How exactly do not know -_-
 ***************************************************************************/

#include<curl/curl.h>
#include<iostream>
#include<cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace std;


class ftp //ładnie zamknięte wysyłanie i pobieranie na ftp za pomocą libcurla. :)
{

  static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE* stream)
  {
    /* in real-world cases, this would probably get this data differently
       as this fread() stuff is exactly what the library already would do
       by default internally */
    size_t retcode = fread(ptr, size, nmemb, stream);
    //fprintf(stderr, "*** We read %" SIZE_T " bytes from file\n", retcode);

    return retcode;
  }

  struct FtpFile
  {
    const char *filename;
    FILE *stream;
  };

  static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
  {
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream) {
      /* open file for writing */
      out->stream=fopen(out->filename, "wb");
      if(!out->stream)
        return -1; /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);
  }

  CURL *curl;
  CURLcode res;

public:
  ftp()
  {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

  }

  int get(string url,string name_save_to_disk)
  {

    struct FtpFile ftpfile={
      name_save_to_disk.c_str(), /* name to store the file as if succesful */
      NULL
    };

    if(curl)
      {
      curl_easy_setopt(curl, CURLOPT_URL,
                       url.c_str());
      /* Define our callback to get called when there's data to be written */
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
      /* Set a pointer to our struct to pass to the callback */
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
      /* Switch on full protocol/debug output */
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      res = curl_easy_perform(curl);
      /* always cleanup */
      curl_easy_cleanup(curl);

      if(CURLE_OK != res) {
        /* we failed */
        fprintf(stderr, "curl told us %d\n", res);
      }
    }

    if(ftpfile.stream)
      fclose(ftpfile.stream); /* close the local file */

    curl_global_cleanup();

    return 0;

  }


  int upl(string LOCAL_FILE,string  REMOTE_URL)
  {
      FILE *hd_src;
      struct stat file_info;
      curl_off_t fsize;

      struct curl_slist *headerlist=NULL;

      /* get the file size of the local file */
      if(stat(LOCAL_FILE.c_str(), &file_info))
        {
          cout<<"\nNIe ma pliku/blad otwarcia"<<LOCAL_FILE;
        return 1;
        }
      fsize = (curl_off_t)file_info.st_size;

      printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

      /* get a FILE * of the same file */
      hd_src = fopen(LOCAL_FILE.c_str(), "rb");

      /* get a curl handle */
      curl = curl_easy_init();
      if(curl) {
          curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);   //verbose mode on
        /* build a list of commands to pass to libcurl */

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

        /* enable uploading */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* specify target */
        curl_easy_setopt(curl,CURLOPT_URL, REMOTE_URL.c_str());

        /* pass in that last of FTP commands to run after the transfer */
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

        /* now specify which file to upload */
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

        /* Set the size of the file to upload (optional).  If you give a *_LARGE
           option you MUST make sure that the type of the passed-in argument is a
           curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
           make sure that to pass in a type 'long' argument. */
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                         (curl_off_t)fsize);

        /* Now run off and do what you've been told! */
        res = curl_easy_perform(curl);

        /* clean up the FTP commands list */
        curl_slist_free_all (headerlist);

        /* always cleanup */
        curl_easy_cleanup(curl);
      }
      fclose(hd_src); /* close the local file */

      curl_global_cleanup();
      return 0;
  }

};

int main(void)
{
  ftp ftp;
  //Pobieranie. Download from FTP
  ftp.get("URL","NAME_SAVE_TO_DISK"); //podajemy np. ("ftp://ftp.aster.pl/pub/speed/1m","1m")
  //Upload file to FTP.
  ftp.upl("LOCAL_FILE_NAME","ftp://USER:PASSWD@URL");  //podajemy np. ("READ","URL")

  return 0;
}
