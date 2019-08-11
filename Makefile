NAME := bomberman

all: 
	sh scripts/build-release.sh

debug: 
	sh scripts/build-debug.sh

bundle:
	sh scripts/build-pack.sh

clean:
	rm -rf build

fclean: clean
	rm -rf bomberman.app

re: fclean all

.PHONY: all, clean, fclean, re
.NOTPARALLEL: all, clean, fclean, re
.SILENT:
