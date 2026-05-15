.PHONY: all test format lint check

all: test

test:
	uv run --extra dev pytest

format:
	uv run --extra dev ruff format .

lint:
	uv run --extra dev ruff check --fix .

check:
	uv run --extra dev basedpyright yiofstring.py

clean:
	rm -rf .venv .pytest_cache .ruff_cache
