# NewRFRam

This is a sketch for your generic Arduino UNO that does some funky stuff with
the Adafruit SPI FRAM breakout board and the RFM96 LoRa radio board.

More specificially, it allows you to write random values to the first 128
bytes of RAM, transmit said bytes over the air, and do other funky stuff.

#####WARNING

This sketch uses the 70cm ham band in the US, and transmitting on this band
without a license may make the FCC mad at you, and also make boomers complain
about you clogging the airwaves. Granted, I checked the laws regarding spread
spectrum transmissions, and the band plan info I could find, and picked a neat
frequency, but BASICALLY don't transmit without a valid ham radio license.
Unless you have the RFM95, in which case transmit to your heart's content.
