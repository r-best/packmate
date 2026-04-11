.PHONY: upload
upload:
	./scripts/upload.sh

.PHONY: upload-sprites
upload-sprites:
	cp sprites/*.rgb332 /media/bobby/1220-094F/sprites/

.PHONY: serial
serial:
	minicom -b 115200 -o -D /dev/ttyACM0
