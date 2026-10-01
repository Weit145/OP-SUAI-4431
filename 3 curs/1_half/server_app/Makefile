.DEFAULT_GOAL := all

MVN := mvn

.PHONY: all run test build clean help

all: test run

run:
	$(MVN) spring-boot:run

test:
	$(MVN) test

build:
	$(MVN) clean package

clean:
	$(MVN) clean

help:
	@echo "make        - run tests and start HTTPS server on port 8443"
	@echo "make run    - start HTTPS server on port 8443"
	@echo "make test   - run tests"
	@echo "make build  - build the executable JAR"
	@echo "make clean  - remove Maven build files"
