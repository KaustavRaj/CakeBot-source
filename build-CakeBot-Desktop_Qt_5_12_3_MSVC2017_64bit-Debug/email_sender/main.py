from __future__ import print_function
import httplib2
import sys
import os

from apiclient import discovery
from oauth2client import tools

import auth
import send_email

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()

except ImportError:
    flags = None


def get_labels():
    results = service.users().labels().list(userId='me').execute()
    labels = results.get('labels', [])

    if not labels:
        print('No labels found.')
    else:
        print('Labels:')
        for label in labels:
            print(label['name'])

SCOPES = 'https://mail.google.com/'
CLIENT_SECRET_FILE = 'client_secret.json'
APPLICATION_NAME = 'Email_Sender'
authInst = auth.auth(SCOPES, CLIENT_SECRET_FILE, APPLICATION_NAME)
credentials = authInst.get_credentials()

http = credentials.authorize(httplib2.Http())
service = discovery.build('gmail', 'v1', http=http)

# For sending email

sendInst = send_email.send_email(service)

userID = ""
__path = (sys.argv[0])[:len(sys.argv[0])-7] + 'userdata.txt'
__email = (sys.argv[0])[:len(sys.argv[0])-7] + 'useremail.txt'


try:
    userID = open(__path, "rt")

except Exception as e:
    print("File opening error.....reason : " + str(e))

try:
    filename = userID.read()
    userID.close()
    rewriteFile = open(__path, "wt")
    rewriteFile.write("")
    # print(filename)
    getEmail = open(__email)
    user_email = getEmail.read()
    if user_email == "":
        user_email = None
        getEmail.close()
    else:
        getEmail.close()
        rewriteFile = open(__email, "wt")
        rewriteFile.write("")
        rewriteFile.close()

    html_order_detail_text = open(filename, "rt").read()

    html_opening_text = """
                        <h1 style="text-align: left;">
                            <span style="text-decoration: underline; color: #800080;">
                                <strong>CakeBot</strong>
                            </span>
                        </h1>
                        <p style="text-align: left;">Your recent cake order has been booked successfully .
                            Please find the order details below.
                        </p>
                        <p style="text-align: left;">&nbsp;</p>
                        <p style="text-align: left;">&nbsp;</p>
                        <hr />
                        """
    html_ending_text = """
                        <p style="text-align: left;">
                            <em>This is a system generated mail. Please do not reply.</em>
                        </p>
                        """

    html_message_text = html_opening_text + str(html_order_detail_text) + html_ending_text

    message = sendInst.create_message('cakebotcpp@gmail.com', 'merchantemail2000@gmail.com',
                                                      'New Cake Order',
                                                      html_message_text,
                                                      user_email)

    sendInst.send_message('me', message)

except Exception as exp:
    print("Errors.....reason : " + str(exp))

