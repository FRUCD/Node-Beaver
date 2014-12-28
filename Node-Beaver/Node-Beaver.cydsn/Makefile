CMD=/cygdrive/c/Program\ Files\ \(x86\)/Cypress/PSoC\ Creator/3.0/PSoC\ \
Creator/bin/cyprjmgr.exe

SRC="../Node-Beaver.cywrk"

FLAGS= -wrk $(SRC) -warn High

.PHONY: all build rebuild clean



# RULES

all: build 

build:
	$(CMD) $(FLAGS) -build

rebuild:
	$(CMD) $(FLAGS) -rebuild

clean:
	$(CMD) $(FLAGS) -clean
