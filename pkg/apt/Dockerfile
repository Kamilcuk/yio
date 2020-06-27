ARG FROM
FROM ${FROM}
RUN \
	DEBIAN_FRONTEND=noninteractive apt update -y && \
	DEBIAN_FRONTEND=noninteractive apt install -y build-essential vim gpg m4 cmake \
		gcc git ca-certificates curl libunistring-dev ninja-build reprepro \
		git-extras devscripts \
	&& \
	rm -rf /var/lib/apt/lists/*



