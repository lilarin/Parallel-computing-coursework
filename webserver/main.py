from fastapi import FastAPI

from webserver.api.router import router

app = FastAPI()

app.include_router(router, prefix="/api")
