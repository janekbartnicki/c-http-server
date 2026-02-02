FROM gcc:latest

WORKDIR /usr/src/server-c

COPY main.c .

RUN gcc -o main main.c

EXPOSE 8080

CMD ["./main"] 