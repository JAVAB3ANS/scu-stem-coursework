
# Behnam Dezfouli
#  CSEN, Santa Clara University

# This program implements a simple smtp mail client to send an email to a local smtp server
# the program runs the ping command, and emails the result using smtp.gmail.com

# NOTE: Do not forget to allow login from less secure apps in your gmail account. Otherwise gmail will complain about username and password.
 
import smtplib, ssl
import subprocess

port = 465  # For SSL
email_address = input("Please enter your email address: ")
password = input("Please enter your password: ")
receiver_email = input("Please enter receiver's email address: ")
 
# ping google.com and save the result
# STUDENT WORK

ping = subprocess.Popen(["ping", "google.com"], stdout=subprocess.PIPE)
ping_result = ping.communicate()[0]
ping_result = ping_result.decode("utf-8")

print("\nNow contacting the mail server...")
# STUDENT WORK
 
print("\nSending email...")

# STUDENT WORK

# Create a secure SSL context

context = ssl.create_default_context()

with smtplib.SMTP_SSL("smtp.gmail.com", port, context=context) as server:
    server.login(email_address, password)
    server.sendmail(email_address, receiver_email, ping_result)

print("\nEmail sent!")