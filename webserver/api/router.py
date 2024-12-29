from fastapi import APIRouter

from webserver.api.index import router as index_router

router = APIRouter()

router.include_router(index_router, tags=["Index"])
