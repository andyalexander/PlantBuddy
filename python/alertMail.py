__author__ = 'Andrew'

import smtplib                                                              # allows us to send out emails
import string

# Information for email. This will be used by the Plant Friends system to send out emails.
EMAIL_ADDRESS = 'andrew@homeemail.me.uk'
EMAIL_PASSWORD = 'escher123'
EMAIL_SERVER = 'smtp.gmail.com:587'                                         # Gmail SMTP server address

EMAIL_SUBJECT = 'PlantBuddy Alert'
EMAIL_SENDER = 'PlantBuddy'

emailList = ['andrew@homeemail.me.uk']

# This function is for sending out emails.
def alert_mail(msg):
    for emaildest in emailList:
        EMAIL_BODY = string.join((
            "From: %s" % EMAIL_ADDRESS,
            "To: %s" % emaildest,
            "Subject: %s" % EMAIL_SUBJECT,
            "",
            msg + "\r\nK. thanks.\r\n\r\n- %s" % EMAIL_SENDER,
            ), "\r\n")
        server = smtplib.SMTP(EMAIL_SERVER)
        server.ehlo()
        server.starttls()
        server.login(EMAIL_ADDRESS, EMAIL_PASSWORD)
        server.sendmail(EMAIL_SENDER, [emaildest], EMAIL_BODY)
        server.quit()