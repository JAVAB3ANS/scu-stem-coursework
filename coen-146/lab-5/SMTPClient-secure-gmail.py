# Behnam Dezfouli
#  CSEN, Santa Clara University

# This program implements a simple smtp mail client to send an email to a local smtp server
# the program runs the ping command, and emails the result using smtp.gmail.com

# NOTE: Do not forget to allow login from less secure apps in your gmail account. Otherwise gmail will complain about username and password.
 
import smtplib, ssl
import subprocess

# 1: Allow Two-Factor Authentication (2FA) on your Google Mail account
# 2: Generate an app password where it says "App Passwords" below "2-Step Verification"

port = 465  # For SSL
email_address = input("Please enter your email address: ")
password = input("Please enter your password: ")
receiver_email = input("Please enter receiver's email address: ")
message = """\
Subject: Hi there

This message is sent from Python."""
 
# ping google.com and save the result
# STUDENT WORK

ping_result = subprocess.check_output("ping -c 1 google.com", shell=True)
print("\nPing result: ", ping_result) 

print("\nNow contacting the mail server...")
# STUDENT WORK
 
print("\nSending email...")

# STUDENT WORK

# Create a secure SSL context

server = smtplib.SMTP_SSL("smtp.gmail.com", port)
server.login(email_address, password)
server.sendmail(email_address, receiver_email, message)

print("\nEmail sent!")