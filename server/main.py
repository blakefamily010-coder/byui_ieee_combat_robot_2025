# import pygame
from inputs import get_gamepad
from inputs import devices
import requests
import time

ESP32_IP_ADDR = "http://192.168.0.0"  # should be the address of a router
UPDATE_INTERVAL_S = 0.1
TRIGGER_THRESHOLD = 700

for device in devices:
    print(device)


def send(route):
    # """Send a GET request to ESP32 route, ignoring the response."""
    url = f"{ESP32_IP_ADDR}/{route}"
    try:
        # requests.get(url, timeout=0.2)
        print(f"sent {url}")
    except requests.RequestException:
        print(f"failed to send {url}")


check = False
lstickx = 0
lsticky = 0
rstickx = 0
rsticky = 0
while True:
    # # events
    #
    # ABS_X -> left stick x
    # ABS_Y -> left stick y
    # ABS_RX -> right stick x
    # ABS_RY -> right stick y
    # ABS_Z -> left trigger
    # ABS_RZ -> right trigger
    #
    # ----------------------
    #    dpad layout
    # ----------------------
    #        -1
    #      -1 0 1
    #         1
    # ABS_HAT0X -> dpad x
    # ABS_HAT0Y -> dpad y
    #
    # BTN_SOUTH -> a button
    # BTN_EAST -> b button
    # BTN_NORTH -> x button
    # BTN_WEST -> y button
    #
    # BTN_TL -> left bumper
    # BTN_TR -> right bumper
    #
    for event in get_gamepad():
        # TODO: ensure that routes only get called once per loop
        #
        print(event.ev_type, event.code, event.state)
        match event.code:
            case "ABS_X":
                lstickx = event.state
                send(f"control/{lstickx:x}/{lsticky:x}")
            case "ABS_Y":
                lsticky = event.state
                send(f"control/{lstickx:x}/{lsticky:x}")
            case "ABS_RX":
                rstickx = event.state
                send(f"control/{rstickx:x}/{rsticky:x}")
            case "ABS_RY":
                rsticky = event.state
                send(f"control/{rstickx:x}/{rsticky:x}")
            # case "ABS_HAT0X":
            #     match event.state:
            #         case 1:
            #             send("")
            #         case -1:
            #         case 0:
            case "BTN_SOUTH":
                if event.state:
                    send("control/a_button")
                else:
                    send("control/a_button_off")
            case "BTN_EAST":
                if event.state:
                    send("control/y_button")
                else:
                    send("control/y_button_off")
            case "BTN_NORTH":
                if event.state:
                    send("control/x_button")
                else:
                    send("control/x_button_off")
            case "BTN_WEST":
                if event.state:
                    send("control/y_button")
                else:
                    send("control/y_button_off")
            case "BTN_TL":

    time.sleep(UPDATE_INTERVAL_S)
