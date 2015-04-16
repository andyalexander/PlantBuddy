from unittest import TestCase
import resultDAO
import collections

__author__ = 'Andrew'


class TestResultHandler(TestCase):
    def test_saveResult(self):
        print 'Writing test result set:'
        handler = resultDAO.ResultHandler()

        testResultSet = collections.namedtuple('testResultSet', 'errorLevel, humidity, temp, moisture, voltage, nodeId')
        temp = testResultSet(1234, 1, 1, 1, 1, 1)

        handler.saveResult(temp)

        print 'done\n'
