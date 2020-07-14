/* <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "cmfc.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  memcpy((char *)stream, (char *)ptr, size * nmemb);
  return size * nmemb;
}

int get_servaddr(char *servip, short *port)
{
  CURL *curl;
  CURLcode res;

  char buf[256];
  char *ind;

  memset(buf, 0, 256);
  curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8000/bsites/l");
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  if ((ind = strstr(buf, "\n")) > buf)
  {
    memcpy(servip, buf, ind-buf);
    *port = (short)atoi(ind + 1);
    // printf("%s:%d", servip, *port);
  }
  

  return 0;
}
