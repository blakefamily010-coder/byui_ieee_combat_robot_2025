ignore_time:
	touch space_program/ -a
	touch drumbot/ -a
	touch server/ -a
space_program: space_program_c ignore_time
	mkdir -p .build/
	arduino-cli upload -b esp32:esp32:esp32c6 -p ttyUSB0 --build-path .build/
space_program_c:
	mkdir -p .build/
	arduino-cli compile ./space_program/ -b esp32:esp32:esp32c6 --build-path .build/
drumbot: drumbot_c ignore_time
	mkdir -p .build/
	arduino-cli upload -b esp32:esp32:esp32c6 -p ttyUSB0 --build-path .build/
drumbot_c:
	mkdir -p .build/
	arduino-cli compile ./drumbot/ -b esp32:esp32:esp32c6 --build-path .build/
clean:
	rm -R ./.build/
server: ignore_time
	python3 server/main.py
minicom:
	minicom -b 9600 -D /dev/ttyUSB0
