IMAGE_NAME = yio-godbolt
PORT = 10240

.PHONY: build run stop clean

build:
	docker build -t $(IMAGE_NAME) -f Dockerfile ..

run: build
	docker run --rm -p $(PORT):$(PORT) --name $(IMAGE_NAME) $(IMAGE_NAME)

stop:
	docker stop $(IMAGE_NAME)

clean:
	docker rmi $(IMAGE_NAME)
