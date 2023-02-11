import socket

server_socket =  socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.bind(("", 8013))
server_socket.listen(1)

while True:
    connectionSocket, addr = server_socket.accept()

    message = connectionSocket.recv(1024)

    connectionSocket.send("HTTP/1.1 200 OK\n".encode('utf-8'))
    connectionSocket.send('\r\n'.encode())
    connectionSocket.send("<html><body><h1>HELLO WORLD</h1>,</body></html>".encode());

    connectionSocket.close()