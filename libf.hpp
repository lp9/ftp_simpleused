/*
 * libf.h
 *
 *  Created on: 12-01-2012
 *      Author: lp9
 */


#ifndef LIBF_HPP_
#define LIBF_HPP_

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

  CURL *curl;
  CURLcode res;

public:
  ftp();

  int get(string url,string name_save_to_disk);
  int upl(string LOCAL_FILE,string  REMOTE_URL);

};


#endif /* LIBF_H_ */
