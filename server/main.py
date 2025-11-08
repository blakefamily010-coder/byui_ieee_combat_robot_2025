import pygame
import requests
import time

ESP32_IP_ADDR = "http://192.168.0.0"  # should be the address of a router
UPDATE_INTERVAL_S = 0.1

pygame.init()
# pygame.joystick.init()

while pygame.joystick.get_count() == 0:
    print('.')
    time.sleep(UPDATE_INTERVAL_S)

controller = pygame.joystick.Joystick(0)

print(controller.get_numaxes())
print(controller.get_numbuttons())


def send(route):
    """Send a GET request to ESP32 route, ignoring the response."""
    url = f"{ESP32_IP_ADDR}{route}"
    try:
        requests.get(url, timeout=0.2)
        print(f"sent {url}")
    except requests.RequestException:
        print(f"failed to send {url}")


check = False
while True:
    # TODO: toggle logic goes here
    #
    for n in range(16):
        b = controller.get_button(0)
        check |= b
    print(check)

    # if controller.get_button(0):
    # send("/a_button")
    # else:
    # send("/a_button_off")

    time.sleep(UPDATE_INTERVAL_S)
