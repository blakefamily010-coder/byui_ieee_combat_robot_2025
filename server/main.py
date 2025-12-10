# import pygame
from inputs import get_gamepad
from inputs import devices
import requests
import time

ESP32_IP_ADDR = "http://10.57.236.200"  # should be the address of a router
SEND_WINDOW_S = 0.1
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
ltrig = 0
button_triggered = False
button_time = float("inf")
gamepad = devices.gamepads[0];
events = gamepad.read()
send_timer = 0.0
while True:
    timer = time.clock_gettime(time.CLOCK_MONOTONIC);
    if button_triggered:
        send("control/a_button")
        button_triggered = False
        button_time = timer + 0.7
    if (timer > button_time):
        button_time = float("inf")
        send("control/a_button_off")

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
    for event in gamepad.read():
        # TODO: ensure that routes only get called once per loop
        #
        # print(event.ev_type, event.code, event.state)
        match event.code:
            case "ABS_X":
                lstickx = event.state
                # send(f"control/lstick/0x{lstickx:x}/0x{lsticky:x}")
            case "ABS_Y":
                lsticky = event.state
                # send(f"control/lstick/0x{lstickx:x}/0x{lsticky:x}")
            case "ABS_RX":
                rstickx = event.state
                # send(f"control/rstick/0x{rstickx:x}/0x{rsticky:x}")
            case "ABS_RY":
                rsticky = event.state
                # send(f"control/rstick/0x{rstickx:x}/0x{rsticky:x}")
            case "ABS_Z":
                ltrig = event.state
                # send(f"control/l_trigger/0x{ltrig:x}")

            # case "ABS_HAT0X":
            #     match event.state:
            #         case 1:
            #             send("")
            #         case -1:
            #         case 0:
            case "BTN_SOUTH":
                if event.state:
                    button_triggered = True
                    # send("control/a_button")
                else:
                    pass
                    # send("control/a_button_off")
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
            # case "BTN_TL":
    timer = time.clock_gettime(CLOCK_MONOTONIC)
    if send_timer >= timer:
        send_timer = timer + SEND_WINDOW_S
        ltrig /= 16
        lstickx /= 16
        lsticky /= 16
        rstickx /= 16
        rsticky /= 16
        send(f"control/l_trigger/{ltrig}")
        send(f"control/lstick/{lstickx}/{lsticky}")
        send(f"control/rstick/{rstickx:x}/{rsticky}")
        send("connect")

    # time.sleep(UPDATE_INTERVAL_S)
