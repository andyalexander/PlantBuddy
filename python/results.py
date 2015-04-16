__author__ = 'Andrew'
import threading
import alertMail


class Results():
    # ERROR MESSAGES
    ERROR_MOISTURE = "Plant needs watering ERROR! \r\n"
    ERROR_TEMPERATURE = "Temperature ERROR!  \r\n"
    ERROR_HUMIDITY = "Humidity ERROR!  \r\n"
    ERROR_VOLTAGE = "Battery low ERROR! \r\n"

    DAO = None

    def setDAO(self, resultDAO):
        Results.DAO = resultDAO

    def __init__(self, parseString):

        # Our data format uses colon to deliminate data types. We split them into an array here.
        nodedata = parseString.strip().split(':')
        nodedata = [x.strip('\x00') for x in nodedata]

        self.nodeId = int(nodedata[0])

        # # Grab the Alias from the cached index
        # alias = NodeCache[idex][1]
        # aliasID = alias + str(NodeID)
        # aliasID = aliasID.strip()
        # aliasID = aliasID.replace(" ", "")

        # Break data out from array
        self.errorLevel = nodedata[1]
        self.moisture = nodedata[2]
        self.temp = nodedata[3]
        self.humidity = nodedata[4]
        self.voltage = nodedata[5]

    def toString(self):
        return 'ID: {0} | Error: {1} | Moisture: {2} | Temp: {3} | Humidity: {4} | Voltage: {5}'.format(self.nodeId, self.errorLevel, self.moisture, self.temp, self.humidity, self.voltage)

    def getErrorMessage(self):
        # Check if the sensor node marked an error. Check the type of error. Generate message.
        if int(self.errorLevel) > 0:
            msg = ""

            if "1" in self.errorLevel:
                msg = msg + Results.ERROR_MOISTURE

            if "2" in self.errorLevel:
                msg = msg + Results.ERROR_TEMPERATURE

            if "3" in self.errorLevel:
                msg = msg + Results.ERROR_HUMIDITY

            if "4" in self.errorLevel:
                msg = msg + Results.ERROR_VOLTAGE

        return msg

    def sendAlerts(self):
        # Send out email. This gets spawned as a thread so it doesn't block the rest of the program.
        print 'Sending alert mail: ' + self.getErrorMessage()
        # mailThread = threading.Thread(target=alertMail.alert_mail(self.getErrorMessage()))
        # mailThread.start()

    def save(self):
        print 'Saving: ' + self.toString()
        Results.DAO.saveResult(self)
