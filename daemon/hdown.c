#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "cmfc.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

static size_t copy_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    memcpy((char *)stream, (char *)ptr, size * nmemb);
    return size * nmemb;
}

int http_down(char *url, char *pathname)
{
    FILE *file;

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        file = fopen(pathname, "wb");
        if (!file)
        {
            curl_easy_cleanup(curl);
            fclose(file);
            return(-1);
        }

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        fclose(file);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return(0);
}

int http_read(char *url, char *str)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, copy_data);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return(0);
}