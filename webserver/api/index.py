from fastapi import APIRouter, Query

from webserver.client.client import client
from webserver.middleware import parse_response_middleware
from webserver.schemas import ServerResponse, UploadFile, ServerRawResponse

router = APIRouter()


@router.post("/upload")
@parse_response_middleware
async def upload_file(request: UploadFile) -> ServerResponse:
    filename = request.filename
    content = request.content

    status, data = await client.upload_file(filename, content)
    return ServerRawResponse(response=data, status_code=status)


@router.get("/search")
@parse_response_middleware
async def search_files(
        content: str = Query(..., description="The content to search")
) -> ServerResponse:
    status, data = await client.search_files(content)
    return ServerRawResponse(response=data, status_code=status)


@router.delete("/delete")
@parse_response_middleware
async def delete_file(
        file_name: str = Query(..., description="The filename to delete from index")
) -> ServerResponse:
    status, data = await client.delete_file(file_name)
    return ServerRawResponse(response=data, status_code=status)

