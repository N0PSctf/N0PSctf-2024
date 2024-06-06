FROM python:latest

WORKDIR /app

RUN pip install Flask

COPY ./src/ .
RUN useradd app-user

EXPOSE 80

USER app-user

CMD [ "python", "./app.py" ]