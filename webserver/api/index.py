from fastapi import APIRouter, Query

from client.client import client
from webserver.schemas import ServerResponse, UploadFile

router = APIRouter()


@router.post("/upload")
async def upload_file(request: UploadFile) -> ServerResponse:
    filename = request.filename
    content = request.content

    status, data = await client.upload_file(filename, content)
    return ServerResponse(
        status_code=status,
        response=data,
    )


@router.get("/search")
async def search_files(
        content: str = Query(..., description="The content to search")
) -> ServerResponse:
    status, data = await client.search_files(content)
    return ServerResponse(
        status_code=status,
        response=data,
    )


@router.delete("/delete")
async def delete_file(
        file_name: str = Query(..., description="The filename to delete from index")
) -> ServerResponse:
    status, data = await client.delete_file(file_name)
    return ServerResponse(
        status_code=status,
        response=data,
    )

