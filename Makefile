.PHONY: all
all:
	idf.py build flash monitor
clean:
	cat .gitignore | xargs rm -rf