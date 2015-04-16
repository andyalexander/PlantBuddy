from unittest import TestCase
import results

__author__ = 'Andrew'


class TestResults(TestCase):
    def testResults(self):
        r = results.Results('1:1234:3:4:5:6')
        print r.toString()
        print r.getErrorMessage()