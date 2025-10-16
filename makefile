ignore_time:
	touch space_program/ -a
	touch drumbot/ -a
space_program: space_program_c ignore_time
	mkdir -p .build/
	arduino-cli upload -b esp32:esp32:esp32c6 -p dev/ttyACM0 --build-path .build/
space_program_c:
	mkdir -p .build/
	arduino-cli compile ./space_program/ -b esp32:esp32:esp32c6 -p dev/ttyACM0 --build-path .build/
drumbot: drumbot_c ignore_time
	mkdir -p .build/
	arduino-cli upload -b esp32:esp32:esp32c6 -p dev/ttyACM0 --build-path .build/
drumbot_c:
	mkdir -p .build/
	arduino-cli compile ./drumbot/ -b esp32:esp32:esp32c6 -p dev/ttyACM0 --build-path .build/
clean:
	rm -R ./.build/
