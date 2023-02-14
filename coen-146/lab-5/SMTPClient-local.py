
# Behnam Dezfouli
#  CSEN, Santa Clara University

# This program implements a simple smtp mail client to send an email to a local smtp server

# Note:
# Run a local smtp mail server using the following command before running this code:
# python -m smtpd -c DebuggingServer -n localhost:6000


from socket import *
import ssl 
 
# Choose a mail server
mailserver = "localhost"
 
# Create socket called clientSocket and establish a TCP connection with mailserver
# STUDENT WORK

clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((mailserver, 6000))
 
# Port number may change according to the mail server
# STUDENT WORK 
 
recv = clientSocket.recv(1024).decode()
print(recv)
if recv[:3] != "220":
    print("220 reply not received from server.")
 
# Send HELLO command along with the server address
# STUDENT WORK

heloCommand = "HELLO bob\r "

clientSocket.send(heloCommand.encode())

recv1 = clientSocket.recv(1024).decode()

print(recv1)

 
# Send MAIL FROM command and print server response
# STUDENT WORK

mailFrom = "MAIL FROM: <bob@localhost>\r "

clientSocket.send(mailFrom.encode())

recv2 = clientSocket.recv(1024).decode()

print(recv2)
 
# Send RCPT TO command and print server response
# STUDENT WORK

rcptTo = "RCPT TO: <alice@localhost>\r "

clientSocket.send(rcptTo.encode())

recv3 = clientSocket.recv(1024).decode()

print(recv3) 

# Send DATA command and print server response
# STUDENT WORK

data = "DATA\r "

clientSocket.send(data.encode())

recv4 = clientSocket.recv(1024).decode()

print(recv4)
 
# Send message data.
# STUDENT WORK

subject = "Subject: SMTP mail client testing\r "

clientSocket.send(subject.encode())

msg = "I love computer networks!\r "

clientSocket.send(msg.encode())

endmsg = "\r . \r "

clientSocket.send(endmsg.encode())

recv5 = clientSocket.recv(1024).decode()

print(recv5)
 
# Message to send
# STUDENT WORK
 
# Message ends with a single period
# STUDENT WORK
 
 
# Send QUIT command and get server response
# STUDENT WORK

quit = "QUIT\r "

clientSocket.send(quit.encode())

recv6 = clientSocket.recv(1024).decode()

print(recv6) 