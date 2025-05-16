
FROM python:3.11

ENV DEPLOY=1

WORKDIR /app

COPY . .

RUN pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

EXPOSE 80

CMD ["./start.sh"]
