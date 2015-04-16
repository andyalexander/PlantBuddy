# Import modules
import time
# import sys
# import serial                                               # allows us to use the serial port directly

from RF24 import *                                          # load the RF24 drivers
import threading

import resultDAO
import alertMail
import wireless
import struct
import results

FMT = '<cifL'

# These are email address that you want to send alerts TO. Email addresses are stored in an array.
# emailList = ['andrew@homeemail.me.uk']

RADIO = RF24(RPI_BPLUS_GPIO_J8_22, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)

# Initialise the handlers
results.Results.DAO = resultDAO.ResultDAO()

# nodeCache = RESULTDAO.loadNodes()


def dataCallback(data):
    time.sleep(1); #Sleep for 1 sec to prevent node spamming (if a node goes crazy)

    # nodeCache = RESULTDAO.loadNodes()                     # Load the nodes

    myResult = results.Results(data)
    myResult.sendAlerts()
    myResult.save()


WIRELESS = wireless.WirelessHandler(RADIO)
# WIRELESS.listen(dataCallback)
wirelessThread = threading.Thread(target=WIRELESS.listen(dataCallback))
wirelessThread.start()
