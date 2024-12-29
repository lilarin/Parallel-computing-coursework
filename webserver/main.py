from fastapi import FastAPI, Response

from webserver.api.router import router

app = FastAPI()

app.include_router(router, prefix="/api")
