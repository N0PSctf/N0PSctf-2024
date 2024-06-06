FROM python:3

WORKDIR /app

COPY requirements.txt ./

COPY .env ./

RUN pip install --no-cache-dir -r requirements.txt

COPY joe_dohn.py ./

CMD [ "python", "./joe_dohn.py" ]