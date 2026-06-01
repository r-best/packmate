.PHONY: upload
upload:
	./scripts/upload.sh

.PHONY: upload-sprites
upload-sprites:
	./sprites/scripts/convert_all.sh
	rm -rf /media/bobby/PACKMATE/sprites/
	rsync -av --include='*/' --include='*.rgb565' --exclude='*' ./sprites/ /media/bobby/PACKMATE/sprites/
	udisksctl unmount -b /dev/disk/by-label/PACKMATE
	udisksctl power-off -b /dev/disk/by-label/PACKMATE

.PHONY: serial
serial:
	minicom -b 115200 -o -D /dev/ttyACM0
