.DEFAULT = all
.PHONY = clean

all:
	cd src/server; $(MAKE)
	cd src/client; $(MAKE)

clean:
	cd src/server; $(MAKE) clean
	cd src/client; $(MAKE) clean
