# Behnam Dezfouli
#  CSEN, Santa Clara University

# This program implements a simple smtp mail client to send an email to a local smtp server

# Note:
# Run a local smtp mail server using the following command before running this code:
# python -m smtpd -c DebuggingServer -n localhost:6000

from socket import *
import ssl

# Choose a mail server
mailserver = 'localhost'

# Create socket called clientSocket and establish a TCP connection with mailserver
clientSocket = socket(AF_INET, SOCK_STREAM)

# Port number may change according to the mail server
clientSocket.connect(("", 6000))

recv = clientSocket.recv(1024).decode()
print(recv)
if recv[:3] != '220':
   print('220 reply not received from server.')

# Send HELO command along with the server address

clientSocket.send('HELO localhost.com\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)

# Send MAIL FROM command and print server response

clientSocket.send('MAIL FROM: <MrBeast@localhost.com>\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)

# Send RCPT TO command and print server response

clientSocket.send('RCPT TO: <jvu@scu.edu>\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)

# Send DATA command and print server response

clientSocket.send('DATA\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)

# Send message data.

clientSocket.send('354\r\n'.encode())
clientSocket.send('Date: Wed, 30 July 2019 06:04:34\r\n'.encode())
clientSocket.send('From: MrBeast@gmail.com\r\n'.encode())
clientSocket.send('Subject: Please join us on Squid Game 2!\r\n'.encode())
clientSocket.send('To: jvu@scu.edu\r\n'.encode())

# Message to send

clientSocket.send('''Yo yo yo it's ya boi Jimmy Beast.
\nActually JK this email is from SMTPClient-local.py. PRANKED.
\nLike, comment, subscribe,
\n"Mr.Beast"'''.encode())

# Message ends with a single period
clientSocket.send('\r\n.\r\n')

# Send QUIT command and get server response
clientSocket.send('QUIT\r\n')
recv = clientSocket.recv(1024).decode()

if recv[:3] != '221':
   print('221 reply not received from server.')

clientSocket.close() 