__author__ = 'Andrew'

import sqlite3 as sqlite

class ResultDAO():
    def __init__(self):
        # Database setup
        self.CON = sqlite.connect('data.db')

    # Loads all the id and aliases from nodes table into array
    def loadNodes(self):
        with self.CON:
            cur = self.CON.cursor()
            cur.execute("SELECT id, alias FROM nodes")
            nodes = cur.fetchall()
            return nodes

    def saveResult(self, data):
        # Insert sensor node data into the database
        datasql = "INSERT INTO readings(nodeID, obsDate, moisture, temperature, humidity, voltage, errorCode) VALUES ({}, CURRENT_TIMESTAMP, {}, {}, {}, {}, {})".format(data.nodeId, data.moisture, data.temp, data.humidity, data.voltage, data.errorLevel)
        print 'Executing query: ' + datasql

        with self.CON:
            cur = self.CON.cursor()
            cur.execute(datasql)
            self.CON.commit()

    def getLastResult(self, nodeId):
        datasql = "SELECT "