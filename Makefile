.PHONY: upload
upload:
	./scripts/upload.sh

.PHONY: upload-sprites
upload-sprites:
	./sprites/convert_all.sh
	rsync -av --include='*/' --include='*.rgb332' --exclude='*' ./sprites/ /media/bobby/1220-094F/sprites/

.PHONY: serial
serial:
	minicom -b 115200 -o -D /dev/ttyACM0
