
// -----------------------------------
// CSCI 340 - Operating Systems
// Spring 2019
// server.h header file
// Homework 2
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and
// then binds it to the specified port_number
// for incoming client connections
//
//
// Arguments:	port_number = port number the server
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;

    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
//
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//
int accept_client( int server_socket_fd ) {

	int exit_status = OK;

	int client_socket_fd = -1;

	socklen_t client_length;

	struct sockaddr_in client_address;

	char request[2048];

	char keyValTable[50];

	//char *keyValPair[2048];

	int i;

	client_length = sizeof( client_address );

	 pid_t pid = fork();
	 char copyRequest[strlen(request)];

    client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );

	// -------------------------------------
	// TODO:
	// -------------------------------------
	// Modify code to fork a child process
	// -------------------------------------

	if ( client_socket_fd >= 0 ) {

		bzero( request, 2048 );

		read( client_socket_fd, request, 2047 );

		if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );

		// -------------------------------------
		// TODO:
		// -------------------------------------
		// Generate the correct http response when a GET or POST method is sent
		// from the client to the server.
		//
		// In general, you will parse the request character array to:
		// 1) Determine if a GET or POST method was used
		// 2) Then retrieve the key/value pairs (see below)
		// -------------------------------------

		/*
		 ------------------------------------------------------
		 GET method key/values are located in the URL of the request message
		 ? - indicates the beginning of the key/value pairs in the URL
		 & - is used to separate multiple key/value pairs
		 = - is used to separate the key and value

		 Example:

		 http://localhost/?first=tony&last=leclerc

		 two &'s indicated two key/value pairs (first=tony and last=leclerc)
		 key = first, value = tony
		 key = last, value = leclerc
		 ------------------------------------------------------
		 */

		/*
		 ------------------------------------------------------
		 POST method key/value pairs are located in the entity body of the request message
		 ? - indicates the beginning of the key/value pairs
		 & - is used to delimit multiple key/value pairs
		 = - is used to delimit key and value

		 Example:

		 first=tony&last=leclerc

		 two &'s indicated two key/value pairs (first=tony and last=leclerc)
		 key = first, value = tony
		 key = last, value = leclerc
		 ------------------------------------------------------
		 */
		 //char delims[] = " ";
		 if(pid == 0){
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
 				char* requestNoGET = strchr(copyRequest, '?')+ 1;
 				printf("howMany: %s\n", requestNoGET);

 				//get only the pairs
 				char* pairs = strsep(&requestNoGET, delims);
 				printf("pairs: %s\n", pairs);

 				//seperate at &
 			char* keyValPairs;

 			//ref for the while: https://c-for-dummies.com/blog/?p=1769
 			while ( (keyValPairs=strsep(&pairs, "&")) !=NULL){
 				char* key = strtok(keyValPairs, "=");
 				printf("key: %s\n", key);
 				char* value = strtok(NULL, "\0");

 				printf("value: %s\n",value );
				char row[50];
	      sprintf(row, "<tr><td>%s</td><td>%s</td></tr>", key, value);
				//strcat to add to the keyValTable in each iterations
				strcat(keyValTable, row);
			}//END WHILE
			// close html table
 			strcat(keyValTable, "</table>");


 			printf("\n");
 		}//end GET IF
		else if(strcmp(httpMethod, "POST") == 0){
			printf("I AM IN THE ELSE IF ");
			//returns pointer to first occurncce of ? we add +1 to get stuff after it.
			char* requestNoGET = strchr(copyRequest, '\n')+ 1;
			printf("howMany: %s\n", requestNoGET);

			char* keyValPairs;

			//ref for the while: https://c-for-dummies.com/blog/?p=1769
			while ( (keyValPairs=strsep(&requestNoGET, "&")) !=NULL){
				char* key = strtok(keyValPairs, "=");
				printf("key: %s\n", key);
				char* value = strtok(NULL, "\0");
				printf("value: %s\n",value );
				char row[50];
				sprintf(row, "<tr><td>%s</td><td>%s</td></tr>", key, value);
				strcat(keyValTable, row);
			}//END WHILE
			// close html table
			strcat(keyValTable, "</table>");

			if ( DEBUG ) printf( "%s\n",keyValTable);

				write( client_socket_fd, keyValTable, strlen(keyValTable ) );



    }//end POST if




  else{
      printf("INVALID HTTP REQUEST\n");
      exit(-1);
                    }


	}//end chid pid if



		// THIS IS AN EXAMPLE ENTITY BODY
		char* entity_body = "<html><body><h2>CSCI 340 (Operating Systems) Homework 2</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr></table></body></html>";

		char response[512];
		sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body ), entity_body );

		if ( DEBUG ) printf( "%s\n", response );

    	write( client_socket_fd, response, strlen( response ) );

		close( client_socket_fd );


	}else {

		exit_status = FAIL;

	}

	if ( DEBUG ) printf("Exit status = %d\n", exit_status );

	return exit_status;

} // end accept_client function
