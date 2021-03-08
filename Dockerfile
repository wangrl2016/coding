FROM ubuntu:20.04

COPY . /app

RUN apt install -y openjdk-11-jdk

CMD python3 /app/git_sync_deps.py