from unittest import TestCase
from alertMail import alert_mail

__author__ = 'Andrew'


class TestAlert_mail(TestCase):
    def test_alert_mail(self):
        print 'Sending test mail:'
        # alert_mail('test message', ['andrew@homeemail.me.uk'])

        print 'done\n'
