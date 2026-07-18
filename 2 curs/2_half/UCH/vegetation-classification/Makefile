COMPOSE ?= docker-compose

.DEFAULT_GOAL := help

.PHONY: help build up down restart logs ps

help:
	@echo "Available commands:"
	@echo "  make build    Build backend and frontend images"
	@echo "  make up       Build and start the application"
	@echo "  make down     Stop the application"
	@echo "  make restart  Recreate the application"
	@echo "  make logs     Follow container logs"
	@echo "  make ps       Show container status"

build:
	$(COMPOSE) build

up:
	$(COMPOSE) up -d --build

down:
	$(COMPOSE) down

restart:
	$(COMPOSE) up -d --build --force-recreate

logs:
	$(COMPOSE) logs -f --tail=100

ps:
	$(COMPOSE) ps
