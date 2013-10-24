import time
import RPi.GPIO as io
io.setmode(io.BCM)

pir_pin = 1

io.setup(pir_pin, io.IN)         # activate input

while True:
    if io.input(pir_pin):
        print("PIR ALARM!")
    time.sleep(0.5)
