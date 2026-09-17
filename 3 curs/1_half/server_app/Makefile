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
	@echo "make        - run tests and start the application"
	@echo "make run    - start the application"
	@echo "make test   - run tests"
	@echo "make build  - build the executable JAR"
	@echo "make clean  - remove Maven build files"
