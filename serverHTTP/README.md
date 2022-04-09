===================================================================================
This is a simple example that create a server on port 8080:
- Listen for incomming request
- parse it to verify syntax of the request, if OK get HTTP-message portion of the request (the whole request)
then send back to the client the HTTP-message following a 200 OK
- if syntax error send back 400 error


===================================================================================


**** compile main.c with:

gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -o main main.c -lparser -lrequest


where:  -I points to include directory
	-L points to libraries directory

**** launch it with:

export LD_LIBRARY_PATH=.
./server

:Comm: export LD_LIBRARY_PATH=../libnotreparser/:../librequest-0.5/ <--bypass symbolic links

**** Server will isten on 8080 TCP port.
**** Server will wait for a request
**** Server will analyse the request and
****** If the request's syntax is ok answer as an echo server
****** If the request's sytax is not OK will answer a error response

**** And enjoy getting a 200 OK response, when you send it a  HTTP request with a remote client ==>

	firefox http://127.0.0.1:8080/
OR...
	nc -C 127.0.0.1 8080
