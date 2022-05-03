/*
	Ici sont définis les messages d'erreurs par défault
	(sans le début version http "HTTP/X.Y")
*/

#define ERROR_400 " 400 Bad Request\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>400 Bad Request - YOU SUCKA</h1>\
<img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/f/fb/718smiley.svg/512px-718smiley.svg.png\"> <img src=\"https://http.cat/400\"> </body></html>"

#define ERROR_404 " 404 Not Found\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>404 Not Found</h1>\
<img src=\"https://http.cat/404\"> </body></html>"

#define ERROR_403 " 403 Forbidden\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>403 Forbidden</h1>\
<img src=\"https://http.cat/403\"> </body></html>"

#define ERROR_405 " 405 Method Not Allowed\r\nAllow: GET,HEAD\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>405 Method Not Allowed</h1>\
<img src=\"https://http.cat/405\"> </body></html>"

#define ERROR_414 " 414 Request-Uri Too Long\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>414 Request-Uri Too Long</h1>\
<img src=\"https://http.cat/414\"> </body></html>"

#define ERROR_500 " 500 Internal Server Error\r\nContent-Type: text/html\r\nAllow:GET,HEAD\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>500 Internal Server Error</h1>\
<img src=\"https://http.cat/500\"> </body></html>"

#define ERROR_501 " 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>501 Not Implemented</h1>\
<img src=\"https://http.cat/501\"> </body></html>"

#define ERROR_505 " 505 HTTP Version Not Supported\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>505 HTTP Version Not Supported</h1>\
<img src=\"https://http.cat/505\"> </body></html>"


#define TAB_ERROR (char*[]){"",ERROR_400,ERROR_403,ERROR_404,ERROR_405,ERROR_414,ERROR_500,ERROR_501,ERROR_505}
