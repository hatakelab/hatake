#include <curl/curl.h>  // use libcurl (https://curl.se/download.html)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
	if(!operand[1]){
		perror("hatake: curl: missing operand");
		return -1;
	}

	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, operand[1]);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			fprintf(stderr, "hatake: curl: %s: url not found\n", operand[1]);
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;
}
