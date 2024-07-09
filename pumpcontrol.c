import board
import digitalio
import time

# Set up pins
hx710_out = digitalio.DigitalInOut(board.D2)  # HX710 OUT pin
hx710_sck = digitalio.DigitalInOut(board.D3)  # HX710 SCK pin

hx710_out.direction = digitalio.Direction.INPUT  # Set as input
hx710_sck.direction = digitalio.Direction.OUTPUT  # Set as output

relaypin = digitalio.DigitalInOut(board.D0)
relaypin.direction = digitalio.Direction.OUTPUT
relaypin.value = True

# Set up serial console
print("Serial console started")

dropstith_low_point = 10000000
dropstith_high_point = 10500000
motor = "ON"

while True:
    # Wait for current reading to finish
    while hx710_out.value:
        pass

    # Read 24 bits
    result = 0
    for i in range(24):
        hx710_sck.value = True
        hx710_sck.value = False
        result <<= 1
        if hx710_out.value:
            result += 1

    # Get 2's complement
    result ^= 0x800000

    # Pulse clock line 3 times to start next pressure reading
    for i in range(3):
        hx710_sck.value = True
        hx710_sck.value = False

    # Display pressure
    print(motor+str(result))

    if result > dropstith_high_point:
        motor = "OFF"
        relaypin.value = False
    if result < dropstith_low_point:
        motor = "ON"
        relaypin.value = True

