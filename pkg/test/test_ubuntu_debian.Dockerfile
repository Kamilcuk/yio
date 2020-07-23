ARG FROM
FROM $FROM
RUN set -x && \
		DEBIAN_FRONTEND=noninteractive apt update && \
		DEBIAN_FRONTEND=noninteractive apt install -y sudo gnupg ca-certificates git cmake make gcc

