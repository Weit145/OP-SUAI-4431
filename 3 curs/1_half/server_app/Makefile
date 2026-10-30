.DEFAULT_GOAL := all

MVN := mvn
JAR := target/lab7-real-estate-rental.jar

.PHONY: all run test build clean help kafka run1 run2

kafka:
	docker compose up -d

# Build the missing JAR once; the second replica reuses the same artifact.
$(JAR):
	$(MVN) package -DskipTests

run1: $(JAR)
	java -jar $(JAR) --spring.profiles.active=replication --app.instance-id=instance-1 --server.port=8443

run2: $(JAR)
	java -jar $(JAR) --spring.profiles.active=replication --app.instance-id=instance-2 --server.port=9443

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
	@echo "make kafka; make build - start Kafka and build Lab 7"
	@echo "make run1 / make run2 - replicas on HTTPS ports 8443 / 9443; builds missing JAR automatically"
	@echo "make        - run tests and start HTTPS server on port 8443"
	@echo "make run    - start HTTPS server on port 8443"
	@echo "make test   - run tests"
	@echo "make build  - build the executable JAR"
	@echo "make clean  - remove Maven build files"
