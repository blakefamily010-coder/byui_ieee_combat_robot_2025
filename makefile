ignore_time:
	touch space_program/ -a
	touch drumbot/ -a
space_program: ignore_time
	arduino-cli upload ./space_program/ -b esp32:esp32:esp32c6 -p dev/ttyACM0
drumbot: ignore_time
	arduino-cli upload ./drumbot/ -b esp32:esp32:esp32c6 -p dev/ttyACM0
