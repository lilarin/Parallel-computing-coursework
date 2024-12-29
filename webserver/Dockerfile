FROM python:3.12-slim

ENV PYTHONUNBUFFERED 1

WORKDIR /webserver

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

ENV PYTHONPATH=/

EXPOSE 8000

CMD ["uvicorn", "webserver.main:app", "--host", "0.0.0.0", "--port", "8000"]
