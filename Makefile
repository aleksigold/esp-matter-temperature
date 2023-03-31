.PHONY: all
all:
	idf.py build flash monitor

.PHONY: build
build:
	idf.py build

.PHONY: clean
clean:
	cat .gitignore | xargs rm -rf

.PHONY: erase
erase:
	idf.py erase-flash