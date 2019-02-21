#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

int main( int argc, char *argv[] ) {
	char delims[] = "/, ?";
	char request[] = "GET /?first=misae&last=evans/ HTTP/1.1";
	char copyRequest[strlen(request)];
	strcpy(copyRequest, request);
	char* httpMethod = strtok(request, " ");

	printf("copyRequest: %s\n", copyRequest );
	//if GET is in the array then true
	if (strcmp(httpMethod, "GET")==0){
		printf("GET METHOD\n");


		//returns pointer to first occurncce of ? we add +1 to get stuff after it.
		char* requestNoGET = strchr(copyRequest, '?') + 1;
		printf("howMany: %s\n", requestNoGET);

		//get only the pairs
		char* pairs = strsep(&requestNoGET, delims);
		printf("pairs: %s\n", pairs);

		//seperate at &
	char* keyValPairs;
	//printf("keyValPairs: %s\n",keyValPairs );
	//char* key = strtok(keyValPairs, "=");
	//printf("Key: %s\n", key);
	//char* value = strtok(NULL, "\0");
	//printf("Value: %s\n", value);
	//ref for the while: https://c-for-dummies.com/blog/?p=1769
	while ( (keyValPairs=strsep(&pairs, "&")) !=NULL){
		char* key = strtok(keyValPairs, "=");
		printf("key: %s\n", key);
		char* value = strtok(NULL, "\0");
		printf("value: %s\n",value );}
		//char row[240];
		//sprintf(row, "<tr><td>%s</td><td>%s</td></tr>", key, value);
		//strcat(keyValTable, row);


	printf("\n");
}//end GET IF
	else if (strcmp(httpMethod, "POST")==0){
		printf("POST METHOD");
		//grab everything after the new line
		char* requestNoGET = strrchr(copyRequest, '\n');
		//dont have delims like GET
  	char* keyValPairs = strtok(requestNoGET, "&");
		printf("keyValPairs: %s\n", keyValPairs);
	}//END POST IF
}
