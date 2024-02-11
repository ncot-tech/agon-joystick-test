### Agon Light Joystick Test

Test for the Agon Light C Compiler and Joystick

This uses the GPIO pins on the eZ80 as joystick ports.
Depending on your Agon you will need to set the bits correctly, probably through trial and error.

Note - if you want to use the UART as well, you can't use GPIO_PC0 and GPIO_PC1 as they are the UART TX/RX lines.
This poses a bit of a problem on the Console8.

James Grimwood 11/2/2024
james@ncot.uk
