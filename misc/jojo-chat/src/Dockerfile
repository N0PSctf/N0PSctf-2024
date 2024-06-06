FROM ubuntu:latest

WORKDIR /app

RUN apt-get update && apt-get install ncat python3 -y

COPY ./src/main.py ./main.py
COPY ./src/admin.py ./admin.py
COPY ./src/log/* ./log/

EXPOSE 1234

CMD ncat -l -p 1234 --sh-exec "python3 /app/main.py" --keep-open