NAME := bomberman

all: 
	sh scripts/build-pack.sh

debug: 
	sh scripts/build-debug.sh

release:
	sh scripts/build-release.sh

clean:
	rm -rf build

fclean: clean
	rm -rf bomberman.app
	rm -rf bomberman.dmg
	
re: fclean all

.PHONY: all, clean, fclean, re
.NOTPARALLEL: all, clean, fclean, re
.SILENT:
