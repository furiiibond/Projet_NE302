
#define ERROR_200 "HTTP/1.0 200 OK\r\nContent-type:%s\r\nContent-length:%ld\r\n\r\n", file->type, file->length

#define ERROR_400 "HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>400 Bad Request - YOU SUCKA</h1>\
<img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/f/fb/718smiley.svg/512px-718smiley.svg.png\"> <img src=\"https://http.cat/400\"> </body></html>"

#define ERROR_404 "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>404 Not Found</h1>\
<img src=\"https://http.cat/404\"> </body></html>"

#define ERROR_405 "HTTP/1.0 405 Not Found\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>405 Method Not Allowed</h1>\
<img src=\"https://http.cat/405\"> </body></html>"

#define ERROR_500 "HTTP/1.0 500 Internal Server Error\r\nContent-Type: text/html\r\nAllow:GET,HEAD\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>500 Internal Server Error</h1>\
<img src=\"https://http.cat/500\"> </body></html>"

#define ERROR_501 "HTTP/1.0 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>501 Not Implemented</h1>\
<img src=\"https://http.cat/501\"> </body></html>"

#define ERROR_505 "HTTP/1.0 505 HTTP Version Not Supported\r\nContent-Type: text/html\r\n\r\n\
<!DOCTYPE HTML> <html><body> <h1>505 HTTP Version Not Supported</h1>\
<img src=\"https://http.cat/505\"> </body></html>"


#define TAB_ERROR (char*[]){"",ERROR_400,ERROR_404,ERROR_405,ERROR_500,ERROR_501,ERROR_505}
