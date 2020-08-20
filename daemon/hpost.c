#include <stdio.h>
#include <curl/curl.h>

static size_t copy_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    memcpy((char *)stream, (char *)ptr, size * nmemb);
    return size * nmemb;
}

int http_post(char *url, char *param, char *str)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, copy_data);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}

int http_postb(char *url, char *data, size_t size, char *str)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();

    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* Now specify the POST data */
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        /* set the size of the postfields data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);
        /* pass our list of custom made headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, copy_data);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* free the header list */
        curl_slist_free_all(headers);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}