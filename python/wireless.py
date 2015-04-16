
import time


class WirelessHandler():
    pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]
    min_payload_size = 4
    max_payload_size = 32
    payload_size_increments_by = 1
    next_payload_size = min_payload_size
    # inp_role = '0'                                                          # receiver role
    millis = lambda: int(round(time.time() * 1000))

    def __init__(self, radio):
        self.run = True

        ########### USER CONFIGURATION ###########
        # See https://github.com/TMRh20/RF24/blob/master/RPi/pyRF24/readme.md

        # CE Pin, CSN Pin, SPI Speed
        # Note: pin numbers are BOARD HEADER pin numbers

        #RPi B+
        # Setup for GPIO 22 CE and CE0 CSN for RPi B+ with SPI Speed @ 8Mhz
        # radio = RF24(RPI_BPLUS_GPIO_J8_15, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)
        self.radio = radio
        ##########################################

        print 'Initialising radio...'
        self.radio.begin()
        self.radio.enableDynamicPayloads()
        self.radio.setRetries(5, 15)
        self.radio.printDetails()

        print 'Receiver mode, awaiting transmission'
        self.radio.openWritingPipe(self.pipes[1])
        self.radio.openReadingPipe(1, self.pipes[0])
        self.radio.startListening()

    def listen(self, payloadProcessor):
        self.run = True
        # forever loop
        while self.run:
            # if there is data ready
            if self.radio.available():
                while self.radio.available():
                    # Fetch the payload, and see if this was the last one.
                    len = self.radio.getDynamicPayloadSize()
                    receive_payload = self.radio.read(len)

                    # Spew it
                    print 'Got payload size=', len, ' value="', receive_payload, '"'

                # First, stop listening so we can talk
                self.radio.stopListening()

                # Send the final one back.
                self.radio.write(receive_payload)
                print 'Sent response.'

                # Process the payload
                payloadProcessor(receive_payload)

                # Now, resume listening so we catch the next packets.
                self.radio.startListening()

    def stop(self):
        self.run = False
