from __future__ import print_function
import httplib2
import sys

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


def cancel():
    __cancel_order_path = (sys.argv[0])[:len(sys.argv[0])-15] + 'cancel_order.txt'

    try:
        with open(__cancel_order_path, "rt") as file:
            lines = file.readlines()

        message = sendInst.create_message('cakebotcpp@gmail.com', 'merchantemail2000@gmail.com', 'Cake Order Cancellation',
                                      'Cake order with Order ID = ' + str(
                                          lines[0]) + " is to be cancelled as requested by user with Email ID = " + str(
                                          lines[1]), lines[1])

        sendInst.send_message('me', message)

        rewrite = open(__cancel_order_path, "wt")
        rewrite.write("")
        rewrite.close()

    except Exception as exp:
        print("Errors.....reason : " + str(exp))


if __name__ == '__main__':
    cancel()

