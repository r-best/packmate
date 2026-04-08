.PHONY: upload
upload:
	./scripts/upload.sh

.PHONY: serial
serial:
	minicom -b 115200 -o -D /dev/ttyACM0
