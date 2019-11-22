from __future__ import print_function
import os

from oauth2client import tools
from googleapiclient.errors import HttpError
from email.mime.application import MIMEApplication

import base64
from email.mime.text import MIMEText
from email.mime.audio import MIMEAudio
from email.mime.image import MIMEImage
# from email.mime.image import MIMEImageprint_function
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
import mimetypes

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()

except ImportError:
    flags = None

class send_email:

    def __init__(self,service):
        self.service = service

    def create_message(self, sender, to, subject, message_text, cc=None):
        message = MIMEText(message_text, "html")
        message['to'] = to
        message['from'] = sender
        message['subject'] = subject
        if cc is not None:
            message['cc'] = cc
        return {'raw': base64.urlsafe_b64encode(message.as_bytes()).decode("utf-8")}

    def create_message_with_attachment(self, sender, to, subject, message_text, file, cc=None):
        message = MIMEMultipart()
        message['to'] = to
        message['from'] = sender
        message['subject'] = subject
        if cc is not None:
            message['cc'] = cc

        msg = MIMEText(message_text, "html")
        message.attach(msg)

        content_type, encoding = mimetypes.guess_type(file)

        if content_type is None or encoding is not None:
            content_type = 'application/octet-stream'

        main_type, sub_type = content_type.split('/', 1)

        if main_type == 'text':
            fp = open(file, 'rb')
            msg = MIMEText(fp.read(), _subtype=sub_type, _charset='utf-8')
            fp.close()

        elif main_type == 'image':
            fp = open(file, 'rb')
            msg = MIMEImage(fp.read(), _subtype=sub_type)
            fp.close()

        elif main_type == 'audio':
            fp = open(file, 'rb')
            msg = MIMEAudio(fp.read(), _subtype=sub_type)
            fp.close()

        else:
            fp = open(file, 'rb')
            msg = MIMEApplication(fp.read(),'pdf')
            fp.close()

        filename = os.path.basename(file)
        msg.add_header('Content-Disposition', "fp; filename="+filename)

        message.attach(msg)

        #print(base64.urlsafe_b64encode(message.as_bytes()).decode())

        return {'raw': base64.urlsafe_b64encode(message.as_bytes()).decode()}

    def send_message(self, user_id, message):
        try:
            message = (self.service.users().messages().send(userId=user_id, body=message).execute())
            print('Message Id: %s' % message['id'])
            return message

        except HttpError as error:
            print('An error occurred: %s' % error)
