===================================================================================
This is a simple example with two cases:
- A - experimental (but not so much) api to write message back to the client, 
you can send message as soon as you have a string to send with "writeDirectClient"
In order to let the librequest know you have nothing more to send, when this is the case,
do not forget to call "endWriteDirectClient". This is mandatory when connection could be 
keealived for e new request (ex: HTTP/1.1 default behavior ro HTTP/1.0 with connection header)

- B - Legacy api to write message back to client, this API is much more annoying to use since
you have to create the whole response (dont forget to malloc) before sending with 
"sendReponse" 


===================================================================================


**** compile main.c with: 

gcc -Wall -g -o sock main.c -L. -lrequest

**** launch it with: 

export LD_LIBRARY_PATH=. 
./sock 

**** And enjoy getting a 200 OK when u send it HTTP request with a remote client ==> 

exemple: 

	echo "GET / HTTP/1.0
	
	" | nc 127.0.0.1 

--> Watch out of CRLF.... 

OR... 

	userir@strecth:~$  nc -C 127.0.0.1 8080
	GET / HTTP/1.0

OR... 
	firefox http://127.0.0.1:8080/ 

to get an exciting white html page. 



