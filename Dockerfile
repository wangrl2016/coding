FROM ubuntu:20.04

COPY . /app

RUN apt update && \
    apt install -y git vim openjdk-11-jdk

CMD python3 /app/git_sync_deps.py