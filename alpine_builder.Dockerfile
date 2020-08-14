FROM alpine
RUN apk add \
		alpine-sdk sudo gnupg bash monkeysphere libunistring-dev git atools cmake m4
SHELL ["/bin/bash", "-c"]

