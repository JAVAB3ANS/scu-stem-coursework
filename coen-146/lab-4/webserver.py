# Behnam Dezfouli
# CSEN, Santa Clara University

# This program implements a simple web server that serves html and jpg files

# Input arguments:
# argv[1]: Server"s port number

import socket    # Import socket module
import sys       # To terminate the program

if __name__ == "__main__":

    # check if port number is provided
    if len(sys.argv) != 2:
        print("Usage: python %s <port number>" % (sys.argv[0]))
        sys.exit() 
    
    # STUDENT WORK
    port_number = (int)(sys.argv[1])

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_socket.bind(("", port_number))
    server_socket.listen(1)  
 
    # Server should be up and running and listening to the incoming connections
    while True:
        connectionSocket, addr = server_socket.accept()
        print("The server is ready to respond to incoming requests...")

        # gets the request from the client
        message = connectionSocket.recv(1024)
  
        # TypeError: argument should be integer or bytes-like object, not 'str'
        message = message.decode("utf-8")
        print(message)

        # STUDENT WORK

        # get file name from the request

        start_f = message.find("/")
        end_f = message.find(" HTTP/1.1")

        file_name = message[start_f + 1: end_f]

        # STUDENT WORK

        # get file extension from the request 
        
        start_index = message.find(".")
        end_index = message.find(" HTTP/1.1") 

        file_extension = message[start_index + 1: end_index]
        print(file_extension)
 
        try:

            # STUDENT WORK 
            if (file_extension == "html"):
                response_headers = { "Content-Type": "text/html; encoding=utf8" }
            elif (file_extension == "jpg"):
                response_headers = { "Content-Type": "image/jpeg; encoding=utf8" }
            else:
                print("Invalid file type, we only support html and jpg!")
                continue 

            # fix TypeError: a bytes-like object is required, not 'str'
 
            connectionSocket.send("HTTP/1.1 200 OK\n".encode("utf-8"))
  
            f = open(file_name, "r")
            content = f.read()
            connectionSocket.send(content)
                 
        # Raised when an I/O operation (such as a print statement, the built-in open() function or a method of a file object) fails for an I/O-related reason, e.g., "file not found" or "disk full"
        except IOError:
            # STUDENT WORK 
            connectionSocket.send("HTTP/1.1 404 Not Found\n" + "Content-Type: text/html; encoding=utf8\n\r\n")
            connectionSocket.send("<html><body><h1>404 ERROR</h1>,</body></html>")

        connectionSocket.close()

# STUDENT WORK