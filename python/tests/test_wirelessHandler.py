from unittest import TestCase
from mock import MagicMock

import wireless

__author__ = 'Andrew'


class TestWirelessHandler(TestCase):
    # sideeffect for the available method
    def setAvailable(self):
        self.radio.available.side_effect = [True, True, None, None]

    def setUp(self):
        self.radio = MagicMock()
        self.radio.getDynamicPayloadSize.return_value = 5
        self.radio.read.return_value = 'ABCDE'
        self.setAvailable()

    # callback fired when there is data
    def myCallback(self, msg):
        print 'Callback fired: ' + msg
        self.setAvailable()
        self.myListener.stop()

    # main test method
    def testListener(self):
        self.myListener = wireless.WirelessHandler(self.radio)

        self.myListener.listen(self.myCallback)

