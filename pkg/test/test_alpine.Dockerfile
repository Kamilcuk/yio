FROM alpine
RUN set -x && \
		apk add wget tar cmake make gcc git bash build-base

